#include <gtk/gtk.h>
#include <libmicro.h>

void create_request_key_dialog(gchar **keys)
{

    GtkWidget *dialog, *info_request_key, *link, *image, *table;
    GtkWidget *pin_label, *pin_entry;
    gboolean reply;
    gint result;

    dialog = gtk_dialog_new_with_buttons("Request key",
            NULL,
            GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
            NULL);

    gchar *text = NULL;
    text = g_strconcat("It seems like this is the first time you run Vitweet.\n",
                   "We need you to grant access Vitweet, so please copy the",
                   "following link on your browser or double click on it.\n",
                    "After you grant authorization to Vitweet, please copy ",
                    "the PIN on the entry and click OK", NULL);

    info_request_key = gtk_label_new(text);

    gtk_label_set_justify (GTK_LABEL (info_request_key), GTK_JUSTIFY_FILL);

    gtk_widget_set_size_request(info_request_key, 400, 150);

    gtk_label_set_line_wrap(GTK_LABEL (info_request_key), TRUE);

    gchar *url = micro_get_twitter_authorize_url();
    
    link = gtk_link_button_new_with_label (url, "Grant Access");

    pin_label = gtk_label_new("PIN:");

    pin_entry = gtk_entry_new();

    table = gtk_table_new(3, 2, FALSE);

    gtk_table_attach_defaults (GTK_TABLE (table), info_request_key, 0, 2, 0, 1);
    gtk_table_attach_defaults (GTK_TABLE (table), link, 0, 2, 1, 2);
    gtk_table_attach_defaults (GTK_TABLE (table), pin_label, 0, 1, 2, 3);
    gtk_table_attach_defaults (GTK_TABLE (table), pin_entry, 1, 2, 2, 3);

    gtk_table_set_row_spacings (GTK_TABLE (table), 5);
    gtk_table_set_col_spacings (GTK_TABLE (table), 5);

    gtk_container_set_border_width (GTK_CONTAINER (table), 5);
    gtk_box_pack_start(GTK_BOX (GTK_DIALOG (dialog)->vbox), table, FALSE, FALSE, 5);

    gtk_widget_set_size_request(dialog, 500, 300);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_widget_show_all(dialog);
    result = gtk_dialog_run(GTK_DIALOG (dialog));
    if(result == GTK_RESPONSE_OK) {
        gchar *pin_text = (gchar *)gtk_entry_get_text (GTK_ENTRY (pin_entry)); 
        *keys = micro_get_access_key_full_reply(pin_text);
    }

    gtk_widget_destroy(dialog);
}
