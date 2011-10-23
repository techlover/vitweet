#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "libgsocial.h"
#include "vitweet-tweet-input-gtk.h"
#include "vitweet-request-key-dialog.h"
#include "vitweet-webkit-tweet-column-gtk.h"
#include "vitweet-treeview-tweet-column-gtk.h"
#include <glib/gi18n.h>

char consu[] = "zoCegrqiH5zv7dBJoLNw";
char consu_secret[] = "e9xprZdGKksRiQtVcSrsS9RnlOZenBGkptUApmjxs";
char *key = NULL;
char *secret = NULL;

TweetInput *tweet_input;


static void read_keys(char **key, char **secret)
{
    gchar *filename, *content;
    gchar *path;
    gsize bytes;

    GError *error = NULL;

    filename = g_build_filename (g_get_current_dir(), "keys", NULL);

    content = NULL;

    if(!g_file_test (filename, G_FILE_TEST_EXISTS)) {
        path = g_build_path ("/", g_get_home_dir(), ".vitweet", NULL);
        filename = g_build_filename (path, "keys", NULL);
    }
    if(!g_file_test (filename, G_FILE_TEST_EXISTS)) {
        create_request_key_dialog(&content);
        g_mkdir(path, 0700);
        if(content)
            g_file_set_contents(filename, content, strlen(content), &error);
        else
            g_error("PIN not entered");
    }


    g_file_get_contents(filename, &content, &bytes, &error);

    if(gsocial_parse_reply_access(content, key, secret))
        g_error("Error: Can't read file");

    g_free(content);
    g_free(filename);
    g_free(path);
}


static void destroy(GtkWidget *window, gpointer data)
{
    g_slice_free(TweetInput, tweet_input);
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    gsocial_init();
    gsocial_set_consumer_keys(consu, consu_secret);
    read_keys(&key, &secret);
    gsocial_set_access_keys(key, secret);
    GtkWidget *window, *treeview, *scrolled_win, *vbox, *tweet_list;

    tweet_input = create_tweet_input();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");
    

    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);

    vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), tweet_input->vbox, FALSE, FALSE, 2);

    int i;
    GList *list = NULL;
    list = gsocial_get_home_timeline(NULL);
    

    /* Create a webkit based column */
    /*tweet_list = create_webkit_column(list);*/

    /* Create a treeview based column */
    treeview = create_treeview_column(list);

    scrolled_win = gtk_scrolled_window_new (NULL, NULL);

    gtk_container_add (GTK_CONTAINER (scrolled_win), treeview);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 2);


    gtk_container_add (GTK_CONTAINER (window), vbox);

    gtk_widget_set_size_request (window, 400, 700);


    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

