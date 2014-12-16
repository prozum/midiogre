#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define SEMITONES 12
#define CHANNELS 16

/* semitones per finger_prn */
#define FINGER_PRN_LEN 7
#define FINGER_PRN_CMP_LEN 6

/* finger_prns per channel */
#define FINGER_PRNS 3

/* channels that are to be generated fingerprints from */
#define TOP_CHANNELS 3

#include <mid/mid.h>

/** Histogram type
 *
 */
typedef struct histogram_s
{
    double *semitones; /**< Value of semitones */
} histogram_t;

/** Fingerprint type
 *
 */
typedef struct f_prn_s
{
    uint8_t *f_prn; /**< Fingerprint array  */
    double dist;    /**< Euclidian distance */
} f_prn_t;

/** Contains the information of a single note
 *
 */
typedef struct note_s
{
    uint8_t pitch; /**< Pitch value                                          */
    double onset;  /**< Time from song start until note start in miliseconds */
    double offset; /**< Time from song start until note ends in miliseconds  */
} note_t;

/** Contains information about notes on a given channel
 *
 */
typedef struct channel_s
{
    note_t *note;   /**< Array containing notes in specific channel */
    uint32_t notes; /**< Elements in note */
    double dist;    /**< Euclidian distance */
} channel_t;

/** Returns 3 fingerprints from a track */
f_prn_t *finger_prn_gen(track_t *track);

/** Returns the elements of notes and the note array as a channel array */
channel_t *channel_extract(track_t *track);

/** Returns the note off time for a note_off */
uint32_t note_off_time(list_t *events, uint32_t position);

/** Top Rank algorithm */
void top_rank(channel_t *channels);

/** Calculates the Euclidean distance for an array of channels into the channel's dists */
void calc_euclid_chan_dist(channel_t *channels);

/** Returns fingerprints from an array of channels, does not do the prework for it */
f_prn_t *finger_prn_extract(channel_t *channels);

/** Inner function fro finger_prn_extract */
uint32_t finger_prn_extract_inner(note_t *note, uint32_t notes, f_prn_t **f_prn, uint8_t pos);

/** Returns the most frequent semitone from a channel histogram */
uint8_t freq_semitone(histogram_t *chan_histogram);

/** Calculates the Euclidean distance for an array of fingerprints */
void calc_f_prn_dist(f_prn_t *f_prn, uint32_t f_prns);

/** Returns a fingerprint histogram */
histogram_t *calc_f_prn_histogram(f_prn_t *f_prn, uint32_t f_prns);

/** Returns a normalized fingerprint histogram */
histogram_t *calc_f_prn_norm_histogram(histogram_t *f_prn_histogram, uint32_t f_prns);

/** Calculates the Euclidean distance for two doubles */
double calc_euclid_dist(double x, double y);

/** Quicksort compare function to sort fingerprints with Euclidean distances */
int compar_f_prn_dist(const void *a, const void *b);

/** Quicksort compare function to sort channels with Euclidean distances */
int compar_chan_dist(const void *a, const void *b);

/** Chooses the fingerprints to be returned from finger_prn_gen() */
f_prn_t *finger_prn_pick(f_prn_t **f_prn, uint32_t *f_prns);

/** Returns the channel histogram for a given channel */
histogram_t *calc_chan_histogram(note_t *note, uint32_t notes);

/** Returns the normalized histogram for an array of channels */
histogram_t *calc_norm_histogram(histogram_t **chan_histogram, channel_t *channels);

/** Returns the shortest Levenshtein length for two set of fingerprints */
uint8_t finger_prn_cmp(f_prn_t *f_prn1, f_prn_t *f_prn2);

/** Calculates the Levenshtein length for two fingerprints */
uint8_t lev_dist(uint8_t *f_prn1, uint8_t *f_prn2);

/** Skyline algorithm */
void skyline(channel_t *channels);

/** Quicksort compare function that puts empty notes in the back of a channel */
int skyline_compar(const void *a, const void *b);

uint8_t finger_prn_arr_cmp(const uint8_t finger_prn1[], const uint8_t finger_prn2[]);

#endif
