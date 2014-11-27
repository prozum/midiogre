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
    
    int rc;
    unsigned int i,j;
    char *sql, *sql2;
    char *error = 0;
    char testfile = "/home/bo/bono-money4bono-1-helptheblacks";
    char *s;
    FILE *file;

    /* Open mid file */
    file = fopen(argv[1],"rb");
    if(file == NULL) {
        perror(argv[1]);
        return -1;
    }
    printf("%c, %d", db, rc);
    /* Read content */
    mid = read_mid(file);
    fclose(file);

    /* Open database */
    rc = sqlite3_open("test.db", &db);

	//database_open_error(rc, db);

    /* Write database structure */
    sql = "CREATE TABLE midiFile("          \
          "ARTIST                  CHAR,"   \
          "ALBUM                   CHAR,"   \
          "TRACKNUM        UNSIGNED INT,"   \
          "TRACK                   CHAR);";
    
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &error);

    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    s = strrchr(testfile, '/');
    printf("%s",s);

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
