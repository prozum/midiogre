#include <mid/mid.h>
#include <mid/mid-str.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main( int argc, char *argv[] )
{
    FILE *mid_file;
    mid_t *mid;

    /* Open file */
    if((mid_file = fopen(argv[1], "rb")) == NULL) {

        perror(argv[1]);
        return -1;
    }

    /* Read mid */
    if ((mid = read_mid(mid_file)) == NULL) {

        fprintf(stderr, "Could not read midi file!\n");
        return -1;
    }

    /* Print mid data */
    print_header(mid);
    print_tracks(mid);
    print_event(mid, 0, 0);

    /* Deallocate mid */
    free_mid(mid);

    /* Close mid_file */
    fclose(mid_file);

    printf("\n");

    return 0;
}
