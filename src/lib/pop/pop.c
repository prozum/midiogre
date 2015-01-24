#include "pop.h"

#include <analyze/analyze.h>

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

void song_score_fprnt(song_t *song, song_t *song_fav)
{
    song->sort_score = finger_prn_cmp(song->fprns, song_fav->fprns);
}

void song_score_best(song_t *song, song_t *song_fav)
{
    unsigned int age, play_rating, edit_dist;

    edit_dist = finger_prn_cmp(song->fprns, song_fav->fprns);

    /* Set sort score to 0 if the song does not meet the required conditions */
    if (edit_dist > MAX_EDITDIST || song->plays > MAX_PLAYS) {
        song->sort_score = 0;
        return;
    }

    if ((age = time(NULL) - song->time_added) == 0) {
        age = 2;
    }

    play_rating = MAX_PLAYS - song->plays;

    song->sort_score = (play_rating * ((18 - edit_dist) / log2(edit_dist + 1)) / log2((double)age)) / 100;
}

void song_score_pop(song_t *song)
{
    unsigned int age, play_rating;

    age = time(NULL) - song->time_added;

    /* Returns a score of 0 if the song does not meet the required conditions */
    if(song->plays > MAX_PLAYS || age == 0) {
        song->sort_score = 0;
        return;
    }

    play_rating = MAX_PLAYS - song->plays;

    song->sort_score = (play_rating / log2((double)age)) / 100;
}

int sort_score(const void *song1, const void *song2, const void *f)
{
    if (((song_t*)song1)->sort_score > ((song_t*)song2)->sort_score) {
        return -1;
    } else if(((song_t*)song1)->sort_score < ((song_t*)song2)->sort_score) {
        return 1;
    } else {
        return 0;
    }

}
