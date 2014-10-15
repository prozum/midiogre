#include <stdio.h>
#include <stdint.h>

#include "midi.h"


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

header_t read_header(FILE *mid_file);
track_t read_track(FILE *mid_file);

void write_header(FILE *midi_file, header_t header);
void write_track(FILE *midi_file, track_t track);
