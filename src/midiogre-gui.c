#include "midiogre-gui.h"
#include "midiogre-mid.h"

#include "pop/pop.h"

#include <string.h>

#include <gtk/gtk.h>

GtkWidget *window_init(void)
{
    GtkWidget *window = NULL;

    GtkWidget *header;
    GtkWidget *button;
    GtkWidget *box;

    /* Setup window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);
    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_widget_destroyed), &window);

    /* Setup header bar */
    header = gtk_header_bar_new ();
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header), TRUE);
    gtk_header_bar_set_title (GTK_HEADER_BAR (header), "Midiogre");
    gtk_window_set_titlebar (GTK_WINDOW (window), header);

    /* Setup folder button */
    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("document-open-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header), button);

    g_signal_connect_swapped(button, "clicked", G_CALLBACK (folder_chooser), window);


    /* Setup backward, play & forward buttons in a box */
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_context_add_class (gtk_widget_get_style_context (box), "linked");

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-skip-backward-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-playback-start-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-skip-forward-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    gtk_header_bar_pack_start (GTK_HEADER_BAR (header), box);

    gtk_widget_show_all(window);

    return window;
}

GtkWidget *listbox_new(GtkWidget *box)
{
    GtkWidget *scrolled, *listbox;

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    listbox = gtk_list_box_new();

    gtk_container_add (GTK_CONTAINER (scrolled), listbox);

    gtk_box_pack_start (GTK_CONTAINER(box), scrolled, FALSE, FALSE, 0);

    return listbox;
}


song_t *song_new(void)
{
    song_t *song;

    song = calloc(1,sizeof(song_t));

    strcpy(song->album, "Best Album");
    strcpy(song->artist, "Worst Artist");
    song->track_num = 1;
    strcpy(song->title, "\"Midiogre\" TitleArtist");
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

