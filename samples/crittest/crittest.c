#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mid/mid.h>
#include <mid/mid-util.h>

int main(int argc, char* argv[])
{
    FILE *mid_file;
    mid_t *mid;
    int i,j,k;

    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }
    /* Read mid */
    mid = read_mid(mid_file);

    for (i = 0; i < mid->tracks; i++){
        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* If progream change */
            if (mid->track[i].event[j].msg >= PRG_CHANGE_1 &&
                mid->track[i].event[j].msg <= PRG_CHANGE_16) {

                if (mid->track[i].events[j].para_1 >= 7) {
                    printf("piano");
                }
                else if (mid->track[i].events[j].para_1 >= 15) {
                printf("Chromatic Percussion");
                }
            }

    /* Close mid_file */
    fclose(mid_file);
    printf("\n");
    return 0;
}
