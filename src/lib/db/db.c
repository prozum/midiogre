#include "db.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqlite3.h>

int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for(i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    putchar('\n');

    return 0;
}

int database_open_error (int rc, sqlite3 *db)
{
    if(rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    } else {
        fprintf(stdout, "Opened database successfully\n");
        return 0;
    }
}

int database_general_error (int rc, char *error, int type) {
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
        return -1;

    }
    switch(type) {
        case 1:
            fprintf(stdout, "Table created successfully\n");
            break;
        case 2:
            fprintf(stdout, "Table created successfully\n");
            break;
        case 3:
            fprintf(stdout, "Data selected successfully\n");
            break;
        default:
        	fprintf(stdout, "The operation completed successfully, no idea which though\n");
    }
    return 0;
}

/* Parse file name for song data */
void parse_filename (char *file_name, char *artist, char *album, int *trackNum, char *trackName)
{
    char *file_pnt;
    file_pnt = basename(file_name); /* Removes file path */

    if (strchr(file_pnt,'-')!=NULL) {
        sscanf(file_pnt, "%[^-]- %[^-]- %d- %s",artist,album,trackNum,trackName);
    } else {
        trackName=file_pnt; artist="N/A"; album="N/A"; *trackNum=0;
    }

    /* Removes file extension from last string */
    trackName[strlen(trackName)-4]=0;

    printf("%s\n\n", artist);
    printf("%s\n\n", album);
    printf("%d\n\n", *trackNum);
    printf("%s\n\n", trackName);
}
