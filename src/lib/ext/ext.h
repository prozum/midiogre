/** @file ext.h Contains functions used to extract data from mid files */

#ifndef _EXT_H_
#define _EXT_H_

#include <mid/mid.h>

/**
 * @brief extract_instr_classes
 * @param mid: Midi file
 * @return
 */
int extract_instr_classes(mid_t *mid);

/**
 * @brief extract_time
 * @param mid: Midi file
 * @return
 */
double extract_time(mid_t *mid);

#endif
