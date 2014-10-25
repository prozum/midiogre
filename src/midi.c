#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi.h"

uintptr_t *ffread(FILE *file, long int offset ,size_t buf_size) {
    uint32_t i;
    uint8_t tmp;

    uintptr_t *result;

    if (buf_size > 8) {
        fprintf(stderr,"Buffer size too big: 8+\n");
        return NULL;
    } else if (buf_size > 4) {
        result = malloc(sizeof(uint64_t));
    } else if (buf_size > 2) {
        result = malloc(sizeof(uint32_t));
    } else if (buf_size > 1) {
        result = malloc(sizeof(uint16_t));
    } else if (buf_size > 0) {
        result = malloc(sizeof(uint8_t));
    } else {
        fprintf(stderr,"Buffer size too small: 0-\n");
        return NULL;
    }

    //Assign result a value (Windows compatibility)
    *result = 0;

    // Setup offset
    fseek(file,offset,SEEK_CUR);

    // Read one byte at a time! (Endianness)
    for(i = 1; buf_size > i; ++i) {
        fread(&tmp,1,1,file);
        *result += tmp;
        *result <<= 8;
    }
    fread(&tmp,1,1,file);
    *result += tmp;

    return result;
}

header_t *read_header(FILE *file) {
    uintptr_t *tmp;

    header_t *header = malloc(sizeof(header_t));

    // Start at the beginning of midi_file
    fseek(file,0,SEEK_SET);


    // Check signature
    tmp = ffread(file,0,4);

    if (*tmp != HEADER_SIGNATURE) {
        fprintf(stderr,"Header signature is invalid\n");
        free(header);
        return NULL;
    }

    // Check length
    tmp = ffread(file,0,4);

    if (*tmp != HEADER_LENGTH) {
        fprintf(stderr,"Header length is invalid\n");
        free(header);
        return NULL;
    }

    // Read format
    tmp = ffread(file,0,2);
    header->format = *tmp;

    if (header->format > 2) {
        fprintf(stderr,"Midi format is invalid\n");
        free(header);
        return NULL;
    }

    // Read number of tracks
    tmp = ffread(file,0,2);
    header->tracks = *tmp;

    // Read division
    tmp = ffread(file,0,2);
    header->division = *tmp;

    return header;
}

track_t *read_tracks(FILE *file, uint16_t n) {
    uint32_t i,j;
    uintptr_t *tmp;
    uint8_t *data;

    track_t *tracks = malloc(sizeof(track_t)*n);

    // Start at the first track
    fseek(file,14,SEEK_SET);

    for (i = 0; n > i; i++) {
        // Check signature
        tmp = ffread(file,0,4);
        if (*tmp != TRACK_SIGNATURE) {
            fprintf(stderr,"Track signature is invalid\n");
            return NULL;
        }

        // Read length
        tmp = ffread(file,0,4);
        tracks[i].len = *tmp;

        // Allocate memory for track data
        data = malloc(sizeof(uint8_t) * tracks[i].len);

        // Read track data
        for (j = 0; tracks[i].len > j; j++) {
            tmp = ffread(file,0,1);
            data[j] = *tmp;
            //printf("data:\t%x\t%u\n",data[j],j);
        }

        // Count events
        tracks[i].num = count_events(data, tracks[i].len);

        // Read events
        tracks[i].events = read_events(data,tracks[i].num);

        // Deallocate memory for track data
        free(data);

    }
    return tracks;
}

event_t *read_events(uint8_t *data, uint16_t n) {
    uint32_t j,e,i = 0;

    event_t *events = malloc(sizeof(event_t) * n);

    for (e =0; e < n; e++) {
        //printf("Start: %x %u %u\n",data[i],i,result);

        // Read delta time
        events[e].delta = 0;
        while ( (events[e].delta + data[i++]) > 0x80 );

        // Read event type
        events[e].type = data[i++];

        // Read event parameters
        if ( (PROGRAM_CHANGE > events[e].type && events[e].type >= NOTE_OFF) ||
             (SYS_EX_MESSAGE > events[e].type && events[e].type >= PITCH_BEND) ) {
            events[e].para_1 = data[i++];
            events[e].para_2 = data[i++];

        } else if (PITCH_BEND > events[e].type && events[e].type >= PROGRAM_CHANGE) {
            events[e].para_1 = data[i++];
        } else if (META_EVENT > events[e].type && events[e].type >= SYS_EX_MESSAGE ) {
            printf("?????");
            return NULL;
        } else if (events[e].type == META_EVENT) {
            events[e].para_1 = data[i++]; // Meta type
            events[e].para_2 = data[i++]; // Meta length

            // Allocate memory for meta event data
            events[e].data = malloc(sizeof(uint8_t) * events[e].para_2 );

            // Read meta event data
            for (j = 0; j < events[e].para_2; j++ ) {
                events[e].data[j] = data[i++];
            }

        } else {
            // Control Change Messages (Data Bytes)
            events[e].para_1 = data[i++];
        }

        //printf("End: %x %u %u\n",data[i],i,result);
    }
    return events;
}

int count_events(uint8_t *data, uint32_t len) {
    uint32_t e,i=0;

    for (e = 0; i < len; e++) {
        //printf("Start: %x %u %u\n",data[i],i,result);

        // Skip delta time
        while (data[i++] > 0x80);

        //printf("Event: %x %u\n",data[i],i);

        // Skip event data
        if ( (PROGRAM_CHANGE > data[i] && data[i] >= NOTE_OFF) ||
             (SYS_EX_MESSAGE > data[i] && data[i] >= PITCH_BEND) ) {
            // Events with 2 parameters
            i+=3;
        } else if (PITCH_BEND > data[i] && data[i] >= PROGRAM_CHANGE) {
            // Events with 1 parameter
            i+=2;
        } else if (META_EVENT > data[i] && data[i] >= SYS_EX_MESSAGE ) {
            printf("?????");
            return -1;
        } else if (data[i] == META_EVENT) {
            i+=2;       // Skib to length
            i+=data[i]; // Skib length
            i++;
        } else {
            // Control Change Messages (Data Bytes)
            i+=2;
        }

        //printf("End: %x %u %u\n",data[i],i,result);
    }
    return e;
}

void write_header(FILE *midi_file, header_t header);
void write_tracks(FILE *midi_file, track_t track);

void free_tracks(track_t *tracks, uint16_t n) {
    uint32_t i,j;

    for (i = 0; i < n; i++ ) {
        for (j = 0; j < tracks[i].num; j++) {
            // Deallocate meta data
            if (tracks[i].events[j].type == META_EVENT) {
                free(tracks[i].events[j].data);
            }
        }
        // Deallocate track events
        free(tracks[i].events);
    }
}
