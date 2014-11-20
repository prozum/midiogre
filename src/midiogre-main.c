#include "midiogre-app.h"
#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gint retval;
    MidiogreApp *app;

    /* Create GTJ aoo */
    app = midiogre_app_new();
    
    /* Run GTK app */
    retval = g_application_run(G_APPLICATION(app), argc, argv);
    
    /* Deallocate app */
    g_object_unref(app);

    return retval;
}
