#ifndef LIBGSOCIAL_H_INCLUDED
#define LIBGSOCIAL_H_INCLUDED
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


char *micro_get_twitter_authorize_url(void);

char *micro_get_access_key_full_reply(char *);

void set_consumer_keys(char *, char *);
void set_access_keys(char *, char *);
void request_token(void);
int parse_reply_access(char *, char **, char**);
int send_tweet(char *);
GList *get_home_timeline(void);

#endif /* LIBGSOCIAL_H_INCLUDED */
