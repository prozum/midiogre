#include <mid/mid.h>
#include <mid/mid-util.h>
#include <distance/distance.h>
#include <analyze/analyze.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    song_data_t song_data;


    
    /* Extracting all the nodes of the song */
    //song = song_extract(mid);
    
    /* Print some different places on different tracks */
   // printf("\n###NODE EXTRACTION###\n");
    //printf("Track 1\n");

    //printf("%s\n\n", song->notes_all[1].notes);

    //printf("%s\n", song->notes_all[2].notes);

    /* Deallocate mid */
    free_mid(mid);
    
    /* Close mid_file */
    fclose(mid_file);
    printf("\n");
    return 0;
}
