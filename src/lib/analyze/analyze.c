#include "analyze.h"

#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

f_prn_t *finger_prn_gen(track_t *track)
{
    song_data_t *song;
    uint32_t i, j;
    f_prn_t *f_prn_arr;

    song = malloc(sizeof(song_data_t));
    song->channels = channel_extract(track);
    skyline(song);

    for (i = 0; i < CHANNELS; i++) {
        song->channels[i].chan_histogram = calc_chan_histogram(&(song->channels[i]));
    }

    song->norm_histogram = calc_norm_histogram(song->channels);
    calc_euclid_dist_set(song);
    qsort(song->channels, CHANNELS, sizeof(channel_t), dist_compar_chan);

    for (i = 0; i < CHANNELS; i++) {
        if (song->channels[i].notes) {
            extract_finger_prn(&(song->channels[i]));
        }

        for (j = 0; j < song->channels[i].f_prns; j++) {
            song->channels[i].f_prn_arr[j].f_prn_histogram = calc_f_prn_histogram(song->channels[i].f_prn_arr[j].f_prn);
        }

        song->channels[i].norm_f_prn_histogram = calc_f_prn_norm(&(song->channels[i]));
        calc_f_prn_dist_set(&(song->channels[i]));

        if (song->channels[i].f_prns) {
            qsort(song->channels[i].f_prn_arr, song->channels[i].f_prns, sizeof(f_prn_t), dist_compar_f_prn);
        }
    }

    f_prn_arr = malloc(sizeof(f_prn_t) * 3);
    f_prn_arr->f_prn = malloc(sizeof(uint8_t) * 7);
    memcpy((void *)f_prn_arr, (void *)finger_prn_pick(song), sizeof(f_prn_t) * FINGER_PRNS);
    free(song);

    return f_prn_arr;
}

void extract_finger_prn(channel_t *chan)
{
    uint8_t freq_tone;
    uint32_t i, j;
    uint8_t k;

    freq_tone = 0;

    for (i = 0; i < SEMITONES; i++) {
        if (chan->chan_histogram->semitones[freq_tone] < chan->chan_histogram->semitones[i]) {
            freq_tone = i;
        }
    }

    chan->f_prn_arr = malloc(sizeof(f_prn_t) * chan->chan_histogram->semitones[freq_tone]);
    chan->f_prns = 0;

    for (j = 0; j < chan->chan_histogram->semitones[freq_tone]; j++) {
        chan->f_prn_arr[j].f_prn = calloc(sizeof(uint8_t), FINGER_PRN_LEN);
    }

    for (i = 0; i < chan->notes; i++) {
        if (chan->note[i].pitch % SEMITONES == freq_tone && i >= 3 && i <= chan->notes - 3) {
            for (j = i - 3, k = 0; j <= i + 3; j++) {
                chan->f_prn_arr[chan->f_prns].f_prn[k] = chan->note[j].pitch % SEMITONES;
                k++;
            }

            chan->f_prns++;
        }
    }
}

int dist_compar_f_prn(const void *a, const void *b)
{
    const f_prn_t *f_prn1 = a;
    const f_prn_t *f_prn2 = b;

    if (f_prn1->euclid_dist > f_prn2->euclid_dist) {
        return -1;
    } else if (f_prn1->euclid_dist < f_prn2->euclid_dist) {
        return 1;
    } else {
        return 0;
    }
}

f_prn_t *finger_prn_pick(song_data_t *song)
{
    uint8_t i;
    uint8_t f_prns;
    f_prn_t *f_prn_arr;

    f_prn_arr = malloc(sizeof(f_prn_t) * FINGER_PRNS);
    f_prn_arr->f_prn = malloc(sizeof(uint8_t) * FINGER_PRN_LEN);

    f_prns = 0;
    i = 0;

    while (f_prns < 3) {
        if (song->channels[i].f_prns) {
            f_prn_arr[f_prns].f_prn = song->channels[i].f_prn_arr[f_prns].f_prn;
            song->channels[i].f_prn_arr++;
            f_prns++;
        } else if (i == 3) {
            i = 0;
        } else {
            i++;
        }
    }

    return f_prn_arr;
}

histogram_t *calc_f_prn_histogram(uint8_t *f_prn)
{
    uint8_t i;
    histogram_t *f_prn_histogram;

    f_prn_histogram = malloc(sizeof(histogram_t));
    f_prn_histogram->semitones = calloc(sizeof(double), SEMITONES);

    for (i = 0; i < FINGER_PRN_LEN; i++) {
        f_prn_histogram->semitones[f_prn[i]]++;
    }

    return f_prn_histogram;
}

histogram_t *calc_f_prn_norm(channel_t *chan)
{
    uint8_t i;
    uint32_t j;
    histogram_t *f_prn_norm;

    f_prn_norm = malloc(sizeof(histogram_t));
    f_prn_norm->semitones = calloc(sizeof(double), SEMITONES);

    for (i = 0; i < SEMITONES; i++) {
        for (j = 0; j < chan->f_prns; j++) {
            f_prn_norm->semitones[i] += chan->f_prn_arr[j].f_prn[i];
        }

        f_prn_norm->semitones[i] /= chan->f_prns;
    }

    return f_prn_norm;
}

void calc_f_prn_dist_set(channel_t *chan)
{
    uint32_t i;
    double *f_prn, *normalized;

    for (i = 0; i < chan->f_prns; i++) {
        normalized = chan->norm_f_prn_histogram->semitones;
        f_prn = chan->f_prn_arr[i].f_prn_histogram->semitones;

        chan->f_prn_arr[i].euclid_dist = calc_f_prn_dist(normalized, f_prn);
    }
}

double calc_f_prn_dist(double *normalized, double *f_prn)
{
    double dist;
    uint8_t i;

    dist = 0;

    for (i = 0; i < SEMITONES; i++) {
        dist += sqrt(pow(normalized[i] - f_prn[i], 2));
    }

    return dist;
}

channel_t *channel_extract(track_t *track)
{
    uint32_t events, position;
    uint32_t start_time;
    uint32_t i;
    uint8_t channel;
    channel_t *channels;

    events = track->events;
    start_time = 0;
    channels = calloc(sizeof(channel_t), CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        channels[i].note = calloc(sizeof(note_t), events);
        channels[i].notes = 0;
    }

    for (i = 0; i < events; i++) {
        start_time += track->event[i].delta;

        if (track->event[i].msg == NOTE_ON) {
            channel = track->event[i].chan;
            position = channels[channel].notes;

            channels[channel].note[position].pitch = track->event[i].byte_1;
            channels[channel].note[position].onset = start_time;
            channels[channel].note[position].offset = start_time + note_off_time(track, position);
            channels[channel].notes++;
        }
    }

    return channels;
}

uint32_t note_off_time(track_t *track, uint32_t position)
{
    uint32_t i, time, event;
    uint32_t pitch, note_off;
    uint8_t msg, channel;

    time = 0;
    event = position;
    pitch = track->event[position].byte_1;
    note_off = track->event[position].chan + NOTE_ON - CHANNELS;

    for (i = 1; i < (track->events - position); i++) {
        event++;
        time += track->event[event].delta;
        msg = track->event[event].msg;
        channel = track->event[event].chan;

        if ((msg + channel) == note_off && track->event[event].byte_1 == pitch) {
            return time;
        }
    }

    return time;
}


histogram_t *calc_chan_histogram(channel_t *chan)
{
    uint32_t i;
    uint32_t semitone;
    histogram_t *chan_histogram;

    chan_histogram = malloc(sizeof(histogram_t));
    chan_histogram->semitones = calloc(sizeof(double), SEMITONES);

    for (i = 0; i < chan->notes; i++) {
        semitone = chan->note[i].pitch % SEMITONES;
        chan_histogram->semitones[semitone]++;
    }

    return chan_histogram;
}

histogram_t *calc_norm_histogram(channel_t *channels)
{
    uint32_t i, j;
    uint8_t nonzero_channels = 0;
    histogram_t *norm_histogram;

    norm_histogram = malloc(sizeof(histogram_t));
    norm_histogram->semitones = calloc(sizeof(double), SEMITONES);

    for (i = 0; i < CHANNELS; i++) {
        if (channels[i].notes) {
            nonzero_channels++;
        }
    }

    for (i = 0; i < SEMITONES; i++) {
        for (j = 0; j < CHANNELS; j++) {
            norm_histogram->semitones[i] += channels[j].chan_histogram->semitones[i];
        }

        norm_histogram->semitones[i] /= nonzero_channels;
    }

    return norm_histogram;
}

void calc_euclid_dist_set(song_data_t *song_data)
{
    uint8_t i;
    double dist;
    double *normalized, *channel;

    for (i = 0; i < CHANNELS; i++) {
        if (song_data->channels[i].notes) {
            normalized = song_data->norm_histogram->semitones;
            channel = song_data->channels[i].chan_histogram->semitones;

            dist = calc_euclid_dist(normalized, channel);
            song_data->channels[i].euclid_dist = dist;
        }
    }
}

double calc_euclid_dist(double *normalized, double *channel)
{
    uint8_t i;
    double dist;

    dist = 0;

    for (i = 0; i < SEMITONES; i++) {
        dist += sqrt( pow(normalized[i] - channel[i], 2));
    }

    return dist;
}

int dist_compar_chan(const void *a, const void *b)
{
    const channel_t *channel1 = a;
    const channel_t *channel2 = b;

    if (channel1->notes && channel2->notes) {
        return channel1->euclid_dist - channel2->euclid_dist;
    } else if (channel1->notes && !channel2->notes) {
        return -1;
    } else if (!channel1->notes && channel2->notes) {
        return 1;
    } else {
        return 0;
    }

    return 0;
}

uint8_t finger_prn_cmp(f_prn_t *f_prn1, f_prn_t *f_prn2)
{
    uint8_t *dist;
    uint8_t ret_dist;
    uint8_t i;

    dist = malloc(sizeof(uint8_t) * FINGER_PRNS);

    dist[0] = lev_dist(f_prn1[0].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[1].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[2].f_prn, f_prn2[2].f_prn);

    dist[1] = lev_dist(f_prn1[2].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[0].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[1].f_prn, f_prn2[2].f_prn);

    dist[2] = lev_dist(f_prn1[1].f_prn, f_prn2[0].f_prn) +
              lev_dist(f_prn1[2].f_prn, f_prn2[1].f_prn) +
              lev_dist(f_prn1[0].f_prn, f_prn2[2].f_prn);


    for (i = 1; i < FINGER_PRNS; i++) {
        if (dist[0] > dist[i]) {
            dist[0] = dist[i];
        }
    }

    ret_dist = dist[0];
    free(dist);

    return ret_dist;
}

uint8_t lev_dist(uint8_t *f_prn1, uint8_t *f_prn2)
{
    uint8_t dist, i;

    dist = 0;

    for (i = 0; i < FINGER_PRN_LEN; i++) {
        if (f_prn1[i] > f_prn2[i] || f_prn1[i] < f_prn2[i]) {
            dist++;
        }
    }

    return dist;
}

void skyline(song_data_t *song)
{
    uint8_t i;
    uint32_t j;
    uint32_t elim;
    note_t *tmp_note;

    for (i = 0; i < CHANNELS; i++) {
        elim = 0;

        for (j = 0; j < song->channels[i].notes; j++) {
            tmp_note = song->channels[i].note;

            if (tmp_note[j].onset == tmp_note[j+1].onset) {
                if (tmp_note[j].pitch > tmp_note[j+1].pitch) {
                    tmp_note[j+1].onset = tmp_note[j].offset;

                } else if (tmp_note[j].pitch < tmp_note[j+1].pitch) {
                    tmp_note[j].onset = tmp_note[j+1].offset;
                }

            } else if (tmp_note[j].offset > tmp_note[j+1].onset) {
                if (tmp_note[j].pitch > tmp_note[j+1].pitch) {
                    tmp_note[j+1].onset = tmp_note[j].offset;

                } else if (tmp_note[j].pitch < tmp_note[j+1].pitch) {
                    tmp_note[j].offset = tmp_note[j+1].onset;
                }

            } else if (tmp_note[j].offset > tmp_note[j+1].offset) {
                /* 2^8 - 1 = 255, the highest possible number in 8 bits */
                tmp_note[j+1].pitch = pow(2, 8) - 1;
                elim++;
                j++;
            }
        }

        qsort((void *)song->channels[i].note, song->channels[i].notes, sizeof(note_t), skyline_compar);
        song->channels[i].notes -= elim;
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
