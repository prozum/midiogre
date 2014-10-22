#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi.h"


int main(void)
{
    int i;
    FILE *mid_file;
    header_t *header;
    track_t *tracks;


    // Open file
    mid_file = fopen("test.mid", "rb");
    if( mid_file == NULL ) {
        perror("test.mid");
        return -1;
    }

    /*
    uint64_t *tmp;
    tmp = ffread(mid_file,0,4);

    printf("%x\n", *tmp);
    */


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
    }

    // Deallocate bytes
    free(header);
    // Close mid_file
    fclose(mid_file);

    return 0;
}
