#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "midi/midi.h"
#include "analyze/distance.h"
#include "analyze/analyze.h"


int main( int argc, char *argv[] )
{
    uint32_t i,j,t,e;
    FILE *mid_file;
    header_t *header;
    track_t *tracks;
    song_t *song;

    // Open file
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }

    // Read midi header
    header = read_header(mid_file);
    if ( header == NULL) {
        return -1;
    }

    // Print header info
    printf("<###HEAD INFO###>\n");
    printf("Format:\t\t%i\n",header->format);
    printf("Tracks:\t\t%i\n",header->tracks);
    printf("Division:\t%i\n",header->division);

    // Read midi tracks
    tracks = read_tracks(mid_file,header->tracks);
    if ( tracks == NULL) {
        return -1;
    }

    // Print info for tracks
    printf("\n<###TRACK INFO###>\n");
    for (i = 0; header->tracks > i; i++) {
        printf("Track: \t%u\n",i+1);
        printf("Track len: \t%u\n",tracks[i].len);
        printf("Track events: \t%u\n",tracks[i].num);
    }


    // Print data for third event in first track
    t = 1;
    e = 3;
    printf("\n<###EVENT INFO###>\n");
    printf("Track %2u | Event %2u\n",t,e);
    printf("Type : %x\n",tracks[t-1].events[e-1].type);
    printf("Para1: %x\n",tracks[t-1].events[e-1].para_1);
    printf("Para2: %x\n",tracks[t-1].events[e-1].para_2);
    printf("Delta: %x\n",tracks[t-1].events[e-1].delta);

    // Meta event data
    if (tracks[t-1].events[e-1].type == META_EVENT) {
        printf("Data: ");
        for (i = 0; i < tracks[t-1].events[e-1].para_2; i++ ) {
            printf("%x ",tracks[t-1].events[e-1].data[i]);
        }
    }

    /* extracting all the nodes of the song */
    song = song_extract(header, tracks);

    /* print some different places on different tracks */
    printf("\n###NODE EXTRACTION###\n");
    printf("Track 1 | Node 2");
    printf("The note is: %i\n", song->notes_all[0].notes[1]);

    // Deallocate header
    free(header);

    // Deallocate tracks
    free_tracks(tracks,header->tracks);

    // Close mid_file
    fclose(mid_file);

    return 0;
}
