#include <pop/pop.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){
    unsigned int min_p, max_p;
    int i = 0, j = 0;

    min_p = 0;
    max_p = 10000;


    printf("finger\\textbackslash plays & 0 & 1000 & 2000 & 3000 & 4000 & 5000 & 6000 & 7000 & 8000 & 9000 & 10000 \\\\ \\hline\n");
    for(i = 0; i <= 18; i++) {
        printf("%d & ", i);
        for(j = 0; j <= 10000; j += 1000) {
            printf("%.1f",song_score(j, time(NULL) - 3600, min_p, max_p, i));
            if(j != 10000) {
                printf(" & ");
            } else {
                printf(" \\\\ \\hline");
            }
        }

        printf("\n");
    }

    return 0;
}
