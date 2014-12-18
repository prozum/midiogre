/** @file ext.h Contains functions used to extract data from mid files */

#ifndef _EXT_H_
#define _EXT_H_

#include <mid/mid.h>

int extract_instr_classes(mid_t *mid);
double extract_time(mid_t *mid);

#endif
