#include "analyze.h"

#include <mid/mid.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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

            track_notes.notes[j] = track[num].event[i].para_1;
            track_notes.velocity[j] = track[num].event[i].para_2;

            j++;
        }
    }

    track_notes.notes[j] = '\0';
    track_notes.velocity[j] = '\0';

    return track_notes;
}
