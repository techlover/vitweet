#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "libmicro.h"
#include "tweet-box.h"

TweetBox *create_tweet_box(gchar *text)
{
    TweetBox *tweet_box = g_slice_new(TweetBox);
    GtkTextBuffer *buffer;
    tweet_box->text = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW (tweet_box->text), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tweet_box->text), GTK_WRAP_WORD);

    
    
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tweet_box->text));
    gtk_text_buffer_set_text(buffer, text, -1);

    tweet_box->vbox = gtk_vbox_new(FALSE,0);

    gtk_box_pack_start(GTK_BOX(tweet_box->vbox), tweet_box->text, TRUE, TRUE, 2);


    return tweet_box;
}
