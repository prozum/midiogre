#ifndef __MIDIOGRE_SONG_H__
#define __MIDIOGRE_SONG_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

#define TYPE_SONG_ROW              (song_row_get_type())
#define SONG_ROW(song_row)      (G_TYPE_CHECK_INSTANCE_CAST ((song_row), TYPE_SONG_ROW, SongRow))
#define SONG_ROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_SONG_ROW, SongRowClass))
#define IS_SONG_ROW(song_row)   (G_TYPE_CHECK_INSTANCE_TYPE ((song_row), TYPE_SONG_ROW))
#define IS_SONG_ROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_SONG_ROW))
#define SONG_ROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_SONG_ROW, SongRowClass))

typedef struct _SongRow         SongRow;
typedef struct _SongRowClass    SongRowClass;
typedef struct _SongRowPrivate  SongRowPrivate;

/** SongRow - SongRow struct with instance variables */
struct _SongRow
{
    GtkListBoxRow parent;  /**< Parent instance struct     */

    SongRowPrivate *priv;  /**< Instance private variables */
};

/** SongRowClass - SongRow struct with class variables */
struct _SongRowClass
{
    GtkListBoxRowClass parent_class; /**< Parent class struct */
};

/** SongRowPrivate - SongRow struct with private instance variables */
struct _SongRowPrivate
{
    song_t *song; /**< Song contain ing song data */

    GtkLabel *title_label;  /**< Song title  label */
    GtkLabel *artist_label; /**< Song artist label */
    GtkLabel *album_label;  /**< Song album  label */
    GtkLabel *time_label;   /**< Song time   label */

    GtkButton *play_button;     /**< Song play button     */
    GtkButton *playlist_button; /**< Song playlist button */
    GtkButton *fav_button;      /**< Song favorite button */
};

GType      song_row_get_type(void) G_GNUC_CONST;

GtkListBox *songbox_new(GtkBox *winbox, char *title, GtkListBoxSortFunc sort_func);
void songbox_update(GtkListBox *songbox, GQueue *songs, gint limit);
void songbox_delete_rows(GtkListBox *songbox);
song_t *song_new(void);
SongRow *song_row_new(song_t *song);

int song_row_sort(SongRow *a, SongRow *b, gpointer data);



#endif
