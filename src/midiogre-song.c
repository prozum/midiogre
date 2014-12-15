#include "midiogre-song.h"
#include "midiogre-app.h"

#include <gtk/gtk.h>

#include <stdlib.h>
#include <string.h>


G_DEFINE_TYPE_WITH_PRIVATE(SongRow, song_row, GTK_TYPE_LIST_BOX_ROW);

static void play_clicked(SongRow *row, GtkButton *button)
{
    SongRowPrivate *priv = row->priv;

    priv->song->plays++;
}

static void playlist_clicked(SongRow *row, GtkButton *button)
{
    SongRowPrivate *priv = row->priv;

    priv->song->plays++;
}

static void fav_clicked(SongRow *row, GtkButton *button)
{
    SongRowPrivate *priv = row->priv;

    priv->song->plays++;
}

int song_row_sort(SongRow *a, SongRow *b, gpointer data)
{
    return a->priv->song->time_added - b->priv->song->time_added;
}


static void song_row_update(SongRow *row)
{
    SongRowPrivate *priv = row->priv;

    gtk_label_set_text(priv->title_label, priv->song->title);
    gtk_label_set_text(priv->album_label, priv->song->album);
    gtk_label_set_text(priv->artist_label, priv->song->artist);
}

static void song_row_class_init(SongRowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/prozum/midiogre/ui/songbox.ui");

    gtk_widget_class_bind_template_child_private(widget_class, SongRow, title_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, artist_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, album_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, time_label);

    gtk_widget_class_bind_template_child_private(widget_class, SongRow, play_button);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, playlist_button);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, fav_button);

    gtk_widget_class_bind_template_callback(widget_class, play_clicked);
    gtk_widget_class_bind_template_callback(widget_class, playlist_clicked);
    gtk_widget_class_bind_template_callback(widget_class, fav_clicked);

}

static void song_row_init(SongRow *row)
{
    row->priv = song_row_get_instance_private(row);

    gtk_widget_init_template(GTK_WIDGET(row));
}

GtkWidget *songbox_new(GtkBox *winbox, char *title, GtkListBoxSortFunc sort_func)
{
    GtkBox *box;
    GtkLabel *label;
    GtkWidget *separator, *scrolled, *listbox;

    /* Add seperator */
    separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(winbox, separator, FALSE, FALSE, 0);

    /* Box for songbox widgets */
    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_box_pack_start(winbox, GTK_WIDGET(box), TRUE, TRUE, 0);

    /* Add title */
    label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(label, title);
    gtk_box_pack_start(box, GTK_WIDGET(label), FALSE, FALSE, 0);

    /* Add seperator */
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(box, separator, FALSE, FALSE, 0);

    /* Add scroll container */
    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    /* Setup listbox */
    listbox = gtk_list_box_new();
    gtk_list_box_set_sort_func(GTK_LIST_BOX(listbox), sort_func, listbox, NULL);

    /* Add listbox to scroll container */
    gtk_container_add(GTK_CONTAINER (scrolled), listbox);
    gtk_box_pack_start(box, scrolled, TRUE, TRUE, 0);

    return listbox;
}

void update_songboxes(MidiogreApp *app)
{
    SongRow *row;
    song_t *song;

    guint i = 0;
    while ((song = g_queue_pop_head(app->songs)) != NULL) {

        row = song_row_new(song);
        gtk_widget_show(row);
        gtk_container_add (GTK_CONTAINER (app->songbox[0]), GTK_WIDGET (row));

        row = song_row_new(song);
        gtk_widget_show(row);
        gtk_container_add (GTK_CONTAINER (app->songbox[1]), GTK_WIDGET (row));

        row = song_row_new(song);
        gtk_widget_show(row);
        gtk_container_add (GTK_CONTAINER (app->songbox[2]), GTK_WIDGET (row));

        if (i++ > 10) {
            break;
        }
    }
}

song_t *song_new(void)
{
    song_t *song;

    song = calloc(1, sizeof(song_t));

    strcpy(song->album, "Best Album");
    strcpy(song->artist, "Worst Artist");
    song->num = 1;
    strcpy(song->title, "\"Midiogre\" Title");
    song->length = 400;
    //song->fingerprint = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    song->plays =500;
    song->time_added = 100000;

    return song;
}

SongRow *song_row_new(song_t *song)
{
    SongRow *row;

    row = g_object_new(song_row_get_type(), NULL);
    row->priv->song = song;
    song_row_update(row);

    return row;
}
