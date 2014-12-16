#include "midiogre-search.h"
#include "midiogre-app.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

#include <string.h>
#include <math.h>

gint search_songs(MidiogreApp *app)
{

    sqlite3 *db;

    gchar *sql_base;

    gint instr_classes;
    const gchar *artist_value;
    const gchar *album_value;
    const gchar *title_value;


    sqlite3_open("mid.db", &db);



    /* Gather search criterias */
    instr_classes = read_instr_classes(app);

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

    sql_base = g_strdup_printf("SELECT * FROM songs WHERE artist LIKE '%%%s%%' AND album LIKE '%%%s%%' AND title LIKE '%%%s%%';",
                               artist_value,
                               album_value,
                               title_value);





    search_alpha(app, db, sql_base);
    //search_best(app,sql);
    //search_finger(app,sql);
    //search_pop(app,sql);
    //search_date(app,sql);



    g_free(sql_base);
    sqlite3_close(db);



    songboxes_update(app);


    return 0;
}

int search_alpha(MidiogreApp *app, sqlite3 *db, gchar *base_sql)
{
    char *error = 0;

    sqlite3_exec(db, base_sql, search_handler, app->songs, &error);

    return 0;
}

int search_best(MidiogreApp app, gchar base_sql);
int search_finger(MidiogreApp app, gchar base_sql);
int search_pop(MidiogreApp app, gchar base_sql);
int search_date(MidiogreApp app, gchar base_sql);

gint search_handler(void *s, int argc, char **argv, char **col_name)
{
    song_t *song;
    GQueue *songs = s;

    song = calloc(1, sizeof(song_t));

    /* Load columns for song row */

    /* Inforce max string length ARTIST_MAX_NAME */
    argv[0][ARTIST_MAX_NAME] = '\0';
    strcpy(song->artist, argv[0]);

    /* Inforce max string length ALBUM_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->album,  argv[1]);

    song->num = atoi(argv[2]);

    /* Inforce max string length TITLE_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->title,  argv[3]);

    song->length = atoi(argv[5]);

    song->plays = atoi(argv[6]);

    /* Push column to queue */
    g_queue_push_tail(songs, song);

    return 0;
}

gint read_instr_classes(MidiogreApp *app)
{
    gint i;
    gint rv = 0;

    for (i = 0; i < INSTR_CLASSES; i++) {

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->instr_buttons[i]))) {

            rv += pow(2, i);
        }
    }

    return rv;
}
