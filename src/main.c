#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-input.h"
#include "tweet-box.h"

static void destroy (GtkWidget*,gpointer);
static void read_keys(char **, char **);
char consu[] = "zoCegrqiH5zv7dBJoLNw";
char consu_secret[] = "e9xprZdGKksRiQtVcSrsS9RnlOZenBGkptUApmjxs";
char *key = NULL;
char *secret = NULL;

TweetInput *tweet_input;
TweetBox *tweet_box[20];


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    micro_init();
    read_keys(&key, &secret);
    set_consumer_keys(consu, consu_secret);
    set_access_keys(key, secret);
    GtkWidget *window, *vbox;
    tweet_input = create_tweet_input();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");
    

    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);

    vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), tweet_input->vbox, TRUE, TRUE, 2);
    //gtk_container_add (GTK_CONTAINER (window), tweet_input->vbox);
    
    int i;
    GList *list = NULL;
    list = get_home_timeline();
    Tweet *tweet;
    tweet = g_slice_new(Tweet);
    for(i = 0; i<g_list_length(list); i++){
        tweet = (Tweet *)g_list_nth_data (list, i);
        tweet_box[i] = create_tweet_box(tweet->text);
        gtk_box_pack_start(GTK_BOX(vbox), tweet_box[i]->vbox, TRUE, TRUE, 2);
    }





    gtk_container_add (GTK_CONTAINER (window), vbox);

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
    g_slice_free(TweetInput, tweet_input);
    gtk_main_quit();
}
