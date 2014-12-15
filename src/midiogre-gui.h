#ifndef __MIDIOGRE_GUI_H__
#define __MIDIOGRE_GUI_H__

#include <mid/mid.h>

#include <gtk/gtk.h>

typedef struct {
    GtkBox *win_box;
    GtkBox *panel_box;
    GtkBox *search_box;
    GtkBox *instr_box;

    GtkButton *instr_buttons[INSTR_CLASSES];
    GtkEntry *album_entry;
    GtkEntry *artist_entry;
    GtkEntry *title_entry;

    GtkButton *search_button;

    GtkWidget *songbox[3];

} GlobalWidgets;

GtkWidget *window_init(void);
GlobalWidgets *g_wgs_init(GtkWidget *window);

#endif
