#ifndef __MIDIOGRE_SEARCH_H__
#define __MIDIOGRE_SEARCH_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

gint search_event(MidiogreApp *app);

int search_db(GQueue *songs, sqlite3 *db, gchar *base, gchar *body, gint limit);

GCompareFunc *sort_pop(gconstpointer a, gconstpointer b, gpointer data);

gint search_handler(void *a, int argc, char **argv, char **col_name);

gint read_instr_classes(MidiogreApp *app);

#endif
