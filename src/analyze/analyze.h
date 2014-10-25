#include "../midi/midi.h"
#include "distance.h"
#include <stdint.h>
#include <stdio.h>


typedef struct song_s
{
    char *nodes_arr[16];
}song_t;

song_t song_extract(header_t *header, track_t *tracks);

char *nodes_extract(track_t *tracks, uint32_t event_count, uint32_t track_num);
