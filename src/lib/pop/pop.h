/** @pop.h Contains functions used in the MidiOgre Ranking Algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#include <db/db.h>

#include <stdint.h>

#define MAX_EDITDIST 10
#define MAX_PLAYS 10000

/** Gets the difference in time between upload time and current time*/
unsigned int delta_time(unsigned int upload_time);

/** Calculates the ranking score using all three parameters */
double song_score_all(unsigned int plays, unsigned int upload_time, uint8_t fingerprint);

/** Calculates the ranking score using only plays and upload time */
double song_score_play(unsigned int plays, unsigned int upload_time);

/** Compare function used when comparing two midi files for song_score_play */
int song_compare_pop(const void *p1, const void *p2);

#endif
