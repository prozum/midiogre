#include "midiogre-gui.h"
#include "midiogre-import.h"
#include "midiogre-song.h"

#include "mid/mid.h"
#include "mid/mid-str.h"
#include "pop/pop.h"

#include <gtk/gtk.h>

#include <string.h>

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

GlobalWidgets *g_wgs_init(GtkWidget *window)
{
    GlobalWidgets *g_wgs;
    GtkBox *box;
    GtkWidget *label,*frame;
    GtkWidget *align;

    gint i;

    g_wgs = calloc(1,sizeof(GlobalWidgets));


    /* Global horizontal box */
    g_wgs->win_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER (window), GTK_WIDGET(g_wgs->win_box));


    /* Sidepanel box */
    g_wgs->panel_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_box_pack_start(g_wgs->win_box, g_wgs->panel_box, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(g_wgs->panel_box), 10);

    /* Frame with search criteria widgets */
    frame = gtk_frame_new("Search");
    gtk_box_pack_start(g_wgs->panel_box, frame, FALSE, FALSE, 0);

    g_wgs->search_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_container_set_border_width(GTK_CONTAINER(g_wgs->search_box), 10);

    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(g_wgs->search_box));
    gtk_widget_set_size_request(frame, 280, 0);


    /* Title entry */
    frame = gtk_frame_new("Title");
    gtk_box_pack_start(g_wgs->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    g_wgs->title_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(g_wgs->title_entry), TRUE, TRUE, 0);


    /* Artist entry */
    frame = gtk_frame_new("Artist");
    gtk_box_pack_start(g_wgs->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    g_wgs->artist_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(g_wgs->artist_entry), TRUE, TRUE, 0);


    /* Album entry */
    frame = gtk_frame_new("Album");
    gtk_box_pack_start(g_wgs->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    g_wgs->album_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(g_wgs->album_entry), TRUE, TRUE, 0);


    /* Instrument class buttons */
    frame = gtk_frame_new("Instrument Classes");
    gtk_box_pack_start(g_wgs->search_box, frame, FALSE, FALSE, 0);
    g_wgs->instr_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_container_set_border_width(GTK_CONTAINER(g_wgs->instr_box), 10);
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(g_wgs->instr_box));

    for (i = 0; i < INSTR_CLASSES; i++) {

        box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
        gtk_box_pack_start(g_wgs->instr_box, GTK_WIDGET(box), FALSE, FALSE, 0);

        label = gtk_label_new(instrument_classes[i]);
        gtk_box_pack_start(box, label, FALSE, FALSE, 0);

        align = gtk_alignment_new(1,0,1,0);
        gtk_box_pack_start(box, align, TRUE, TRUE, 0);

        g_wgs->instr_buttons[i] = GTK_BUTTON(gtk_check_button_new());
        gtk_box_pack_start(box, GTK_WIDGET(g_wgs->instr_buttons[i]), FALSE, FALSE, 0);

        /* Add instrument classes button */
    }

    /* Search button */
    g_wgs->search_button = GTK_BUTTON(gtk_button_new_with_label("Search"));
    gtk_box_pack_start(g_wgs->search_box, GTK_WIDGET(g_wgs->search_button), FALSE, FALSE, 0);

    /* Playlist */
    /* TODO */

    /* Favorites */
    /* TODO */


    align = gtk_alignment_new(1,1,1,1);
    gtk_box_pack_start(g_wgs->panel_box, align, TRUE, TRUE, 0);


    /* Add song boxes */
    g_wgs->songbox[0] = songbox_new(g_wgs->win_box, "<span size=\"large\">Alphabetical Order</span>", (GtkListBoxSortFunc) song_row_sort);
    g_wgs->songbox[1] = songbox_new(g_wgs->win_box, "<span size=\"large\">Date Order</span>", (GtkListBoxSortFunc)song_row_sort);
    g_wgs->songbox[2] = songbox_new(g_wgs->win_box, "<span size=\"large\">Match Order</span>", (GtkListBoxSortFunc)song_row_sort);

    gtk_widget_show_all(GTK_WIDGET(g_wgs->win_box));





    //gtk_box_pack_start (GTK_BOX(box), button, FALSE, FALSE, 0);

    return g_wgs;
}
