#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "libgsocial.h"
#include "vitweet-tweet-input-gtk.h"
#include "vitweet-request-key-dialog.h"
#include "vitweet-webkit-tweet-column-gtk.h"

#include <glib/gi18n.h>


static void destroy (GtkWidget*,gpointer);
static void read_keys(char **, char **);

char consu[] = "zoCegrqiH5zv7dBJoLNw";
char consu_secret[] = "e9xprZdGKksRiQtVcSrsS9RnlOZenBGkptUApmjxs";
char *key = NULL;
char *secret = NULL;

TweetInput *tweet_input;

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

static void
setup_tree_view (GtkWidget *treeview)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();

    g_object_set(G_OBJECT (renderer), "wrap-width", 370, NULL);

    g_object_set(G_OBJECT (renderer), "wrap-mode", PANGO_WRAP_WORD_CHAR, NULL);

    column = gtk_tree_view_column_new_with_attributes(
           "Home Time Line", renderer, "markup", 0, NULL);

    gtk_tree_view_append_column (GTK_TREE_VIEW (treeview), column);
}


int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    gsocial_init();
    gsocial_set_consumer_keys(consu, consu_secret);
    read_keys(&key, &secret);
    gsocial_set_access_keys(key, secret);
    GtkWidget *window, *treeview, *scrolled_win, *vbox, *tweet_list;
    GtkListStore *store;
    GtkTreeIter iter;

    tweet_input = create_tweet_input();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Vitweet");
    

    g_signal_connect (G_OBJECT (window), "destroy",
            G_CALLBACK (destroy), NULL);

    vbox = gtk_vbox_new(FALSE, 5);

    gtk_box_pack_start(GTK_BOX(vbox), tweet_input->vbox, FALSE, FALSE, 2);
    //gtk_container_add (GTK_CONTAINER (window), tweet_input->vbox);

    treeview = gtk_tree_view_new();
    setup_tree_view(treeview);
    
    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    int i;
    GList *list = NULL;
    list = gsocial_get_home_timeline();
    //insert_tweets(list, 
    /*GSLTweet *tweet;*/
    /*gchar *parsed_tweet;*/
    /*tweet = g_slice_new(GSLTweet);*/
    /*for(i = 0; i<g_list_length(list); i++){*/
        /*tweet = (GSLGSLTweet *)g_list_nth_data (list, i);*/
        /*parsed_tweet = g_strconcat("<b>", tweet->screen_name, "</b> ",*/
                                    /*tweet->name, "\n",*/
                                    /*"<small>", tweet->text, "</small>",*/
                                    /*NULL);*/
        /*gtk_list_store_append(store, &iter);*/
        /*gtk_list_store_set(store, &iter, 0, parsed_tweet, -1);*/
    /*}*/

    gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (store));
    g_object_unref(store);

    /* Create a webkit based column */
    tweet_list = create_webkit_column(list);

    scrolled_win = gtk_scrolled_window_new (NULL, NULL);

    gtk_container_add (GTK_CONTAINER (scrolled_win), tweet_list);

    gtk_box_pack_start(GTK_BOX(vbox), scrolled_win, TRUE, TRUE, 2);


    gtk_container_add (GTK_CONTAINER (window), vbox);

    gtk_widget_set_size_request (window, 400, 700);

    /*insert_tweets(list, tweet_list);*/

    /*gchar *tmp, *css;*/

    /*tmp = g_base64_encode (CSS, strlen((gchar *) CSS));*/
    /*css = g_strconcat ("data:text/css;charset=utf-8;base64,",*/
            /*tmp, NULL);*/
    /*g_object_set (webkit_web_view_get_settings (WEBKIT_WEB_VIEW (tweet_list)),*/
            /*"user-stylesheet-uri", css, NULL);*/
    /*g_free (css);*/
    /*g_free (tmp);*/


    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}


static void read_keys(char **key, char **secret)
{
    gchar *filename, *content;
    gchar *path;
    gsize bytes;

    GError *error = NULL;

    filename = g_build_filename (g_get_current_dir(), "keys", NULL);

    content = NULL;

    if(!g_file_test (filename, G_FILE_TEST_EXISTS)) {
        path = g_build_path ("/", g_get_home_dir(), ".vitweet", NULL);
        filename = g_build_filename (path, "keys", NULL);
    }
    if(!g_file_test (filename, G_FILE_TEST_EXISTS)) {
        create_request_key_dialog(&content);
        g_mkdir(path, 0700);
        if(content)
            g_file_set_contents(filename, content, strlen(content), &error);
        else
            g_error("PIN not entered");
    }


    g_file_get_contents(filename, &content, &bytes, &error);

    if(gsocial_parse_reply_access(content, key, secret))
        g_error("Error: Can't read file");

    g_free(content);
    g_free(filename);
}


static void destroy(GtkWidget *window, gpointer data)
{
    g_slice_free(TweetInput, tweet_input);
    gtk_main_quit();
}
