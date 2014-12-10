#include <pop/pop.h>

#include <stdio.h>
#include <stdlib.h>

int main(void){
    unsigned int timestamp,plays, min_p, max_p, fingerprint = 0;
    double score;
    
    /* Testing value generated 16/11/2014 kl. 22:55 - 1416174915*/

    timestamp = 1416174915; /* This will be the value given to a song upon adding it to Midiogre(c). 
                                             Please make sure this number is always bigger than BASETIME */
    plays = 5000; /* times the song has been played */

    min_p = 0;

    max_p = 10000;

    for(fingerprint = 1; fingerprint <= 21; fingerprint++) {
        if(score > 0) {
            score = song_score(plays, timestamp, min_p, max_p, fingerprint);
            printf("%f\n",score);
        }
    }

    return 0;
}