#include "midiogre-app.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gint retval;
    MidiogreApp *app;

    /* Needed for Glib older than 2.36 (Windows) */
    //g_type_init();

    app = midiogre_app_new();
    
    retval = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);

    return retval;
}
