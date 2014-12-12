#include "midiogre-gui.h"
#include "midiogre-mid.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *listbox1, *listbox2, *listbox3;

    gtk_init(&argc, &argv);

    window = window_init();

    /* Setup box */
    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_container_add (GTK_CONTAINER (window), box);

    /* Add list boxes */
    listbox1 = listbox_setup(box);
    listbox2 = listbox_setup(box);
    listbox3 = listbox_setup(box);
    gtk_widget_show_all(box);

    gtk_main();

    return 0;
}
