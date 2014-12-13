#include "db.h"

#include <mid/mid.h>
#include <ext/ext.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glib.h>
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
    sql = "CREATE TABLE IF NOT EXISTS `songs` ("  \
          "`artist`                  VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`album`                   VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`num`                    UNSIGNED INT DEFAULT 0,"                                                \
          "`title`                   VARCHAR(64) DEFAULT \'N/A\',"                                          \
          "`instr_classes`          UNSIGNED INT DEFAULT 0,"                                                \
          "`length`                 UNSIGNED INT DEFAULT 0,"                                                \
          "`plays`                      UNSIGNED DEFAULT 0,"                                                \
          "`import_date`               timestamp DEFAULT CURRENT_TIMESTAMP,"                                \
          "`finger_prints`              CHAR(21) DEFAULT \'\',"   \
          "`addr`                  VARCHAR(1024) DEFAULT \'N/A\');";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &error);

    /* Table error check */
    database_general_error(rc, error, 1);

    return rc;
}

int db_import_mid(sqlite3 *db, char *mid_addr)
{
    FILE * mid_file;
    mid_t *mid, *mid_tmp;

    char *sql, *error = 0;
    int rc;

    char artist[32];
    char album[32];
    char track_name[32];
    int  track_num;
    int instr_classes;

    /* Open mid file */
    if ((mid_file = fopen(mid_addr, "rb")) == NULL) {

        perror(mid_addr);
        return -1;
    }

    /* Read midi content */
    if ((mid_tmp = read_mid(mid_file)) == NULL) {

        fprintf(stderr,"Could not read mid!\n");
        fclose(mid_file);
        return -1;
    }
    fclose(mid_file);

    /* Merge mid tracks */
    mid = merge_tracks(mid_tmp);

    /* Instruments */
    instr_classes = extract_instr_classes(mid);

    free_mid(mid);
    free_mid(mid_tmp);

    /* Exec data import */
    parse_filename(mid_addr, artist, album, &track_num, track_name);
    sql = g_strdup_printf("INSERT INTO songs (artist, album, num, title, instr_classes, addr) VALUES ('%s', '%s', %d, '%s', %d, '%s');",
                          artist,
                          album,
                          track_num,
                          track_name,
                          instr_classes,
                          mid_addr);

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

    /* Removes file extension from last string */
    file_pnt[strlen(file_pnt)-4] = '\0';

    if (4 != sscanf(file_pnt, "%[^-]- %[^-]- %d- %s",
                    artist,
                    album,
                    track_num,
                    track_name)) {

        strcpy(track_name, file_pnt);
        strcpy(artist, "N/A");
        strcpy(album, "N/A");
        *track_num = 0;
    }

}
