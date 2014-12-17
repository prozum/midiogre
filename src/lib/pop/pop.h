/** @pop.h Contains functions used in the reddit algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#include <db/db.h>

#include <stdint.h>

#define MAX_EDITDIST 10
#define MAX_PLAYS 10000
#define TIMECONSTANT 45000
#define BASETIME 1416174518 /*Added 22:48 16/11/2014*/

/** Alt time calculator based on the actual model used på reddit as found here: amix.dk/blog/post/19588*/
unsigned int delta_time(unsigned int upload_time);

double song_score_all(unsigned int plays, unsigned int upload_time, uint8_t fingerprint);
int song_compare_all(const void *s1, const void *s2);

double song_score_play(unsigned int plays, unsigned int upload_time);
int song_compare_pop(const void *p1, const void *p2);

int song_compare_fingerprint(const void *p1, const void *p2);

int compare_song_score(song_t *song_original, song_t *song1, song_t *song2);
#endif
