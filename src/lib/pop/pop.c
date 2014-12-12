#include "pop.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_PLAYS 10000
#define MAX_FINGERPPRINT 10

unsigned int calcAge(unsigned int upload_time){
    return time(NULL) - upload_time;
}

/* The type for fingerprint may be changed based on the type given to it */
double song_score(unsigned int plays, unsigned int upload_time, unsigned int min_play, unsigned int max_play, unsigned int fingerprint) {
    unsigned int age = 0, play_rating = 0;
    double score = 0;

    if(max_play > MAX_PLAYS) {
        max_play = MAX_PLAYS;
    }

    /* Returns a score of 0 if the song does not meet the required conditions */
    if(plays < min_play) {
        return 0;
    } else if(fingerprint > MAX_FINGERPPRINT) {
        return 0;
    } else if(fingerprint == 0) {
        return 0;
    } else if(plays > MAX_PLAYS) {
        return 0;
    }

    age = calcAge(upload_time);

    play_rating = max_play - plays;

    score = ((play_rating)*((21-fingerprint)/log2(fingerprint+1))/log2((double)age))/100;

    return score;

}

int compare_song(const void *s1, const void *s2) {

    const song_t *song1 = s1;
    const song_t *song2 = s2;

    double song1_score = 0, song2_score = 0;

    song1_score = song_score(song1->Plays,song1->TimeAdded,0,MAX_PLAYS,0); /* Argument 5 needed */
    song2_score = song_score(song2->Plays,song2->TimeAdded,0,MAX_PLAYS,0); /* Argument 5 needed */

    if(song1_score > song2_score) {
        return 1;
    } else if(song2_score < song2_score) {
        return -1;
    } else {
        return 0;
    }
}
