#include "midi/midi.h"
#include "analyze.h"

song_t *song_extract(mid_t *mid) {
    uint32_t i;

    song_t *song = calloc(sizeof( song_t ), mid->tracks);
    song->notes_all = malloc(sizeof( note_t ));

    for (i = 0; i < mid->tracks; i++) {

        song->notes_all[i] = *note_extract( &mid->track[i], mid->track[i].events, i);
    }

    return song;
}

note_t *note_extract(track_t *track, uint32_t elements, uint32_t num) {
    uint32_t i, j = 0;

    note_t *track_notes = malloc(sizeof( note_t ));
    track_notes->notes = malloc(sizeof( uint8_t ) * (elements + 1));
    track_notes->velocity = malloc(sizeof( uint8_t ) * (elements + 1));


    for (i = 0; i <= elements; i++) {
        if (track[num].event[i].msg >= NOTE_ON_1 && track[num].event[i].msg <= NOTE_ON_16) {

            track_notes->notes[j] = track[num].event[i].para_1;
            track_notes->velocity[j] = track[num].event[i].para_2;

            j++;
        }
    }

    track_notes->notes[j] = '\0';
    track_notes->velocity[j] = '\0';

    return track_notes;
}
