#include "pop.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VIEWS 10000
#define MAX_FINGERPPRINT 10

unsigned int calcAge(unsigned int upload_time){
    return time(NULL) - upload_time;
}

/* The type for fingerprint may be changed based on the type given to it */
double song_score(unsigned int views, unsigned int upload_time, unsigned int min_view, unsigned int max_view, unsigned int fingerprint) {
    unsigned int age = 0, view_rating = 0;
    double score = 0;

    if(max_view > MAX_VIEWS) {
        max_view = MAX_VIEWS;
    }

    /* Returns a score of 0 if the song does not meet the required conditions */
    if(views < min_view) {
        return 0;
    } else if(fingerprint > MAX_FINGERPPRINT) {
        return 0;
    } else if(fingerprint == 0) {
        return 0;
    } else if(views > MAX_VIEWS) {
        return 0;
    }

    age = calcAge(upload_time);

    view_rating = max_view - views;

    score = ((view_rating)*((21-fingerprint)/log2(fingerprint+1))/log2((double)age))/100;

    return score;

}
