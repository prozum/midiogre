#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#include <mid/mid.h>

/** Struct that contains the information of notes on a single track */
typedef struct note_s {
    uint8_t *notes;    /**< Contains notes for a single track */
    uint8_t *velocity; /**< Contains velocity of notes for a single track */
} note_t;

/** Struct that contains an array with all the information of nodes for all tracks */
typedef struct song_s {
    note_t *notes_all;    /**< Contains all the notes for all the tracks */
    note_t *velocity_all; /**< Contains all the velocities for all notes on all tracks */
} song_t;

/** Returns all the nodes for all the tracks */
song_t *song_extract(mid_t *mid);

/** Returns all the nodes for a single track */
note_t *note_extract(track_t *track, uint32_t event_count, uint32_t track_num);

#endif
