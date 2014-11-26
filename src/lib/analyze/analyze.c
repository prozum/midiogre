#include "analyze.h"
#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

channel_t *channel_extract(mid_t *mid)
{
    uint32_t total_events = 0;
    uint32_t i;
    channel_t *channels = calloc(sizeof(channel_t), CHANNELS);

    for (i = 0; i < mid->tracks; i++) {
        total_events += mid->track[i].events;
    }

    /* allocate enough memory for worst case scenario */
    for (i = 0; i < CHANNELS; i++) {
        channels[i].notes = malloc( sizeof( note_t ) * total_events );
        channels[i].channel_length = 0;
    }

    for (i = 0; i < mid->tracks; i++) {
        note_extract(mid->track[i], channels);
    }

    for (i = 0; i < mid->tracks; i++) {
        qsort(channels[i].notes, channels[i].channel_length, sizeof(note_t), compar_onset);
    }

    return channels;
}

void note_extract(track_t track, channel_t *channels)
{
    uint32_t time = 0;
    uint32_t i;

    for (i = 0; i < track.events; i++) {
        time += track.event[i].delta;

        if (track.event[i].msg >= NOTE_ON_1 && track.event[i].msg <= NOTE_ON_16) {
            uint8_t tmp = track.event[i].msg - NOTE_ON_1;
            uint32_t channel_length = channels[tmp].channel_length;

            channels[tmp].notes[channel_length].pitch = track.event[i].para_1;
            channels[tmp].notes[channel_length].onset = time;
            channels[tmp].notes[channel_length].offset = time + note_off_time(track, i);
            channels[tmp].channel_length++;
        }
    }
}

uint32_t note_off_time(track_t track, uint32_t event_position)
{
    uint32_t time = 0;
    uint32_t i;

    uint8_t note_off = track.event[event_position].msg - 0x10;
    uint8_t pitch = track.event[event_position].para_1;
    uint8_t velocity = track.event[event_position].para_1;

    for (i = 1; i < (track.events - event_position - 1); i++) {
        uint8_t tmp = event_position + i;

        if (track.event[tmp].msg == note_off && track.event[tmp].para_1 == pitch && track.event[tmp].para_2 == velocity) {
            time += track.event[tmp].delta;
            return time;
        } else {
            time += track.event[tmp].delta;
        }
    }
}

int compar_onset(const void *a, const void *b)
{
    const note_t *note1 = a;
    const note_t *note2 = b;

    return note1->onset - note2->onset;
}

/*
channel_t *skyline_extract(channel_t *channels)
{
    channel_t *skyline = calloc( sizeof( channel_t), CHANNELS );

    for (i = 0; i < CHANNELS; i++) {
        skyline[i].notes = malloc( sizeof( note_t ) * channels[i].channel_length );
        skyline[i].channel_length = channels[i].channel_length
    }
}
*/

histogram_t *calc_histogram(channel_t *channels)
{
    uint8_t i;
    uint32_t j;

    histogram_t *histogram_set = malloc(sizeof(histogram_t) * CHANNELS);

    /* allocate memory for histogram_set elements */
    for (i = 0; i < CHANNELS; i++) {
        histogram_set[i].histogram_length = 0;
        if (channels[i].channel_length) {
            histogram_set[i].histogram_length = SEMITONES;
            histogram_set[i].histogram = calloc(sizeof(uint8_t), SEMITONES);
        }
    }

    /* calculating histogram elements */
    for (i = 0; i < CHANNELS; i++) {
        if (channels[i].channel_length) {
            for (j = 0; j < channels[i].channel_length; j++) {
                uint8_t pitch = channels[i].notes[j].pitch % SEMITONES;
                histogram_set[i].histogram[pitch] += 1;
            }
        }
    }

    return histogram_set;
}

histogram_t *calc_norm_histogram(histogram_t *histogram_set)
{
    histogram_t *histogram_norm = malloc(sizeof(histogram_t));
    uint8_t i;
    uint32_t j;

    histogram_norm->histogram_length = SEMITONES;
    histogram_norm->histogram = calloc(sizeof(uint8_t), CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        for (j = 0; j < histogram_length) {
            uint8_t elements = histogram_set[i].histogram[j];
            histogram_norm->histogram_set[i] += elements;
        }
    }

    return histogram_norm;
} 
