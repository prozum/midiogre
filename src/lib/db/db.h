/** @file db.h*/

#ifndef __DB_H__
#define __DB_H__

#include <analyze/analyze.h>

#include <sqlite3.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/** Maximum length of artist name */
#define ARTIST_MAX_NAME 64

/** Maximum length of album name */
#define ALBUM_MAX_NAME 64

/** Maximum length of song title name */
#define TITLE_MAX_NAME 64

/** song_t - Struct containing song variables from database */
typedef struct {
    uint32_t id;                  /**< Id of song in database            */
    char artist[ARTIST_MAX_NAME]; /**< Name of artist                    */
    char album[ALBUM_MAX_NAME];   /**< Name of album                     */
    char title[TITLE_MAX_NAME];   /**< Name of song                      */
    uint32_t num;                 /**< Track number                      */
    uint32_t length;              /**< Length of song                    */
    uint32_t plays;               /**< Number of plays                   */
    uint32_t time_added;          /**< Time when added                   */
    f_prn_t *fprns;               /**< Fingerprints                      */
    float sort_score;           /**< Score used to sort songs          */
    char *addr;                   /**< Song file addr                    */
} song_t;

/**
 * @brief database_open_error
 * @param rc: Stores returned error
 * @param db: Database file
 * @return
 */
int database_open_error (int rc, sqlite3 *db);

/**
 * @brief database_general_error
 * @param rc: Stores returned error
 * @param error: Contained error message
 * @param type: Type of error
 * @return
 */
int database_general_error (int rc, char *error, int type);

/**
 * @brief db_init
 * @param db: Database file
 * @return
 */
int db_init(sqlite3 *db);

/**
 * @brief db_import_mid
 * @param db: Database file
 * @param mid_addr: file path to mid file
 * @return
 */
int db_import_mid(sqlite3 *db, char *mid_addr);

/**
 * @brief db_song_handler
 * @param s:
 * @param argc: Argument vector
 * @param argv: Argument Count
 * @param col_name: Name of column
 * @return
 */
int db_song_handler(void *s, int argc, char **argv, char **col_name);

/**
 * @brief db_import_mid
 * @param db: Database file
 * @param mid_addr: File path to mid file
 * @return
 */
int db_import_mid(sqlite3 *db, char *mid_addr);

/**
 * @brief check_sql
 * @param sql: String
 * @return
 */
int check_sql(const char *sql);

/**
 * @brief parse_filename
 * @param file_name: Name of file
 * @param artist: Name of artist
 * @param album: Name of album
 * @param num: track number
 * @param title: Name of song
 */
void parse_filename (char *file_name, char *artist, char *album, unsigned *num, char *title);

/**
 * @brief db_increment_plays
 * @param id: ID in database
 */
void db_increment_plays(uint32_t id);

/**
 * @brief db_delete_song
 * @param id: ID in database
 */
void db_delete_song(uint32_t id);

#endif
