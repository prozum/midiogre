#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "analyze.h"
#include "skyline.h"

channel_t skyline(song_data_t song_data){
    unsigned int i, j, k, e, s;

    /* Creates a skyline_dynamic array to carry the notes that are not eliminated */

    channel_t *skyline_dynamic = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

    for (i = 0; i < CHANNELS; i++) {
        skyline_dynamic[i].note = malloc( sizeof( note_t ) * song_data.channels[i].notes );
        skyline_dynamic[i].notes = song_data.channels[i].notes;
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
        for(j = 0; j < skyline_dynamic[i].notes; j++) {
            k = j + 1;

            if(skyline_dynamic[i].note[j].onset == skyline_dynamic[i].note[k].onset) {
                if(skyline_dynamic[i].note[j].pitch > skyline_dynamic[i].note[k].pitch) {
                    skyline_dynamic[i].note[k].onset = skyline_dynamic[i].note[j].offset;
                }
                if (skyline_dynamic[i].note[j].pitch < skyline_dynamic[i].note[k].pitch) {
                    skyline_dynamic[i].note[j].onset = skyline_dynamic[i].note[k].offset;
                }
            }

            if(skyline_dynamic[i].note[j].offset > skyline_dynamic[i].note[k].onset) {
                if(skyline_dynamic[i].note[j].pitch > skyline_dynamic[i].note[k].pitch) {
                    skyline_dynamic[i].note[k].onset = skyline_dynamic[i].note[j].offset;
                }
                if(skyline_dynamic[i].note[j].pitch < skyline_dynamic[i].note[k].pitch) {
                    skyline_dynamic[i].note[j].offset = skyline_dynamic[i].note[k].onset;
                }
            }

            if(skyline_dynamic[i].note[j].offset > skyline_dynamic[i].note[k].offset) {
                if(skyline_dynamic[i].note[j].pitch > skyline_dynamic[i].note[k].pitch) {

                    /* Eliminate nk */
                    for(e = k; e < skyline_dynamic[i].notes; e++) {
                        skyline_dynamic[i].note[e] = skyline_dynamic[i].note[e+1];
                    }

                    channel_t *lmt = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

                    for (e = 0; e < CHANNELS; e++) {
                        if (e != i) {
                            lmt[e].notes = skyline_dynamic[i].notes;
                        } else {
                            lmt[e].notes = skyline_dynamic[i].notes - 1;
                        }
                    }

                    for (e = 0; e < CHANNELS; e++) {
                        for (s = 0; s < lmt[e].notes; s++) {
                            lmt[e].note[s].pitch = skyline_dynamic[e].note[s].pitch;
                            lmt[e].note[s].onset = skyline_dynamic[e].note[s].onset;
                            lmt[e].note[s].offset = skyline_dynamic[e].note[s].offset;
                        }
                    }

                    free(skyline_dynamic);

                    channel_t *skyline_dynamic = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

                    for (e = 0; e < CHANNELS; e++) {
                        skyline_dynamic[e].notes = lmt[e].notes;

                        for (s = 0; s < skyline_dynamic[e].notes; s++) {
                            skyline_dynamic[e].note[s].pitch = lmt[e].note[s].pitch;
                            skyline_dynamic[e].note[s].onset = lmt[e].note[s].onset;
                            skyline_dynamic[e].note[s].offset = lmt[e].note[s].offset;
                        }
                    }

                    free(lmt);

                }
                if(skyline_dynamic[i].note[j].pitch < skyline_dynamic[i].note[k].pitch) {
                    /* Split */

                    channel_t *lmt = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

                    for (e = 0; e < CHANNELS; e++) {
                        if(e != i) {
                            lmt[e].notes = skyline_dynamic[e].notes;
                        } else {
                            lmt[e].notes = skyline_dynamic[e].notes + 1;
                        }
                    }

                    for (e = 0; e < CHANNELS; e++) {
                        if ( e != i) {
                            for (s = 0; s < lmt[e].notes; s++){
                                lmt[e].note[s].pitch = skyline_dynamic[e].note[s].pitch;
                                lmt[e].note[s].onset = skyline_dynamic[e].note[s].onset;
                                lmt[e].note[s].offset = skyline_dynamic[e].note[s].offset;
                            }
                        }
                        if (e == i) {
                            for (s = 0; s < j; s++){
                                lmt[e].note[s].pitch = skyline_dynamic[e].note[s].pitch;
                                lmt[e].note[s].onset = skyline_dynamic[e].note[s].onset;
                                lmt[e].note[s].offset = skyline_dynamic[e].note[s].offset;
                            }


                        }
                    }

                    free(skyline_dynamic);

                    channel_t *skyline_dynamic = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);


                }
            }
        }
    }

    return *skyline_dynamic;
}
