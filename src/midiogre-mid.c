#define _GNU_SOURCE

#include "midiogre-mid.h"

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

int mid_import(GtkWindow *window, GQueue *mid_addrs)
{
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *progress_bar;

    sqlite3 *db;

    int n,i = 0;

    char *mid_addr;
    char *tmp;

    /* Setup mid import dialog */
    dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                     GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     "Importing mid files");

    g_signal_connect (dialog, "response",
                      G_CALLBACK(gtk_widget_destroy), dialog);

    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    progress_bar = gtk_progress_bar_new ();
    gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(content_area), progress_bar, FALSE, FALSE, 0);
    gtk_widget_show (progress_bar);

    gtk_widget_show(GTK_WIDGET(dialog));

    /* Open db */
    sqlite3_open("mid.db", &db);
    db_init(db);

    /* Get number of mid addrs */
    n = g_queue_get_length(mid_addrs);

    if (n == 0) {

        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1);

        tmp = g_strdup_printf("No mid files in folder...");
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), tmp);
        g_free(tmp);

    } else {

        while ((mid_addr = g_queue_pop_head(mid_addrs)) != NULL) {


            /* Update progress bar */
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), i / n);

            /* Update progress bar text */
            tmp = g_strdup_printf("%d of %d mid files imported...", i, n);
            gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), tmp);
            g_free(tmp);

            /* Clever function to handle mid file here! */
            db_import_mid(db, mid_addr);

            g_print("Import %s", mid_addr);
            g_free(mid_addr);

            i++;
        }

        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar), 1);

        tmp = g_strdup_printf("%d of %d mid files imported...", i, n);
        gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress_bar), tmp);
        g_free(tmp);

    }

    sqlite3_close(db);

    return 0;
}

#ifdef WIN32
int folder_handler(char* folder_addr, GQueue *mid_addrs)
{
    WIN32_FIND_DATA file;
    HANDLE hFind;

    DWORD error;

    char *tmp;

    if ((hFind = FindFirstFile(folder_addr, &file)) == INVALID_HANDLE_VALUE) {

        if ((error = GetLastError()) == ERROR_FILE_NOT_FOUND) {

            g_print("win: folder not found: %s\n", folder_addr);

        } else {

            g_print("win: unknown error %d: %s\n", error, folder_addr);

        }

        return -1;
    }

    while (FindNextFile(hFind, &file) != 0) {

       /* If folder */
       if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

           tmp = g_strdup_printf("%s/%s", folder_addr, file.cFileName);
           g_print("win: next folder: %s\n", tmp);

           folder_handler(tmp, mid_addrs);
           g_free(tmp);

           g_print("win: back folder: %s\n",folder_addr);

       /* If file */
       } else {

           /* Check if file have ".mid" extention */
           if (strcmp(file.cFileName + strlen(file.cFileName) - 4, ".mid") == 0) {
               tmp = g_strdup_printf("%s/%s", folder_addr, file.cFileName);

               /* Add address to mid_addr */
               g_queue_push_tail(mid_addrs, tmp);

           }
       }

   }

   if (FindClose(hFind) < 0) {

       return -1;
   }

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

           /* If file */
           } else {

               /* Check if file have ".mid" extention */
               if (strcmp(file->d_name + strlen(file->d_name) - 4, ".mid") == 0) {
                   tmp = g_strdup_printf("%s/%s", folder_addr, file->d_name);

                   /* Add address to mid_addr */
                   g_queue_push_tail(mid_addrs, tmp);

               }
           }
       }
   }

   if (closedir(directory) < 0) {

       return -1;
   }

   return 0;
}
#endif



void folder_chooser(GtkWindow *window)
{
    GtkWidget      *dialog;
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

        g_print("dialog: folder: %s\n",folder_addr);

        /* Create list to store mid files adresses */
        mid_addrs = g_queue_new();

        /* Find mid files in folder_addr */
        folder_handler(folder_addr, mid_addrs);
        g_free(folder_addr);

        /* Import mid files */
        mid_import(window, mid_addrs);

    }

    gtk_widget_destroy(dialog);

}
