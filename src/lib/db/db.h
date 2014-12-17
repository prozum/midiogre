/** @db.h*/

#ifndef __DB_H__
#define __DB_H__

#include <analyze/analyze.h>

#include <sqlite3.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define ARTIST_MAX_NAME 64
#define ALBUM_MAX_NAME 64
#define TITLE_MAX_NAME 64

/** song_t - Struct containing song variables from database */
typedef struct {
    char artist[ARTIST_MAX_NAME]; /**< Name of artist                    */
    char album[ALBUM_MAX_NAME];   /**< Name of album                     */
    char title[TITLE_MAX_NAME];   /**< Name of song                      */
    uint32_t num;                 /**< Track number                      */
    uint32_t length;              /**< Length of song                    */
    uint32_t plays;               /**< Number of plays                   */
    f_prn_t *finger_prints;       /**< Fingerprint value                 */
    int16_t edit_score;           /**< Score used to sort by fingerprint */
    uint32_t time_added;          /**< Time when added                   */
    char *addr;                   /**< Song file addr                    */
} song_t;

int database_open_error (int rc, sqlite3 *db);

int database_general_error (int rc, char *error, int type);

int db_init(sqlite3 *db);

int db_import_mid(sqlite3 *db, char *mid_addr);

int db_song_handler(void *s, int argc, char **argv, char **col_name);

int db_read_songs(sqlite3 *db, song_t *songs);

int check_sql(const char *sql);

void parse_filename (char *file_name, char *artist, char *album, unsigned *num, char *title);

#endif
