#include "midiogre-mid.h"

#include <mid/mid.h>
#include <mid/mid-util.h>

#include <gtk/gtk.h>

/** Setup tree view */
/*
static GtkTreeView *
create_event_tree_view()
{

}
*/

/** Import mid struct to tree view */
void
mid_import(GFile *mid_gfile, GtkWidget *treeview)
{
    int i;
    FILE *mid_file;
    mid_t *mid;

    GtkTreeModel *model = NULL;
    GtkListStore *store;
    GtkTreeIter iter;

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    /* Open mid file */
    mid_file = fopen( g_file_get_path(mid_gfile),"rb" );

    /* Read content */
    mid = read_mid(mid_file);
    fclose(mid_file);

    /* Setup column data types */
    store = gtk_list_store_new (NUM_COLUMNS,
                                  G_TYPE_UINT,
                                  G_TYPE_UINT,
                                  G_TYPE_UINT,
                                  G_TYPE_UINT,
                                  G_TYPE_STRING);

    for (i = 0; i < mid->track[0].events; i++) {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            COLUMN_TYPE,   mid->track[0].event[i].msg,
                            COLUMN_PARA_1, mid->track[0].event[i].para_1,
                            COLUMN_PARA_2, mid->track[0].event[i].para_2,
                            COLUMN_DELTA,  mid->track[0].event[i].delta,
                            COLUMN_MDATA, "N/A",
                            -1);
    }

    /* Create model from store */
    model = GTK_TREE_MODEL(store);

    /* Create tree view */
    //treeview = gtk_tree_view_new_with_model(model);
    gtk_tree_view_set_model(treeview,model);

    /* Type column */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Type",
                                                         renderer,
                                                         "text",
                                                         COLUMN_TYPE,
                                                         NULL);
    gtk_tree_view_append_column (treeview, column);

    /* Para_1 column */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Para 1",
                                                         renderer,
                                                         "text",
                                                         COLUMN_PARA_1,
                                                         NULL);
    gtk_tree_view_append_column (treeview, column);

    /* Para_2 column */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Para 2",
                                                         renderer,
                                                         "text",
                                                         COLUMN_PARA_2,
                                                         NULL);
    gtk_tree_view_append_column (treeview, column);

    /* Delta column */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Delta",
                                                         renderer,
                                                         "text",
                                                         COLUMN_DELTA,
                                                         NULL);
    gtk_tree_view_append_column (treeview, column);

    /* MData column */
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("MData",
                                                         renderer,
                                                         "text",
                                                         COLUMN_MDATA,
                                                         NULL);
    gtk_tree_view_append_column (treeview, column);

    /* Show tree view */
    gtk_widget_show(treeview);

}

/** Export tree view data to mid struct */
void mid_export(mid_t *mid);
