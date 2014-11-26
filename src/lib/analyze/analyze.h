#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define CHANNELS 16
#define SEMITONES 12

#include <mid/mid.h>

/** Contains the information of a single note */
typedef struct note_s
{
    uint8_t pitch;
    uint8_t onset;
    uint8_t offset;
} note_t;

/** Contains information about notes on a given channel */
typedef struct channel_s
{
    note_t *notes;
    uint32_t channel_length;
} channel_t;

typedef struct histrogram_s
{
    uint8_t *histogram;
    uint32_t histogram_length;
} histogram_t;

/** Contains song data */
typedef struct song_data_s
{
    channel_t *channels;    /** Contains the 16 channels */
    histogram_t *histograms; /** Array of pitch class histograms for channels */
    histogram_t *histogram_normalized; /** The normalized histogram for all the notes */
    uint8_t nonzero_channels; /** Amount of channels that contains notes */
} song_data_t;

/** Returns channels for a MIDI file */
channel_t *channel_extract(mid_t *mid);

/** Extracts the notes for channels on a track */
void note_extract(track_t track, channel_t *channels);

/** Returns the offset time for the NOTE_OFF that belongs to a given NOTE_OFF */
uint32_t note_off_time(track_t track, uint32_t event_position);

/** Compare function for qsort to sort by onset time */
int compar_onset(const void *a, const void *b);



#endif
