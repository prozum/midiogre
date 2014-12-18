#include "midiogre-app.h"
#include "midiogre-search.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    /* Init gtk */
    gtk_init(&argc, &argv);

    /* Setup MidiogreApp */
    app = midiogre_app_init();

    /* Populate songboxes with songs */
    search_event();

    /* Run gtk main loop */
    gtk_main();

    return 0;
}
