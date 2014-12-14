#include "midiogre-gui.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *songbox1, *songbox2, *songbox3;

    song_t *song1, *song2, *song3;
    SongRow *row1, *row2, *row3;

    gtk_init(&argc, &argv);

    window = window_init();
    box = window_box_init(window);

    /* Add list boxes */
    songbox1 = songbox_new(box);
    songbox2 = songbox_new(box);
    songbox3 = songbox_new(box);
    gtk_widget_show_all(box);

    song1 = song_new();
    song2 = song_new();
    song3 = song_new();

    row1 = song_row_new(song1);
    row2 = song_row_new(song2);
    row3 = song_row_new(song3);


    gtk_widget_show (GTK_WIDGET (row1));
    gtk_container_add (GTK_CONTAINER (songbox1), GTK_WIDGET (row1));

    gtk_widget_show (GTK_WIDGET (row2));
    gtk_container_add (GTK_CONTAINER (songbox2), GTK_WIDGET (row2));

    gtk_widget_show (GTK_WIDGET (row3));
    gtk_container_add (GTK_CONTAINER (songbox3), GTK_WIDGET (row3));

    gtk_main();

    return 0;
}
