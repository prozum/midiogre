#include <pop/pop.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){
    unsigned int timestamp,plays, min_p, max_p, fingerprint = 0;
    double score;
    int i = 0, j = 0, f = 0;
    /* Testing value generated 16/11/2014 kl. 22:55 - 1416174915*/

    timestamp = time(NULL); /* This will be the value given to a song upon adding it to Midiogre(c).
                                             Please make sure this number is always bigger than BASETIME */
    plays = 5000; /* times the song has been played */

    min_p = 0;

    max_p = 10000;


    printf("finger\\plays & 0 & 100 & 200 & 300 & 400 & 500 & 600 & 700 & 800s & 900 & 1000 \\\\ \\hline\n");
    for(i = 0; i <= 10; i++) {
        printf("%d & ", i);
        for(j = 0; j <= 1000; j += 100) {
            printf("%.1f",song_score(j, timestamp - 60, min_p, max_p, i));
            if(j != 1000) {
                printf(" & ");
            } else {
                printf(" \\\\ \\hline");
            }
        }

        printf("\n");
    }

    return 0;
}
