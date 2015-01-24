/** @pop.h Contains functions used in the MidiOgre Ranking Algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#include <db/db.h>

#include <stdint.h>

#define MAX_EDITDIST 10
#define MAX_PLAYS 10000

void song_score_fprnt(song_t *song, song_t *song_fav);

/** Calculates the ranking score using all three parameters
 * @param plays: The number of plays for the song
 * @param upload_time: The time which the file was uploaded to db in unix time
 * @param edit_dist: the edit distance for the song
 * @return
 */
void song_score_best(song_t *song, song_t *song_fav);

/** Calculates the ranking score using only plays and upload time
 * @param plays: The number of plays for the song
 * @param upload_time: The time which the file was uploaded to db in unix time
 * @return
 */
void song_score_pop(song_t *song);

/** Qsort function for songs
 * @param song1: Pointer to song 1
 * @param song2: Pointer to song 2
 * @return
 */
int sort_score(const void *song1, const void *song2, const void *f);

/**
 * @brief edit_dist_best
 * @param s
 * @param f
 * @return
 */
uint8_t edit_dist_best(song_t *s, song_t *f);

#endif
