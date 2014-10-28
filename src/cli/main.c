#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi/midi.h"



int main( int argc, char *argv[] )
{
    uint32_t i,j,t,e;
    FILE *mid_file;
    header_t *header;
    track_t *tracks;

    // Open file
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    // Read midi header
    header = read_header(mid_file);
    if ( header == NULL) {
        return -1;
    }

    // Print header info
    printf("<###HEAD INFO###>\n");
    printf("Format:\t\t%i\n",header->format);
    printf("Tracks:\t\t%i\n",header->tracks);
    printf("Division:\t%i\n",header->division);

    // Read midi tracks
    tracks = read_tracks(mid_file,header->tracks);
    if ( tracks == NULL) {
        return -1;
    }

    // Print info for tracks
    printf("\n<###TRACK INFO###>\n");
    for (i = 0; header->tracks > i; i++) {
        printf("Track: \t%u\n",i+1);
        printf("Track len: \t%u\n",tracks[i].len);
        printf("Track events: \t%u\n",tracks[i].num);
    }


    // Print data for third event in first track
    t = 1;
    e = 3;
    printf("\n<###EVENT INFO###>\n");
    printf("Track %2u | Event %2u\n",t,e);
    printf("Type : %x\n",tracks[t-1].events[e-1].type);
    printf("Para1: %x\n",tracks[t-1].events[e-1].para_1);
    printf("Para2: %x\n",tracks[t-1].events[e-1].para_2);
    printf("Delta: %x\n",tracks[t-1].events[e-1].delta);

    // Meta event data
    if (tracks[t-1].events[e-1].type == META_EVENT) {
        printf("Data: ");
        for (i = 0; i < tracks[t-1].events[e-1].para_2; i++ ) {
            printf("%x ",tracks[t-1].events[e-1].data[i]);
        }
    }

    // Deallocate tracks
    free_tracks(tracks,header->tracks);

    // Deallocate header
    free(header);

    // Close mid_file
    fclose(mid_file);

    // When running in therminal, moves "Press <RETURN> to close this window..." to new line
    printf("\n");

    return 0;
}
