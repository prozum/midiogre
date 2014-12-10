#define _GNU_SOURCE

#include <gtk/gtk.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

void folder_chooser(GtkWindow *window);

GtkWidget *window_init(void)
{
    GtkWidget *window = NULL;
    GIcon *icon;

    GtkWidget *header;
    GtkWidget *button;
    GtkWidget *box;
    GtkWidget *image;

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

    /* Setup header bar */
    header = gtk_header_bar_new ();
    gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (header), TRUE);
    gtk_header_bar_set_title (GTK_HEADER_BAR (header), "Midiogre");
    gtk_header_bar_set_has_subtitle (GTK_HEADER_BAR (header), FALSE);

    /* Add folder button */
    button = gtk_button_new ();
    icon = g_themed_icon_new ("document-open-symbolic.symbolic");
    image = gtk_image_new_from_gicon (icon, GTK_ICON_SIZE_BUTTON);
    g_object_unref (icon);

    g_signal_connect_swapped(button, "clicked", G_CALLBACK (folder_chooser), window);


    /* Backward, play & forward buttons */
    gtk_container_add (GTK_CONTAINER (button), image);
    gtk_header_bar_pack_end (GTK_HEADER_BAR (header), button);

    box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_style_context_add_class (gtk_widget_get_style_context (box), "linked");
    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-skip-backward-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-playback-start-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    button = gtk_button_new ();
    gtk_container_add (GTK_CONTAINER (button), gtk_image_new_from_icon_name ("media-skip-forward-symbolic.symbolic", GTK_ICON_SIZE_BUTTON));
    gtk_container_add (GTK_CONTAINER (box), button);

    gtk_header_bar_pack_start (GTK_HEADER_BAR (header), box);

    gtk_window_set_titlebar (GTK_WINDOW (window), header);

    gtk_container_add (GTK_CONTAINER (window), gtk_text_view_new ());

    return window;
}

void mid_import(char* mid_addr)
{
    g_print("mid file: %s \n",mid_addr);
}

int folder_handler(char* folder_addr)
{
    DIR *directory = NULL;
    struct dirent *file;
    char *tmp;

   if ((directory = opendir(folder_addr)) == NULL) {

       return -1;
   }

   while ((file = readdir(directory)) != NULL) {

       /* Don't try to open hidden or previous folders */
       if (file->d_name[0] != '.') {

           /* If folder */
           if (file->d_type == DT_DIR) {

               asprintf(&tmp, "%s/%s", folder_addr, file->d_name);

               g_print("folder: %s\n", tmp);

               folder_handler(tmp);


               free(tmp);
            g_print("folder: %s\n",folder_addr);

           /* If file */
           } else {

               /* Check if file have ".mid" extention */
               if (strcmp(file->d_name + strlen(file->d_name) - 4, ".mid") == 0) {
                   asprintf(&tmp, "%s/%s", folder_addr, file->d_name);

                   mid_import(tmp);

                   free(tmp);
               }
           }
       }
   }


   if (closedir(directory) < 0) {

       return -1;
   }

   return 0;

}

void folder_chooser(GtkWindow *window)
{
    GtkWidget      *dialog;
    gint res;
    char *folder_addr = NULL;

    dialog = gtk_file_chooser_dialog_new("Pick a Folder",
                                         window,
                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                         ("_Cancel"),
                                         GTK_RESPONSE_CANCEL,
                                         ("_Open"),
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);


    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {

        folder_addr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));


        g_print("folder: %s\n",folder_addr);
        folder_handler(folder_addr);

        g_free(folder_addr);
    }

    gtk_widget_destroy(dialog);

}

int main(int argc, char *argv[])
{
    GtkWidget *window = NULL;


    gtk_init(&argc, &argv);

    window = window_init();


    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_widget_destroyed), &window);


    gtk_widget_show_all(window);

    gtk_main();


    return 0;
}
