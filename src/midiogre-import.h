/** @file midiogre-import.h */

#ifndef __MIDIOGRE_IMPORT_H__
#define __MIDIOGRE_IMPORT_H__

#include <list/list.h>

#include <gtk/gtk.h>
#include <sqlite3.h>

/** ImportStatus - Struct containing information about import progress */
typedef struct
{
    sqlite3 *db;                  /**< Items db            */
    guint i;                      /**< Items done          */
    guint n;                      /**< Items total         */
    gboolean stop;                /**< Import stop status  */
    GQueue *queue;                /**< Items queue         */
    GtkWidget *dialog;            /**< Progress dialog     */
    GtkProgressBar *progress_bar; /**< Progress bar widget */
} ImportStatus;

/**
 * @brief mid_import
 * @param status:
 * @return
 */
void mid_import(ImportStatus *status);

/**
 * @brief folder_handler
 * @param folder_addr
 * @param mid_addrs
 * @return
 */
int folder_handler(char* folder_addr, GQueue *mid_addrs);

/**
 * @brief progress_dialog_update
 * @param s
 * @return
 */
gboolean progress_dialog_update(gpointer s);

/**
 * @brief import_dialog
 * @param window
 * @param queue
 * @return
 */
ImportStatus *import_dialog(GtkWindow *window, GQueue *queue);

/**
 * @brief import_cancel
 * @param status
 */
gboolean import_cancel(GtkDialog *dialog, gint *id, ImportStatus *status);

/**
 * @brief folder_chooser
 * @param window
 */
void folder_chooser(GtkWindow *window);

#endif
