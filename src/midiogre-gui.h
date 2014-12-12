#ifndef __MIDIOGRE_GUI_H__
#define __MIDIOGRE_GUI_H__

#include <gtk/gtk.h>

#define GTK_TYPE_SONG_ROW              (gtk_song_row_get_type())
#define GTK_SONG_ROW(message_row)      (G_TYPE_CHECK_INSTANCE_CAST ((song_row), GTK_TYPE_SONG_ROW, SongRow))
#define GTK_SONG_ROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SONG_ROW, SongRowClass))
#define GTK_IS_SONG_ROW(message_row)   (G_TYPE_CHECK_INSTANCE_TYPE ((song_row), GTK_TYPE_SONG_ROW))
#define GTK_IS_SONG_ROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SONG_ROW))
#define GTK_SONG_ROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SONG_ROW, SongRowClass))

typedef struct _SongClass  SongClass;
typedef struct _SongRow   SongRow;
typedef struct _SongRowClass  SongRowClass;
typedef struct _SongRowPrivate  SongRowPrivate;

struct _SongClass
{
    GObjectClass parent_class;
};

struct _SongRow
{
    GtkListBoxRow parent;

    SongRowPrivate *priv;
};

struct _SongRowClass
{
    GtkListBoxRowClass parent_class;
};

struct _SongRowPrivate
{
    song_t *song;

    GtkLabel *artist;
    GtkLabel *album;
    GtkLabel *title;

    GtkButton *favorite_button;
    GtkButton *play_button;
};

GType      gtk_song_row_get_type  (void) G_GNUC_CONST;


GtkWidget *window_init(void);

GtkWidget *listbox_new(GtkWidget *box);

#endif
