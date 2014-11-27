#include "mid.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Fixed fread. 
 * - Reads one byte at the time. 
 * - Adds offset
 */
uint32_t ffread(FILE *file, long int offset, size_t buf_size)
{
    uint32_t i,rv;
    uint8_t tmp;

    uint32_t result = 0;

    /* Check buf_size */
    if (buf_size > 4) {
        fprintf(stderr,"Buffer size invalid\n");
        return 0;
    }
    
    /* Setup offset from current position */
    if ( fseek(file,offset,SEEK_CUR) != 0)
    {
        fprintf(stderr,"could not set pos\n");
        return 0;
    }
    
    /* First bytes */
    for(i = 1; buf_size > i; ++i) {
        rv = fread(&tmp, 1, 1, file);
        
        if (rv == 0) {
            fprintf(stderr,"could not read\n");
            return 0;
        }
 
        result += tmp;
        result <<= 8;
    }
 
    /* Last byte */   
    rv = fread(&tmp, 1, 1, file);
    
    if (rv == 0) {
        fprintf(stderr,"could not read\n");
        return 0;
    }
    
    result += tmp;

    return result;
}

/** Read mid file */
mid_t *read_mid(FILE *file)
{
    uint32_t tmp;

    /* Allocate memory for header data */
    mid_t *mid = malloc(sizeof(mid_t));

    /* Start at the beginning of midi_file */
    fseek(file, 0, SEEK_SET);

    /* Read signature */
    tmp = ffread(file, 0, 4);

    /* Check if signature is valid */
    if (tmp != HEADER_SIGNATURE) {
        fprintf(stderr,"Header signature is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read length */
    tmp = ffread(file, 0, 4);

    /* Check if length is valid */
    if (tmp != HEADER_LENGTH) {
        fprintf(stderr, "Header length is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read format */
    tmp = ffread(file, 0, 2);
    mid->format = tmp;

    /* Check if format has a valid value */
    if (mid->format > MULTI_TRACK_ASYNC) {
        fprintf(stderr, "Midi format is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read number of tracks */
    tmp = ffread(file,0,2);
    mid->tracks = tmp;

    /* Read division */
    tmp = ffread(file, 0, 2);
    mid->division = tmp;

    /* Read tracks */
    mid->track = read_tracks(file, mid->tracks);
    if ( mid->track == NULL) {
        fprintf(stderr, "Midi tracks are invalid\n");
        free(mid);
        return NULL;
    }

    return mid;
}

/** Read tracks */
track_t *read_tracks(FILE *file, uint16_t num)
{
    uint32_t i, j;
    uint32_t tmp;
    uint8_t *data;
    
    track_t *tracks;

    /* Start at the first track */
    if (fseek(file, FIRST_TRACK_POS,SEEK_SET) != 0 ) {
        fprintf(stderr,"fseek failed\n");
        return NULL;
    }
    
    tracks = calloc(sizeof(track_t), num);

    /* For each track */
    for (i = 0; i < num; i++) {
        
        /* Check signature */
        tmp = ffread(file,0,4);
        if (tmp != TRACK_SIGNATURE) {
            fprintf(stderr, "Track signature is invalid\n");
            free(tracks);
            return NULL;
        }

        /* Read length */
        tmp = ffread(file, 0, 4);
        tracks[i].len = tmp;

        /* Allocate memory for track data */
        data = malloc(sizeof(uint8_t) * tracks[i].len);

        /* Read track data */
        for (j = 0; tracks[i].len > j; j++) {
            tmp = ffread(file, 0, 1);
            data[j] = tmp;
        }

        /* Count events */
        tracks[i].events = count_events(data, tracks[i].len);

        /* Read events */
        tracks[i].event = read_events(data,tracks[i].events);

        /* Deallocate memory for track data */
        free(data);

    }
    return tracks;
}

/** Read events */
event_t *read_events(uint8_t *data, uint16_t num)
{
    uint32_t i, j, e;
    event_t *event;
    
    /* Start at first byte */
    i = 0;

    /* Allocate memory for events */
    event = calloc(sizeof(event_t), num);

    /* Until end last event */
    for (e = 0; e < num; e++) {
        /* Read delta time */
        event[e].delta = 0;
        while ((event[e].delta += data[i++]) > 0x80 );
    
        /* If channel message */
        if (data[e] >= NOTE_OFF &&
            data[e] <= PITCH_BEND) {
            
            /* Read event channel */
            event[e].chan = data[i] % 0x10;
            
            /* Read event msg */
            event[e].msg = data[i++] - event[e].chan;
        } else {
            event[e].msg = data[i++];
        }
        
        /* Read message parameters */
        switch (event[e].msg) {
            /* Messages with two parameters */
            case NOTE_OFF:
            case NOTE_ON:
            case POLY_AFT:
            case CTRL_MODE:
            case PITCH_BEND:
            case SONG_POS_PTR:
                event[e].para_1 = data[i++];
                event[e].para_2 = data[i++];
                break;
            
            /* Meta message with */
            case META_MSG:
                event[e].para_1 = data[i++]; /* Meta message */
                event[e].para_2 = data[i++]; /* Meta length  */

                /* Allocate memory for meta message data */
                event[e].mdata = calloc(sizeof(uint8_t), event[e].para_2);

                /* Read meta event data */
                for (j = 0; j < event[e].para_2; j++) {
                    event[e].mdata[j] = data[i++];
                }
                break;

            /* System exclusive message */
            case SYS_EXCLUSIVE:
                event[e].para_1 = data[i++]; /* Manufacturer ID */

                /* Count data length */
                j = i;
                while (data[j++] != END_SYSEX);
                event[e].para_2 = j - i;    /* Data length */
                
                /* Read system exclusive message data */
                for (j = 0; j < event[e].para_2; j++) {
                    event[e].mdata[j] = data[i++];
                }
                break;
                
            /* Messages with zero parameters */
            case TUNE_REQ:
            case TIMING_CLOCK:
            case FUNC_START:
            case FUNC_CONTINUE:
            case FUNC_STOP:
            case ACTIVE_SENSING:
            case END_SYSEX:
                break;
            
            /* Messages with one parameter 
             * - All data bytes
             * - PRG_CHANGE
             * - CHAN_AFT
             * - TIME_CODE
             * - SONG_SELECT             */
            default:
                event[e].para_1 = data[i++];
                
        }
    }
    return event;
}

/** Count events in event data */
uint32_t count_events(uint8_t *data, uint32_t len)
{
    uint32_t e,i,j;

    i = 0;

    /* Until end of data */
    for (e = 0; i < len; e++) {
        /* Skip delta time */
        while (data[i++] > 0x80);

        switch (data[i] - data[i] % 0x10) {
            /* Events with two parameters */
            case NOTE_OFF:
            case NOTE_ON:
            case POLY_AFT:
            case CTRL_MODE:
            case PITCH_BEND:
            case SONG_POS_PTR:
                i += 2;
                break;
            
            /* Meta message */
            case META_MSG:
                i++;          /* Skib to length */
                i += data[i]; /* Skib length    */
                i++;
                break;

            /* System exclusive message */
            case SYS_EXCLUSIVE:
                while (data[i++] != END_SYSEX);
                break;
                 
            /* Messages with zero parameters */
            case TUNE_REQ:
            case TIMING_CLOCK:
            case FUNC_START:
            case FUNC_CONTINUE:
            case FUNC_STOP:
            case ACTIVE_SENSING:
            case END_SYSEX:
                break;
            
            /* Messages with one parameter 
             * - All data bytes
             * - PRG_CHANGE
             * - CHAN_AFT
             * - TIME_CODE
             * - SONG_SELECT             */
            default:
                i++;
        }

    }
    return e;
}

void write_mid(FILE *midi_file, mid_t mid);

/** Deallocate data in mid_t */
void free_mid(mid_t *mid)
{
    uint32_t i,j;

    /* For each track */
    for (i = 0; i < mid->tracks; i++ ) {
        
        /* For events in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* If meta event */
            if (mid->track[i].event[j].msg == META_MSG) {
                
                /* Deallocate meta event data */
                free(mid->track[i].event[j].mdata);
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
