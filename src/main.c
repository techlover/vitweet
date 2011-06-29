#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"

static void destroy (GtkWidget*,gpointer);
static void send_text(GtkWidget *, GtkTextView*);
static void read_keys(char **, char **);
static void update_num_carac(GtkWidget *, GdkEvent *, GtkLabel *);
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
    GtkWidget *tweet_text, *scrolled_win, *send_tweet, *vbox, *window,
              *send_and_status_hbox, *num_carac, *send_dm,
              *bottom_hbox;
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");


    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);


    tweet_text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_text), TRUE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tweet_text), GTK_WRAP_WORD);
    

    scrolled_win = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win), 
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_container_add (GTK_CONTAINER (scrolled_win), tweet_text);


    send_tweet = gtk_button_new_with_label("Send");

    num_carac = gtk_label_new("140");

    send_and_status_hbox = gtk_hbox_new(TRUE, 3);
    
    gtk_box_pack_start (GTK_BOX (send_and_status_hbox), num_carac, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (send_and_status_hbox), send_tweet, TRUE, TRUE, 0);

    send_dm = gtk_button_new_with_label ("Send DM");

    bottom_hbox = gtk_hbox_new (FALSE, 220);

    gtk_box_pack_start (GTK_BOX (bottom_hbox), send_dm, TRUE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX (bottom_hbox), send_and_status_hbox, TRUE, TRUE, 5);



    g_signal_connect (G_OBJECT (send_tweet), "clicked",
            G_CALLBACK (send_text), (gpointer)tweet_text);



    vbox = gtk_vbox_new(FALSE, 10);

    gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX (vbox), bottom_hbox, FALSE, FALSE, 2);

    gtk_container_add (GTK_CONTAINER (window), vbox);

    gtk_widget_set_size_request (window, 420, 150);
    g_signal_connect(G_OBJECT (tweet_text), "key-release-event",
            G_CALLBACK(update_num_carac), (gpointer)num_carac);

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

static void send_text(GtkWidget *button, GtkTextView *tweet_text)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;

    buffer = gtk_text_view_get_buffer(tweet_text);
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
    send_tweet(tweet);
    gtk_text_buffer_set_text(buffer, "", 0);

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

static void update_num_carac
(GtkWidget *tweet_text, GdkEvent *event, GtkLabel *num_carac)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;
    gchar remaining[5];
    int left = 0;
    int total = 140;

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(tweet_text));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);

    left = total - strlen(tweet);
    sprintf(remaining, "%d", left);
    gtk_label_set_text(num_carac, remaining);
}


static void destroy(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}
