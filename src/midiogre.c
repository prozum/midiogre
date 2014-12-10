#include <gtk/gtk.h>


GtkWidget *window_init(void)
{
    GtkWidget *window = NULL;
    GIcon *icon;

    GtkWidget *header;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *image;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

    header = gtk_header_bar_new ();
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header), TRUE);
    gtk_header_bar_set_title (GTK_HEADER_BAR (header), "Midiogre");
    gtk_header_bar_set_has_subtitle (GTK_HEADER_BAR (header), FALSE);

    button = gtk_button_new ();
    icon = g_themed_icon_new ("document-open-symbolic.symbolic");
    image = gtk_image_new_from_gicon (icon, GTK_ICON_SIZE_BUTTON);
    g_object_unref (icon);
    gtk_container_add (GTK_CONTAINER (button), image);
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header), button);

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

    gtk_window_set_titlebar (GTK_WINDOW (window), header);

    gtk_container_add (GTK_CONTAINER (window), gtk_text_view_new ());

    return window;
}



int main(int argc, char *argv[])
{
    GtkWidget *window = NULL;


    gtk_init(&argc, &argv);

    window = window_init();


    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_widget_destroyed), &window);


    gtk_widget_show_all(window);

    gtk_main();


    return 0;
}
