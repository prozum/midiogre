#include "midiogre-mid.h"

#include <mid/mid.h>
#include <mid/mid-str.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

/** Import mid struct to tree view */
void mid_import(GFile *mid_gfile, GtkWidget *notebook)
{
    uint32_t i, j, k;
    uint32_t len;
    char tmp[3];
    char *data_str[NUM_COLUMNS];

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
                                      G_TYPE_STRING,
                                      G_TYPE_STRING);

        /* For each event in track */
        for (j = 0; j < mid->track[i].events; j++) {

            /* Convert data to hex */
            data_str[COLUMN_MSG] = g_strdup_printf("%x",mid->track[i].event[j].msg);
            data_str[COLUMN_BYTE_1] = g_strdup_printf("%x",mid->track[i].event[j].byte_1);
            data_str[COLUMN_BYTE_2] = g_strdup_printf("%x",mid->track[i].event[j].byte_2);
            data_str[COLUMN_DELTA] = g_strdup_printf("%x",mid->track[i].event[j].delta);
            
            /* If channel message */ 
            if (mid->track[i].event[j].msg >= NOTE_OFF &&
                mid->track[i].event[j].msg <= PITCH_BEND) { 
                
                data_str[COLUMN_CHAN] = g_strdup_printf("%x",mid->track[i].event[j].chan);
            } else {
                data_str[COLUMN_CHAN] = g_strdup_printf("N/A");
            }

            
            /* If meta message */
            if (mid->track[i].event[j].msg == META_MSG) {

                /* Find string length */
                len = mid->track[i].event[j].byte_2 * 3;

                data_str[COLUMN_DATA] = (char *)g_malloc((len + 1) * sizeof(char));
                strcpy(data_str[COLUMN_DATA],"");

                /* For each byte in meta data */
                for (k = 0; k < mid->track[i].event[j].byte_2; k++) {

                    /* Convert Data to hex */
                    if (mid->track[i].event[j].data[k] > 0xF) {
                        g_sprintf(tmp,"%x ",mid->track[i].event[j].data[k]);
                    } else {
                        g_sprintf(tmp,"0%x ",mid->track[i].event[j].data[k]);
                    }
                    strcat(data_str[COLUMN_DATA],tmp);
                }
            /* Not meta message */
            } else{
                data_str[COLUMN_DATA] = g_strdup_printf("N/A");
            }

            /* Put strings in store */
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               COLUMN_MSG,    data_str[COLUMN_MSG],
                               COLUMN_BYTE_1, data_str[COLUMN_BYTE_1],
                               COLUMN_BYTE_2, data_str[COLUMN_BYTE_2],
                               COLUMN_CHAN,   data_str[COLUMN_CHAN],
                               COLUMN_DELTA,  data_str[COLUMN_DELTA],
                               COLUMN_DATA,   data_str[COLUMN_DATA],
                               -1);

            /* Deallocate strings */
            for (k = 0; k < NUM_COLUMNS; k++){
                free(data_str[k]);
            }
        }

        /* Create model from store */
        model = GTK_TREE_MODEL(store);

        /* Create tree view */
        treeview = gtk_tree_view_new_with_model(model);

        /* Message column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Message",
                                                          renderer,
                                                          "text",
                                                          COLUMN_MSG,
                                                          NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Para_1 column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Byte 1",
                                                          renderer,
                                                          "text",
                                                          COLUMN_BYTE_1,
                                                           NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Para_2 column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Byte 2",
                                                          renderer,
                                                          "text",
                                                          COLUMN_BYTE_2,
                                                          NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Channel column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Channel",
                                                          renderer,
                                                          "text",
                                                          COLUMN_CHAN,
                                                          NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
       
        /* Delta column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Delta Time",
                                                          renderer,
                                                          "text",
                                                          COLUMN_DELTA,
                                                          NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        /* Data column */
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Data",
                                                          renderer,
                                                          "text",
                                                          COLUMN_DATA,
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
                                 gtk_label_new(g_strdup_printf("Track %i",i+1)));
    }
    free_mid(mid);
}

/** Export tree view data to mid struct */
void mid_export(mid_t *mid);
