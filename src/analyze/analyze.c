#include "analyze.h"

song_t *song_extract(header_t *header, track_t *tracks) {

    song_t *song = malloc(sizeof( song_t ) * header->tracks);
    song->notes_all = malloc(sizeof( note_t ));

    uint32_t i;

    for (i = 0; i <= (header->tracks - 1); i++) {

        song->notes_all[i] = *note_extract(tracks, tracks[i].num, i);
    }

    return song;
}

note_t *note_extract(track_t *tracks, uint32_t elements, uint32_t num) {

    note_t *track_notes = malloc(sizeof( note_t ));
    track_notes->notes = malloc(sizeof( uint8_t ) * (elements + 1));
    track_notes->velocity = malloc(sizeof( uint8_t ) * (elements + 1));

    uint32_t i, j = 0;

    for (i = 0; i <= elements; i++) {
        if (tracks[num].events[i].type >= 144 && tracks[num].events[i].type <= 159) {

            track_notes->notes[j] = tracks[num].events[i].para_1;
            track_notes->velocity[j] = tracks[num].events[i].para_2;

            j++;
        }
    }

    track_notes->notes[j] = '\0';
    track_notes->velocity[j] = '\0';

    return track_notes;
}
