#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#include <mid/mid.h>

/* struct that contains the node on a single track */
typedef struct note_s
{
    uint8_t *notes;    /**< ADD DOXYGEN DOC HERE */
    uint8_t *velocity; /**< ADD DOXYGEN DOC HERE */
} note_t;

/* struct that contains an array with all the tracks' nodes */
typedef struct song_s
{
    note_t *notes_all;    /**< ADD DOXYGEN DOC HERE */
    note_t *velocity_all; /**< ADD DOXYGEN DOC HERE */
} song_t;

/* returns all the nodes for all the tracks */
song_t *song_extract(mid_t *mid);

/* returns all the nodes for a single track */
note_t *note_extract(track_t *track, uint32_t event_count, uint32_t track_num);

#endif
