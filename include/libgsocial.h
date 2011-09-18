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

} GSLTweet;

void gsocial_init(void);

char *gsocial_get_twitter_authorize_url(void);

char *gsocial_get_access_key_full_reply(char *);

void gsocial_set_consumer_keys(char *, char *);
void gsocial_set_access_keys(char *, char *);
void gsocial_request_token(void);
int gsocial_parse_reply_access(char *, char **, char**);
int gsocial_send_tweet(char *);
GList *gsocial_get_home_timeline(void);

#endif /* LIBGSOCIAL_H_INCLUDED */
