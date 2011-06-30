#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-box.h"


TweetBox *create_tweet_box()
{
    TweetBox *tweet_box = g_slice_new(TweetBox);
    
    tweet_box->tweet_text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_box->tweet_text), TRUE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tweet_box->tweet_text), GTK_WRAP_WORD);
    

    tweet_box->scrolled_win = gtk_scrolled_window_new(NULL, NULL);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (tweet_box->scrolled_win), 
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    g_signal_connect(G_OBJECT (tweet_box->tweet_text), "key-release-event",
            G_CALLBACK(update_num_carac), (gpointer)tweet_box);

    gtk_container_add (GTK_CONTAINER (tweet_box->scrolled_win), tweet_box->tweet_text);


    tweet_box->send_tweet = gtk_button_new_with_label("Send");

    tweet_box->num_carac = gtk_label_new("140");

    tweet_box->send_and_status_hbox = gtk_hbox_new(TRUE, 3);
    
    gtk_box_pack_start (GTK_BOX (tweet_box->send_and_status_hbox), tweet_box->num_carac, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (tweet_box->send_and_status_hbox), tweet_box->send_tweet, TRUE, TRUE, 0);

    tweet_box->send_dm = gtk_button_new_with_label ("Send DM");

    tweet_box->bottom_hbox = gtk_hbox_new (FALSE, 220);

    gtk_box_pack_start (GTK_BOX (tweet_box->bottom_hbox), tweet_box->send_dm, TRUE, TRUE, 5);
    gtk_box_pack_start (GTK_BOX (tweet_box->bottom_hbox), tweet_box->send_and_status_hbox, TRUE, TRUE, 5);



    tweet_box->vbox = gtk_vbox_new(FALSE, 10);
    
    g_signal_connect (G_OBJECT (tweet_box->send_tweet), "clicked",
            G_CALLBACK (send_text), (gpointer)tweet_box);
    
    gtk_box_pack_start (GTK_BOX (tweet_box->vbox), tweet_box->scrolled_win, TRUE, TRUE, 2);
    gtk_box_pack_start (GTK_BOX (tweet_box->vbox), tweet_box->bottom_hbox, FALSE, FALSE, 2);


    return tweet_box;


}

static void send_text(GtkWidget *button, TweetBox *tweet_box)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tweet_box->tweet_text));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
    send_tweet(tweet);
    gtk_text_buffer_set_text(buffer, "", 0);
    gtk_label_set_text(GTK_LABEL(tweet_box->num_carac), "140");

}

static void update_num_carac
(GtkWidget *tweet_text, GdkEvent *event, TweetBox *tweet_box)
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
    gtk_label_set_text(GTK_LABEL(tweet_box->num_carac), remaining);
}


