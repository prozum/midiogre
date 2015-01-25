#include "mid.h"

#include <list/list.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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
        list_free(data);
        free(mid);
        return NULL;
    }

    /* Check length */
    if (list_get_fixed(data, 4) != HEADER_LENGTH) {
        fprintf(stderr, "Header length is invalid\n");
        list_free(data);
        free(mid);
        return NULL;
    }

    /* Get format */
    if ((mid->format = list_get_fixed(data, 2)) > MULTI_TRACK_SYNC) {
        fprintf(stderr, "Midi format is invalid or not supported!\n");
        list_free(data);
        free(mid);
        return NULL;
    }

    /* Get number of tracks and create tracks list */
    mid->tracks = list_create(list_get_fixed(data, 2), sizeof(track_t));

    /* Get division */
    mid->division = list_get_fixed(data, 2);

    /* Get tracks */
    if (read_tracks(data, mid->division, mid->tracks) != 0) {
        fprintf(stderr, "Midi tracks are invalid!\n");
        list_free(data);
        free_mid(mid);
        return NULL;
    }

    /* Free mid file data */
    list_free(data);

    return mid;
}

int read_tracks(list_t *data, uint16_t division, list_t *tracks)
{
    track_t *track;

    uint32_t events;
    uint32_t bytes;        /**< Var for checking bytes overflow      */
    uint32_t start_tempo;  /**< Var for start tempo from first track */

    /* Add HEADER_BYTES to byte overflow check */
    bytes = HEADER_BYTES;

    /* Read start tempo from first track */
    start_tempo = find_start_tempo(data->cur, data->n);

    while ((track = list_next(tracks)) != NULL) {
        
        /* Check signature */
        if (list_get_fixed(data, 4) != TRACK_SIGNATURE) {
            fprintf(stderr, "Read tracks: Track signature is invalid!\n");
            return -1;
        }

        /* Read number of bytes */
        track->bytes = list_get_fixed(data, 4);

        /* Check for bytes overflow */
        if ((bytes += track->bytes + TRACK_BYTES) > data->n) {
            fprintf(stderr, "Read tracks: Track bytes value is invalid!\n");
            return -1;
        }

        /* Count events */
        if ((events = count_events(data->cur, track->bytes)) == 0) {
            fprintf(stderr, "Read tracks: Count events failed!\n");
            return -1;
        }

        /* Create event list */
        track->events = list_create(events, sizeof(event_t));

        /* Read events */
        if (read_events(data, division, start_tempo, track->events) != 0) {
            fprintf(stderr, "Read tracks: Midi events are invalid\n");
            return -1;
        }

    }

    /* Reset current track */
    list_reset(tracks);

    return 0;
}

uint32_t find_start_tempo(uint8_t *data, uint32_t bytes)
{
    uint32_t tempo = 0;  /**< Tempo */
    uint32_t b = 0;      /**< Current byte offset */
    uint32_t msg,n,i;

    /* Skip track signature + number of bytes */
    b += 8;

    while (b < bytes) {

        /* Skip delta time */
        while (data[b++] > 0x80);

        msg = data[b++];

        if (msg == META_MSG) {

            /* Set tempo meta message */
            if (data[b++] == SET_TEMPO) {

                /* Number of bytes in meta message */
                n = data[b++];

                /* Read and return tempo */
                for (i = 0; i < n; i++) {

                    tempo += data[b++];

                    if (i != n - 1) {
                        tempo <<= 8;
                    }
                }
                return tempo;

            /* Skip other meta messages */
            } else {

                /* Number of bytes in meta message */
                n = data[b++];

                b += n;

            }

        /* Skip sysex message */
        } else if (msg == SYSEX_START) {

            while (data[b++] != SYSEX_END);

        /* Message must be meta/sysex */
        } else {

            return SET_TEMPO_DEFAULT;
        }

    }

    return SET_TEMPO_DEFAULT;
}

int read_events(list_t *data, uint16_t division, uint32_t tempo, list_t *events)
{
    double time_last = 0;

    int get_tmp;
    uint32_t tmp;
    uint32_t i;

    uint8_t *byte;
    event_t *event;

    /* Until end last event */
    while ((event = list_next(events)) != NULL) {

        /* Read delta time */
        while ((tmp = list_get(data)) >= 0x80) {

            tmp %= 0x80;
            event->delta += tmp;
            event->delta <<= 7;

            /* Check for overflow */
            if(data->i == data->n) {
                fprintf(stderr, "Delta overflow!\n");
                return -1;
            }
        };
        event->delta += tmp;

        /* Calc total ticks */
        event->time = time_last + event->delta * (tempo/division)/1000;
        time_last = event->time;

        /* Get message */
        tmp = list_get(data);

        /* If channel message */
        if (tmp >= NOTE_OFF &&
            tmp < PITCH_BEND + CHANNELS) {
            
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

                /* Byte 1: Meta message */
                event->byte_1 = list_get(data);

                /* Byte 2: Meta length  */
                if ((get_tmp = list_get(data)) >= 0) {

                    event->byte_2 = get_tmp;

                } else {

                    fprintf(stderr,"No bytes left in buffer of mid file!\n");
                    return - 1;
                }

                /* Allocate memory for meta message data */
                event->data = list_slicing(data, data->i, event->byte_2);

                /* Skip message data */
                list_set(data, event->byte_2, LIST_FORW, LIST_CUR);

                /* Tempo set */
                if (event->byte_1 == SET_TEMPO) {

                    tempo = 0;

                    for (i = 0; i < event->byte_2; i++) {
                        byte = list_index(event->data, i);
                        tempo += *byte;

                        if (i + 1 != event->byte_2) {
                            tempo <<= 8;
                        }
                    }
                }

                break;

            /* System exclusive start message */
            case SYSEX_START:

                /* Manufacturer ID */
                event->byte_1 = list_get(data);

                /* Count data length */
                i = data->i;
                do {

                    byte = list_index(data, i);
                    i++;

                } while (*byte != SYSEX_END && i < data->n);

                /* Check for overflow */
                if (i >= data->n) {
                    fprintf(stderr, "Sysex overflow!\n");
                    return -1;
                }

                /* Data length */
                event->byte_2 = i - data->i;

                /* Allocate memory for sysex message data */
                event->data = list_slicing(data, data->i, event->byte_2);

                /* Skip message data */
                list_set(data, event->byte_2, LIST_FORW, LIST_CUR);

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
                fprintf(stderr, "Invalid message!\n");
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
    list_reset(events);

    return 0;
}

/** Count events in track bytes */
uint32_t count_events(uint8_t *data, uint32_t bytes)
{
    uint32_t b,ev,msg;

    b = 0;

    /* Until end of data */
    for (ev = 0; b < bytes; ev++) {

        /* Skip delta time */
        while (data[b++] > 0x80 && b < bytes)

        /* Check overflow */
        if (b >= bytes) {

            fprintf(stderr, "Count events failed: Byte overflow!");
            return 0;
        }

        /* If channel message */
        if (data[b] >= NOTE_OFF &&
            data[b] < PITCH_BEND + CHANNELS) {

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
                while (data[b++] != SYSEX_END && b < bytes);

                /* Check for overflow */
                if (b == bytes) {

                    fprintf(stderr, "Count events failed: Sysex overflow!\n");
                    return 0;
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
                fprintf(stderr, "Count events failed: Invalid message!\n");
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
mid_t *merge_tracks(mid_t *mid)
{
    mid_t *mid_new;
    track_t *track;
    track_t *track_new;
    event_t *event;
    event_t *event_new;

    double time_min = -1; /**< Minimum time for current events        */
    int n = 1;            /**< Number of events plus 1 (END_OF_TRACK) */

    list_reset(mid->tracks);
    while ((track = list_next(mid->tracks)) != NULL) {

        /* Read last message */
        if ((event = list_index(track->events, track->events->n - 1)) == NULL) {

            fprintf(stderr, "Merge tracks failed: Track %lu is empty!\n",
                    mid->tracks->i);
            return NULL;
        }

        /* Validate last message */
        if (event->msg != META_MSG && event->byte_1 != END_OF_TRACK) {

            fprintf(stderr, "Track %lu is invalid!\n", mid->tracks->i);
            return NULL;
        }

        /* Count total events minus END_OF_TRACK message */
        n += track->events->n - 1;

        /* Reset events list */
        list_reset(track->events);

    }

    /* Create new mid with 1 track with n events */
    mid_new = malloc(sizeof(mid_t));

    mid_new->format = SINGLE_TRACK;
    mid_new->tracks = list_create(1, sizeof(track_t));
    mid_new->division = mid->division;

    track_new = list_index(mid_new->tracks, 0);
    track_new->events = list_create(n, sizeof(event_t));

    switch(mid->format) {

        case SINGLE_TRACK:

            /* Copy events */
            track = list_index(mid->tracks, 0);
            track_new->events = list_copy(track->events);

            /* Copy meta/sysex event data */
            list_reset(track->events);
            while ((event = list_next(track->events)) != NULL) {

                if (event->msg == META_MSG ||
                    event->msg == SYSEX_START) {

                    event_new = list_index(track_new->events, track->events->i - 1);

                    event_new->data = list_copy(event->data);
                }
            }

            return mid_new;

        case MULTI_TRACK_SYNC:

            /* Until only one event is left (1 for END_OF_TRACK) */
            while (track_new->events->i < track_new->events->n - 1) {

                /* Find minimum time for current events */
                list_reset(mid->tracks);
                while ((track = list_next(mid->tracks)) != NULL) {

                    /* Get current event in track */
                    if ((event = track->events->cur) != NULL) {

                        if ((time_min == -1 || event->time < time_min)) {

                            time_min = event->time;
                        }
                    }
                }

                /* No more events! */
                if (time_min == - 1) {
                    fprintf(stderr, "Merge tracks failed!\n");
                    free_mid(mid_new);
                    return NULL;
                }

                /* Add event if event->time = time_min */
                list_reset(mid->tracks);
                while ((track = list_next(mid->tracks)) != NULL) {

                    /* Get current event in track */
                    if ((event = track->events->cur) != NULL) {

                        /* If current event has minimum time */
                        if (event->time == time_min) {

                            /* Copy all events except END_OF_TRACK */
                            if ((event->msg    != META_MSG ||
                                 event->byte_1 != END_OF_TRACK) &&
                                (event_new = list_next(track_new->events)) != NULL) {

                                *event_new = *event;

                                /* Copy meta/sysex */
                                if (event->msg == META_MSG ||
                                    event->msg == SYSEX_START) {

                                    event_new->data = list_copy(event->data);
                                }
                            }

                            /* Skip event */
                            list_set(track->events, 1, LIST_FORW, LIST_CUR);

                        }
                    }
                }

                /* Reset vars for next events */
                time_min = -1;

            }

            /* Get time from last event before END_OF_TRACK  */
            event = list_index(track_new->events, track_new->events->n - 2);

            /* Set last event to END_OF_TRACK meta event */
            event_new = track_new->events->end;
            event_new->time   = event->time;
            event_new->msg    = META_MSG;
            event_new->byte_1 = END_OF_TRACK;
            event_new->byte_2 = 0;
            event_new->data   = list_create(0, sizeof(char));

            /* Reset current event in mid_new->tracks */
            list_reset(track_new->events);

            return mid_new;

        /* TODO */
        case MULTI_TRACK_ASYNC:
            fprintf(stderr, "Mid type is not supported\n!");
            free_mid(mid_new);
            return NULL;
    }

    fprintf(stderr, "Mid type is invalid\n!");
    free_mid(mid_new);
    return NULL;
}

void write_mid(FILE *midi_file, mid_t *mid);

void free_mid(mid_t *mid)
{
    track_t *track;
    event_t *event;

    /* Reset tracks list */
    list_set(mid->tracks, 0, LIST_FORW, LIST_BEG);

    /* For each track */
    while ((track = list_next(mid->tracks)) != NULL) {

        /* Reset events list */
        list_set(track->events, 0, LIST_FORW, LIST_BEG);

        /* For events in track */
        while ((event = list_next(track->events)) != NULL) {

            /* If meta message or sysex */
            if (event->msg == META_MSG ||
                event->msg == SYSEX_START) {

                /* Deallocate meta event data */
                list_free(event->data);
            }
        }
        /* Deallocate track events */
        list_free(track->events);
    }
    /* Deallocate tracks */
    list_free(mid->tracks);

    /* Deallocate mid */
    free(mid);
}
