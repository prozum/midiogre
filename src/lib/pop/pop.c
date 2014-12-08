#include "pop.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VIEWS 100
#define TIME_MODIFIER 86400 /*seconds in a day */
#define TIME_LIMIT 604800 /*seconds in a week */

unsigned int calcAge(unsigned int upload_time){
    return time(NULL) - upload_time;
}

/* The type for fingerprint may be changed based on the type given to it */
double song_score(unsigned int views, unsigned int upload_time, unsigned int min_view, unsigned int max_view, unsigned int fingerprint) {
    unsigned int age = 0, view_rating = 0, use_age = 0;
    double score = 0;

    if(views < min_view) {
        return 0;
    }

    if(max_view > MAX_VIEWS) {
        max_view = MAX_VIEWS;
    }

    age = calcAge(upload_time);

    view_rating = max_view - views;

    if(age < TIME_LIMIT) {
        use_age = 0;
    } else {
        use_age = 1;
    }

    score = (log2(view_rating) - use_age * (age/TIME_MODIFIER))/(log10(fingerprint)+1);

    return score;

}
