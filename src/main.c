#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"

static void destroy (GtkWidget*,gpointer);
static void display_text(GtkWidget *, GtkTextView*);
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
    GtkWidget *tweet_text, *update_button, *vbox, *window, *hbuttonbox;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");


    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);


    tweet_text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_text), TRUE);


    hbuttonbox =  gtk_hbutton_box_new();

    update_button = gtk_button_new_with_label("Send");

    g_signal_connect (G_OBJECT (update_button), "clicked",
            G_CALLBACK (display_text), (gpointer)tweet_text);


    gtk_widget_set_size_request (update_button, 40, 40);


    vbox = gtk_vbox_new(FALSE, 5);

    gtk_button_box_set_layout (GTK_BUTTON_BOX (hbuttonbox), GTK_BUTTONBOX_END);
    gtk_container_add (GTK_CONTAINER (hbuttonbox), update_button);

    gtk_box_pack_start(GTK_BOX (vbox), tweet_text, TRUE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX (vbox), hbuttonbox, FALSE, FALSE, 2);

    gtk_container_add (GTK_CONTAINER (window), vbox);

    gtk_widget_set_size_request (window, 640, 150);
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

static void display_text(GtkWidget *button, GtkTextView *tweet_text)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;

    buffer = gtk_text_view_get_buffer(tweet_text);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
    send_tweet(tweet);

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
