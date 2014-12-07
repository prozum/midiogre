#include <mid/mid.h>
#include <mid/mid-str.h>
//#include <distance/distance.h>
#include <analyze/analyze.h>
#include <analyze/skyline.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    mid_t *mid;
    int i;

    song_data_t *song_data = malloc(sizeof(song_data_t));

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    mid = read_mid(mid_file);

    /* Extract channels */
    song_data->channels = channel_extract(mid->track);

    putchar('\n');

    for (i = 0; i < 2; i++) {
        printf("%d %d %d\n", song_data->channels[0].note[i].pitch, song_data->channels[0].note[i].onset, song_data->channels[0].note[i].offset);
    }
    putchar('\n');

    /*Test skyline*/
    skyline(song_data);
    
    /* Cleanup */
    free_mid(mid);
    fclose(mid_file);

    putchar('\n');
    return 0;
}
