#include <gtk/gtk.h>
#include "libgsocial.h"

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


GtkWidget 
*create_treeview_column(GList *list) 
{

    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeIter iter;

    treeview = gtk_tree_view_new();
    setup_tree_view(treeview);

    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    GSLTweet *tweet;
    gchar *parsed_tweet;
    tweet = g_slice_new(GSLTweet);

    int i;
    for(i = 0; i<g_list_length(list); i++){
        tweet = (GSLTweet *)g_list_nth_data (list, i);
        parsed_tweet = g_strconcat("<b>", tweet->screen_name, "</b> ",
                                    tweet->name, "\n",
                                    "<small>", tweet->text, "</small>",
                                    NULL);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, parsed_tweet, -1);
    }

    gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (store));
    g_object_unref(store);

    return treeview;
}
