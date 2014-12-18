#include "midiogre-song.h"
#include "midiogre-app.h"

#include <analyze/analyze.h>

#include <gtk/gtk.h>

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <process.h>
#endif

G_DEFINE_TYPE_WITH_PRIVATE(SongRow, song_row, GTK_TYPE_LIST_BOX_ROW)

void play_clicked(SongRow *row, GtkButton *button)
{
    GError *err = NULL;
    gchar *tmp;
    SongRowPrivate *priv = row->priv;

#ifdef WIN32
	spawnl( P_NOWAIT, "C:/Program Files/Windows Media Player/wmplayer.exe",
    	"wmplayer.exe", priv->song->addr, NULL );
	
#else

    tmp = g_strdup_printf("file://%s",priv->song->addr);
    g_app_info_launch_default_for_uri(tmp,
                                      NULL,
                                      &err);
    g_free(tmp);

#endif

    tmp = g_strdup_printf("%d", ++priv->song->plays);
    gtk_label_set_text(priv->plays_label, tmp);
    g_free(tmp);

    db_increment_plays(priv->song->id);


}

void delete_clicked(SongRow *row, GtkButton *button)
{
    SongRowPrivate *priv = row->priv;

    db_delete_song(priv->song->id);
    song_row_destroy(row);
}

void fav_clicked(SongRow *row, GtkButton *button)
{
    SongRowPrivate *priv = row->priv;

    /* Deallocate last favorite if exist */
    if (app->cur_fav != NULL) {
        song_free(app->cur_fav);
    }

    /* Create new song to store favorite song */
    app->cur_fav = song_new();

    /* Copy title,artist,album & finger print  */
    app->cur_fav->id  = priv->song->id;
    strcpy(app->cur_fav->title, priv->song->title);
    strcpy(app->cur_fav->artist, priv->song->artist);
    strcpy(app->cur_fav->album, priv->song->album);
    copy_f_prn(app->cur_fav->finger_prints, priv->song->finger_prints);

    gtk_label_set_text(app->fav_title_label, app->cur_fav->title);
    gtk_label_set_text(app->fav_artist_label, app->cur_fav->artist);
    gtk_label_set_text(app->fav_album_label, app->cur_fav->album);
}

static void song_row_class_init(SongRowClass *klass)
{
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gtk_widget_class_set_template_from_resource(widget_class, "/org/prozum/midiogre/ui/songbox.ui");

    gtk_widget_class_bind_template_child_private(widget_class, SongRow, title_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, artist_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, album_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, time_label);

    gtk_widget_class_bind_template_child_private(widget_class, SongRow, edit_dist_label);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, plays_label);

    gtk_widget_class_bind_template_child_private(widget_class, SongRow, play_button);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, delete_button);
    gtk_widget_class_bind_template_child_private(widget_class, SongRow, fav_button);

    gtk_widget_class_bind_template_callback(widget_class, play_clicked);
    gtk_widget_class_bind_template_callback(widget_class, delete_clicked);
    gtk_widget_class_bind_template_callback(widget_class, fav_clicked);
}

static void song_row_init(SongRow *row)
{
    row->priv = song_row_get_instance_private(row);

    gtk_widget_init_template(GTK_WIDGET(row));
}

SongRow *song_row_new(song_t *song)
{
    SongRow *row;

    row = g_object_new(song_row_get_type(), NULL);
    row->priv->song = song;
    song_row_update(row);

    return row;
}

void song_row_update(SongRow *row)
{
    SongRowPrivate *priv = row->priv;

    gchar *tmp;

    tmp = g_strdup_printf("%02d:%02d", priv->song->length/1000 / 60,
                                       priv->song->length/1000 % 60);

    gtk_label_set_text(priv->time_label, tmp);
    g_free(tmp);

    gtk_label_set_text(priv->title_label, priv->song->title);
    gtk_label_set_text(priv->album_label, priv->song->album);
    gtk_label_set_text(priv->artist_label, priv->song->artist);


    tmp = g_strdup_printf("%d", priv->song->edit_score);
    gtk_label_set_text(priv->edit_dist_label, tmp);
    g_free(tmp);

    tmp = g_strdup_printf("%d", priv->song->plays);
    gtk_label_set_text(priv->plays_label, tmp);
    g_free(tmp);
}

void song_row_destroy(SongRow *row)
{
    song_free(row->priv->song);

    gtk_widget_destroy(GTK_WIDGET(row));
}

song_t *song_new(void)
{
    song_t *song;

    song = calloc(1, sizeof(song_t));

    song->finger_prints = create_f_prn();

    return song;
}

void song_free(song_t *song)
{
    g_free(song->addr);
    free_f_prn(song->finger_prints);
    g_free(song);
}

GtkListBox *songbox_new(GtkNotebook *notebook, char *title)
{
    GtkListBox *songbox;
    GtkLabel *label;
    GtkWidget *scrolled;

    /* Add title */
    label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(label, title);

    /* Add scroll container */
    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    /* Setup songbox */
    songbox = GTK_LIST_BOX(gtk_list_box_new());

    /* Add listbox to scroll container */
    gtk_container_add(GTK_CONTAINER(scrolled), GTK_WIDGET(songbox));
    gtk_notebook_append_page(notebook, GTK_WIDGET(scrolled), GTK_WIDGET(label));

    return songbox;
}

void songbox_update(GtkListBox *songbox, GQueue *songs, gint limit)
{
    SongRow *row;
    song_t *song;

    guint i = 0;

    /* Destroy all rows in songbox */
    gtk_container_foreach(GTK_CONTAINER(songbox), (GtkCallback)song_row_destroy, NULL);

    while ((song = g_queue_pop_head(songs)) != NULL) {

        /* Limit number of results by result spinbox */
        if (i++ >= limit) {
            break;
        }

        row = song_row_new(song);
        gtk_widget_show(GTK_WIDGET(row));
        gtk_list_box_prepend(songbox, GTK_WIDGET(row));
    }
}
