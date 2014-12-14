#include <pop/pop.h>

#include <sqlite3.h>

#include <stdio.h>
#include <stdlib.h>

int callback(void *NotUsed, int argc, char **argv, char **azColName);

int database_open_error (int rc, sqlite3 *db);

int database_general_error (int rc, char *error, int type);

int db_init(sqlite3 *db);

int db_import_mid(sqlite3 *db, char *mid_addr);

int db_song_handler(void *s, int argc, char **argv, char **col_name);

int db_export_songs(sqlite3 *db, song_t *songs);

void parse_filename (char *file_name, char *artist, char *album, unsigned *num, char *title);
