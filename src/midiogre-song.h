#ifndef __MIDIOGRE_SONG_H__
#define __MIDIOGRE_SONG_H__

#include <pop/pop.h>

#include <gtk/gtk.h>

#define TYPE_SONG_ROW              (song_row_get_type())
#define SONG_ROW(song_row)      (G_TYPE_CHECK_INSTANCE_CAST ((song_row), TYPE_SONG_ROW, SongRow))
#define SONG_ROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_SONG_ROW, SongRowClass))
#define IS_SONG_ROW(song_row)   (G_TYPE_CHECK_INSTANCE_TYPE ((song_row), TYPE_SONG_ROW))
#define IS_SONG_ROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_SONG_ROW))
#define SONG_ROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_SONG_ROW, SongRowClass))

typedef struct _SongRow   SongRow;
typedef struct _SongRowClass  SongRowClass;
typedef struct _SongRowPrivate  SongRowPrivate;

struct _SongRow
{
    GtkListBoxRow parent;

    SongRowPrivate *priv;
};

struct _SongRowClass
{
    GtkListBoxRowClass parent_class;
};

GType      song_row_get_type(void) G_GNUC_CONST;


song_t *song_new(void);
SongRow *song_row_new(song_t *song);
GtkWidget *songbox_new(GtkWidget *box);



#endif
