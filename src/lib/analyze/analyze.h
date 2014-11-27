#ifndef __ANALYZE_H__
#define __ANALYZE_H__

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
    note_t *note;
    uint32_t notes;
} channel_t;

typedef struct histrogram_s
{
    double *semitones;
} histogram_t;

/** Contains song data */
typedef struct song_data_s
{
    channel_t *channels;
    histogram_t *channels_histogram;
    histogram_t *normalized_histogram;
} song_data_t;

/** Returns channels for a MIDI file */
channel_t *channel_extract(track_t *track);

/** Returns the offset time for the NOTE_OFF that belongs to a given NOTE_OFF */
int64_t note_off_time(track_t *track, uint32_t position);

histogram_t *calc_channel_histogram(channel_t *channels);

histogram_t *calc_normalized_histogram(histogram_t *channels_histogram, channel_t *channels);

double calc_euclid_dist(double *normalized, double *channel, uint8_t counter);

#endif
