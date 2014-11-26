#include "midiogre-window.h"
#include "midiogre-mid.h"

#include <gtk/gtk.h>

struct _MidiogreAppWindow
{
  GtkApplicationWindow parent;
};

struct _MidiogreAppWindowClass
{
  GtkApplicationWindowClass parent_class;
};

struct _MidiogreAppWindowPrivate
{
    GtkWidget *stack;
};

G_DEFINE_TYPE_WITH_PRIVATE(MidiogreAppWindow, midiogre_app_window, GTK_TYPE_APPLICATION_WINDOW)

static void midiogre_app_window_init(MidiogreAppWindow *win)
{
    gtk_widget_init_template(GTK_WIDGET(win));
}

static void midiogre_app_window_class_init(MidiogreAppWindowClass *class)
{
    /* Load ui file */
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),
                                                "/org/gtk/midiogre/midiogre.ui");

    /* Pointer to stack */
    gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MidiogreAppWindow, stack);
}


MidiogreAppWindow *midiogre_app_window_new(MidiogreApp *app)
{
  return g_object_new(MIDIOGRE_APP_WINDOW_TYPE, "application", app, NULL);
}

void midiogre_app_window_open(MidiogreAppWindow *win, GFile *file)
{
    MidiogreAppWindowPrivate *priv;
    gchar *basename;

    GtkWidget *notebook;

    priv = midiogre_app_window_get_instance_private(win);
    basename = g_file_get_basename(file);

    notebook = gtk_notebook_new();

    gtk_widget_show(notebook);

    mid_import(file,notebook);

    /* Add file to stack */
    gtk_stack_add_titled(GTK_STACK(priv->stack), notebook, basename, basename);

    g_free(basename);
}
