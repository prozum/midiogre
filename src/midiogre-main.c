#include "midiogre-gui.h"
#include "midiogre-mid.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    window_init();

    gtk_main();

    return 0;
}
