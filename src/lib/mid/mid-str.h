/** @file mid-str.h */

#ifndef __MIDIUTIL_H__
#define __MIDIUTIL_H__

#include "mid.h"

extern const char *instruments[]; /**< names of instruments */
extern const char *instrument_classes[]; /**< Names of instrument classes */

void print_header(mid_t *mid); /**< prints header */
void print_tracks(mid_t *mid); /**< prints tracks */
void print_event(mid_t *mid, unsigned int t, unsigned int e); /**< prints events */

#endif
