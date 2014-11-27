#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>


static int callback(void *NotUsed, int argc, char **argv, char **azColName);

int database_open_error (int rc, sqlite3 *db);

