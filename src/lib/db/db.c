#include "db.h"

#ifdef _WIN32
#include <win/asprintf.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

int callback(void *data, int argc, char **argv, char **azColName)
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
        return 0;
    }
}

int database_general_error (int rc, char *error, int type){
    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", error);
        sqlite3_free(error);
        return -1;
    } else {    
    	if (type==1){
        	fprintf(stdout, "Table created successfully\n");
    	} else if (type==2){
    	    fprintf(stdout, "Data inserted successfully\n");
    	} else if (type==3) {
    		fprintf(stdout, "Data selected successfully\n");
    	} else {
            fprintf(stdout, "The operation completed successfully, no idea which though\n");
        }

        return 0;
    }
}
/* Parse file name for song data */
char parse_filename (char *file_name, char *artist, char *album, int *trackNum, char *trackName) {
    char *file_pnt;
    file_pnt = basename(file_name); /* Removes file path */

    sscanf(file_pnt, "%[^-]- %[^-]- %d- %s",artist,album,&trackNum,trackName);

    /* Removes file extension from last string */
    trackName[strlen(trackName)-4]=0;
/*
    printf("%s\n\n", artist);
    printf("%s\n\n", album);
    printf("%i\n\n", trackNum);
    printf("%s\n\n", trackName); */
}
