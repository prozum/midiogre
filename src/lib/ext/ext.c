#include "ext.h"

#include <mid/mid.h>

int extract_instr_classes(mid_t *mid)
{
    track_t *track;
    event_t *event;

    unsigned int instr_class;
    unsigned int bitmap = 0;
    
    while ((track = list_next(mid->tracks)) != NULL) {
    
        /* For each event in track */
        while ((event = list_next(track->events)) != NULL) {
    
            /* If program change */
            if (event->msg == PRG_CHANGE) {
    
                /* Find instrument class */
              instr_class = event->byte_1 /  INSTR_PER_CLASS;
                
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
    
    if (bitmap == 0)
      bitmap = 1;
    return bitmap;
}
