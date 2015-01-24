#include "midiogre-search.h"
#include "midiogre-app.h"
#include "midiogre-song.h"

#include <pop/pop.h>

#include <gtk/gtk.h>

#include <string.h>
#include <math.h>

gint search_event(void)
{
    sqlite3 *db;
    gint page;
    gchar *sql_head;

    gint instr_classes;
    const gchar *artist_value;
    const gchar *album_value;
    const gchar *title_value;
    gint limit;

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

    sql_head = g_strdup_printf("SELECT rowid,artist,album,num,title,time,plays,strftime('%%s', import_date),finger_print1,finger_print2,finger_print3,addr FROM songs WHERE artist LIKE '%%%s%%' AND album LIKE '%%%s%%' AND title LIKE '%%%s%%' AND (%d & instr_classes) = %d",
                               artist_value,
                               album_value,
                               title_value,
                               instr_classes,
                               instr_classes);


    /* Get max result from spinbox */
    limit = gtk_spin_button_get_value_as_int(app->result_spinbutton);

    /* Open db */
    sqlite3_open("mid.db", &db);

    /* Act according to current page */
    page = gtk_notebook_get_current_page(app->song_notebook);

    switch (page) {

        case SONGBOX_ALPHA:
            /* Load songs into app->songs */
            search_db(app->songs[SONGBOX_ALPHA], db, sql_head, "%s ORDER BY title,album,artist LIMIT %d;", limit);

            break;

        case SONGBOX_FPRNT:
            /* Only update if a favorite song is selected */
            if (app->cur_fav == NULL) {
                /* Cleanup */
                g_free(sql_head);
                sqlite3_close(db);
                return -1;
            }

            /* Load songs into app->songs */
            search_db(app->songs[SONGBOX_FPRNT], db, sql_head, "%s ORDER BY title,album,artist;", 0);

            /* Calculate sort score */
            g_queue_foreach(app->songs[SONGBOX_FPRNT], (GFunc)song_score_fprnt, app->cur_fav);

            /* Sort according to sort score */
            g_queue_sort(app->songs[SONGBOX_FPRNT], (GCompareDataFunc)sort_score, app->cur_fav);

            break;

        case SONGBOX_BEST:
            /* Only update if a favorite song is selected */
            if (app->cur_fav == NULL) {
                /* Cleanup */
                g_free(sql_head);
                sqlite3_close(db);
                return -1;
            }

            /* Load songs into app->songs */
            search_db(app->songs[SONGBOX_BEST], db, sql_head, "%s ORDER BY title,album,artist;", 0);

            /* Calculate sort score */
            g_queue_foreach(app->songs[SONGBOX_BEST], (GFunc)song_score_best, app->cur_fav);

            /* Sort according to sort score */
            g_queue_sort(app->songs[SONGBOX_BEST], (GCompareDataFunc)sort_score, app->cur_fav);

            break;

        case SONGBOX_POP:
            /* Load songs into app->songs */
            search_db(app->songs[SONGBOX_POP], db, sql_head, "%s ORDER BY title,album,artist;", 0);

            /* Calculate sort score */
            g_queue_foreach(app->songs[SONGBOX_POP], (GFunc)song_score_pop, NULL);

            /* Sort according to sort score */
            g_queue_sort(app->songs[SONGBOX_POP], (GCompareDataFunc)sort_score, NULL);

            break;

        case SONGBOX_NEW:
            /* Load songs into app->songs */
            search_db(app->songs[SONGBOX_NEW], db, sql_head, "%s ORDER BY import_date DESC LIMIT %d;", limit);

            break;
    }

    /* Cleanup */
    g_free(sql_head);
    sqlite3_close(db);

    /* Deallocate unused songs */
    while (app->songs[page]->length > limit) {
        song_free(g_queue_pop_tail(app->songs[page]));
    }

    /* Update all songboxes in current page */
    songbox_update(app->songboxes[page], app->songs[page], limit);

    return 0;
}

int search_db(GQueue *songs, sqlite3 *db, gchar *head, gchar *body, gint limit)
{
    gint rc;
    gchar *sql;
    gchar *error = 0;


    sql = g_strdup_printf(body,
                          head,
                          limit);


    rc = sqlite3_exec(db, sql, search_handler, songs, &error);

    database_general_error(rc, error, 2);

    free(sql);

    return 0;
}



gint search_handler(void *s, int argc, char **argv, char **col_name)
{
    song_t *song;
    GQueue *songs = s;
    uint32_t fingerprints[3];
    song = calloc(1, sizeof(song_t));

    /* Load columns for song row */
    song->id = atoi(argv[0]);

    /* Inforce max string length ARTIST_MAX_NAME */
    argv[0][ARTIST_MAX_NAME] = '\0';
    strcpy(song->artist, argv[1]);

    /* Inforce max string length ALBUM_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->album,  argv[2]);

    song->num = atoi(argv[3]);

    /* Inforce max string length TITLE_MAX_NAME */
    argv[1][ALBUM_MAX_NAME] = '\0';
    strcpy(song->title,  argv[4]);

    song->length = atoi(argv[5]);

    song->plays = atoi(argv[6]);

    song->time_added = atoi(argv[7]);

    fingerprints[0] = atoi(argv[8]);
    fingerprints[1] = atoi(argv[9]);
    fingerprints[2] = atoi(argv[10]);
    song->fprns = convert_to_f_prn(fingerprints);

    song->addr = malloc(strlen(argv[11])+1);
    strcpy(song->addr,argv[11]);

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
