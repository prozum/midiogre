#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "analyze.h"
#include "skyline.h"

channel_t skyline(song_data_t song_data){
    unsigned int i, j, k;

    /* Creates a skyline_dynamic array to carry the notes that are not eliminated */

    channel_t *skyline_dynamic = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

    skyline_dynamic->note = NULL;

    for (i = 0; i < CHANNELS; i++) {
        skyline_dynamic[i].note = malloc( sizeof( note_t ) * song_data.channels[i].notes );
        skyline_dynamic[i].notes = 0;
    }

    /* Copies data from song_data to skyline_dynamic */

    for(i = 0; i < CHANNELS; i++){
        for(j = 0; j < song_data.channels[i].notes; j++){
            skyline_dynamic[i].note[j].pitch = song_data.channels[i].note[j].pitch;
            skyline_dynamic[i].note[j].onset = song_data.channels[i].note[j].onset;
            skyline_dynamic[i].note[j].offset = song_data.channels[i].note[j].offset;
        }
    }

    for(i = 0; i < CHANNELS; i++) {
        for(j = 0; j < song_data.channels[i].notes; j++) {
            k = j + 1;

            if(song_data.channels[i].note[j].onset == song_data.channels[i].note[k].onset) {
                if(song_data.channels[i].note[j].pitch > song_data.channels[i].note[k].pitch) {
                    song_data.channels[i].note[k].onset = song_data.channels[i].note[j].offset;
                }
                if (song_data.channels[i].note[j].pitch < song_data.channels[i].note[k].pitch) {
                    song_data.channels[i].note[j].onset = song_data.channels[i].note[k].offset;
                }
            }

            if(song_data.channels[i].note[j].offset > song_data.channels[i].note[k].onset) {
                if(song_data.channels[i].note[j].pitch > song_data.channels[i].note[k].pitch) {
                    song_data.channels[i].note[k].onset = song_data.channels[i].note[j].offset;
                }
                if(song_data.channels[i].note[j].pitch < song_data.channels[i].note[k].pitch) {
                    song_data.channels[i].note[j].offset = song_data.channels[i].note[k].onset;
                }
            }

            if(song_data.channels[i].note[j].offset > song_data.channels[i].note[k].offset) {
                if(song_data.channels[i].note[j].pitch > song_data.channels[i].note[k].pitch) {
                    /* Eliminate nk */
                }
                if(song_data.channels[i].note[j].pitch < song_data.channels[i].note[k].pitch) {
                    /* Split */
                }
            }
        }
    }

    return *skyline_dynamic;
}
