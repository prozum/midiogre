#include <stdio.h>
#include <stdlib.h>
#include "db.h"

#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int database_open_error (int rc, sqlite3 *db){
    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }
    return 0;
}
