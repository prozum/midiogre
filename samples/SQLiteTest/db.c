#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <mid/mid.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[])
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql, *sql2, *sql3;

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE midiFile("  \
         "ARTIST       CHAR(32)," \
         "Album        CHAR(32)," \
         "Name         CHAR(32)," \
         "Track        INT);";

   sql2 = "INSERT INTO midiFile(ARTIST,Album,Name,Track) \
           VALUES ('BONO','Money For Bono','Raining Kush',1);";

   sql3 = "INSERT INTO midiFile(ARTIST,Album,Name,Track) \
           VALUES ('BONO2','Money For Bono','More4me',2);";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   rc = sqlite3_exec(db, sql2, callback, 0, &zErrMsg);
   rc = sqlite3_exec(db, sql3, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
