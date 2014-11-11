#include "midiogre-app.h"
#include "midiogre-window.h"

#include <gtk/gtk.h>

struct _MidiogreApp
{
    GtkApplication parent;
};

struct _MidiogreAppClass
{
    GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(MidiogreApp, midiogre_app, GTK_TYPE_APPLICATION)

static void
midiogre_app_init (MidiogreApp *app)
{
}

/** Is called when midiogre is launched with no commandline arguments */
static void
midiogre_app_activate (GApplication *app)
{
    MidiogreAppWindow *win;

    win = midiogre_app_window_new (MIDIOGRE_APP (app));
    gtk_window_present (GTK_WINDOW (win));
}

/** Is called when midiogre is launched with commandline arguments */
static void
midiogre_app_open (GApplication  *app,
                  GFile        **files,
                  gint           n_files,
                  const gchar   *hint)
{
    GList *windows;
    MidiogreAppWindow *win;
    int i;

    windows = gtk_application_get_windows (GTK_APPLICATION (app));
    if (windows)
        win = MIDIOGRE_APP_WINDOW (windows->data);
    else
        win = midiogre_app_window_new (MIDIOGRE_APP (app));

    for (i = 0; i < n_files; i++)
        midiogre_app_window_open(win, files[i]);

    gtk_window_present(GTK_WINDOW(win));
}

static void
midiogre_app_class_init (MidiogreAppClass *class)
{
    /* Overwrite default activate function */
    G_APPLICATION_CLASS(class)->activate = midiogre_app_activate;

    /* Overwrite default open function */
    G_APPLICATION_CLASS(class)->open = midiogre_app_open;
}


MidiogreApp *
midiogre_app_new(void)
{
    return g_object_new (MIDIOGRE_APP_TYPE,
         "application-id", "org.prozum.Midiogre",
         "flags", G_APPLICATION_HANDLES_OPEN,
         "inactivity-timeout", 12000,
         "register-session", TRUE,
          NULL);
}


