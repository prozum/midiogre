#include "midiogre-search.h"
#include "midiogre-app.h"
#include "midiogre-song.h"

#include <pop/pop.h>

#include <gtk/gtk.h>

#include <string.h>
#include <math.h>

gint search_event(MidiogreApp *app)
{

    sqlite3 *db;

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

    sql_head = g_strdup_printf("SELECT artist,album,num,title,time,plays,strftime('%%s', import_date),finger_print1,finger_print2,finger_print3,addr FROM songs WHERE artist LIKE '%%%s%%' AND album LIKE '%%%s%%' AND title LIKE '%%%s%%' AND (%d & instr_classes) = %d",
                               artist_value,
                               album_value,
                               title_value,
                               instr_classes,
                               instr_classes);


    limit = gtk_spin_button_get_value_as_int(app->result_spinbutton);


    sqlite3_open("mid.db", &db);




    search_db(app->songs_alpha, db, sql_head, "%s ORDER BY title,album,artist LIMIT %d;", limit);
    search_db(app->songs_new, db, sql_head, "%s ORDER BY import_date DESC LIMIT %d;", limit);
    search_db(app->songs_pop, db, sql_head, "%s ORDER BY title,album,artist;", 0);


    /* Only add songs to fingerprint songbox if a favorite song is chosen. */
    if (app->cur_fav != NULL) {
        search_db(app->songs_fprnt, db, sql_head, "%s ORDER BY title,album,artist;", 0);
        g_queue_sort(app->songs_fprnt, (GCompareDataFunc)sort_fprnt, app);

        while(app->songs_fprnt->length > limit) {
            g_queue_pop_tail(app->songs_fprnt);
        }
    }

    g_queue_sort(app->songs_pop, (GCompareDataFunc)song_compare_pop, NULL);
    while(app->songs_pop->length > limit) {
        g_queue_pop_tail(app->songs_pop);
    }


    g_free(sql_head);
    sqlite3_close(db);



    //search_best(app,sql);
    //search_finger(app,sql);
    //search_pop(app,sql);
    //search_date(app,sql);






    songbox_update(app->songbox_alpha, app->songs_alpha, limit);
    songbox_update(app->songbox_new, app->songs_new, limit);
    songbox_update(app->songbox_pop, app->songs_pop, limit);
    songbox_update(app->songbox_fprnt, app->songs_fprnt, limit);

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

gint sort_fprnt(gpointer s1, gpointer s2, gpointer a)
{
    song_t *song1 = s1;
    song_t *song2 = s2;
    song_t *song_fav = app->cur_fav;


    if (song1->edit_score == -1) {
        song1->edit_score = finger_prn_cmp(song1->finger_prints, song_fav->finger_prints);
    }

    if (song2->edit_score == -1) {
        song2->edit_score = finger_prn_cmp(song2->finger_prints, song_fav->finger_prints);
    }



    if (song1->edit_score > song2->edit_score) {
        return 1;
    } else if (song1->edit_score == song2->edit_score) {
        return 0;
    } else {
        return -1;
    }



    //return song_compare_pop(a,b);
}
GCompareFunc sort_pop(gconstpointer s1, gconstpointer s2, gpointer a)
{
    return song_compare_pop(s1,s2);
}

gint search_handler(void *s, int argc, char **argv, char **col_name)
{
    song_t *song;
    GQueue *songs = s;
    uint32_t fingerprints[3];
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

    song->length = atoi(argv[4]);

    song->plays = atoi(argv[5]);

    song->time_added = atoi(argv[6]);

    fingerprints[0] = atoi(argv[7]);
    fingerprints[1] = atoi(argv[8]);
    fingerprints[2] = atoi(argv[9]);
    song->finger_prints = convert_to_f_prn(fingerprints);

    song->addr = malloc(strlen(argv[10])+1);
    strcpy(song->addr,argv[10]);

    song->edit_score = -1;

    /*song->time_added*/

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
