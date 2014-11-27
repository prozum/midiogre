#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mid/mid.h>
#include <mid/mid-str.h>

int main(int argc, char* argv[])
{
    FILE *mid_file;
    mid_t *mid;
    unsigned int i,j;
	int s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0, s8 = 0, s9 = 0,
	    s10 = 0, s11 = 0, s12 = 0, s13 = 0, s14 = 0, s15 = 0, s16 = 0, sum = 1;
    /* Open file */
    mid_file = fopen(argv[1], "rb");
    if( mid_file == NULL ) {
        perror(argv[1]);
        return -1;
    }
    /* Read mid */
    mid = read_mid(mid_file);

    for (i = 0; i < mid->tracks; i++){
        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* If program change */
            if (mid->track[i].event[j].msg == PRG_CHANGE) {

                if (mid->track[i].event[j].para_1 <= PIANO) {
                    printf("piano - 1\n");
					s1 = 1;
                }
                else if (mid->track[i].event[j].para_1 <= CHROMATIC_PERCUSSION) {
                    printf("Chromatic Percussion - 2\n");
                    s2 = 2;
				}
                else if (mid->track[i].event[j].para_1 <= ORGAN) {
                    printf("Organ - 4\n");
					s3 = 4;
                }
                else if (mid->track[i].event[j].para_1 <= GUITAR) {
                    printf("GUITAR - 8\n");
                    s4 = 8;
				}
                else if (mid->track[i].event[j].para_1 <= BASS) {
                    printf("BASS - 16\n");
                    s5 = 16;
				}
                else if (mid->track[i].event[j].para_1 <= STRINGS) {
                    printf("strings - 32\n");
                    s6 = 32;
				}
                else if (mid->track[i].event[j].para_1 <= ENSEMBLE) {
                    printf("ensemble - 64\n");
                    s7 = 64;
				}
                else if (mid->track[i].event[j].para_1 <= BRASS) {
                    printf("BRASS - 128\n");
                    s8 = 128;
				}
                else if (mid->track[i].event[j].para_1 <= REED) {
                    printf("Reed - 256\n");
                    s9 = 256;
				}
                else if (mid->track[i].event[j].para_1 <= PIPE) {
                    printf("Pipe - 512\n");
                    s10 = 512;
				}
                else if (mid->track[i].event[j].para_1 <= SYNTH_LEAD) {
                    printf("synth lead - 1024\n");
                    s11 = 1024;
				}
                else if (mid->track[i].event[j].para_1 <= SYNTH_PAD) {
                    printf("Synth pad - 2048\n");
                    s12 = 2048;
				}
                else if (mid->track[i].event[j].para_1 <= SYNTH_EFFECT) {
                    printf("SYNTH effect - 4096\n");
                    s13 = 4096;
				}
                else if (mid->track[i].event[j].para_1 <= ETHNIC) {
                    printf("Ethnic - 8192\n");
                    s14 = 8192;
				}
                else if (mid->track[i].event[j].para_1 <= PERCUSSION) {
                    printf("Percussion - 16384\n");
                    s15 = 16384;
				}
                else if (mid->track[i].event[j].para_1 <= SOUND_EFFECTS) {
                    printf("Sound effects - 32768\n");
                    s16 = 32768;
				}
            }
        }
    }	
	sum = s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8 + s9 + s10 + s11 + s12 + s13 + s14 + s15 + s16;
	printf("The sum is: %i\n",sum);
    /* Close mid_file */	
	fclose(mid_file);
    printf("\n");
    return 0;
}
