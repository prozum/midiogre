#include "midiogre-gui.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *listbox1, *listbox2, *listbox3;

    gtk_init(&argc, &argv);

    window = window_init();
    box = window_box_init(window);

    /* Add list boxes */
    listbox1 = listbox_new(box);
    listbox2 = listbox_new(box);
    listbox3 = listbox_new(box);
    gtk_widget_show_all(box);


    /* Listbox add test row */
    //gtk_container_add (GTK_CONTAINER (listbox), GTK_WIDGET (row));


    gtk_main();

    return 0;
}
