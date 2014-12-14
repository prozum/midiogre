#include "midiogre-gui.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GlobalWidgets *g_wgs;
    GtkWidget *box;

    Song *song1, *song2, *song3;
    SongRow *row1, *row2, *row3;

    gtk_init(&argc, &argv);

    /* Setup window */
    window = window_init();

    /* Setup global widgets */
    g_wgs = g_wgs_init(window);


    song1 = song_new();
    song2 = song_new();
    song3 = song_new();

    row1 = song_row_new(song1);
    row2 = song_row_new(song2);
    row3 = song_row_new(song3);


    gtk_widget_show (GTK_WIDGET (row1));
    gtk_container_add (GTK_CONTAINER (g_wgs->songbox[0]), GTK_WIDGET (row1));

    gtk_widget_show (GTK_WIDGET (row2));
    gtk_container_add (GTK_CONTAINER (g_wgs->songbox[1]), GTK_WIDGET (row2));

    gtk_widget_show (GTK_WIDGET (row3));
    gtk_container_add (GTK_CONTAINER (g_wgs->songbox[2]), GTK_WIDGET (row3));

    gtk_main();

    return 0;
}
