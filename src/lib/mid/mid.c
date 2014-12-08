#include "mid.h"

#include <list/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Fixed fread. 
 * - Reads one byte at the time. 
 * - Adds offset
 */
size_t ffread(FILE *file, size_t buf_size)
{
    size_t i,result = 0;

    /* Check buf_size */
    if (buf_size > sizeof(size_t) || buf_size == 0) {
        fprintf(stderr,"Buffer size invalid\n");
        return 0;
    }
    
    for(i = 1; i <= buf_size; i++) {
        result += fgetc(file);
        
        if (i != buf_size) {
            result <<= 8;
        }
    }
 
    return result;
}

/** Read mid file
 * TODO:
 *  - Remove list_get_fixed hack
 *  - Use goto to cleanup
 */
mid_t *read_mid(FILE *file)
{
    mid_t *mid;
    list_t *data;

    /* Allocate memory for header data */
    mid = malloc(sizeof(mid_t));

    /* Dump mid file data */
    data = list_dump_file(file);

    /* Check signature */
    if (list_get_fixed(data, 4) != HEADER_SIGNATURE) {
        fprintf(stderr,"Header signature is invalid\n");
        free(mid);
        return NULL;
    }

    /* Check length */
    if (list_get_fixed(data, 4) != HEADER_LENGTH) {
        fprintf(stderr, "Header length is invalid\n");
        free(mid);
        return NULL;
    }

    /* Get format */
    if ((mid->format = list_get_fixed(data, 2)) > MULTI_TRACK_ASYNC) {
        fprintf(stderr, "Midi format is invalid\n");
        free(mid);
        return NULL;
    }

    /* Get number of tracks and create tracks list */
    mid->tracks = list_create(list_get_fixed(data, 2), sizeof(track_t));

    /* Get division */
    mid->division = list_get_fixed(data, 2);

    /* Get tracks */
    if (read_tracks(data, mid->tracks) != 0) {
        fprintf(stderr, "Midi tracks are invalid\n");
        free(mid);
        return NULL;
    }

    return mid;
}

/** Read tracks */
int read_tracks(list_t *data, list_t *tracks)
{
    track_t *track;

    while ((track = list_next(tracks)) != NULL) {
        
        /* Check signature */
        if (list_get_fixed(data, 4) != TRACK_SIGNATURE) {
            fprintf(stderr, "Track signature is invalid\n");
            list_free(tracks);
            return -1;
        }

        /* Read number of bytes */
        track->bytes = list_get_fixed(data, 4);

        /* Count events to create event list  */
        track->events = list_create(count_events(data->cur, track->bytes),
                                    sizeof(event_t));

        /* Read events */
        if (read_events(data, track->events) != 0) {
            fprintf(stderr, "Midi events are invalid\n");
            list_free(tracks);
            return -1;
        }

    }

    /* Reset current track */
    list_set(tracks, 0, 0, LIST_BEG);

    return 0;
}

/** Read events */
int read_events(list_t *data, list_t *events)
{
    uint32_t tmp;
    int *i,*b;
    event_t *event;

    /* Until end last event */
    while ((event = list_next(events)) != NULL) {

        /* Read delta time */
        while ((tmp = list_get(data)) > 0x80) {
            event->delta += tmp % 0x80;
        };
        event->delta += tmp;

    
        /* Get message */
        tmp = list_get(data);

        /* If channel message */
        if (tmp >= NOTE_OFF &&
            tmp <= PITCH_BEND) {
            
            /* Set channel */
            event->chan = tmp % CHANNELS;
            
            /* Set message */
            event->msg = tmp - event->chan;

        } else {

            event->msg = tmp;
        }

        /* Read message parameters */
        switch (event->msg) {
            /* Messages with two parameters */
            case NOTE_OFF:
            case NOTE_ON:
            case POLY_AFT:
            case CTRL_MODE:
            case PITCH_BEND:
            case SONG_POS_PTR:
                event->byte_1 = list_get(data);
                event->byte_2 = list_get(data);
                break;
            
            /* Meta message */
            case META_MSG:
                event->byte_1 = list_get(data); /* Meta message */
                event->byte_2 = list_get(data); /* Meta length  */

                /* Allocate memory for meta message data */
                event->data = list_slicing(data, data->i, event->byte_2);

                /* Skip message data */
                list_set(data, event->byte_2, LIST_FORW, LIST_CUR);

                break;

            /* System exclusive start message */
            case SYSEX_START:
                event->byte_1 = list_get(data); /* Manufacturer ID */

                /* Count data length */
                i = b = data->cur;
                while (*i++ != SYSEX_END);
                event->byte_2 = i - b;    /* Data length */

                /* Allocate memory for meta message data */
                event->data = list_slicing(data, data->i, event->byte_2);

                /* Skip message data */
                list_set(data, event->byte_2 - 2, LIST_FORW, LIST_CUR);

                break;

            /* Messages with zero parameters */
            case TUNE_REQ:
            case TIMING_CLOCK:
            case FUNC_START:
            case FUNC_CONTINUE:
            case FUNC_STOP:
            case ACTIVE_SENSING:
                break;

            /* Messages which should not be called alone or at all */
            case SYSEX_END:
            case FUNC_UNDEF_1:
            case FUNC_UNDEF_2:
            case FUNC_UNDEF_3:
            case FUNC_UNDEF_4:
                list_free(events);
                return -1;

            /* Messages with one parameter
             * - All data bytes
             * - PRG_CHANGE
             * - CHAN_AFT
             * - TIME_CODE
             * - SONG_SELECT             */
            default:
                event->byte_1 = list_get(data);
                
        }
    }

    /* Reset current event */
    list_set(events, 0, 0, LIST_BEG);

    return 0;
}

/** Count events in event data */
uint32_t count_events(uint8_t *data, uint32_t bytes)
{
    uint32_t b,ev,msg;

    b = 0;

    /* Until end of data */
    for (ev = 0; b < bytes; ev++) {

        /* Skip delta time */
        while (data[b++] > 0x80);

        /* If channel message */
        if (data[b] >= NOTE_OFF &&
            data[b] <= PITCH_BEND) {

            msg = data[b] - (data[b] % CHANNELS);
        } else {
            msg = data[b];
        }

        /* Skib message byte */
        b++;

        switch (msg) {
            /* Messages with two parameters */
            case NOTE_OFF:
            case NOTE_ON:
            case POLY_AFT:
            case CTRL_MODE:
            case PITCH_BEND:
            case SONG_POS_PTR:
                b += 2;
                break;
            
            /* Meta message */
            case META_MSG:
                b++;          /* Skib to length */
                b += data[b]; /* Skib length    */
                b++;
                break;

            /* System exclusive message */
            case SYSEX_START:
                while (data[b++] != SYSEX_END);
                break;
                 
            /* Messages with zero parameters */
            case TUNE_REQ:
            case TIMING_CLOCK:
            case FUNC_START:
            case FUNC_CONTINUE:
            case FUNC_STOP:
            case ACTIVE_SENSING:
                break;

            /* Messages which should not be called alone or at all */
            case SYSEX_END:
            case FUNC_UNDEF_1:
            case FUNC_UNDEF_2:
            case FUNC_UNDEF_3:
            case FUNC_UNDEF_4:
                return 0;

            /* Messages with one parameter
             * - All data bytes
             * - PRG_CHANGE
             * - CHAN_AFT
             * - TIME_CODE
             * - SONG_SELECT             */
            default:
                b++;
        }

    }
    return ev;
}

/** Merge tracks from mid to single track */
list_t *merge_tracks(mid_t *mid) {

    switch(mid->format) {
        case SINGLE_TRACK:
            return list_copy(mid->tracks);

        /* TODO */
        case MULTI_TRACK_SYNC:
            return NULL;

        /* TODO */
        case MULTI_TRACK_ASYNC:
            return NULL;
    }
    return NULL;
}

void write_mid(FILE *midi_file, mid_t *mid);

/** Deallocate data in mid_t */
void free_mid(mid_t *mid)
{
    track_t *track;
    event_t *event;

    /* For each track */
    while ((track = list_next(mid->tracks)) != NULL) {
        
        /* For message in track */
        while ((event = list_next(track->events)) != NULL) {

            /* If meta message or sysex */
            if (event->msg == META_MSG &&
                event->msg == SYSEX_START) {

                /* Deallocate meta event data */
                list_free(event->data);
            }
        }
        /* Deallocate track events */
        list_free(track->events);
    }
    /* Deallocate tracks */
    free(mid->tracks);

    /* Deallocate mid */
    free(mid);
}
