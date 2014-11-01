#ifndef __UTIL_H__
#define __UTIL_H__

/** @file util.h */

#include "midi.h"

void print_header(mid_t *mid);
void print_tracks(mid_t *mid);
void print_event(mid_t *mid, unsigned int t, unsigned int e);

#endif
