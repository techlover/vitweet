#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-box.h"

static void destroy (GtkWidget*,gpointer);
static void read_keys(char **, char **);
char consu[] = "zoCegrqiH5zv7dBJoLNw";
char consu_secret[] = "e9xprZdGKksRiQtVcSrsS9RnlOZenBGkptUApmjxs";
char *key = NULL;
char *secret = NULL;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    read_keys(&key, &secret);
    set_consumer_keys(consu, consu_secret);
    set_access_keys(key, secret);
    GtkWidget *window;
    TweetBox *tweet_box = create_tweet_box();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");
    

    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);

    gtk_container_add (GTK_CONTAINER (window), tweet_box->vbox);

    gtk_widget_set_size_request (window, 420, 150);


    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}


static void read_keys(char **key, char **secret)
{
    gchar *filename, *content;
    gsize bytes;
    GError *error = NULL;

    filename = g_build_filename (g_get_current_dir(), "keys", NULL);

    if(!g_file_test (filename, G_FILE_TEST_EXISTS))
        g_error("Error: File does not exist!");

    g_file_get_contents(filename, &content, &bytes, &error);
    if(parse_reply_access(content, key, secret))
        g_error("Error: Can't read file");

    g_free(content);
    g_free(filename);
}


static void destroy(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}
