#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-box.h"

TweetBox *create_tweet_box(Tweet *tweet)
{
    TweetBox *tweet_box = g_slice_new(TweetBox);
    GtkTextBuffer *buffer;
    tweet_box->name = gtk_label_new (tweet->name);
    tweet_box->screen_name = gtk_label_new (tweet->screen_name);
    tweet_box->name_section = gtk_hbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(tweet_box->name_section),
            tweet_box->screen_name, FALSE, FALSE, 2);

    gtk_box_pack_start(GTK_BOX(tweet_box->name_section), 
            tweet_box->name, FALSE, FALSE, 2);

    tweet_box->text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_box->text), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tweet_box->text), GTK_WRAP_WORD);
    gtk_widget_set_sensitive (tweet_box->text, FALSE);

    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tweet_box->text));
    gtk_text_buffer_set_text(buffer, tweet->text, -1);

    gtk_widget_set_size_request(tweet_box->text, 180, 150);

    tweet_box->vbox = gtk_vbox_new(FALSE,0);

    gtk_box_pack_start(GTK_BOX(tweet_box->vbox),
            tweet_box->name_section, TRUE, TRUE, 2);

    gtk_box_pack_start(GTK_BOX(tweet_box->vbox),
            tweet_box->text, TRUE, TRUE, 2);

    gtk_widget_set_size_request(tweet_box->vbox, 200, 150);

    return tweet_box;
}
