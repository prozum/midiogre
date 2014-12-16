#include "midiogre-app.h"
#include "midiogre-import.h"
#include "midiogre-song.h"
#include "midiogre-search.h"

#include "mid/mid.h"
#include "mid/mid-str.h"

#include <gtk/gtk.h>

#include <string.h>

MidiogreApp *midiogre_app_init(void)
{
    MidiogreApp *app;

    GtkWidget *header;
    GtkWidget *scrolled;
    GtkBox *box;
    GtkWidget *label,*frame;
    GtkWidget *button;
    GtkWidget *align;

    gint i;


    /* Allocate app */
    app = calloc(1, sizeof(MidiogreApp));

    /* Setup window */
    app->window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
    gtk_window_set_default_size(GTK_WINDOW(app->window), 600, 400);
    g_signal_connect(app->window, "destroy",
                      G_CALLBACK(gtk_widget_destroyed), &app->window);

    /* Setup header bar */
    header = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR(header), TRUE);
    gtk_header_bar_set_title (GTK_HEADER_BAR(header), "Midiogre");
    gtk_window_set_titlebar(app->window, header);

    /* Setup folder button */
    button = gtk_button_new();
    gtk_container_add (GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/document-open-symbolic.symbolic.png"));
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), button);

    g_signal_connect_swapped(button, "clicked", G_CALLBACK(folder_chooser), app->window);


    /* Setup backward, play & forward buttons in a box */
    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_style_context_add_class(gtk_widget_get_style_context(GTK_WIDGET(box)), "linked");

    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/media-skip-backward-symbolic.symbolic.png"));
    gtk_container_add(GTK_CONTAINER(box), button);

    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/media-playback-start-symbolic.symbolic.png"));
    gtk_container_add(GTK_CONTAINER(box), button);

    button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(button), gtk_image_new_from_resource("/org/prozum/midiogre/media-skip-forward-symbolic.symbolic.png"));
    gtk_container_add(GTK_CONTAINER(box), button);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), GTK_WIDGET(box));


    /* Global horizontal box */
    app->win_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(app->window), GTK_WIDGET(app->win_box));


    /* Scroll container for sidepanel */
    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(app->win_box, scrolled, TRUE, TRUE, 0);


    /* Sidepanel box */
    app->panel_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_container_add(GTK_CONTAINER (scrolled), GTK_WIDGET(app->panel_box));
    gtk_container_set_border_width(GTK_CONTAINER(app->panel_box), 10);


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
    gtk_box_pack_start(box, GTK_WIDGET(app->title_entry), TRUE, TRUE, 0);


    /* Artist entry */
    frame = gtk_frame_new("Artist");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->artist_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(app->artist_entry), TRUE, TRUE, 0);


    /* Album entry */
    frame = gtk_frame_new("Album");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);

    box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));
    gtk_container_set_border_width(GTK_CONTAINER(box), 10);

    app->album_entry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(box, GTK_WIDGET(app->album_entry), TRUE, TRUE, 0);


    /* Instrument class buttons */
    frame = gtk_frame_new("Instrument Classes");
    gtk_box_pack_start(app->search_box, frame, FALSE, FALSE, 0);
    app->instr_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_container_set_border_width(GTK_CONTAINER(app->instr_box), 10);
    gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(app->instr_box));

    for (i = 0; i < INSTR_CLASSES; i++) {

        box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0));
        gtk_box_pack_start(app->instr_box, GTK_WIDGET(box), FALSE, FALSE, 0);

        label = gtk_label_new(instrument_classes[i]);
        gtk_box_pack_start(box, label, FALSE, FALSE, 0);

        align = gtk_alignment_new(1,0,1,0);
        gtk_box_pack_start(box, align, TRUE, TRUE, 0);

        app->instr_buttons[i] = GTK_CHECK_BUTTON(gtk_check_button_new());
        gtk_box_pack_start(box, GTK_WIDGET(app->instr_buttons[i]), FALSE, FALSE, 0);
    }


    /* Search button */
    app->search_button = GTK_BUTTON(gtk_button_new_with_label("Search"));
    gtk_box_pack_start(app->search_box, GTK_WIDGET(app->search_button), FALSE, FALSE, 0);
    g_signal_connect_swapped(app->search_button, "clicked", G_CALLBACK(search_songs), app);

    /* Result spinbutton */
    app->result_spinbutton = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(1, 50, 1));
    gtk_box_pack_start(app->search_box, GTK_WIDGET(app->result_spinbutton), FALSE, FALSE, 0);
    gtk_spin_button_set_value(app->result_spinbutton, 8);

    /* Playlist */
    /* TODO */

    /* Favorites */
    /* TODO */

    /* Add song boxes */
    app->songbox_alpha = songbox_new(app->win_box, "<span size=\"large\">Alphabetical Order</span>", (GtkListBoxSortFunc)song_row_sort);
    app->songbox_date  = songbox_new(app->win_box, "<span size=\"large\">Date Order</span>", (GtkListBoxSortFunc)song_row_sort);
    app->songbox_match = songbox_new(app->win_box, "<span size=\"large\">Match Order</span>", (GtkListBoxSortFunc)song_row_sort);

    gtk_widget_show_all(GTK_WIDGET(app->window));


    /* Allocate song queue */
    app->songs = g_queue_new();


    return app;
}
