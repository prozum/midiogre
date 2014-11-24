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
/*
int midiWrite(mid_t *mid){
    unsigned int i, j;
    char *sql2;

    for (i = 0; i < mid->tracks; i++)
    {
         For each event in track */ /*
        for (j = 0; j < mid->track[i].events; j++) {

            *//* If meta message */ /*
            if (mid->track[i].event[j].msg >= NOTE_ON_1 &&
                mid->track[i].event[j].msg <= NOTE_ON_16) {

                asprintf(&sql2,"INSERT INTO midiFile(PARA1,PARA2,DELTA) \
                                VALUES (%u, %u, %u);",mid->track[i].event[j].para_1, \
                                                      mid->track[i].event[j].para_2, \
                                                      mid->track[i].event[j].delta);

                }
           }
      }
    return sql2;
}
*/
int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql, *sql2;
    FILE *file;
    mid_t *mid;
    unsigned int i,j;

    /* Open mid file */
    file = fopen(argv[1],"rb");

    /* Read content */
    mid = read_mid(file);
    fclose(file);

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stdout, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "CREATE TABLE midiFile("       \
         "PARA1        UNSIGNED INT,"   \
         "PARA2        UNSIGNED INT,"   \
         "DELTA        UNSIGNED INT);";

   for (i = 0; i < mid->tracks; i++){
       /* For each event in track */
       for (j = 0; j < mid->track[i].events; j++) {

           /* If meta message */
           if (mid->track[i].event[j].msg >= NOTE_ON_1 &&
               mid->track[i].event[j].msg <= NOTE_ON_16) {

               asprintf(&sql2,"INSERT INTO midiFile(PARA1,PARA2,DELTA) \
                               VALUES (%u, %u, %u);",mid->track[i].event[j].para_1, \
                                                     mid->track[i].event[j].para_2, \
                                                     mid->track[i].event[j].delta);
               rc = sqlite3_exec(db, sql2, callback, 0, &zErrMsg);
               free(sql2);
           }
       }
   }

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);

   return 0;
}
