#include "midiogre-app.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    MidiogreApp *app;

    song_t *song1, *song2, *song3;
    SongRow *row1, *row2, *row3;

    gtk_init(&argc, &argv);

    /* Setup MidiogreApp */
    app = midiogre_app_init();


    song1 = song_new();
    song2 = song_new();
    song3 = song_new();

    row1 = song_row_new(song1);
    row2 = song_row_new(song2);
    row3 = song_row_new(song3);


    gtk_widget_show (GTK_WIDGET (row1));
    gtk_container_add (GTK_CONTAINER (app->songbox_alpha), GTK_WIDGET (row1));

    gtk_widget_show (GTK_WIDGET (row2));
    gtk_container_add (GTK_CONTAINER (app->songbox_new), GTK_WIDGET (row2));

    gtk_widget_show (GTK_WIDGET (row3));
    gtk_container_add (GTK_CONTAINER (app->songbox_best), GTK_WIDGET (row3));

    gtk_main();

    return 0;
}
