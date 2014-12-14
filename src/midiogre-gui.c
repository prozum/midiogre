#include "midiogre-gui.h"
#include "midiogre-import.h"

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
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_resource("/org/prozum/midiogre/document-open-symbolic.symbolic.png"));
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header), button);

    g_signal_connect_swapped(button, "clicked", G_CALLBACK (folder_chooser), window);


    /* Setup backward, play & forward buttons in a box */
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_context_add_class (gtk_widget_get_style_context (box), "linked");

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_resource("/org/prozum/midiogre/media-skip-backward-symbolic.symbolic.png"));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_resource("/org/prozum/midiogre/media-playback-start-symbolic.symbolic.png"));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_resource("/org/prozum/midiogre/media-skip-forward-symbolic.symbolic.png"));
    gtk_container_add (GTK_CONTAINER (box), button);

    gtk_header_bar_pack_start (GTK_HEADER_BAR (header), box);

    gtk_widget_show_all(window);

    return window;
}

GtkWidget *window_box_init(GtkWidget *window)
{
    GtkWidget *box;
    GtkWidget *frame;
    GtkWidget *button;

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add (GTK_CONTAINER (window), box);
    gtk_box_set_homogeneous(GTK_BOX(box), TRUE);


    frame = gtk_frame_new ("Search");
    gtk_box_pack_start (GTK_BOX(box), frame, FALSE, FALSE, 0);

    button = gtk_check_button_new();
    gtk_box_pack_start (GTK_BOX(box), button, FALSE, FALSE, 0);

    return box;
}

