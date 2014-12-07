#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define SEMITONES 12
#define CHANNELS 16

/* semitones per finger_prn */
#define FINGER_PRN_LEN 7

/* finger_prns per channel */
#define FINGER_PRNS 3

#include <mid/mid.h>

/** Histogram type */
typedef struct histogram_s
{
    double *semitones;
} histogram_t;

/** Fingerprint type */
typedef struct f_prn_s
{
    uint8_t *f_prn;
    histogram_t *f_prn_histogram;
    double euclid_dist;
} f_prn_t;

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

    f_prn_t *f_prn_arr;
    uint32_t f_prns;

    histogram_t *chan_histogram;
    histogram_t *norm_f_prn_histogram;
    double euclid_dist;
} channel_t;

/** Contains song data */
typedef struct song_data_s
{
    channel_t *channels;
    histogram_t *norm_histogram;
} song_data_t;

/** Returns channels for a MIDI file */
channel_t *channel_extract(track_t *track);

/** Returns the offset time for the NOTE_OFF that belongs to a given NOTE_OFF */
uint32_t note_off_time(track_t *track, uint32_t position);

/** Calculates the histogram for a channel_t */
histogram_t *calc_chan_histogram(channel_t *chan);

/** Calculates the normalized histogram for a song_data_t */
histogram_t *calc_norm_histogram(channel_t *channels);

/** Calculates the euclidean distances for all the channels */
void calc_euclid_dist_set(song_data_t *song_data);

/** Calculates the euclidean distance for a single channel */
double calc_euclid_dist(double *normalized, double *channel);

/** Returns the fingerprints for a MIDI file with a single track */
f_prn_t *finger_prn_gen(track_t *track);

/** Extract all the possible fingerprints from a single file */
void extract_finger_prn(channel_t *channel);

/** Picks the best fingerprints from an array of fingerprints */
f_prn_t *finger_prn_pick(song_data_t *song);

/** Calculates the histogram for a single fingerprint */
histogram_t *calc_f_prn_histogram(uint8_t *f_prn);

/** Calculates the normalized histogram for an array of fingerprints */
histogram_t *calc_f_prn_norm(channel_t *chan);

/** Calculates all the euclidean distances for fingerprints on a given channel */
void calc_f_prn_dist_set(channel_t *chan);

/** Calculates the euclidean distance from a single fingerprint */
double calc_f_prn_dist(double *normalized, double *f_prn);

/** Qsort compare function that sorts a channel_t array by euclidean lengths */
int dist_compar_chan(const void *a, const void *b);

/** Qsort compare function that sorts a f_prn_t array by euclidean lengths */
int dist_compar_f_prn(const void *a, const void *b);

/** Returns the Levensthein distance for two set of fingerprints */
uint8_t finger_prn_cmp(f_prn_t *f_prn1, f_prn_t *f_prn2);

/** Returns the Levenshtein distance for two fingerprints */
uint8_t lev_dist(uint8_t *f_prn1, uint8_t *f_prn2);

/** Skyline algorithm */
void skyline(song_data_t *song);

/** Quicksort algorithm to sort the eliminated notes out after the Skyline algorithm */
int skyline_compar(const void *a, const void *b);

#endif
