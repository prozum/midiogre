/** @file midiogre-app.h */

#ifndef __MIDIOGRE_APP_H__
#define __MIDIOGRE_APP_H__

#include <mid/mid.h>
#include <db/db.h>

#include <gtk/gtk.h>

enum songboxes
{
    SONGBOX_ALPHA, /**< SongBox sorting songs by alphabetical order */
    SONGBOX_FPRNT, /**< SongBox sorting songs by fingerprint        */
    SONGBOX_BEST,  /**< SongBox sorting songs by best match         */
    SONGBOX_POP,   /**< SongBox sorting songs by popularity         */
    SONGBOX_NEW,   /**< SongBox sorting songs by date               */
    SONGBOXES_NUM  /**< Number of songboxes                         */
};


/** MidiogreApp - Struct containing app vars */
typedef struct
{

    /* App widgets */
    GtkWindow *window;         /**< Midiogre window */

    GtkBox *win_box;           /**< Window box containing window widgets            */
    GtkBox *panel_box;         /**< Sidepanel box containing sidepanel widgets      */
    GtkBox *fav_box;           /**< Favorite box containing favorite widgets        */
    GtkBox *search_box;        /**< Search box containing search widgets            */

    GtkGrid *instr_grid;       /**< Instrument grid containing instrument checkboxes */

    GtkCheckButton *instr_buttons[INSTR_CLASSES]; /**< Instrument checkboxes */

    GtkEntry *album_entry;     /**< Album name input widget  */
    GtkEntry *artist_entry;    /**< Artist name input widget */
    GtkEntry *title_entry;     /**< Title name input widget  */

    GtkSpinButton *result_spinbutton; /**< Button to specify number of result */

    GtkButton *search_button;  /**< Search button */

    GtkNotebook *song_notebook; /**< Tabs widget for songboxes */

    GtkListBox *songboxes[SONGBOXES_NUM]; /**< Songboxes contains song rows */

    GQueue *songs[SONGBOXES_NUM]; /**< Used to store and sort songs from db */

    GtkLabel *fav_title_label; /**< Favorite song title name  */
    GtkLabel *fav_artist_label;/**< Favorite song artist name */
    GtkLabel *fav_album_label; /**< Favorite song album name  */

    /* App vars */
    song_t *cur_play;          /**< Current played song                  */
    song_t *cur_fav;           /**< Current favorite song                */
    GQueue *playlist;          /**< Song playlist                        */

} MidiogreApp;

extern MidiogreApp *app; /**< Instance of MidiogreApp */

MidiogreApp *midiogre_app_init(void); /**< Init MidiogreApp widgets */
gboolean key_event(GtkWidget *widget, GdkEventKey *event); /**< Called for every key press in Midiogre */

#endif
