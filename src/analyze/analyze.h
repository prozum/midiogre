#include "../midi/midi.h"
#include "distance.h"
#include <stdint.h>
#include <stdio.h>

/* struct that contains the node on a single track */
typedef struct node_s
{
    uint8_t *nodes;
}node_t;

/* struct that contains an array with all the tracks' nodes */
typedef struct song_s
{
    node_t *nodes_all;
}song_t;

/* returns all the nodes for all the tracks */
song_t *song_extract(header_t *header, track_t *tracks);

/* returns all the nodes for a single track */
node_t *node_extract(track_t *tracks, uint32_t event_count, uint32_t track_num);
