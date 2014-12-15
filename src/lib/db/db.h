#include <sqlite3.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    char artist[64];
    char album[64];
    char title[64];
    uint32_t track_num;
    uint32_t length;
    uint32_t plays;
    uint8_t fingerprint[21];
    uint32_t time_added;
} song_t;

int database_open_error (int rc, sqlite3 *db);

int database_general_error (int rc, char *error, int type);

int db_init(sqlite3 *db);

int db_import_mid(sqlite3 *db, char *mid_addr);

int db_song_handler(void *s, int argc, char **argv, char **col_name);

int db_read_songs(sqlite3 *db, song_t *songs);

int check_sql(char *sql);

void parse_filename (char *file_name, char *artist, char *album, unsigned *num, char *title);
