#ifndef __MIDIOGRE_GUI_H__
#define __MIDIOGRE_GUI_H__

#include <mid/mid.h>

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *winbox;

    GtkWidget *songbox[3];

    GtkButton *instr_buttons[INSTR_CLASSES];
    GtkEntry *album_entry;
    GtkEntry *artist_entry;
    GtkEntry *title_entry;

} GlobalWidgets;

GtkWidget *window_init(void);
GlobalWidgets *g_wgs_init(GtkWidget *window);

#endif
