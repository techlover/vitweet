#include <gtk/gtk.h>
#include <string.h>
#include <webkit/webkit.h>
#include "libgsocial.h"
#include "vitweet-webkit-tweet-column-gtk.h"

static const guchar CSS[] =
"body { margin: 0; padding: 0; }\n"
"div { "
" -webkit-border-radius: 2px;"
" background: -webkit-gradient(linear, 0% 100%, 0% 0%,"
" from(#f1f1f1), to(white));"
" border: solid 1px #c6c6c6;"
" -webkit-box-shadow: 0px 0px 2px #c6c6c6;"
" margin: 12px; padding: 6px;"
"}";

GtkWidget *tweet_list;

static void
insert_tweets (GList *tweets, GtkWidget *view)
{
    WebKitDOMDocument *document;
    WebKitDOMElement *body, *div;

    document = webkit_web_view_get_dom_document (WEBKIT_WEB_VIEW(view));

    GSLTweet *tweet;
    gchar *parsed_tweet;
    tweet = g_slice_new(GSLTweet);
    int i;
    for(i = 0; i<g_list_length(tweets); i++){
        tweet = (GSLTweet *)g_list_nth_data (tweets, i);
        body = webkit_dom_document_query_selector (document, "body", NULL);
        div = webkit_dom_document_create_element (document, "div", NULL);
        webkit_dom_node_set_text_content (WEBKIT_DOM_NODE (div),
                tweet->text,
                NULL);
        /*printf ( "%s\n", tweet->text );*/
        webkit_dom_node_append_child (WEBKIT_DOM_NODE (body),
                WEBKIT_DOM_NODE (div),
                NULL);
    }


}

GtkWidget 
*create_webkit_column(GList *list)
{

    tweet_list = webkit_web_view_new();

    webkit_web_view_load_string (WEBKIT_WEB_VIEW (tweet_list),
                                 "<html><body></body><html>",
                                 "text/html>",
                                 "UTF-8",
                                 NULL);

    insert_tweets(list, tweet_list);
    gchar *tmp, *css;

    tmp = g_base64_encode (CSS, strlen((gchar *) CSS));
    css = g_strconcat ("data:text/css;charset=utf-8;base64,",
            tmp, NULL);
    g_object_set (webkit_web_view_get_settings (WEBKIT_WEB_VIEW (tweet_list)),
            "user-stylesheet-uri", css, NULL);
    g_free (css);
    g_free (tmp);

    return tweet_list;

}


