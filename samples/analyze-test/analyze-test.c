#include <mid/mid.h>
#include <mid/mid-str.h>
#include <analyze/analyze.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    mid_t *mid;
    f_prn_t *finger_prints;

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    mid = read_mid(mid_file);

    /* Extract channels */
    finger_prints = finger_prn_gen(mid->track);

    putchar('\n');

    putchar('\n');

    /*Test skyline*/
    
    /* Cleanup */
    free(finger_prints);
    free_mid(mid);
    fclose(mid_file);

    putchar('\n');
    return 0;
}
