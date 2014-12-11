#ifndef __MIDIOGRE_MID_H__
#define __MIDIOGRE_MID_H__

#include <list/list.h>

#include <gtk/gtk.h>

int mid_handler(char* mid_addr, list_t *mid_addrs);
int folder_handler(char* folder_addr, GQueue *mid_addrs);

void folder_chooser(GtkWindow *window);


#endif
