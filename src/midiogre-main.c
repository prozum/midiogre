#include "midiogre-app.h"
#include "midiogre-song.h"

#include <gtk/gtk.h>

int main(int argc, char *argv[])
{

    gtk_init(&argc, &argv);

    /* Setup MidiogreApp */
    app = midiogre_app_init();


    gtk_main();

    return 0;
}
