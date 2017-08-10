#include <pop/pop.h>
#include <db/db.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int i;
    song_t song;

    printf("finger\\textbackslash plays & 0 & 1000 & 2000 & 3000 & 4000 & 5000 & 6000 & 7000 & 8000 & 9000 & 10000 \\\\ \\hline\n");
    for(i = 0; i <= 10000; i += 1000) {

        song.plays = i;
        song.time_added = time(NULL) - 3600;
        song_score_pop(&song);

        printf("%.1f", song.sort_score);

        if(i != 10000) {

            printf(" & ");

        } else {

            printf(" \\\\ \\hline");
        }

        printf("\n");
    }

    return 0;
}
