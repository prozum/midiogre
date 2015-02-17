#include "midiogre-app.h"
#include "midiogre-search.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    /* Windows hack to fix schemas (No GSettings schemas are installed on the system) */
#ifdef WIN32
    g_setenv("GSETTINGS_SCHEMA_DIR", "share/glib-2.0/schemas/", TRUE);
#endif

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
