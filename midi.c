#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi.h"

void printp(uint8_t *data,size_t buf_size, char format[]) {
    uint32_t i;

    for (i = 0; buf_size > i; ++i) {
        printf(format,data[i]);
    }
}

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

        // Read data
        data = malloc(sizeof(uint8_t)*tracks[i].len);
        for (j = 0; tracks[i].len > j; j++) {
            tmp = ffread(file,0,1);
            data[j] = *tmp;
            //printf("data:\t%x\t%u\n",data[j],j);
        }

        // Count events
        tracks[i].num = count_events(data,tracks[i].len);

    }
    return tracks;
}

int count_events(uint8_t *data, uint32_t len) {
    uint32_t i = 0;
    uint32_t result = 0;

    while (len > i) {
        //printf("Start: %x %u %u\n",data[i],i,result);
        // Skip delta time
        while (data[i++] > 0x80);

        //printf("Event: %x %u\n",data[i],i);

        // Skip event data
        if (PROGRAM_CHANGE > data[i] && data[i] >= NOTE_OFF ||
            SYS_EX_MESSAGE    > data[i] && data[i] >= PITCH_BEND) {
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
        result++;

        //printf("Slut %x %u %u\n",data[i],i,result);
    }
    return result;
}

void write_header(FILE *midi_file, header_t header);
void write_tracks(FILE *midi_file, track_t track);
