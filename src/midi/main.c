#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi.h"


int main( int argc, char *argv[] )
{
    uint32_t i,j;
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
    printf("Format:\t\t%i\n",header->format);
    printf("Tracks:\t\t%i\n",header->tracks);
    printf("Division:\t%i\n",header->division);

    // Read midi tracks
    tracks = read_tracks(mid_file,header->tracks);
    if ( tracks == NULL) {
        return -1;
    }

    // Print info for tracks
    for (i = 0; header->tracks > i; i++) {
        printf("\nTrack: \t%u\n",i+1);
        printf("Track len: \t%u\n",tracks[i].len);
        printf("Track events: \t%u\n",tracks[i].num);
    }


    printf("Type : %x\n",tracks[0].events[ 0 ].type);
    printf("Para1: %x\n",tracks[0].events[ 0 ].para_1);
    printf("Para2: %x\n",tracks[0].events[ 0 ].para_2);
    printf("Delta: %x\n",tracks[0].events[ 0 ].delta);

    printf("Data: ");
    for (i = 0; i < tracks[0].events[0].para_2; i++ ) {
        printf("%c",tracks[0].events[ 0 ].data[i]);
    }

    // Deallocate header
    free(header);

    // Deallocate tracks
    free_tracks(tracks,header->tracks);

    // Close mid_file
    fclose(mid_file);

    return 0;
}
