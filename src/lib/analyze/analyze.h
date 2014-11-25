#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define CHANNELS 16

#include <mid/mid.h>

/** Contains the information of a single note */
typedef struct note_s
{
    uint8_t pitch;
    uint8_t onset;
    uint8_t offset;
} note_t;

/* Contains information about notes on a given channel */
typedef struct channel_s
{
    note_t *notes;
    uint32_t channel_length;
} channel_t;

/** Contains song data */
typedef struct song_data_s
{
    channel_t channels[16];    /**< Contains the 16 channels */
} song_data_t;

/** Returns channels for a MIDI file */
channel_t *channel_extract(mid_t *mid);

/** Extracts the notes for channels on a track */
void note_extract(track_t track, channel_t *channels);

/** Returns the offset time for the NOTE_OFF that belongs to a given NOTE_OFF */
uint32_t note_off_time(track_t track, uint32_t event_position);

int compar_onset(const void *a, const void *b);

#endif
