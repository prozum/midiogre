#include "midiogre-mid.h"

#include <mid/mid.h>
#include <mid/mid-util.h>

#include <gtk/gtk.h>

/** Import mid struct to tree view */
void
mid_import(GFile *mid_gfile, GtkWidget *notebook)
{
    int i,j;
    FILE *mid_file;
    mid_t *mid;
    char *str;

    GtkWidget *treeview, *sw;
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

    for (i = 0; i < mid->tracks; i++)
    {
        /* Setup column data types */
        store = gtk_list_store_new (NUM_COLUMNS,
                                      G_TYPE_UINT,
                                      G_TYPE_UINT,
                                      G_TYPE_UINT,
                                      G_TYPE_UINT,
                                      G_TYPE_STRING);

        for (j = 0; j < mid->track[i].events; j++) {
            gtk_list_store_append (store, &iter);
            gtk_list_store_set (store, &iter,
                                COLUMN_TYPE,   mid->track[i].event[j].msg,
                                COLUMN_PARA_1, mid->track[i].event[j].para_1,
                                COLUMN_PARA_2, mid->track[i].event[j].para_2,
                                COLUMN_DELTA,  mid->track[i].event[j].delta,
                                COLUMN_MDATA,  (char *) mid->track[i].event[j].mdata,
                                -1);
        }

        /* Create model from store */
        model = GTK_TREE_MODEL(store);

        /* Create tree view */
        treeview = gtk_tree_view_new_with_model(model);
        //gtk_tree_view_set_model(treeview,model);

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

        /* Setup scroll container */
        sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_show(sw);
        gtk_widget_set_hexpand(sw, TRUE);
        gtk_widget_set_vexpand(sw, TRUE);

        gtk_container_add(GTK_CONTAINER(sw), treeview );

        str = g_strdup_printf("Track %i",i+1);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebook),
                                        sw,
                                        gtk_label_new (str));
    }
    free_mid(mid);
}

/** Export tree view data to mid struct */
void mid_export(mid_t *mid);
