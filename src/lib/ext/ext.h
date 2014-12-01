/** @ext-instr.h Contains functions used to extract instruments from mid files */

#ifndef _EXT_H_
#define _EXT_H_

#include <mid/mid.h>

int extract_instr_classes(mid_t *mid);
int extract_time(mid_t *mid);

#endif
