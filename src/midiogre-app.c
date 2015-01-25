#include "midiogre-app.h"
#include "midiogre-import.h"
#include "midiogre-song.h"
#include "midiogre-search.h"

#include "mid/mid.h"
#include "mid/mid-str.h"

#include <gtk/gtk.h>

#include <string.h>

MidiogreApp *app;

MidiogreApp *midiogre_app_init(void)
{
    MidiogreApp *app;

    GtkWidget *header;
    GtkWidget *separator;
    GtkWidget *scrolled;
    GtkWidget *label,*frame;
    GtkWidget *button;
    GtkBox *box;

    gint i;
    GError *err;

    /* Allocate app */
    app = calloc(1, sizeof(MidiogreApp));


    /* Setup window */
    app->window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_default_size(GTK_WINDOW(app->window), 600, 400);
    g_signal_connect(app->window, "destroy", G_CALLBACK(gtk_widget_destroyed), NULL);
    g_signal_connect(app->window, "key-release-event", G_CALLBACK(key_event), NULL);
    gtk_window_set_icon(app->window,gdk_pixbuf_new_from_resource("/org/prozum/midiogre/midiogre_logo.png",&err));


    /* Setup header bar */
    header = gtk_header_bar_new();
    gtk_header_bar_set_title (GTK_HEADER_BAR(header), "Midiogre");
    gtk_window_set_titlebar(app->window, header);

#if GTK_MINOR_VERSION >= 12
    /* Setup close button */
    button = gtk_button_new();
    gtk_container_add (GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/window-close-symbolic.symbolic.png"));
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_main_quit), app->window);


    /* Add seperator */
    separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), separator);


    /* Setup folder button */
    button = gtk_button_new();
    gtk_container_add (GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/document-open-symbolic.symbolic.png"));
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(folder_chooser), app->window);
#else
    /* Setup folder button */
    button = gtk_button_new();
    gtk_container_add (GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/document-open-symbolic.symbolic.png"));
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(folder_chooser), app->window);


    /* Add seperator */
    separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), separator);


    /* Setup close button */
    button = gtk_button_new();
    gtk_container_add (GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/window-close-symbolic.symbolic.png"));
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), app->window);
#endif

    /* Global horizontal box */
    app->win_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(app->window), GTK_WIDGET(app->win_box));


    /* Scroll container for sidepanel */
    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(app->win_box, scrolled, FALSE, FALSE, 0);


    /* Sidepanel box */
    app->panel_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_container_add(GTK_CONTAINER (scrolled), GTK_WIDGET(app->panel_box));
    gtk_container_set_border_width(GTK_CONTAINER(app->panel_box), 10);


    /* Frame with favorit info */
    frame = gtk_frame_new("Favorite");
    gtk_box_pack_start(app->panel_box, frame, FALSE, FALSE, 0);
    app->fav_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(app->fav_box));


    /* Current favorite */
    app->fav_title_label = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_max_width_chars(app->fav_title_label, 20);
    gtk_label_set_width_chars(app->fav_title_label, 20);
    gtk_box_pack_start(app->fav_box, GTK_WIDGET(app->fav_title_label), FALSE, FALSE, 0);
    app->fav_artist_label = GTK_LABEL(gtk_label_new("No favorite selected!"));
    gtk_label_set_max_width_chars(app->fav_artist_label, 20);
    gtk_label_set_width_chars(app->fav_artist_label, 20);
    gtk_box_pack_start(app->fav_box, GTK_WIDGET(app->fav_artist_label), FALSE, FALSE, 0);
    app->fav_album_label = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_max_width_chars(app->fav_album_label, 20);
    gtk_label_set_width_chars(app->fav_album_label, 20);
    gtk_box_pack_start(app->fav_box, GTK_WIDGET(app->fav_album_label), FALSE, FALSE, 0);


    /* Frame with search criteria widgets */
    frame = gtk_frame_new("Search");
    gtk_box_pack_start(app->panel_box, frame, FALSE, FALSE, 0);

    app->search_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    gtk_container_set_border_width(GTK_CONTAINER(app->search_box), 10);

    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(app->search_box));


    /* Title entry */
    frame = gtk_frame_new("Title");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->title_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(app->title_entry), TRUE, FALSE, 0);


    /* Artist entry */
    frame = gtk_frame_new("Artist");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->artist_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(app->artist_entry), TRUE, FALSE, 0);


    /* Album entry */
    frame = gtk_frame_new("Album");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->album_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(app->album_entry), TRUE, FALSE, 0);


    /* Instrument class buttons */
    frame = gtk_frame_new("Instrument Classes");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    app->instr_grid = GTK_GRID(gtk_grid_new());

#if GTK_MINOR_VERSION >= 12
    gtk_widget_set_margin_start(GTK_WIDGET(app->instr_grid), 10);
    gtk_widget_set_margin_end(GTK_WIDGET(app->instr_grid), 10);
#else
    gtk_widget_set_margin_left(GTK_WIDGET(app->instr_grid), 10);
    gtk_widget_set_margin_right(GTK_WIDGET(app->instr_grid), 10);
#endif
    gtk_widget_set_margin_bottom(GTK_WIDGET(app->instr_grid), 10);
    gtk_widget_set_margin_top(GTK_WIDGET(app->instr_grid), 10);
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(app->instr_grid));


    for (i = 0; i < INSTR_CLASSES; i++) {

        label = gtk_label_new(instrument_classes[i]);
        gtk_widget_set_halign(GTK_WIDGET(label), GTK_ALIGN_START);
        gtk_grid_attach(app->instr_grid, label, 1, i + 1, 1, 1);

        app->instr_buttons[i] = GTK_CHECK_BUTTON(gtk_check_button_new());
        gtk_widget_set_halign(GTK_WIDGET(app->instr_buttons[i]), GTK_ALIGN_END);
        gtk_grid_attach(app->instr_grid, GTK_WIDGET(app->instr_buttons[i]), 2, i + 1, 1, 1);
    }


    /* Result spinbutton */
    frame = gtk_frame_new("Max result");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->result_spinbutton = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(1, 100, 1));
    gtk_spin_button_set_value(app->result_spinbutton, 25);

    gtk_box_pack_start(box, GTK_WIDGET(app->result_spinbutton), TRUE, TRUE, 0);

    /* Search button */
    app->search_button = GTK_BUTTON(gtk_button_new_with_label("Search"));
    gtk_box_pack_start(app->search_box, GTK_WIDGET(app->search_button), FALSE, FALSE, 0);
    g_signal_connect_swapped(app->search_button, "clicked", G_CALLBACK(search_event), app);


    /* Songboxes notebook (Tabsview) */
    app->song_notebook = GTK_NOTEBOOK(gtk_notebook_new());
    gtk_widget_set_hexpand(GTK_WIDGET(app->song_notebook), TRUE);
    gtk_box_pack_start(app->win_box, GTK_WIDGET(app->song_notebook), TRUE, TRUE, 0);


    /* Add song boxes */
    app->songboxes[SONGBOX_ALPHA] = songbox_new(app->song_notebook, "Alphabetical");
    app->songboxes[SONGBOX_FPRNT] = songbox_new(app->song_notebook, "Fingerprint");
    app->songboxes[SONGBOX_BEST]  = songbox_new(app->song_notebook, "Best Match");
    app->songboxes[SONGBOX_POP]   = songbox_new(app->song_notebook, "Popularity");
    app->songboxes[SONGBOX_NEW]   = songbox_new(app->song_notebook, "Newest");


    /* Show all widgets */
    gtk_widget_show_all(GTK_WIDGET(app->window));


    /* Allocate song queue */
    app->songs[SONGBOX_ALPHA]   = g_queue_new();
    app->songs[SONGBOX_FPRNT]   = g_queue_new();
    app->songs[SONGBOX_BEST]    = g_queue_new();
    app->songs[SONGBOX_POP]     = g_queue_new();
    app->songs[SONGBOX_NEW]     = g_queue_new();


    return app;
}

gboolean key_event(GtkWidget *widget, GdkEventKey *event)
{
    /* Activate search button with return key */
    if (event->keyval == GDK_KEY_Return) {
        gtk_button_clicked(app->search_button);
    }

    return FALSE;
}
