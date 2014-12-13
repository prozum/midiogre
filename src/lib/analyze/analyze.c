#include "analyze.h"

#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

f_prn_t *finger_prn_gen(track_t *track)
{
    uint8_t i, j;

    channel_t *channels;
    f_prn_t *f_prn_tmp;
    f_prn_t *finger_prns;

    /* extract channels and notes */
    channels = channel_extract(track);
    /* run skyline algorithm on the channels */
    skyline(channels);
    /* rank the channels */
    top_rank(channels);

    /* find the fingerprints */
    f_prn_tmp = finger_prn_extract(channels);

    finger_prns = malloc(3 * sizeof(f_prn_t));

    for (i = 0; i < FINGER_PRNS; i++) {
        finger_prns[i].f_prn = calloc(FINGER_PRN_CMP_LEN, sizeof(int16_t));
        for (j = 0; j < FINGER_PRN_CMP_LEN; j++) {
            finger_prns[i].f_prn[j] = abs(f_prn_tmp[i].f_prn[j+1] - f_prn_tmp[i].f_prn[j]);
        }
        free(f_prn_tmp[i].f_prn);
    }

    free(f_prn_tmp);

    for (i = 0; i < CHANNELS; i++) {
        free(channels[i].note);
    }

    free(channels);

    return finger_prns;
}

channel_t *channel_extract(track_t *track)
{
    uint32_t position;
    uint32_t i;
    event_t *event;
    uint8_t channel;
    channel_t *channels;

    channels = calloc(sizeof(channel_t), CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        channels[i].note = calloc(sizeof(note_t), track->events->n);
        channels[i].notes = 0;
        channels[i].dist = 0;
    }

    /* extract notes into channels */
    list_set(track->events, 0, 0, LIST_BEG);
    while ((event = list_next(track->events)) != NULL) {

        if (event->msg == NOTE_ON) {
            channel = event->chan;
            position = channels[channel].notes;

            channels[channel].note[position].pitch = event->byte_1;
            channels[channel].note[position].onset = event->time;
            channels[channel].note[position].offset = note_off_time(track->events, position);
            channels[channel].notes++;
        }
    }

    for (i = 0; i < CHANNELS; i++) {
        if (!channels[i].notes) {
            free(channels[i].note);
            channels[i].note = NULL;
        }
    }

    return channels;
}

void top_rank(channel_t *channels)
{
    calc_euclid_chan_dist(channels);
    qsort((void *)channels, CHANNELS, sizeof(channel_t), compar_chan_dist);
}

void calc_euclid_chan_dist(channel_t *channels)
{
    histogram_t **chan_histogram;
    histogram_t *norm_histogram;
    uint8_t i, j;

    chan_histogram = malloc(sizeof(histogram_t*) * CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        chan_histogram[i] = calc_chan_histogram(channels[i].note, channels[i].notes);
    }

    norm_histogram = calc_norm_histogram(chan_histogram, channels);

    /* calculates the Euclidean distance for all the channels */
    for (i = 0; i < CHANNELS; i++) {
        if (channels[i].notes) {

            for (j = 0; j < SEMITONES; j++) {
                channels[i].dist += calc_euclid_dist(norm_histogram->semitones[j], chan_histogram[i]->semitones[j]);
            }
        }
    }


    for (i = 0; i < CHANNELS; i++) {
        free(chan_histogram[i]->semitones);
        free(chan_histogram[i]);
    }

    free(chan_histogram);
    free(norm_histogram->semitones);
    free(norm_histogram);

}

int compar_chan_dist(const void *a, const void *b)
{
    const channel_t *chan1 = a;
    const channel_t *chan2 = b;

    if (!chan1->note && chan2->note) {
        return 0;
    } else if (chan1->notes && !chan2->notes) {
        return -1;
    } else if (chan2->notes && !chan1->notes) {
        return 1;
    } else if (chan1->dist > chan2->dist) {
        return -1;
    } else if (chan2->dist > chan1->dist) {
        return 1;
    }

    return 0;
}

f_prn_t *finger_prn_extract(channel_t *channels)
{
    f_prn_t **f_prn;
    f_prn_t *ret_f_prn;
    uint32_t *f_prns;
    uint32_t j;
    uint8_t i;

    f_prn = malloc(sizeof(f_prn_t *) * TOP_CHANNELS);
    f_prns = calloc(TOP_CHANNELS, sizeof(uint32_t));

    /* find how many fingerprints for each top channel and extract the fingerprints */
    for (i = 0; i < TOP_CHANNELS; i++) {
        if (channels[i].notes) {
            f_prns[i] = finger_prn_extract_inner(channels[i].note, channels[i].notes, f_prn, i);
        } else {
            f_prns[i] = 0;
        }
    }

    /* if there are more than 3 fingerprints, pick them, else return NULL */
    if (f_prns[0] + f_prns[1] + f_prns[2] >= FINGER_PRNS) {
        ret_f_prn = finger_prn_pick(f_prn, f_prns);
    } else {
        ret_f_prn = NULL;
    }

    for (i = 0; i < TOP_CHANNELS; i++) {
        for (j = 0; j < f_prns[i]; j++) {
            free(f_prn[i][j].f_prn);
        }

        if (f_prns[i]) {
            free(f_prn[i]);
        }
    }

    free(f_prn);
    free(f_prns);

    return ret_f_prn;
}


uint32_t finger_prn_extract_inner(note_t *note, uint32_t notes, f_prn_t **f_prn, uint8_t pos)
{
    histogram_t *chan;
    uint32_t i;
    uint32_t f_prns;
    uint8_t semitone;
    uint8_t j;

    chan = calc_chan_histogram(note, notes);
    semitone = freq_semitone(chan);

    f_prns = 0;
    f_prn[pos] = malloc(sizeof(f_prn_t) * chan->semitones[semitone]);

    for (i = 0; i < chan->semitones[semitone]; i++) {
        f_prn[pos][i].f_prn = calloc(FINGER_PRN_LEN, sizeof(int16_t));
    }

    /* extract the fingerprints */ 
    for (i = 0; i < notes; i++) {
        if (note[i].pitch % SEMITONES == semitone && i >= 3 && i + 3 <= notes) {
            for (j = 0 ; j < FINGER_PRN_LEN; j++ ) {
                f_prn[pos][f_prns].f_prn[j] = note[i-3].pitch % SEMITONES;
                i++;
            }
            f_prns++;
        }
    }

    /* calculate the fingerprint distances */
    calc_f_prn_dist(f_prn[pos], f_prns);
    /* sort according to the fingerprint distances */
    qsort((void *)f_prn[pos], f_prns, sizeof(f_prn_t), compar_f_prn_dist);

    /* some of the alloced space might not be used so add the space alloced to the f_prns */
    f_prns += (chan->semitones[semitone] - f_prns);

    free(chan->semitones);
    free(chan);

    return f_prns;
}

uint8_t freq_semitone(histogram_t *chan_histogram)
{
    uint8_t i;
    uint8_t semitone;

    semitone = 0;

    for (i = 1; i < SEMITONES; i++) {
        if (chan_histogram->semitones[i] > chan_histogram->semitones[semitone]) {
            semitone = i;

        }

    }

    return semitone;
}

void calc_f_prn_dist(f_prn_t *f_prn, uint32_t f_prns)
{
    histogram_t *f_prn_histogram;
    histogram_t *f_prn_norm_histogram;
    uint32_t i;
    uint8_t j;

    /* calculates the Euclidean distance for a set of fingerprints */
    if (f_prns) {
        f_prn_histogram = calc_f_prn_histogram(f_prn, f_prns);
        f_prn_norm_histogram = calc_f_prn_norm_histogram(f_prn_histogram, f_prns);

        for (i = 0; i < f_prns; i++) {
            f_prn[i].dist = 0;
            for (j = 0; j < SEMITONES; j++) {
                f_prn[i].dist += calc_euclid_dist(f_prn_norm_histogram->semitones[j], f_prn_histogram[i].semitones[j]);
            }
        }

        for (i = 0; i < f_prns; i++) {
            free(f_prn_histogram[i].semitones);
        }

        free(f_prn_histogram);
        free(f_prn_norm_histogram->semitones);
        free(f_prn_norm_histogram);
    } else {
        f_prn = NULL;
    }
}

histogram_t *calc_f_prn_histogram(f_prn_t *f_prn, uint32_t f_prns)
{
    histogram_t *f_prn_histogram;
    uint32_t i;
    uint8_t j;
    uint8_t semitone;

    f_prn_histogram = malloc(sizeof(histogram_t) * f_prns);

    for (i = 0; i < f_prns; i++) {
        f_prn_histogram[i].semitones = calloc(SEMITONES, sizeof(double));
    }

    /* calculate fingerprint histogram */
    for (i = 0; i < f_prns; i++) {
        for (j = 0; j < FINGER_PRN_LEN; j++) {
            semitone = f_prn[i].f_prn[j] % SEMITONES;
            f_prn_histogram[i].semitones[semitone] = f_prn[i].f_prn[j];
        }
    }

    return f_prn_histogram;
}

histogram_t *calc_f_prn_norm_histogram(histogram_t *f_prn_histogram, uint32_t f_prns)
{
    histogram_t *f_prn_norm_histogram;
    uint8_t i;
    uint32_t j;

    f_prn_norm_histogram = malloc(sizeof(histogram_t));
    f_prn_norm_histogram->semitones = calloc(SEMITONES, sizeof(double));

    /* calculate normalized histogram for a set of fingerprints */
    if (f_prns) {
        for (i = 0; i < SEMITONES; i++) {
            for (j = 0; j < f_prns; j++) {
                f_prn_norm_histogram->semitones[i] += f_prn_histogram[j].semitones[i];
            }

            f_prn_norm_histogram->semitones[i] /= f_prns;
        }
    } else {
        free(f_prn_norm_histogram->semitones);
        free(f_prn_norm_histogram);
        f_prn_norm_histogram = NULL;
    }

    return f_prn_norm_histogram;
}

double calc_euclid_dist(double x, double y)
{
    return sqrt(pow(x - y, 2));
}

int compar_f_prn_dist(const void *a, const void *b)
{
    const f_prn_t *f_prn1 = a;
    const f_prn_t *f_prn2 = b;

    if (f_prn1->dist > f_prn2->dist) {
        return 1;
    } else if (f_prn1->dist < f_prn2->dist) {
        return -1;
    }

    return 0;
}

f_prn_t *finger_prn_pick(f_prn_t **f_prn, uint32_t *f_prns)
{
    uint32_t i, j, k;
    f_prn_t *f_prn_ret;

    f_prn_ret = malloc(sizeof(f_prn_t) * FINGER_PRNS);

    for (i = 0; i < FINGER_PRNS; i++) {
        f_prn_ret[i].f_prn = calloc(FINGER_PRN_LEN, sizeof(int16_t));
    }

    i = 0;
    j = 0;
    k = 0;

    /* picking fingerprints */
    while (k < 3) {
        if (f_prns[i]) {
            memcpy((void *)f_prn_ret[k].f_prn, (void *)f_prn[i][j].f_prn, sizeof(int16_t) * FINGER_PRN_LEN);
            k++;
            i++;
        } else if (i == 3){
            j++;
            i = 0;
        } else {
            i++;
        }
    }

    return f_prn_ret;
}

uint32_t note_off_time(list_t *events, uint32_t position)
{
    uint32_t i;
    event_t *event, *event_cur;

    event_cur = list_index(events, position);

    /* find the time before the note_off event activates */
    for (i = position; i < events->n; i++) {

        event = list_index(events, i);

        if (event->msg    == NOTE_OFF        &&
            event->chan   == event_cur->chan &&
            event->byte_1 == event_cur->byte_1) {

            return event->time;
        }
    }

    return event->time;
}


histogram_t *calc_chan_histogram(note_t *note, uint32_t notes)
{
    uint32_t i;
    uint32_t semitone;
    histogram_t *chan_histogram;

    chan_histogram = malloc(sizeof(histogram_t));
    chan_histogram->semitones = calloc(SEMITONES, sizeof(double));

    /* calculate channel histogram */
    if (notes) { 
        for (i = 0; i < notes; i++) {
            semitone = note[i].pitch % SEMITONES;
            chan_histogram->semitones[semitone]++;
        }
    }

    return chan_histogram;
}


histogram_t *calc_norm_histogram(histogram_t **chan_histogram, channel_t *channels)
{
    uint32_t i, j;
    uint8_t nonzero_channels = 0;
    histogram_t *norm_histogram;

    norm_histogram = malloc(sizeof(histogram_t));
    norm_histogram->semitones = calloc(sizeof(double), SEMITONES);

    /* count nonzero channels */
    for (i = 0; i < CHANNELS; i++) {
        if (channels[i].notes) {
            nonzero_channels++;
        }
    }

    /* calculate the normalized histogram */
    for (i = 0; i < SEMITONES; i++) {
        for (j = 0; j < CHANNELS; j++) {
            norm_histogram->semitones[i] += chan_histogram[j]->semitones[i];
        }

        /* divide by nonzero channels */
        norm_histogram->semitones[i] /= nonzero_channels;
    }

    return norm_histogram;
}


uint8_t finger_prn_cmp(f_prn_t *f_prn1, f_prn_t *f_prn2)
{
    uint8_t *dist;
    uint8_t ret_dist;
    uint8_t i;

    dist = malloc(sizeof(uint8_t) * FINGER_PRNS);

    /* calculate the distance for all possible combinations */
    dist[0] = lev_dist(f_prn1[0].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[1].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[2].f_prn, f_prn2[2].f_prn);

    dist[1] = lev_dist(f_prn1[2].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[0].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[1].f_prn, f_prn2[2].f_prn);

    dist[2] = lev_dist(f_prn1[1].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[2].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[0].f_prn, f_prn2[2].f_prn);


    /* choose the shortest distance */
    for (i = 1; i < FINGER_PRNS; i++) {
        if (dist[0] > dist[i]) {
            dist[0] = dist[i];
        }
    }

    ret_dist = dist[0];
    free(dist);

    return ret_dist;
}

uint8_t lev_dist(int16_t *f_prn1, int16_t *f_prn2)
{
    uint8_t dist, i;

    dist = 0;

    for (i = 0; i < FINGER_PRN_CMP_LEN; i++) {
        if (f_prn1[i] > f_prn2[i] || f_prn1[i] < f_prn2[i]) {
            dist++;
        }
    }

    return dist;
}

void skyline(channel_t *channels)
{
    uint8_t i;
    uint32_t j;
    uint32_t elim;
    note_t *tmp_note;

    for (i = 0; i < CHANNELS; i++) {
        elim = 0;

        for (j = 0; j < channels[i].notes; j++) {
            tmp_note = channels[i].note;

            if (tmp_note[j].offset >= tmp_note[j+1].offset) {
                if (tmp_note[j].pitch > tmp_note[j+1].pitch) {
                    tmp_note[j+1].pitch = 255;
                    elim++;
                    j++;
                } else {
                    tmp_note[j].offset = tmp_note[j+1].onset;
                }
            } else if (tmp_note[j].offset > tmp_note[j+1].onset) {
                if (tmp_note[j].pitch > tmp_note[j+1].pitch) {
                    tmp_note[j+1].onset = tmp_note[j].offset;
                } else {
                    tmp_note[j].offset = tmp_note[j+1].onset;
                }

            }
        }

        qsort((void *)channels[i].note, channels[i].notes, sizeof(note_t), skyline_compar);
        channels[i].notes -= elim;
    }
}

int skyline_compar(const void *a, const void *b)
{
    const note_t *note1 = a;
    const note_t *note2 = b;

    if (note1->pitch == 255) {
        return 1;
    } else if (note2->pitch == 255) {
        return -1;
    } 

    return note1->onset - note2->onset;
}

uint8_t finger_prn_arr_cmp(const uint8_t finger_prn1[18], const uint8_t finger_prn2[18])
{
    uint8_t i, j;
    uint8_t dist;

    f_prn_t *f_prn1;
    f_prn_t *f_prn2;

    f_prn1 = malloc(sizeof(f_prn_t) * FINGER_PRNS);
    f_prn2 = malloc(sizeof(f_prn_t) * FINGER_PRNS);

    for (i = 0; i < FINGER_PRNS; i+=FINGER_PRN_CMP_LEN) {
        f_prn1->f_prn = calloc(FINGER_PRN_CMP_LEN, sizeof(uint8_t));
        f_prn2->f_prn = calloc(FINGER_PRN_CMP_LEN, sizeof(uint8_t));

        for (j = 0; j < FINGER_PRN_CMP_LEN; j++) {
            f_prn1->f_prn[j + i] = finger_prn1[j + i];
            f_prn2->f_prn[j + i] = finger_prn2[j + i];
        }
    }

    dist = finger_prn_cmp(f_prn1, f_prn2);

    for (i = 0; i < FINGER_PRNS; i++) {
        for (j = 0; j < FINGER_PRN_CMP_LEN; j++) {
            free(f_prn1[i].f_prn);
            free(f_prn2[i].f_prn);
        }
    }

    free(f_prn1);
    free(f_prn2);

    return dist;
}
