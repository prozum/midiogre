/** @pop.h Contains functions used in the reddit algorithm */

#ifndef _POPULARITY_H_
#define _POPULARITY_H_

#include <stdint.h>

#define TIMECONSTANT 45000
#define BASETIME 1416174518 /*Added 22:48 16/11/2014*/

/** Alt time calculator based on the actual model used på reddit as found here: amix.dk/blog/post/19588*/
unsigned int calcAge(unsigned int upload_time);

double song_score(unsigned int plays, unsigned int upload_time, unsigned int min_play, unsigned int max_play, unsigned int fingerprint);
int compare_score(const void *s1, const void *s2);

typedef struct {
    char Artist[64];
    char Album[64];
    char Title[64];
    uint32_t TrackNum;
    uint32_t Length;
    uint32_t Plays;
    uint8_t Fingerprint[21];
    uint32_t TimeAdded;
} song_t;

#endif
