#include "midiogre-window.h"

#include <gtk/gtk.h>

struct _MidiogreAppWindow
{
  GtkApplicationWindow parent;
};

struct _MidiogreAppWindowClass
{
  GtkApplicationWindowClass parent_class;
};

G_DEFINE_TYPE(MidiogreAppWindow, midiogre_app_window, GTK_TYPE_APPLICATION_WINDOW);

static void
midiogre_app_window_init (MidiogreAppWindow *app)
{
}

static void
midiogre_app_window_class_init (MidiogreAppWindowClass *class)
{
}


MidiogreAppWindow *
midiogre_app_window_new (MidiogreApp *app)
{
  return g_object_new (MIDIOGRE_APP_WINDOW_TYPE, "application", app, NULL);
}

void
midiogre_app_window_open (MidiogreAppWindow *win,
                         GFile            *file)
{
}
