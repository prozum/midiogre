#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include <mid/mid.h>
#include <mid/mid-str.h>

#include <database/db.c>
#include <database/db.h>

#ifdef _WIN32
#include <win/asprintf.h>
#endif

int main(int argc, char* argv[])
{
    mid_t *mid;
    
    sqlite3 *db;
    
    int  rc;
    unsigned int i,j;
    char *sql, *sql2;
    char *error = 0;
    FILE *file;

    /* Open mid file */
    file = fopen(argv[1],"rb");
    if(file == NULL) {
        perror(argv[1]);
        return -1;
    }

    /* Read content */
    mid = read_mid(file);
    fclose(file);

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }
    
    /* Write database structure */
    sql = "CREATE TABLE midiFile("       \
          "PARA1        UNSIGNED INT,"   \
          "PARA2        UNSIGNED INT,"   \
          "DELTA        UNSIGNED INT);";
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &error);

    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    /* For each track in mid */
    for (i = 0; i < mid->tracks; i++) {
        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {
    
            /* If meta message */
            if (mid->track[i].event[j].msg == NOTE_ON) {
    
                asprintf(&sql2, "INSERT INTO midiFile(PARA1,PARA2,DELTA) \
                                VALUES (%u, %u, %u);",mid->track[i].event[j].para_1, \
                                                      mid->track[i].event[j].para_2, \
                                                      mid->track[i].event[j].delta);
                
                rc = sqlite3_exec(db, sql2, callback, 0, &error);
                
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", error);
                    sqlite3_free(error);
                } else {
                    fprintf(stdout, "Data inserted successfully\n");
                }
                
                free(sql2);
            }
        }
    }
    
    sqlite3_close(db);    
 
    return 0;
}
