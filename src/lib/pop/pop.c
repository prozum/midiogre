#include "pop.h"

#include <analyze/analyze.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#define MAX_PLAYS 10000
#define MAX_FINGERPPRINT 18

unsigned int calcAge(unsigned int upload_time)
{
    return time(NULL) - upload_time;
}

/* The type for fingerprint may be changed based on the type given to it */
double song_score_all(unsigned int plays, unsigned int upload_time, uint8_t fingerprint)
{
    unsigned int age = 0, play_rating = 0;
    double score = 0;

    age = calcAge(upload_time);

    /* Returns a score of 0 if the song does not meet the required conditions */
    if(fingerprint > MAX_FINGERPPRINT) {
        return 0;
    } else if(fingerprint == 0) {
        return 0;
    } else if(plays > MAX_PLAYS) {
        return 0;
    } else if(age == 0) {
        return 0;
    }

    play_rating = MAX_PLAYS - plays;

    score = ((play_rating)*((18-fingerprint)/log2(fingerprint+1))/log2((double)age))/100;

    return score;

}

double song_score_play(unsigned int plays, unsigned int upload_time)
{
    unsigned int age = 0, play_rating = 0;
    double score = 0;

    age = calcAge(upload_time);

    /* Returns a score of 0 if the song does not meet the required conditions */
    if(plays > MAX_PLAYS) {
        return 0;
    } else if(age == 0) {
        return 0;
    }

    play_rating = MAX_PLAYS - plays;

    score = ((play_rating)/log2((double)age))/100;

    return score;

}

int song_compare_all(const void *s1, const void *s2)
{
    const song_t *song1 = s1;
    const song_t *song2 = s2;

    double song1_score = 0, song2_score = 0;

    song1_score = song_score_all(song1->plays,song1->time_added, finger_prn_arr_cmp(song1->fingerprint, song1->fingerprint));
    song2_score = song_score_all(song2->plays,song2->time_added, finger_prn_arr_cmp(song2->fingerprint, song2->fingerprint));

    if(song1_score > song2_score) {
        return 1;
    } else if(song2_score < song2_score) {
        return -1;
    } else {
        return 0;
    }
}

int song_compare_play(const void *s1, const void *s2) {
    const song_t *song1 = s1;
    const song_t *song2 = s2;

    double song1_score = 0, song2_score = 0;

    song1_score = song_score_play(song1->plays,song1->time_added);
    song2_score = song_score_play(song2->plays,song2->time_added);

    if(song1_score > song2_score) {
        return 1;
    } else if(song2_score < song2_score) {
        return -1;
    } else {
        return 0;
    }
}

int song_compare_fingerprint(const void *s1, const void *s2) {
    const song_t *song1 = s1;
    const song_t *song2 = s2;

    if(song1->fingerprint > song2->fingerprint) {
        return 1;
    } else if(song1->fingerprint < song2->fingerprint) {
        return -1;
    } else {
        return 0;
    }
}
