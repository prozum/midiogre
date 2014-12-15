#define _GNU_SOURCE

#include "midiogre-import.h"

#include <list/list.h>
#include <db/db.h>

#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

gpointer mid_import(gpointer s)
{
    ImportStatus *status;
    sqlite3 *db;
    char *mid_addr;

    status = s;

    /* Open db */
    sqlite3_open("mid.db", &db);
    db_init(db);

    while ((mid_addr = g_queue_pop_head(status->queue)) != NULL) {

        g_print("Import mid: %s\n", mid_addr);

        /* Clever function to handle mid file here! */
        db_import_mid(db, mid_addr);

        g_free(mid_addr);

        status->i++;
    }

    sqlite3_close(db);

    return NULL;
}

#ifdef WIN32
int folder_handler(char* folder_addr, GQueue *mid_addrs)
{
    WIN32_FIND_DATA file;
    HANDLE hFind;

    DWORD error;

    char *tmp, *folder_addr_fixed;


    folder_addr_fixed = g_strdup_printf("%s\\*", folder_addr);


    if ((hFind = FindFirstFile(folder_addr_fixed, &file)) == INVALID_HANDLE_VALUE) {

        if ((error = GetLastError()) == ERROR_FILE_NOT_FOUND) {

            g_print("win: folder not found: %s\n", folder_addr_fixed);

        } else {

            g_print("win: unknown error %d: %s\n", error, folder_addr_fixed);

        }

        return -1;
    }

    do
    {
       /* Don't try to open hidden or previous folders */
       if (file.cFileName[0] != '.') {

           /* If folder */
           if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

               tmp = g_strdup_printf("%s\\%s", folder_addr, file.cFileName);
               g_print("win: next folder: %s\n", tmp);

               folder_handler(tmp, mid_addrs);
               g_free(tmp);

               g_print("win: back folder: %s\n",folder_addr);

           /* If file with ".mid" extention */
           } else if (strcmp(file.cFileName + strlen(file.cFileName) - 4, ".mid") == 0) {

               tmp = g_strdup_printf("%s\\%s", folder_addr, file.cFileName);

               /* Add address to mid_addr */
               g_queue_push_tail(mid_addrs, tmp);
           }
       }
    }
    while (FindNextFile(hFind, &file) != 0);

    /* Cleanup */
    g_free(folder_addr_fixed);
    FindClose(hFind);

    return 0;
}
#else
int folder_handler(char* folder_addr, GQueue *mid_addrs)
{
    DIR *directory = NULL;
    struct dirent *file;
    char *tmp;

   if ((directory = opendir(folder_addr)) == NULL) {

       return -1;
   }

   while ((file = readdir(directory)) != NULL) {

       /* Don't try to open hidden or previous folders */
       if (file->d_name[0] != '.') {

           /* If folder */
           if (file->d_type == DT_DIR) {

               tmp = g_strdup_printf("%s/%s", folder_addr, file->d_name);
               g_print("unix: next folder: %s\n", tmp);

               folder_handler(tmp, mid_addrs);
               g_free(tmp);

               g_print("unix: back folder: %s\n",folder_addr);

           /* If file with ".mid" extention */
           } else if (g_strcasecmp(file->d_name + strlen(file->d_name) - 4, ".mid") == 0) {

               tmp = g_strdup_printf("%s/%s", folder_addr, file->d_name);

               /* Add address to mid_addr */
               g_queue_push_tail(mid_addrs, tmp);

           }
       }
   }

   if (closedir(directory) < 0) {

       return -1;
   }

   return 0;
}
#endif

gboolean progress_dialog_update(gpointer s)
{
    ImportStatus *status;

    float frac;
    char *tmp;

    status = s;

    /* Update progress bar */
    if (status->n != 0) {

        frac = (float)status->i / (float)status->n;
        gtk_progress_bar_set_fraction(status->progress_bar, frac);

        tmp = g_strdup_printf("%d of %d files imported", status->i, status->n);

    } else {

        gtk_progress_bar_set_fraction(status->progress_bar, 1);
        tmp = g_strdup_printf("No mid files in folder!", status->i, status->n);
    }

    gtk_progress_bar_set_text(status->progress_bar, tmp);
    g_free(tmp);

    if (status->i == status->n) {
        return FALSE;
    } else {
        return TRUE;
    }
}

ImportStatus *import_dialog(GtkWindow *window, GQueue *queue)
{
    ImportStatus *status;
    GtkWidget *content_area;


    status = malloc(sizeof(ImportStatus));
    status->i = 0;
    status->n = queue->length;
    status->queue = queue;

    /* Setup progress dialog */
    status->dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "Importing files");

    g_signal_connect (status->dialog, "response",
                      G_CALLBACK(gtk_widget_destroy), status->dialog);

    /* Setup progress bar */
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (status->dialog));
    status->progress_bar = GTK_PROGRESS_BAR(gtk_progress_bar_new());
    gtk_progress_bar_set_show_text(status->progress_bar, TRUE);
    gtk_box_pack_start(GTK_BOX(content_area), GTK_WIDGET(status->progress_bar), FALSE, FALSE, 0);

    gtk_widget_show_all(status->dialog);

    /* Update progressbar every 100 ms */
    g_timeout_add(100, progress_dialog_update, status);


    return status;
}

void folder_chooser(GtkWindow *window)
{
    GtkWidget      *dialog;
    ImportStatus *status;
    gint res;
    char *folder_addr = NULL;

    GQueue *mid_addrs = NULL;

    dialog = gtk_file_chooser_dialog_new("Pick a Folder",
                                         window,
                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                         ("_Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         ("_Open"),
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);


    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {

        folder_addr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        gtk_widget_destroy(dialog);

        g_print("dialog: folder: %s\n",folder_addr);

        /* Create list to store mid files adresses */
        mid_addrs = g_queue_new();

        /* Find mid files in folder_addr */
        folder_handler(folder_addr, mid_addrs);
        g_free(folder_addr);

        /* Setup progress dialog */
        status = import_dialog(window, mid_addrs);

        /* Import mid files */
        g_thread_new("mid_import", mid_import, status);

    }

}
