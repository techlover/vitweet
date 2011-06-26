#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <oauth.h>

const char consumer_key[] = "zoCegrqiH5zv7dBJoLNw";
const char consumer_secret[] = "e9xprZdGKksRiQtVcSrsS9RnlOZenBGkptUApmjxs";
const char request_token_uri[] = "https://api.twitter.com/oauth/request_token";
const char access_token[] = "https://api.twitter.com/oauth/access_token";
const char twitter_authorize_uri[] = "http://api.twitter.com/oauth/authorize?oauth_token=";
const char tweet[] =  "que calor!!! La aplicación de mi hermano esta chida";

char *access_key = NULL;
char *access_secret = NULL;


static int parse_reply_access(char *reply, char **token, char **secret)
{
    int retval = 1;
    int rc;  /* Number of url parameters */
    char **rv = NULL; /* url parameters */
    rc = oauth_split_url_parameters(reply, &rv);
    qsort(rv, rc, sizeof(char *), oauth_cmpstringp);
    if (rc == 2 || rc == 4) {
        if (!strncmp(rv[0], "oauth_token=", 11) &&
                !strncmp(rv[1], "oauth_token_secret=", 18)) {
            if (token)
                *token = strdup(&(rv[0][12]));
            if (secret)
                *secret = strdup(&(rv[1][19]));

            retval = 0;
        }
    } else if (rc == 3)
        if(!strncmp(rv[1], "oauth_token=", 11) &&
                !strncmp(rv[2], "oauth_token_secret=", 18)) {
            if(token)
                *token = strdup(&rv[1][12]);
            if(secret)
                *secret = strdup(&rv[2][19]);
            retval = 0;
        }
    if(rv)
        free(rv);


    return retval;
}

void request_token()
{
    int verifier;
    char line[100];
    char *req_key = NULL;
    char *req_secret = NULL;
    char *req_url;
    char *reply;
    char ath_uri[90];
    char *new_reply;

    req_url = oauth_sign_url2(request_token_uri, NULL, OA_HMAC, NULL,
            consumer_key, consumer_secret, NULL, NULL);
    printf("%s\n", req_url);

    reply = oauth_http_get(req_url, NULL);
    printf("%s\n", reply);
    if(parse_reply_access(reply, &req_key, &req_secret))
        printf("Something is wrong!\n");

    free(reply);
    fprintf(stdout,
            "Please open the link on you browser and allow vitweet. Then "
            "paste the PIN here\n");
    fprintf(stdout,"%s%s\nPIN: ", twitter_authorize_uri, req_key);
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &verifier);

    sprintf(ath_uri, "%s?oauth_verifier=%d", access_token, verifier);

    req_url = oauth_sign_url2(ath_uri, NULL, OA_HMAC, NULL, consumer_key,
            consumer_secret, req_key, req_secret);

    printf("%s\n", req_url);
    new_reply = oauth_http_get(req_url, NULL);
    printf("%s\n", new_reply);



    if(parse_reply_access(new_reply, &access_key, &access_secret))
        printf("Something is wrong!\n");

    fprintf(stdout, "We have access to twitter! I can't believe it!"
            "our access token key: %s\n"
            "our access token secret: %s\n"
            "\n",req_key, req_secret);
}

static void send_tweet() 
{
    char *escaped_tweet = NULL;
    char endpoint[500];
    char *req_url;
    char *reply;
    char *postarg = NULL;
    escaped_tweet = oauth_url_escape(tweet);
    sprintf(endpoint, "%s%s?status=%s","http://api.twitter.com/1/statuses",
            "/update.xml", escaped_tweet);

    printf("%s\n", endpoint);
    req_url = oauth_sign_url2(endpoint, &postarg, OA_HMAC, NULL,
            consumer_key, consumer_secret, access_key, access_secret);
    reply = oauth_http_post(req_url, postarg);
    printf("%s\n", postarg);
    printf("%s\n", reply);
}

int main(int argc, char *argv[])
{

    request_token();  
    send_tweet();
    return 0;
}
