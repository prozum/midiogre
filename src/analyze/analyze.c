#include "analyze.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

song_t *song_extract(header_t *header, track_t *tracks)
{
    song_t *song = malloc(sizeof( song_t ) * header->tracks);
    song->nodes_all = malloc(sizeof ( node_t ));
    uint32_t i;

    for (i = 0; i <= (header->tracks - 1); i++) {
        song->nodes_all[i] = *node_extract(tracks, tracks[i].num, i);
    }

    return song;
}

node_t *node_extract(track_t *tracks, uint32_t elements, uint32_t num)
{
    node_t *track_nodes = malloc(sizeof( node_t ));
    track_nodes->nodes = malloc(sizeof ( uint8_t ) * elements);
    uint32_t i;

    for (i = 0; i <= elements; i++) {
        if (tracks[num].events[i].type == (144 + num)) {
            track_nodes->nodes[i] = tracks[num].events[i].para_1;
        }
    }

    return track_nodes;
}
