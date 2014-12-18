/** @file midiogre-search.h */

#ifndef __MIDIOGRE_SEARCH_H__
#define __MIDIOGRE_SEARCH_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

/**
 * @brief search_event
 * @return
 */
gint search_event(void);

/**
 * @brief search_db
 * @param songs
 * @param db
 * @param base
 * @param body
 * @param limit
 * @return
 */
int search_db(GQueue *songs, sqlite3 *db, gchar *base, gchar *body, gint limit);

/**
 * @brief sort_fprnt
 * @param s1
 * @param s2
 * @param a
 * @return
 */
gint sort_fprnt(gpointer s1, gpointer s2, gpointer a);

/**
 * @brief sort_best
 * @param s1
 * @param s2
 * @param a
 * @return
 */
gint sort_best(gpointer s1, gpointer s2, gpointer a);

/**
 * @brief search_handler
 * @param a
 * @param argc: Argument count
 * @param argv: Argument vector
 * @param col_name: Name of collumn
 * @return
 */
gint search_handler(void *a, int argc, char **argv, char **col_name);

/**
 * @brief read_instr_classes: Read instrument class
 * @param app: aaplication
 * @return
 */
gint read_instr_classes(MidiogreApp *app);

#endif
