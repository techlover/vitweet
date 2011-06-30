typedef struct {
    GtkWidget *tweet_text, 
              *scrolled_win, 
              *send_tweet, 
              *vbox, 
              *send_and_status_hbox, 
              *num_carac, 
              *send_dm,
              *bottom_hbox;
} TweetBox;

static void send_text(GtkWidget *, TweetBox *);

static void update_num_carac(GtkWidget *, GdkEvent *, TweetBox *);

