/** @pop.h Contains functions used in the MidiOgre Ranking Algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#include <db/db.h>

#include <stdint.h>

#define MAX_EDITDIST 10
#define MAX_PLAYS 10000

/** Gets the difference in time between upload time and current time
 * @param upload_time: The time which the file was uploaded to db in unix time.
 * @return
 */
unsigned int delta_time(unsigned int upload_time);

/** Calculates the ranking score using all three parameters
 * @param plays: The number of plays for the song
 * @param upload_time: The time which the file was uploaded to db in unix time
 * @param edit_dist: the edit distance for the song
 * @return
 */
double song_score_all(unsigned int plays, unsigned int upload_time, uint8_t edit_dist);

/** Calculates the ranking score using only plays and upload time
 * @param plays: The number of plays for the song
 * @param upload_time: The time which the file was uploaded to db in unix time
 * @return
 */
double song_score_play(unsigned int plays, unsigned int upload_time);

/** Compare function used when comparing two midi files for song_score_play
 * @param p1: Pointer to song 1
 * @param p2: Pointer to song 2
 * @return
 */
int song_compare_pop(const void *p1, const void *p2);

#endif
