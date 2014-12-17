#include <mid/mid.h>
#include <mid/mid-str.h>
#include <analyze/analyze.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main( int argc, char *argv[] )
{
    FILE *mid_file1, *mid_file2;
    mid_t *mid1, *tmp_mid1;
    mid_t *mid2, *tmp_mid2;
    f_prn_t *finger_prints1, *finger_prints2;
    uint8_t i, j;

    /* Open file */
    if((mid_file1 = fopen(argv[1], "rb")) == NULL ) {
        perror(argv[1]);
        return -1;
    }


    if ((mid_file2 = fopen(argv[2], "rb")) == NULL) {
        perror(argv[2]);
        return -1;
    }

    /* Read mid */
    tmp_mid1 = read_mid(mid_file1);
    tmp_mid2 = read_mid(mid_file2);

    fclose(mid_file1);
    fclose(mid_file2);

    mid1 = merge_tracks(tmp_mid1);
    mid2 = merge_tracks(tmp_mid2);

    /* Extract channels */
    finger_prints1 = finger_prn_gen(mid1->tracks->ptr);
    finger_prints2 = finger_prn_gen(mid2->tracks->ptr);

    i = finger_prn_cmp(finger_prints1, finger_prints2);

    printf("TEST RESULTS:\n%d\nFingerprints for first argument\n\n", i);

    for (i = 0; i < FINGER_PRNS; i++) {
        for (j = 0; j < FINGER_PRN_LEN; j++) {
            printf("%d ", finger_prints2[i].f_prn[j]);
        }

        printf("\n");
    }

    putchar('\n');

    putchar('\n');

    /*Test skyline*/
    
    /* Cleanup */



    for (i = 0; i < FINGER_PRNS; i++) {
        free(finger_prints1[i].f_prn);
        free(finger_prints2[i].f_prn);
    }

    free(finger_prints1);
    free(finger_prints2);
    free_mid(tmp_mid1);
    free_mid(tmp_mid2);
    free_mid(mid1);
    free_mid(mid2);

    putchar('\n');
    return 0;
}
