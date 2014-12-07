#include "mid.h"

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

/** Read mid file */
mid_t *read_mid(FILE *file)
{
    /* Allocate memory for header data */
    mid_t *mid = malloc(sizeof(mid_t));

    /* Start at the beginning of midi_file */
    fseek(file, 0, SEEK_SET);

    /* Read signature */
    if (ffread(file, 4) != HEADER_SIGNATURE) {
        fprintf(stderr,"Header signature is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read length */
    if (ffread(file, 4) != HEADER_LENGTH) {
        fprintf(stderr, "Header length is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read format */
    mid->format = ffread(file, 2);

    /* Check if format has a valid value */
    if (mid->format > MULTI_TRACK_ASYNC) {
        fprintf(stderr, "Midi format is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read number of tracks */
    mid->tracks = ffread(file, 2);

    /* Read division */
    mid->division = ffread(file, 2);

    /* Read tracks */
    if ((mid->track = read_tracks(file, mid->tracks)) == NULL) {
        fprintf(stderr, "Midi tracks are invalid\n");
        free(mid);
        return NULL;
    }

    return mid;
}

/** Read tracks */
track_t *read_tracks(FILE *file, uint16_t tracks)
{
    uint32_t i, j;
    uint8_t *data;
    
    track_t *track;

    /* Start at the first track */
    if (fseek(file, FIRST_TRACK_POS,SEEK_SET) != 0 ) {
        fprintf(stderr,"fseek failed\n");
        return NULL;
    }
    
    track = calloc(sizeof(track_t), tracks);

    /* For each track */
    for (i = 0; i < tracks; i++) {
        
        /* Check signature */
        if (ffread(file, 4) != TRACK_SIGNATURE) {
            fprintf(stderr, "Track signature is invalid\n");
            free(track);
            return NULL;
        }

        /* Read number of bytes */
        track[i].bytes = ffread(file, 4);

        /* Allocate memory for track data */
        data = calloc(sizeof(uint8_t), track[i].bytes);

        /* Read track data */
        for (j = 0; j < track[i].bytes; j++) {
            data[j] = ffread(file, 1);
        }

        /* Count events */
        track[i].events = count_events(data, track[i].bytes);

        /* Read events */
        track[i].event = read_events(data, track[i].events);

        /* Deallocate memory for track data */
        free(data);
    }
    return track;
}

/** Read events */
event_t *read_events(uint8_t *data, uint16_t events)
{
    uint32_t i,b,ev;
    event_t *event;

    /* Start at first byte */
    b = 0;

    /* Allocate memory for events */
    event = calloc(sizeof(event_t), events);

    /* Until end last event */
    for (ev = 0; ev < events; ev++) {
        /* Read delta time */
        do {
            event[ev].delta += data[b] % 0x80;
        } while (data[b++] > 0x80);

    
        /* If channel message */
        if (data[b] >= NOTE_OFF &&
            data[b] <= PITCH_BEND) {
            
            /* Read event channel */
            event[ev].chan = data[b] % CHANNELS;
            
            /* Read event msg */
            event[ev].msg = data[b++] - event[ev].chan;
        } else {
            event[ev].msg = data[b++];
        }

        /* Read message parameters */
        switch (event[ev].msg) {
            /* Messages with two parameters */
            case NOTE_OFF:
            case NOTE_ON:
            case POLY_AFT:
            case CTRL_MODE:
            case PITCH_BEND:
            case SONG_POS_PTR:
                event[ev].byte_1 = data[b++];
                event[ev].byte_2 = data[b++];
                break;
            
            /* Meta message with */
            case META_MSG:
                event[ev].byte_1 = data[b++]; /* Meta message */
                event[ev].byte_2 = data[b++]; /* Meta length  */

                /* Allocate memory for meta message data */
                event[ev].data = calloc(sizeof(uint8_t), event[ev].byte_2);

                /* Read meta message data */
                for (i = 0; i < event[ev].byte_2; i++) {
                    event[ev].data[i] = data[b++];
                }
                break;

            /* System exclusive start message */
            case SYSEX_START:
                event[ev].byte_1 = data[b++]; /* Manufacturer ID */

                /* Count data length */
                i = b;
                while (data[i++] != SYSEX_END);
                event[ev].byte_2 = i - b;    /* Data length */
                
                /* Read system exclusive message data */
                for (i = 0; i < event[ev].byte_2; i++) {
                    event[ev].data[i] = data[b++];
                }
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
                return NULL;

            /* Messages with one parameter
             * - All data bytes
             * - PRG_CHANGE
             * - CHAN_AFT
             * - TIME_CODE
             * - SONG_SELECT             */
            default:
                event[ev].byte_1 = data[b++];
                
        }
    }
    return event;
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
track_t *merge_tracks(mid_t *mid) {

    switch(mid->format) {
        case SINGLE_TRACK:
            return mid->track;

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
    uint32_t i,j;

    /* For each track */
    for (i = 0; i < mid->tracks; i++ ) {
        
        /* For message in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* If meta message or sysex */
            if (mid->track[i].event[j].msg == META_MSG &&
                mid->track[i].event[j].msg == SYSEX_START) {

                /* Deallocate meta event data */
                free(mid->track[i].event[j].data);
            }
        }
        /* Deallocate track events */
        free(mid->track[i].event);
    }
    /* Deallocate tracks */
    free(mid->track);

    /* Deallocate mid */
    free(mid);
}
