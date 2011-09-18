#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libgsocial.h"
#include "vitweet-tweet-input-gtk.h"
#include "vitweet-tweet-input.h"

void
send_text(GtkWidget *button, TweetInput *tweet_input)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gchar *tweet;

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tweet_input->tweet_text));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    tweet = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);

    if(!gsocial_send_tweet(tweet))
        g_print("Not Working\n");

    gtk_text_buffer_set_text(buffer, "", 0);
    gtk_label_set_text(GTK_LABEL(tweet_input->num_carac), "140");

}

void 
update_num_carac (GtkWidget *tweet_text, GdkEvent *event, TweetInput *tweet_input)
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
