#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "midi.h"

uint8_t header_signature[4] = HEADER_SIGNATURE;
uint8_t header_length[4]    = HEADER_LENGTH;
uint8_t track_signature[4]  = TRACK_SIGNATURE;

void printp(uint8_t *data,size_t buf_size, char format[]) {
    uint8_t i;

    for (i = 0; buf_size > i; ++i) {
        printf(format,data[i]);
    }
}

uint8_t * ffread(FILE *file, long int offset ,size_t buf_size) {
    uint32_t i;

    // Allocate memory for data
    uint8_t *tmp = malloc(sizeof(uint8_t)*buf_size);

    // Setup offset
    fseek(file,offset,SEEK_SET);

    // Read one byte at the time! (Endianness)
    for(i = 0; buf_size > i; ++i) {
        fread(&tmp[i],1,1,file);
    }

    return tmp;
}

header_t *read_header(FILE *file) {
    int i;
    uint8_t tmp[2];

    header_t *header = malloc(sizeof(header_t));

    // Start at the beginning of midi_file
    fseek(file,0,SEEK_SET);


    // Check signature
    for (i = 0; 4 > i; i++) {
        tmp[0] = fgetc(file);
        if (tmp[0] != header_signature[i]) {
            fprintf(stderr,"Header signature is invalid\n");
            return NULL;
        }
    }

    // Check lenght
    for (i = 0; 4 > i ; i++) {
        tmp[0] = fgetc(file);

        if (tmp[0] != header_lenght[i]) {
            fprintf(stderr,"Header lenght is invalid\n");
            return NULL;
        }
    }

    // Read format
    tmp[0] = fgetc(file);
    tmp[1] = fgetc(file);
    header->format = tmp[0]*256+tmp[1];

    if (header->format > 2) {
        fprintf(stderr,"Midi format is invalid\n");
        return NULL;
    }

    // Read number of tracks
    tmp[0] = fgetc(file);
    tmp[1] = fgetc(file);
    header->tracks = tmp[0]*256+tmp[1];

    // Read division
    tmp[0] = fgetc(file);
    tmp[1] = fgetc(file);
    header->division = tmp[0]*256+tmp[1];

    return header;
}

track_t read_track(FILE *mid_file);

void write_header(FILE *midi_file, header_t header);
void write_track(FILE *midi_file, track_t track);
