#include "analyze.h"
#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

channel_t *channel_extract(mid_t *mid)
{
    uint32_t total_events = 0;
    uint32_t i;
    channel_t *channels = calloc( sizeof( channel_t ), CHANNELS );

    for (i = 0; i < mid->tracks; i++) {
        total_events += mid->track[i].events;
    }

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
    uint32_t i, j;

    for (i = 0, j = 0; i < track.events; i++) {
        time += track.event[i].delta;

        if (track.event[i].msg >= NOTE_ON_1 && track.event[i].msg <= NOTE_ON_16) {
            uint8_t tmp = track.event[i].msg - NOTE_ON_1;
            uint32_t channel_length = channels[tmp].channel_length;

            channels[tmp].notes[channel_length].pitch = track.event[i].para_1;
            channels[tmp].notes[channel_length].onset = time;
            channels[tmp].notes[channel_length].offset = time + note_off_time(track, i, track.event[i].msg - 0x10, track.event[i].para_1, track.event[i].para_2);
            channels[tmp].channel_length++;
        }
    }
}

uint32_t note_off_time(track_t track, uint32_t event_position, uint8_t note_off, uint8_t pitch, uint8_t velocity)
{
    uint32_t time = 0;
    uint32_t i;

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
