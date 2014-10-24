#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi.h"


int main( int argc, char *argv[] )
{
    int i;
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

    // Print info
    printf("Format:\t\t%i\n",header->format);
    printf("Tracks:\t\t%i\n",header->tracks);
    printf("Division:\t%i\n",header->division);

    // Read midi tracks
    tracks = read_tracks(mid_file,header->tracks);
    if ( tracks == NULL) {
        return -1;
    }

    for (i = 0; header->tracks > i; i++) {
        printf("Track: %u\n",i+1);
        printf("Track len: %u\n",tracks[i].len);
        printf("Track events: %u\n",tracks[i].num);
    }

    // Deallocate bytes
    free(header);
    // Close mid_file
    fclose(mid_file);

    return 0;
}
