#include "analyze.h"
#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void extract_finger_prn(track_t *track)
{
    song_data_t *song;
    double *dist_set;
    uint8_t dist_set_len;

    int i;

    dist_set = NULL;

    song = malloc(sizeof(song_data_t));
    song->channels = channel_extract(track);
/*
    for (i = 0; i < CHANNELS; i++) {
        printf("%d\n", song->channels[i].notes);
    }
*/
    song->channels_histogram = calc_channel_histogram(song->channels);
    song->normalized_histogram = calc_normalized_histogram(song->channels_histogram, song->channels);
/*
    for (i = 0; i < 12; i++) {
        printf("%lf", song->normalized_histogram->semitones[i]);
    }
*/
    dist_set_len = calc_euclid_dist_set(song, dist_set);

    printf("%lf %lf\n", dist_set[0], dist_set[1]);
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


histogram_t *calc_channel_histogram(channel_t *channels)
{
    uint32_t i, j;
    uint32_t semitone;
    histogram_t *channel_histogram;

    channel_histogram = calloc(sizeof(histogram_t), CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        channel_histogram[i].semitones = calloc(sizeof(double), SEMITONES);
    }

    for (i = 0; i < CHANNELS; i++) {
        for (j = 0; j < channels[i].notes; j++) {
            semitone = channels[i].note[j].pitch % SEMITONES;
            channel_histogram[i].semitones[semitone]++;
        }
    }

    return channel_histogram;
}

histogram_t *calc_normalized_histogram(histogram_t *channels_histogram, channel_t *channels)
{
    uint32_t i, j;
    uint8_t nonzero_channels;
    histogram_t *normalized_histogram;

    normalized_histogram = malloc(sizeof(histogram_t));
    normalized_histogram->semitones = calloc(sizeof(double), SEMITONES);

    for (i = 0; i < CHANNELS; i++) {
        if (channels[i].notes) {
            nonzero_channels++;
        }
    }

    for (i = 0; i < SEMITONES; i++) {
        for (j = 0; j < CHANNELS; j++) {
            normalized_histogram->semitones[i] += channels_histogram[j].semitones[i];
        }
    }

    for (i = 0; i < SEMITONES; i++) {
        normalized_histogram->semitones[i] /= nonzero_channels;
        printf("%lf ", normalized_histogram->semitones[i]);
    }

    return normalized_histogram;
}

uint8_t calc_euclid_dist_set(song_data_t *song_data, double *dist_arr)
{
    uint8_t arr_len, i;
    double dist;

    dist_arr = malloc(sizeof(double) * CHANNELS);
    arr_len = 0;

    for (i = 0; i < CHANNELS; i++) {
        if (song_data->channels[i].notes) {
            dist = calc_euclid_dist(song_data->normalized_histogram->semitones, song_data->channels_histogram[i].semitones, 0);
            dist_arr[arr_len] = dist;
            arr_len++;
        }
    }

    return arr_len;
}

double calc_euclid_dist(double *normalized, double *channel, uint8_t counter)
{
    double tmp = sqrt( pow(normalized[0], 2) - pow(channel[0], 2)); 
    if (counter > SEMITONES) {
        return tmp + calc_euclid_dist(normalized + 1, channel + 1, counter + 1);
    } else {
        return tmp;
    }
}
