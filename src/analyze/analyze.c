#include "analyze.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

song_t song_extract(header_t *header, track_t *tracks)
{
    song_t song;

    /* elements in tracks */
    uint32_t elements;

    /* quantity of tracks */
    uint32_t track_quant = header->tracks;
    uint32_t i;

    for (i = 0; i < track_quant; i++) {
        elements =  tracks[i].num;

        song.nodes_arr[i] = nodes_extract(tracks, elements, i);
    }

    return song;
}

char *nodes_extract(track_t *tracks, uint32_t elements, uint32_t num)
{
    char *track_nodes;
    uint32_t i;

    for (i = 0; i <= elements; i++) {
        if (tracks[num].events[num].type == (144 + num)) {
            track_nodes[i] = tracks[num].events[num].para_1;
        }
    }

    return track_nodes;
}
