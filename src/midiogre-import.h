#ifndef __MIDIOGRE_IMPORT_H__
#define __MIDIOGRE_IMPORT_H__

#include <list/list.h>

#include <gtk/gtk.h>

typedef struct
{
    guint i;
    guint n;
    GQueue *queue;
    GtkWidget *dialog;
    GtkProgressBar *progress_bar;
} ImportStatus;

int mid_handler(char* mid_addr, list_t *mid_addrs);
int folder_handler(char* folder_addr, GQueue *mid_addrs);

ImportStatus *import_dialog(GtkWindow *window, GQueue *queue);
void folder_chooser(GtkWindow *window);


#endif
