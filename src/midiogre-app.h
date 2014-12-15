#ifndef __MIDIOGRE_APP_H__
#define __MIDIOGRE_APP_H__

#include <mid/mid.h>
#include <db/db.h>

#include <gtk/gtk.h>

typedef struct {

    /* App widgets */
    GtkWindow *window;

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

    /* App structs */
    song_t *cur_play;
    song_t *cur_fav;

} MidiogreApp;

GtkWidget *window_init(void);
MidiogreApp *midiogre_app_init(void);

#endif
