enum action {
    ACTION_HOME_TIMELINE,
    ACTION_UPDATE,
};

typedef struct
{
    char *tweet;
    int exit_code;
    enum action action;
} Session;

typedef struct
{
    gchar *text;
    gchar *name;
    gchar *screen_name;
    gchar *created_at;
    gchar *id;

} Tweet;


void set_consumer_keys(char *, char *);
void set_access_keys(char *, char *);
void request_token(void);
int parse_reply_access(char *, char **, char**);
int send_tweet(char *);
GList *get_home_timeline(void);

