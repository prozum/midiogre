#include "midiogre-song.h"

#include <gtk/gtk.h>

#include <stdlib.h>
#include <string.h>


GtkWidget *listbox_new(GtkWidget *box)
{
    GtkWidget *scrolled, *listbox;

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    listbox = gtk_list_box_new();

    gtk_container_add(GTK_CONTAINER (scrolled), listbox);

    gtk_box_pack_start(GTK_BOX(box), scrolled, FALSE, FALSE, 0);

    return listbox;
}

/*
void gtk_message_row_class_init(GtkMessageRowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/prozum/midiogre/ui/songbox.ui");
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, content_label);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, source_name);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, source_nick);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, short_time_label);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, detailed_time_label);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, extra_buttons_box);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, details_revealer);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, avatar_image);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, resent_box);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, resent_by_button);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, n_reshares_label);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, n_favorites_label);
  gtk_widget_class_bind_template_child_private (widget_class, GtkMessageRow, expand_button);
  gtk_widget_class_bind_template_callback (widget_class, expand_clicked);
  gtk_widget_class_bind_template_callback (widget_class, reshare_clicked);
  gtk_widget_class_bind_template_callback (widget_class, favorite_clicked);

  widget_class->state_flags_changed = gtk_message_row_state_flags_changed;
}
*/

song_t *song_new(void)
{
    song_t *song;

    song = calloc(1,sizeof(song_t));

    strcpy(song->album, "Best Album");
    strcpy(song->artist, "Worst Artist");
    song->track_num = 1;
    strcpy(song->title, "\"Midiogre\" Title");
    song->length = 400;
    //song->fingerprint = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    song->plays =500;
    song->time_added = 100000;

    return song;
}

GtkWidget *song_row_new(song_t *song)
{
    GtkWidget* SongBox;

    SongBox = gtk_list_box_row_new();


    //row->priv->song = song;
    return SongBox;
}

