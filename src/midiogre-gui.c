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
    GtkWidget *tmp;
    GtkWidget *grid;

    gint i;

    g_wgs = calloc(1,sizeof(GlobalWidgets));


    /* Global horizontal box */
    g_wgs->winbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add (GTK_CONTAINER (window), g_wgs->winbox);


    /* Frame with search criteria widgets */
    tmp = gtk_frame_new ("Search");
    gtk_box_pack_start (GTK_BOX(g_wgs->winbox), tmp, TRUE, TRUE, 10);

    grid = gtk_grid_new();
    gtk_widget_set_hexpand(grid, TRUE);
    gtk_container_add(GTK_CONTAINER(tmp), grid);


    /* Title entry */
    tmp = gtk_label_new("Title:");
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(tmp),
                    1,
                    1,
                    1,
                    1);
    g_wgs->title_entry = GTK_ENTRY(gtk_entry_new());
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(g_wgs->title_entry),
                    2,
                    1,
                    1,
                    1);

    /* Artist entry */
    tmp = gtk_label_new("Artist:");
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(tmp),
                    1,
                    2,
                    1,
                    1);
    g_wgs->artist_entry = GTK_ENTRY(gtk_entry_new());
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(g_wgs->artist_entry),
                    2,
                    2,
                    1,
                    1);

    /* Album entry */
    tmp = gtk_label_new("Album:");
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(tmp),
                    1,
                    3,
                    1,
                    1);
    g_wgs->album_entry = GTK_ENTRY(gtk_entry_new());
    gtk_grid_attach(GTK_GRID(grid),
                    GTK_WIDGET(g_wgs->album_entry),
                    2,
                    3,
                    1,
                    1);


    /* Instrument class buttons */
    for (i = 0; i < INSTR_CLASSES; i++) {

        /* Instrument classes label */
        tmp = gtk_label_new(instrument_classes[i]);
        gtk_label_set_justify(GTK_LABEL(tmp), GTK_JUSTIFY_RIGHT);
        gtk_grid_attach(GTK_GRID(grid),
                        GTK_WIDGET(tmp),
                        1,
                        4 + i,
                        1,
                        1);

        g_wgs->instr_buttons[i] = GTK_BUTTON(gtk_check_button_new());

        /* Add instrument classes button */
        gtk_grid_attach(GTK_GRID(grid),
                        GTK_WIDGET(g_wgs->instr_buttons[i]),
                        2,
                        4 + i,
                        1,
                        1);
    }

    /* Add song boxes */
    g_wgs->songbox[0] = songbox_new(g_wgs->winbox, "<span size=\"large\">Best Match</span>", (GtkListBoxSortFunc) song_row_sort);
    g_wgs->songbox[1] = songbox_new(g_wgs->winbox, "<span size=\"large\">Random</span>", (GtkListBoxSortFunc)song_row_sort);
    g_wgs->songbox[2] = songbox_new(g_wgs->winbox, "<span size=\"large\">Newest</span>", (GtkListBoxSortFunc)song_row_sort);

    gtk_widget_show_all(g_wgs->winbox);





    //gtk_box_pack_start (GTK_BOX(box), button, FALSE, FALSE, 0);

    return g_wgs;
}
