#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi/midi.h"
#include "midi/util.h"
#include "analyze/distance.h"
#include "analyze/analyze.h"

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    mid_t *mid;
    song_t *song;

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    mid = read_mid(mid_file);

    print_header(mid);
    print_tracks(mid);
    print_event(mid,1,1);

    /* Extracting all the nodes of the song */
    //song = song_extract(header, tracks);

    /* Print some different places on different tracks */
    //printf("\n###NODE EXTRACTION###\n");
    //printf("Track 1 | Node 0\n");
    //printf("The note is: %i\n", song->notes_all[0].notes[0]);


    /* Deallocate mid */
    free_mid(mid);

    /* Close mid_file */
    fclose(mid_file);

    printf("\n");

    return 0;
}
