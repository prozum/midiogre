#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>


static int callback(void *NotUsed, int argc, char **argv, char **azColName);
