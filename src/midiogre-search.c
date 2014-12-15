#include "midiogre-search.h"
#include "midiogre-app.h"

#include <gtk/gtk.h>

#include <string.h>
#include <math.h>

gint search_songs(MidiogreApp *app)
{
    char *error = 0;

    sqlite3 *db;

    gchar *sql;
    gchar *tmp[5];





    gint instr_classes;
    const gchar *artist_value;
    const gchar *album_value;
    const gchar *title_value;



    sql = "SELECT * FROM songs;";


    instr_classes = read_instr_classes(app);


    tmp[1] =g_strdup_printf("");

    artist_value = gtk_entry_get_text(app->artist_entry);
    if (check_sql(artist_value) == -1) {

        return -1;
    }



    album_value = gtk_entry_get_text(app->album_entry);
    if (check_sql(album_value) == -1) {

        return -1;
    }

    title_value = gtk_entry_get_text(app->title_entry);
    if (check_sql(title_value) == -1) {

        return -1;
    }



    sqlite3_open("mid.db", &db);

    sqlite3_exec(db, sql, search_handler, app, &error);

    sqlite3_close(db);



    update_songboxes(app);
    //db_export_songs(sqlite3 *db, sql, songs);


    return 0;
}

int search_handler(void *a, int argc, char **argv, char **col_name)
{
    int i;
    song_t *song;
    MidiogreApp *app = a;

    song = calloc(1, sizeof(song_t));

    g_queue_push_tail(app->songs, song);

    strcpy(song->artist, argv[0]);
    strcpy(song->album,  argv[1]);
    song->num = (argv[2]);
    strcpy(song->title,  argv[3]);
    song->length = atoi(argv[5]);
    song->plays = atoi(argv[6]);

    return 0;
}

gint read_instr_classes(MidiogreApp *app)
{
    gint i;
    gint rv = 0;

    for (i = 0; i < INSTR_CLASSES; i++) {

        if (gtk_toggle_button_get_active(app->instr_buttons[i])) {

            rv += pow(2, i);
        }
    }

    return rv;
}
