typedef struct
{
    GtkWidget *text,
              *id,
              *screen_name,
              *name,
              *created_at,
              *name_section,
              *option_section,
              *vbox;
} TweetBox;
TweetBox *create_tweet_box(Tweet *);

