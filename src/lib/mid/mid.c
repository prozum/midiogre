#include "mid.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * Fixed ffread. 
 * - Reads one byte at the time. 
 * - Adds offset
 */
uint32_t ffread(FILE *file, long int offset, size_t buf_size)
{
    uint32_t i;
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
        fprintf(stderr,"fseek failed\n");
        return 0;
    }
    
    /* Read one byte at a time! (Endianness) */
    for(i = 1; buf_size > i; ++i) {
        fread(&tmp,1,1,file);
        result += tmp;
        result <<= 8;
    }
    fread(&tmp,1,1,file);
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
    fseek(file,0,SEEK_SET);

    /* Read signature */
    tmp = ffread(file,0,4);

    /* Check if signature is valid */
    if (tmp != HEADER_SIGNATURE) {
        fprintf(stderr,"Header signature is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read length */
    tmp = ffread(file,0,4);

    /* Check if length is valid */
    if (tmp != HEADER_LENGTH) {
        fprintf(stderr,"Header length is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read format */
    tmp = ffread(file,0,2);
    mid->format = tmp;

    /* Check if format has a valid value */
    if (mid->format > MULTI_TRACK_ASYNC) {
        fprintf(stderr,"Midi format is invalid\n");
        free(mid);
        return NULL;
    }

    /* Read number of tracks */
    tmp = ffread(file,0,2);
    mid->tracks = tmp;

    /* Read division */
    tmp = ffread(file,0,2);
    mid->division = tmp;

    /* Read tracks */
    mid->track = read_tracks(file,mid->tracks);
    if ( mid->track == NULL) {
        fprintf(stderr,"Midi tracks are invalid\n");
        free(mid);
        return NULL;
    }

    return mid;
}

/** Read tracks */
track_t *read_tracks(FILE *file, uint16_t num)
{
    uint32_t i,j;
    uint32_t tmp;
    uint8_t *data;

    /* Start at the first track */
    if ( fseek(file,FIRST_TRACK_POS,SEEK_SET) != 0 )
    {
        fprintf(stderr,"fseek failed\n");
        return NULL;
    }
    
    track_t *tracks = calloc(sizeof(track_t),num);

    /* For each track */
    for (i = 0; i < num; i++) {
        
        /* Check signature */
        tmp = ffread(file,0,4);
        if (tmp != TRACK_SIGNATURE) {
            fprintf(stderr,"Track signature is invalid\n");
            free(tracks);
            return NULL;
        }

        /* Read length */
        tmp = ffread(file,0,4);
        tracks[i].len = tmp;

        /* Allocate memory for track data */
        data = malloc(sizeof(uint8_t) * tracks[i].len);

        /* Read track data */
        for (j = 0; tracks[i].len > j; j++) {
            tmp = ffread(file,0,1);
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

/* Read events */
event_t *read_events(uint8_t *data, uint16_t num)
{
    uint32_t i,j,e;
    event_t *event;
    
    /* Start at first byte */
    i = 0;

    /* Allocate memory for events */
    event = calloc(sizeof(event_t),num);

    /* Until end last event */
    for (e = 0; e < num; e++) {
        /* Read delta time */
        event[e].delta = 0;
        while ( (event[e].delta += data[i++]) > 0x80 );

        /* Read event msg */
        event[e].msg = data[i++];

        /* Read event parameters */
        if ( (CTRL_MODE_16 >= event[e].msg && event[e].msg >= NOTE_OFF_1) ||
             (PITCH_BEND_16 >= event[e].msg && event[e].msg >= PITCH_BEND_1) ) {
            /* Events with two parameters */
            event[e].para_1 = data[i++];
            event[e].para_2 = data[i++];

        } else if (CHAN_AFT_16 >= event[e].msg && event[e].msg >= PRG_CHANGE_1) {
            /* Events with one parameters */
            event[e].para_1 = data[i++];
        } else if (ACTIVE_SENSING > event[e].msg && event[e].msg >= SYS_EXCUSIVE) {
            /* TODO */
            printf("?????");
            free(event);
            return NULL;
        } else if (event[e].msg == META_MSG) {
            /* Meta events */
            event[e].para_1 = data[i++]; /* Meta msg   */
            event[e].para_2 = data[i++]; /* Meta length */

            /* Allocate memory for meta event data */
            event[e].mdata = calloc(sizeof(uint8_t), event[e].para_2 );

            /* Read meta event data */
            for (j = 0; j < event[e].para_2; j++ ) {
                event[e].mdata[j] = data[i++];
            }

        } else {
            /* Control Change Messages (Data Bytes) */
            event[e].para_1 = data[i++];
        }
    }
    return event;
}

/* Count events in event data */
uint32_t count_events(uint8_t *data, uint32_t len)
{
    uint32_t e,i=0;

    /* Until end of data */
    for (e = 0; i < len; e++) {
        /* Skip delta time */
        while (data[i++] > 0x80);

        /* Skip event data */
        if ( (CTRL_MODE_16 >= data[i] && data[i] >= NOTE_OFF_1) ||
             (PITCH_BEND_16 >= data[i] && data[i] >= PITCH_BEND_1) ) {
            /* Events with 2 parameters */
            i+=3;
        } else if (CHAN_AFT_16 >= data[i] && data[i] >= PRG_CHANGE_1) {
            /* Events with 1 parameter */
            i+=2;
        } else if (ACTIVE_SENSING > data[i] && data[i] >= SYS_EXCUSIVE) {
            /* TODO */
            printf("?????");
            return -1;
        } else if (data[i] == META_MSG) {
            i+=2;       /* Skib to length */
            i+=data[i]; /* Skib length    */
            i++;
        } else {
            /* Control Change Messages (Data Bytes) */
            i+=2;
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
