#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-input.h"


TweetInput *create_tweet_input()
{
    TweetInput *tweet_input = g_slice_new(TweetInput);
    
    tweet_input->tweet_text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_input->tweet_text), TRUE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tweet_input->tweet_text), GTK_WRAP_WORD);
    
    gtk_widget_set_size_request(tweet_input->tweet_text, 200, 200);

    tweet_input->scrolled_win = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (tweet_input->scrolled_win), 
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    g_signal_connect(G_OBJECT (tweet_input->tweet_text), "key-release-event",
            G_CALLBACK(update_num_carac), (gpointer)tweet_input);

    gtk_container_add (GTK_CONTAINER (tweet_input->scrolled_win), tweet_input->tweet_text);


    tweet_input->send_tweet = gtk_button_new_with_label("Send");

    tweet_input->num_carac = gtk_label_new("140");

    tweet_input->send_and_status_hbox = gtk_hbox_new(TRUE, 3);
    
    gtk_box_pack_start (GTK_BOX (tweet_input->send_and_status_hbox), tweet_input->num_carac, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (tweet_input->send_and_status_hbox), tweet_input->send_tweet, FALSE, FALSE, 0);

    tweet_input->send_dm = gtk_button_new_with_label ("Send DM");

    tweet_input->bottom_hbox = gtk_hbox_new (FALSE, 220);

    gtk_box_pack_start (GTK_BOX (tweet_input->bottom_hbox), tweet_input->send_dm, FALSE, FALSE, 5);
    gtk_box_pack_start (GTK_BOX (tweet_input->bottom_hbox), tweet_input->send_and_status_hbox, FALSE, FALSE, 5);



    tweet_input->vbox = gtk_vbox_new(FALSE, 10);
    
    g_signal_connect (G_OBJECT (tweet_input->send_tweet), "clicked",
            G_CALLBACK (send_text), (gpointer)tweet_input);

    
    gtk_box_pack_start (GTK_BOX (tweet_input->vbox), tweet_input->scrolled_win, FALSE, FALSE, 2);
    gtk_box_pack_start (GTK_BOX (tweet_input->vbox), tweet_input->bottom_hbox, FALSE, FALSE, 2);


    return tweet_input;


}

static void send_text(GtkWidget *button, TweetInput *tweet_input)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tweet_input->tweet_text));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);

    if(!send_tweet(tweet))
        g_print("Not Working\n");



    gtk_text_buffer_set_text(buffer, "", 0);
    gtk_label_set_text(GTK_LABEL(tweet_input->num_carac), "140");

}

static void update_num_carac
(GtkWidget *tweet_text, GdkEvent *event, TweetInput *tweet_input)
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
    gtk_label_set_text(GTK_LABEL(tweet_input->num_carac), remaining);
}


