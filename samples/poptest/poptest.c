#include <popularity/popularity.h>

#include <stdio.h>
#include <stdlib.h>

int main(void){

    /* Testing value generated 15/11-14 kl. 19:30 - 1416076249*/

    unsigned int timestamp = 1416076249; /* This will be the value given to a song upon adding it to Midiogre(c) */
    unsigned int plays = 0; /* times the song has been played */

    double score = weigth(timestamp, plays);
    printf("%f\n",score);

    return 0;
}
