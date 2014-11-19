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
    }

    for (i = 0; i < mid->tracks; i++) {
        note_extract(mid->track[i], channels);
    }
}

uint32_t note_extract(track_t track, channel_t *channels)
{
    uint32_t time = 0;
    uint32_t i, j;

    for (i = 0, j = 0; i < track.events; i++) {
        time += track.event[i].delta;
        switch (track.event[i].msg) {
            case NOTE_ON_1:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_1 - 0x10);
                j++;
                break;
            case NOTE_ON_2:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_2 - 0x10);
                j++;
                break;
            case NOTE_ON_3:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_3 - 0x10);
                j++;
                break;
            case NOTE_ON_4:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_4 - 0x10);
                j++;
                break;
            case NOTE_ON_5:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_5 - 0x10);
                j++;
                break;
            case NOTE_ON_6:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_6 - 0x10);
                j++;
                break;
            case NOTE_ON_7:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_7 - 0x10);
                j++;
                break;
            case NOTE_ON_8:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_8 - 0x10);
                j++;
                break;
            case NOTE_ON_9:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_9 - 0x10);
                j++;
                break;
            case NOTE_ON_10:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_10 - 0x10);
                j++;
                break;
            case NOTE_ON_11:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_11 - 0x10);
                j++;
                break;
            case NOTE_ON_12:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_12 - 0x10);
                j++;
                break;
            case NOTE_ON_13:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_13 - 0x10);
                j++;
                break;
            case NOTE_ON_14:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_14 - 0x10);
                j++;
                break;
            case NOTE_ON_15:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_15 - 0x10);
                j++;
                break;
            case NOTE_ON_16:
                channels[j][0] = track.events[i].para_1;
                channels[j][1] = time;
                channels[j][2] = time + find_note_off(track, i, NOTE_ON_16 - 0x10);
                j++;
                break;
        }
    }
}

uint32_t find_note_off(track_t track, uint32_t event_position, uint8_t note_off)
{
    uint32_t time = 0;
    uint32_t i;

    for (i = 1; i < (track.events - event_position - 1); i++) {
        if (track.event[event_position + i].msg == note_off) {
            time += track.event[event_position + i].delta;
            return time;
        } else {
            time += track.event[event_position + i].delta;
        }
    }
}

/*
song_t *song_extract(mid_t *mid) {
    uint32_t i;

    song_t *song = calloc(sizeof( song_t ), mid->tracks);
    song->notes_all = calloc(sizeof( note_t ), mid->tracks);
    song->velocity_all = calloc(sizeof( note_t ), mid->tracks);

    for (i = 0; i < mid->tracks; i++) {
        song->notes_all[i] = note_extract( mid->track, mid->track[i].events, i);
    }

    return song;
}

note_t note_extract(track_t *track, uint32_t elements, uint32_t num) {
    uint32_t i, j = 0;

    note_t track_notes;
    track_notes.notes = calloc(sizeof( uint8_t ), (elements + 1));
    track_notes.velocity = calloc(sizeof( uint8_t ), (elements + 1));

    for (i = 0; i <= elements; i++) {
        if (track[num].event[i].msg >= NOTE_ON_1 && track[num].event[i].msg <= NOTE_ON_16) {

            track_notes.tone[j] = track[num].event[i].para_1;
            track_notes.velocity[j] = track[num].event[i].para_2;

            j++;
        }
    }

    track_notes.tone[j] = '\0';
    track_notes.velocity[j] = '\0';

    return track_notes;
}
*/
