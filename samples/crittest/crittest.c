#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mid/mid.h>
#include <mid/mid-str.h>

int main(int argc, char* argv[])
{
    FILE *mid_file;
    mid_t *mid;
    unsigned int i,j,instr_class,bitmap;

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

                /* Find instrument class */
                instr_class = mid->track[i].event[j].byte_1 - (mid->track[i].event[j].byte_1 % INSTR_CLASSES);

                switch (instr_class) {
                    case PIANO:
                        bitmap |= 0x1;
                        break;
                    case CHROMATIC_PERCUSSION:
                        bitmap |= 0x2;
                        break;
                    case ORGAN:
                        bitmap |= 0x4;
                        break;
                    case GUITAR:
                        bitmap |= 0x8;
                        break;
                    case BASS:
                        bitmap |= 0x10;
                        break;
                    case STRINGS:
                        bitmap |= 0x20;
                        break;
                    case ENSEMBLE:
                        bitmap |= 0x40;
                        break;
                    case BRASS:
                        bitmap |= 0x80;
                        break;
                    case REED:
                        bitmap |= 0x100;
                        break;
                    case PIPE:
                        bitmap |= 0x200;
                        break;
                    case SYNTH_LEAD:
                        bitmap |= 0x400;
                        break;
                    case SYNTH_PAD:
                        bitmap |= 0x800;
                        break;
                    case SYNTH_EFFECT:
                        bitmap |= 0x1000;
                        break;
                    case ETHNIC:
                        bitmap |= 0x2000;
                        break;
                    case PERCUSSION:
                        bitmap |= 0x4000;
                        break;
                    case SOUND_EFFECTS:
                        bitmap |= 0x8000;
                        break;
                }
            }
        }
    }

    printf("The sum is: %i\n",bitmap);

    /* Close mid_file */
	fclose(mid_file);
    printf("\n");
    return 0;
}
