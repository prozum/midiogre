#include "midiogre-mid.h"

#include <mid/mid.h>
#include <mid/mid-util.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <gtk/gtk.h>
#include <glib.h>

/** Import mid struct to tree view */
void mid_import(GFile *mid_gfile, GtkWidget *notebook)
{
    uint32_t i, j, k;
    uint32_t len;
    char tmp[3], *str[NUM_COLUMNS];

    FILE *mid_file;
    mid_t *mid;
    
    GtkWidget *treeview,*sw;
    GtkTreeModel *model;
    GtkListStore *store;
    GtkTreeIter iter;

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    /* Open mid file */
    mid_file = fopen(g_file_get_path(mid_gfile),"rb");

    /* Read content */
    mid = read_mid(mid_file);
    fclose(mid_file);

    /* For each track */
    for (i = 0; i < mid->tracks; i++)
    {
        /* Setup column data types */
        store = gtk_list_store_new (NUM_COLUMNS,
                                      G_TYPE_STRING,
                                      G_TYPE_STRING,
                                      G_TYPE_STRING,
                                      G_TYPE_STRING,
                                      G_TYPE_STRING);

        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* Convert data to hex */
            str[0] = g_strdup_printf("%x",mid->track[i].event[j].msg);
            str[1] = g_strdup_printf("%x",mid->track[i].event[j].para_1);
            str[2] = g_strdup_printf("%x",mid->track[i].event[j].para_2);
            str[3] = g_strdup_printf("%x",mid->track[i].event[j].delta);

            /* If meta message */
            if (mid->track[i].event[j].msg == META_MSG){

                /* Find string length */
                len = mid->track[i].event[j].para_2 * 3;

                str[4] = (char *)g_malloc((len + 1) * sizeof(char));
                strcpy(str[4],"");

                /* For each byte in meta data */
                for (k = 0; k < mid->track[i].event[j].para_2; k++) {

                    /* Convert MData to hex */
                    if (mid->track[i].event[j].mdata[k] > 0xF) {
                        g_sprintf(&tmp,"%x ",mid->track[i].event[j].mdata[k]);
                    } else{
                        g_sprintf(&tmp,"0%x ",mid->track[i].event[j].mdata[k]);
                    }
                    strcat(str[4],tmp);
                }
            /* Not meta message */
            } else{
                str[4] = g_strdup_printf("N/A");
            }

            /* Put strings in store */
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               COLUMN_TYPE,   str[0],
                               COLUMN_PARA_1, str[1],
                               COLUMN_PARA_2, str[2],
                               COLUMN_DELTA,  str[3],
                               COLUMN_MDATA,  str[4],
                               -1);

            /* Deallocate strings */
            for (k = 0; k < NUM_COLUMNS; k++){
                free(str[k]);
            }
        }

        /* Create model from store */
        model = GTK_TREE_MODEL(store);

        /* Create tree view */
        treeview = gtk_tree_view_new_with_model(model);

        /* Type column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type",
                                                           renderer,
                                                          "text",
                                                           COLUMN_TYPE,
                                                           NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Para_1 column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Para 1",
                                                           renderer,
                                                          "text",
                                                           COLUMN_PARA_1,
                                                            NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Para_2 column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Para 2",
                                                           renderer,
                                                          "text",
                                                           COLUMN_PARA_2,
                                                           NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Delta column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Delta",
                                                           renderer,
                                                          "text",
                                                           COLUMN_DELTA,
                                                           NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* MData column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("MData",
                                                           renderer,
                                                          "text",
                                                           COLUMN_MDATA,
                                                           NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Show tree view */
        gtk_widget_show(treeview);

        /* Setup scroll container */
        sw = gtk_scrolled_window_new(NULL, NULL);
        gtk_widget_show(sw);
        gtk_widget_set_hexpand(sw, TRUE);
        gtk_widget_set_vexpand(sw, TRUE);

        gtk_container_add(GTK_CONTAINER(sw), treeview );

        gtk_notebook_append_page(GTK_NOTEBOOK(notebook),
                                    sw,
                                    gtk_label_new (g_strdup_printf("Track %i",i+1)));
    }
    free_mid(mid);
}

/** Export tree view data to mid struct */
void mid_export(mid_t *mid);
