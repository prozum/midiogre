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

    uintptr_t  *result;

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

    //Init result
    *result = 0;

    // Setup offset
    fseek(file,offset,SEEK_CUR);

    // Read one byte at the time! (Endianness)
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
        fprintf(stderr,"Header lenght is invalid\n");
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
    uint32_t i;
    uintptr_t *tmp;

    track_t *tracks = malloc(sizeof(track_t)*n);

    // Start at the beginning of midi_file
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

        fseek(file,tracks[i].len,SEEK_CUR);

        // Read data
        /*
        for (i = 0; tracks[i].len > i; i++) {
            printf("buuuuu");
            tmp[0] = fgetc(file);
            printf("test:%c",tmp[0]);
        }
        */
    }
    return tracks;
}

void write_header(FILE *midi_file, header_t header);
void write_tracks(FILE *midi_file, track_t track);
