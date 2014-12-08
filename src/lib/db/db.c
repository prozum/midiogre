#include "db.h"

#include <stdio.h>
#include <stdlib.h>

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
char parse_filename (FILE *file) 
{
    char artist[32], album[32], track[32];
    int tracknum, file_pnt;
    char file1[20]="a - b - 3 - d";

    //file_pnt = strrchr(file1ile, '/');

    sscanf(file, "%s - %s - %i - %s",artist,album,&tracknum,track);

    //printf("%s\n\n", artist);
    printf("%s\n\n", file);
    return 0;
}
