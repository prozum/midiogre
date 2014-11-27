#include "mid-str.h"

#include "mid.h"

#include <stdio.h>

void print_header(mid_t *mid)
{
    printf("\n<###HEAD INFO###>\n");
    printf("Format:\t\t%i\n",mid->format);
    printf("Tracks:\t\t%i\n",mid->tracks);
    printf("Division:\t%i\n",mid->division);
}

void print_tracks(mid_t *mid)
{
    int i;

    printf("\n<###TRACK INFO###>\n");
    for (i = 0; i < mid->tracks; i++) {
        printf("Track: \t%u\n",i+1);
        printf("Track bytes: \t%u\n",mid->track[i].bytes);
        printf("Track events: \t%u\n",mid->track[i].events);
    }
}

void print_event(mid_t *mid, unsigned int t, unsigned int e)
{
    int i;

    printf("\n<###EVENT INFO###>\n");
    printf("Track %2u | Event %2u\n",t,e);
    printf("Msg : %x\n",mid->track[t].event[e].msg);
    printf("Para1: %x\n",mid->track[t].event[e].para_1);
    printf("Para2: %x\n",mid->track[t].event[e].para_2);
    printf("Delta: %x\n",mid->track[t].event[e].delta);

    /* Meta message data */
    if (mid->track[t].event[e].msg == SYS_RESET) {
        printf("MData: ");
        for (i = 0; i < mid->track[t].event[e].para_2; i++ ) {
            printf("%x ",mid->track[t].event[e].data[i]);
        }
    }
    printf("\n");
}
