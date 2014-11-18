#include <popularity/popularity.h>

#include <stdio.h>
#include <stdlib.h>

int main(void){

    /* Testing value generated 16/11/2014 kl. 22:55 - 1416174915*/

    unsigned int timestamp = 1416174915; /* This will be the value given to a song upon adding it to Midiogre(c). Please make sure this number is always bigger than BASETIME */
    unsigned int plays = 0; /* times the song has been played */

    printf("weigth:\n");
    double score = weigth(timestamp, plays);
    printf("%f\n",score);

    return 0;
}
