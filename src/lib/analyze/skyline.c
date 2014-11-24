#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "analyze.h"
#include "skyline.h"

/*testcode*/

void skyline(song_data_t song_data, mid_t *mid){
    unsigned int i, j, k;

    channel_t *result = malloc(sizeof(channel_t) * CHANNELS);
    /* result carries the note which is not eliminated */
    result->notes = NULL;

    uint32_t total_events = 0;

    for (i = 0; i < mid->tracks; i++) {
        total_events += mid->track[i].events;
    }

    for (i = 0; i < CHANNELS; i++) {
        result[i].notes = malloc( sizeof( note_t ) * total_events );
        result[i].channel_length = 0;
    }

    //printf("Notes: %d",song_data.channels->notes->onset);
    /*
    for(i = 0; i < CHANNELS; i++){
        for(j = 0; j < 16; j++){
            k = j + 1;

            while(song_data.channels[i].notes[j].onset == song_data.channels[i].notes[k].onset){

                if(song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].pitch){
                    result[i].notes[j].pitch = song_data.channels[i].notes[k].pitch;
                    j = k;
                }else{
                    result[i].notes[j].pitch = song_data.channels[i].notes[j].pitch;
                    k++;
                }

                if(song_data.channels[i].notes[j].offset > song_data.channels[i].notes[k].onset){
                    result[i].notes[j].offset = song_data.channels[i].notes[k].onset;
                }

            }

        }
    }
    */

    for(i = 0, j = 0; i < CHANNELS; i++){
        k = j + 1;

        while(song_data.channels[i].notes[j].onset == song_data.channels[i].notes[k].onset){

            if(song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].pitch){
                result[i].notes[j].pitch = song_data.channels[i].notes[k].pitch;
                j = k;
            }else{
                result[i].notes[j].pitch = song_data.channels[i].notes[j].pitch;
                k++;
            }

            if(song_data.channels[i].notes[j].offset > song_data.channels[i].notes[k].onset){
                result[i].notes[j].offset = song_data.channels[i].notes[k].onset;
            }
        }
        j = k;

    }

    printf("\nEliminate: \n");

    for(i =0; i < CHANNELS; i++){
        for(j = 0; j < 2; j++){
            printf("Pitch: %d\n", result[i].notes[j].pitch);
            printf("On Set: %d\n", result[i].notes[j].onset);
            printf("Off Set: %d\n", result[i].notes[j].offset);

        }
    }

}
