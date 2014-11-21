#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <mid/mid.h>
#include <mid/mid-util.h>

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
   FILE *mid_file;
   mid_t *mid;

   /* Open file */
   mid_file = fopen(argv[1], "rb");
   if( mid_file == NULL ) {
      perror(argv[1]);
      return -1;
   }
   /* Read mid */
   mid = read_mid(mid_file);

   sqlite3 *db;
   char *zErrMsg = 0;
   int  rc;
   char *sql;

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
         "ARTIST      CHAR(32)  ," \
         "Album       CHAR(32)  ," \
         "Name        CHAR(32)  ," \
         "INSERT INTO midiFile (Name),"  \
         "VALUES ('lol');";
         
         
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);


   /* Deallocate mid */
    free_mid(mid);

    /* Close mid_file */
    fclose(mid_file);

    printf("\n");


   return 0;
}
