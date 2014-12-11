#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

int callback(void *NotUsed, int argc, char **argv, char **azColName);

int database_open_error (int rc, sqlite3 *db);

int database_general_error (int rc, char *error, int type);

int db_init(sqlite3 *db);

int db_import_mid(sqlite3 *db, char *mid_addr);

void parse_filename (char *file_name, char *artist, char *album, int *trackNum, char *trackName);
