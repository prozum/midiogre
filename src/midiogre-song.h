/** @file midiogre-song.h */

#ifndef __MIDIOGRE_SONG_H__
#define __MIDIOGRE_SONG_H__

#include "midiogre-app.h"

#include <gtk/gtk.h>

/** Song row get type */
#define TYPE_SONG_ROW              (song_row_get_type())

/** Song row check instance*/
#define SONG_ROW(song_row)      (G_TYPE_CHECK_INSTANCE_CAST ((song_row), TYPE_SONG_ROW, SongRow))

/** Song row check class */
#define SONG_ROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_SONG_ROW, SongRowClass))

/** Song row check instance type */
#define IS_SONG_ROW(song_row)   (G_TYPE_CHECK_INSTANCE_TYPE ((song_row), TYPE_SONG_ROW))

/** Song row check class type */
#define IS_SONG_ROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_SONG_ROW))

/** Song row get class */
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

    GtkLabel *date_label;      /**< Date      label */
    GtkLabel *plays_label;     /**< Plays     label */

    GtkButton *play_button;     /**< Song play button     */
    GtkButton *delete_button;   /**< Song playlist button */
    GtkButton *fav_button;      /**< Song favorite button */
};

/** song row get type */
GType      song_row_get_type(void) G_GNUC_CONST;

void play_clicked(SongRow *row, GtkButton *button);
void delete_clicked(SongRow *row, GtkButton *button);
void fav_clicked(SongRow *row, GtkButton *button);

static void song_row_class_init(SongRowClass *klass);
static void song_row_init(SongRow *row);
void song_row_update(SongRow *row);
SongRow *song_row_new(song_t *song);
void song_row_destroy(SongRow *row);

song_t *song_new(void);
void song_free(song_t *song);

GtkListBox *songbox_new(GtkNotebook *notebook, char *title);
void songbox_update(GtkListBox *songbox, GQueue *songs, gint limit);


#endif
