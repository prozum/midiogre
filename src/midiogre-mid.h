#ifndef __MID_H
#define __MID_H

#include <mid/mid.h>

#include <gtk/gtk.h>

enum
{
    COLUMN_TYPE,
    COLUMN_PARA_1,
    COLUMN_PARA_2,
    COLUMN_DELTA,
    COLUMN_MDATA,
    NUM_COLUMNS
};


void mid_import(GFile *mid_gfile, GtkWidget *notebook);
void mid_export(mid_t *mid);

#endif
