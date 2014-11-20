#include "analyze.h"
#include <mid/mid.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

channel_t *channel_extract(mid_t *mid)
{
    channel_t *channels = malloc( sizeof( channel_t ) * CHANNELS );
    channels->notes = NULL;

    uint32_t total_events = 0;
    uint32_t i;

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
            channels[tmp].notes[channel_length].offset = time + note_off_time(track, i, track.event[i].msg - 0x10);
            channels[tmp].channel_length++;
        }
    }
}

uint32_t note_off_time(track_t track, uint32_t event_position, uint8_t note_off)
{
    uint32_t time = 0;
    uint32_t i;

    for (i = 1; i < (track.events - event_position - 1); i++) {
        uint8_t tmp = event_position + i;

        if (track.event[tmp].msg == note_off) {
            time += track.event[tmp].delta;
            return time;
        } else {
            time += track.event[tmp].delta;
        }
    }
}
