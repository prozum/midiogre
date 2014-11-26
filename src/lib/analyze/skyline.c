#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "analyze.h"
#include "skyline.h"

channel_t skyline(song_data_t song_data){
    unsigned int i, j, k;

    /* Creates a skyline_dynamic array to carry the notes that are not eliminated */

    channel_t *skyline_dynamic = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

    skyline_dynamic->notes = NULL;

    for (i = 0; i < CHANNELS; i++) {
        skyline_dynamic[i].notes = malloc( sizeof( note_t ) * song_data.channels[i].channel_length );
        skyline_dynamic[i].channel_length = 0;
    }

    /* Copies data from song_data to skyline_dynamic */

    for(i = 0; i < CHANNELS; i++){
        for(j = 0; j < song_data.channels[i].channel_length; j++){
            skyline_dynamic[i].notes[j].pitch = song_data.channels[i].notes[j].pitch;
            skyline_dynamic[i].notes[j].onset = song_data.channels[i].notes[j].onset;
            skyline_dynamic[i].notes[j].offset = song_data.channels[i].notes[j].offset;
        }
    }

    for(i = 0; i < CHANNELS; i++) {
        for(j = 0; j < song_data.channels[i].channel_length; j++) {
            k = j + 1;

            if(song_data.channels[i].notes[j].onset == song_data.channels[i].notes[k].onset) {
                if(song_data.channels[i].notes[j].pitch > song_data.channels[i].notes[k].pitch) {
                    song_data.channels[i].notes[k].onset = song_data.channels[i].notes[j].offset;
                }
                if (song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].pitch) {
                    song_data.channels[i].notes[j].onset = song_data.channels[i].notes[k].offset;
                }
            }

            if(song_data.channels[i].notes[j].offset > song_data.channels[i].notes[k].onset) {
                if(song_data.channels[i].notes[j].pitch > song_data.channels[i].notes[k].pitch) {
                    song_data.channels[i].notes[k].onset = song_data.channels[i].notes[j].offset;
                }
                if(song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].pitch) {
                    song_data.channels[i].notes[j].offset = song_data.channels[i].notes[k].onset;
                }
            }

            if(song_data.channels[i].notes[j].offset > song_data.channels[i].notes[k].offset) {
                if(song_data.channels[i].notes[j].pitch > song_data.channels[i].notes[k].pitch) {
                    /* Eliminate nk */
                }
                if(song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].pitch) {
                    /* Split */
                }
            }
        }
    }

    return *skyline_dynamic;
}
