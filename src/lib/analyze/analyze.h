#ifndef __ANALYZE_H__
#define __ANALYZE_H__

#define SEMITONES 12
#define CHANNELS 16

/* semitones per finger_prn */
#define FINGER_PRN_LEN 7

/* finger_prns per channel */
#define FINGER_PRNS 3

#define TOP_CHANNELS 3

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
    double dist;
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
    double dist;
} channel_t;

f_prn_t *finger_prn_gen(track_t *track);

channel_t *channel_extract(track_t *track);

uint32_t note_off_time(track_t *track, uint32_t);

void top_rank(channel_t *channels);

void calc_euclid_chan_dist(channel_t *channels);

f_prn_t *finger_prn_extract(channel_t *channels);

uint32_t finger_prn_extract_inner(note_t *note, uint32_t notes, f_prn_t **f_prn, uint8_t pos);

uint8_t freq_semitone(histogram_t *chan_histogram);

void calc_f_prn_dist(f_prn_t *f_prn, uint32_t f_prns);

histogram_t *calc_f_prn_histogram(f_prn_t *f_prn, uint32_t f_prns);

histogram_t *calc_f_prn_norm_histogram(histogram_t *f_prn_histogram, uint32_t f_prns);

double calc_euclid_dist(double x, double y);

int compar_f_prn_dist(const void *a, const void *b);

int compar_chan_dist(const void *a, const void *b);

f_prn_t *finger_prn_pick(f_prn_t **f_prn, uint32_t *f_prns);

histogram_t *calc_chan_histogram(note_t *note, uint32_t notes);

histogram_t *calc_norm_histogram(histogram_t **chan_histogram, channel_t *channels);

uint8_t finger_prn_cmp(f_prn_t *f_prn1, f_prn_t *f_prn2);

uint8_t lev_dist(uint8_t *f_prn1, uint8_t *f_prn2);

void skyline(channel_t *channels);

int skyline_compar(const void *a, const void *b);

#endif
