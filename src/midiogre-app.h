#ifndef __MIDIOGRE_APP_H__
#define __MIDIOGRE_APP_H__

#include <mid/mid.h>
#include <db/db.h>

#include <gtk/gtk.h>

/** MidiogreApp - Struct containing app vars */
typedef struct {

    /* App widgets */
    GtkWindow *window;  /**< Midiogre window */

    GtkBox *win_box;    /**< Window box containing window widgets            */
    GtkBox *panel_box;  /**< Sidepanel box containing sidepanel widgets      */
    GtkBox *search_box; /**< Search box containing search widgets            */

    GtkGrid *instr_grid;  /**< Instrument grid containing instrument checkboxes */

    GtkCheckButton *instr_buttons[INSTR_CLASSES]; /**< Instrument checkboxes */

    GtkEntry *album_entry;    /**< Album name input widget  */
    GtkEntry *artist_entry;   /**< Artist name input widget */
    GtkEntry *title_entry;    /**< Title name input widget  */

    GtkSpinButton *result_spinbutton; /**< Button to specify number of result */

    GtkButton *search_button; /**< Search button */

    GtkListBox *songbox_alpha; /**< SongBox sorting songs by alphabetical order */
    GtkListBox *songbox_best;  /**< SongBox sorting songs by best match         */
    GtkListBox *songbox_fprnt; /**< SongBox sorting songs by fingerprint        */
    GtkListBox *songbox_pop;   /**< SongBox sorting songs by popularity         */
    GtkListBox *songbox_new;   /**< SongBox sorting songs by date               */

    /* App vars */
    GQueue *songs;            /**< Used to store and sort songs from db */
    song_t *cur_play;         /**< Current played song                  */
    song_t *cur_fav;          /**< Current favorite song                */
    GQueue *playlist;         /**< Song playlist                        */

} MidiogreApp;

GtkWidget *window_init(void);
MidiogreApp *midiogre_app_init(void);

#endif
