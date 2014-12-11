#include "db.h"

#include <mid/mid.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libgen.h>
#include <sqlite3.h>

int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    putchar('\n');

    return 0;
}

int database_open_error (int rc, sqlite3 *db)
{
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
        return 0;
    }
}

int database_general_error (int rc, char *error, int type)
{

    if (rc != SQLITE_OK) {

        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
        return -1;

    }

    switch(type) {

        case 1:
            fprintf(stdout, "Table created successfully\n");
            break;

        case 2:
            fprintf(stdout, "Table changed successfully\n");
            break;

        case 3:
            fprintf(stdout, "Data selected successfully\n");
            break;

        default:
            fprintf(stdout, "The operation completed successfully, no idea which though\n");
    }

    return 0;
}

int db_init(sqlite3 *db)
{
    char *sql, *error = 0;
    int rc;

    /* Write database structure */
    sql = "CREATE TABLE IF NOT EXISTS midiFile ("          \
          "ARTIST                  VARCHAR(32),"   \
          "ALBUM                   VARCHAR(32),"   \
          "TRACKNUM               UNSIGNED INT,"   \
          "TRACK                   VARCHAR(32),"   \
          "INSTRUMENTS            UNSIGNED INT,"   \
          "Fp1                         CHAR(7),"   \
          "Fp2                         CHAR(7),"   \
          "Fp3                         CHAR(7),"   \
          "UploadTime             UNSIGNED INT);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &error);

    /* Table error check */
    database_general_error(rc, error, 1);

    return rc;
}

int db_import_mid(sqlite3 *db, char *mid_addr)
{
    FILE * mid_file;
    mid_t *mid;

    char *sql, *error = 0;
    int rc;

    char artist[32];
    char album[32];
    char track_name[32];
    int  track_num;

    /* Open mid file */
    if ((mid_file = fopen(mid_addr, "rb")) == NULL) {

        perror(mid_addr);
        return -1;
    }

    /* Read midi content */
    if ((mid = read_mid(mid_file)) == NULL) {

        fprintf(stderr, "Could not read mid!\n");
        fclose(mid_file);
        return -1;
    }
    fclose(mid_file);

    /* Exec data import */
    parse_filename(mid_addr, artist, album, &track_num, track_name);
    sql = g_strdup_printf("INSERT INTO midiFile (ARTIST, ALBUM, TRACKNUM, TRACK) VALUES ('%s', '%s', %d, '%s');",
                                          artist,
                                          album,
                                          track_num,
                                          track_name);

    rc = sqlite3_exec(db, sql, callback, 0, &error);
    free(sql);

    database_general_error(rc, error, 2);

    return rc;
}

/* Parse file name for song data */
void parse_filename (char *file_name, char *artist, char *album, int *track_num, char *track_name)
{
    char *file_pnt;

    /* Removes file path */
    file_pnt = basename(file_name);

    if (strchr(file_pnt,'-') != NULL) {

        sscanf(file_pnt, "%[^-]- %[^-]- %d- %s", artist, album, track_num, track_name);

    } else {

        strcpy(track_name, file_pnt);
        strcpy(artist, "N/A");
        strcpy(album, "N/A");
        *track_num=0;
    }

    /* Removes file extension from last string */
    track_name[strlen(track_name)-4] = '\0';
}
