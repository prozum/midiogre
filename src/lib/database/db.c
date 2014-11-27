#include <stdio.h>
#include <stdlib.h>
#include "db.h"

#ifdef _WIN32
#include <win/asprintf.h>
#endif

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