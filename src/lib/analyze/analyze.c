#include "analyze.h"
#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

channel_t *channel_extract(track_t *track)
{
    uint32_t events, position;
    uint32_t start_time; int64_t d_time;
    uint32_t i;
    uint8_t channel;
    channel_t *channels;

    events = track->events;
    start_time = 0;
    d_time = 0;
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

            channels[channel].note[position].pitch = track->event[i].para_1;
            channels[channel].note[position].onset = start_time;
            d_time = note_off_time(track, position);

            if (d_time == -1) {
                exit(-1);
            }

            channels[channel].note[position].offset = start_time + d_time;
        }
    }

    return channels;
}

int64_t note_off_time(track_t *track, uint32_t position)
{
    uint32_t i, time, event;
    uint32_t pitch, note_off;
    uint8_t msg, channel;

    time = 0;
    event = position;
    pitch = track->event[position].para_1;
    note_off = track->event[position].channel + NOTE_ON - CHANNELS;

    for (i = 1; i < (track->events - position); i++) {
        event++;
        time += track->event[event].delta;
        msg = track->event[event].msg;
        channel = track->event[event].chan;

        if ((msg + channel) == note_off && track->event[event].para_1 == pitch) {
            return time;
        }
    }

    return -1;
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
            normalized_histogram->semitones[i] = (channels_histogram[j].semitones[i] / nonzero_channels);
        }
    }

    return normalized_histogram;
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
