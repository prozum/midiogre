#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "analyze.h"
#include "skyline.h"

/*testcode*/

channel_t skyline(song_data_t song_data){
    unsigned int i, j, k;

    /* Creates a result array to carry the notes that are not eliminated */

    channel_t *result = (channel_t *)malloc(sizeof(channel_t) * CHANNELS);

    result->notes = NULL;

    for (i = 0; i < CHANNELS; i++) {
        result[i].notes = malloc( sizeof( note_t ) * song_data.channels[i].channel_length );
        result[i].channel_length = 0;
    }

    /* Copies data from song_data to result */

    for(i = 0; i < CHANNELS; i++){
        for(j = 0; j < song_data.channels[i].channel_length; j++){
            result[i].notes[j].pitch = song_data.channels[i].notes[j].pitch;
            result[i].notes[j].onset = song_data.channels[i].notes[j].onset;
            result[i].notes[j].offset = song_data.channels[i].notes[j].offset;
        }
    }

    /* Skyline algorithm */

    for(i = 0, j = 0; i < CHANNELS; i++){
        k = j + 1;

        while(song_data.channels[i].notes[j].onset == song_data.channels[i].notes[k].onset && j < song_data.channels[i].channel_length){
            if(song_data.channels[i].notes[j].pitch < song_data.channels[i].notes[k].onset){
                result[i].notes[j].pitch = song_data.channels[i].notes[k].pitch;
                /* song_data.channels[i].notes[j].pitch = 0; */
                j = k;
            }else{
                result[i].notes[k].pitch = song_data.channels[i].notes[j].pitch;
                /* song_data.channels[i].notes[k].pitch = 0; */
                k++;
            }

            if(song_data.channels[i].notes[j].offset > song_data.channels[i].notes[j].offset){
                result[i].notes[j].offset = song_data.channels[i].notes[k].onset;
            }
        }

        j = k;
    }

    /* Compare results with the original file */

    for(i = 0; i < CHANNELS; i++){
        for(j = 0; j < result[i].channel_length; j++){
            if(song_data.channels[i].notes[j].pitch != result[i].notes[j].pitch){
                printf("Channel %d note %d pitch is different! Pitch is %d vs %d\n", i, j, song_data.channels[i].notes[j].pitch, result[i].notes[j].pitch);
            }

            if(song_data.channels[i].notes[j].onset != result[i].notes[j].onset){
                printf("Channel %d note %d onset is different! Onset is %d vs %d\n", i, j, song_data.channels[i].notes[j].onset, result[i].notes[j].onset);
            }
        }
        if(song_data.channels[i].notes[j].offset != result[i].notes[j].offset){
            printf("Channel %d note %d offset is different! Offset is %d vs %d\n", i, j, song_data.channels[i].notes[j].offset, result[i].notes[j].offset);
        }
    }

    return *result;
}
