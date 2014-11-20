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
    song_data_t song_data;

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    mid = read_mid(mid_file);
 
    /* Read mid */

    song_data.channels = channel_extract(mid);

    free_mid(mid);
    
    /* Close mid_file */
    fclose(mid_file);
    printf("\n");
    return 0;
}
