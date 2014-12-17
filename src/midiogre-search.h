#ifndef __MIDIOGRE_SEARCH_H__
#define __MIDIOGRE_SEARCH_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

gint search_event(MidiogreApp *app);

int search_db(GQueue *songs, sqlite3 *db, gchar *base, gchar *body, gint limit);

gint sort_fprnt(gpointer s1, gpointer s2, gpointer a);
GCompareFunc sort_pop(gconstpointer s1, gconstpointer s2, gpointer a);

gint search_handler(void *a, int argc, char **argv, char **col_name);

gint read_instr_classes(MidiogreApp *app);

#endif
