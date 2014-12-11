#include <stdio.h>
#include <stdlib.h>

#include <glib.h>

#include <mid/mid.h>
#include <mid/mid-str.h>

#include <db/db.h>

int main(int argc, char* argv[])
{
    sqlite3 *db;

    int rc;

    /* Open database */
    rc = sqlite3_open("mid.db", &db);

    /* Database open error check */
	database_open_error(rc, db);

    /* Write table structure */
    db_init(db);
    
    /* Import mid to db */
    db_import_mid(db, argv[1]);

    /* Create SQL statement, view data */
    /*
    sql3 = "SELECT * from midiFile";
    rc = sqlite3_exec(db, sql3, callback, (void*)data, &error);

    database_general_error(rc, error, 3);

	*/
	
    sqlite3_close(db);    
 
    return 0;
}
