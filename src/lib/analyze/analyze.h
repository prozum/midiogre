#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define CHANNELS 16

#include <mid/mid.h>

/** Struct that contains the information of notes on a single track */
typedef struct note_s
{
    uint8_t pitch;    /**< Contains tones for a single track */
    uint8_t onset;
    uint8_t offset;
} note_t;

typedef struct channel_s
{
    note_t *notes;
} channel_t;

/** Struct that contains an array with all the information of nodes for all tracks */
typedef struct song_data_s
{
    channel_t *channels;    /**< Contains all the notes for all the tracks */
} song_data_t;

channel_t *channel_extract(mid_t *mid);

void note_extract(mid_t *mid, channel_t *channels, uint32_t events);

uint32_t find_note_off(track_t track, uint32_t event_position, uint8_t note_off);

/** Returns all the nodes for all the tracks */
//song_t *song_extract(mid_t *mid);

/** Returns all the nodes for a single track */
//note_t note_extract(track_t *track, uint32_t event_count, uint32_t track_num);

#endif
