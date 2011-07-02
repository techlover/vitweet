#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <oauth.h>
#include <glib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "libmicro.h"

const char twitter_statuses[] = "http://api.twitter.com/1/statuses";
const char request_token_uri[] = "https://api.twitter.com/oauth/request_token";
const char access_token[] = "https://api.twitter.com/oauth/access_token";
const char twitter_authorize_uri[] = "http://api.twitter.com/oauth/authorize?oauth_token=";

char *consumer_key = NULL;
char *consumer_secret = NULL;
char *access_key = NULL;
char *access_secret = NULL;
Session *request;
GList *tweet_list;

static void request_free(Session *request)
{
    if(!request)
        return;
    free(request);
}

void micro_init()
{
    request = calloc(1, sizeof(Session));
}

void set_consumer_keys(char *consumer, char *secret)
{
    consumer_key = consumer;
    consumer_secret = secret;
}

void set_access_keys(char *key, char *secret)
{
    access_key = key;
    access_secret = secret;
}

int parse_reply_access(char *reply, char **token, char **secret)
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


static Tweet *parse_statuses(Session *session,
        xmlDocPtr doc, xmlNodePtr current)
{
    xmlChar *text = NULL;
    xmlChar *screen_name = NULL;
    xmlChar *created_at = NULL;
    xmlChar *id = NULL;
    xmlNodePtr userinfo;
    Tweet *tweet = g_slice_new(Tweet);
    tweet->text = NULL;
    tweet->screen_name = NULL;
    tweet->id = NULL;
    tweet->created_at = NULL;

    current = current->xmlChildrenNode;
    while (current != NULL) {
        if (current->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(current->name, (const xmlChar *)"created_at"))
                created_at = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
            if (!xmlStrcmp(current->name, (const xmlChar *)"text"))
                text = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
            if (!xmlStrcmp(current->name, (const xmlChar *)"id"))
                id = xmlNodeListGetString(doc, current->xmlChildrenNode, 1);
            if (!xmlStrcmp(current->name, (const xmlChar *)"user")) {
                userinfo = current->xmlChildrenNode;
                while (userinfo != NULL) {
                    if ((!xmlStrcmp(userinfo->name, (const xmlChar *)"screen_name"))) {
                        if (screen_name)
                            xmlFree(screen_name);
                        screen_name = xmlNodeListGetString(doc, userinfo->xmlChildrenNode, 1);
                    }
                    userinfo = userinfo->next;
                }
            }

            if (screen_name && text && created_at && id) {
                tweet->screen_name = screen_name;
                tweet->text = text;
                tweet->created_at = created_at;
                tweet->id = id;
                

            }
        }
        current = current->next;
    }

    return tweet;
}

static void parse_timeline(char *document, Session *session)
{
    xmlDocPtr doc;
    xmlNodePtr current;
    tweet_list = NULL;
    Tweet *tweet = g_slice_new(Tweet);
    tweet->text = NULL;

    doc = xmlReadMemory(document, strlen(document), "timeline.xml",
            NULL, XML_PARSE_NOERROR);
    if (doc == NULL)
        return;

    current = xmlDocGetRootElement(doc);
    if (current == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(current->name, (const xmlChar *) "statuses")) {
        fprintf(stderr, "unexpected document type\n");
        xmlFreeDoc(doc);
        return;
    }

    current = current->xmlChildrenNode;
    while (current != NULL) {
        if ((!xmlStrcmp(current->name, (const xmlChar *)"status"))){
            tweet_list = g_list_prepend(tweet_list, 
                    (gpointer*)parse_statuses(session, doc, current));
            tweet = (Tweet *)g_list_nth_data(tweet_list, 0); 
        }
        current = current->next;
    }



    xmlFreeDoc(doc);

    return;
}


static void send_request(Session *request)
{
    request->exit_code = 0;
    char *escaped_tweet = NULL;
    int is_post = 0;
    char endpoint[500];
    char *req_url;
    char *reply;
    char *postarg = NULL;
    switch(request->action) {
        case ACTION_HOME_TIMELINE:
            sprintf(endpoint, "%s%s","http://api.twitter.com/1/statuses",
                    "/home_timeline.xml&count=2");
            break;
        case ACTION_UPDATE:
            escaped_tweet = oauth_url_escape(request->tweet);
            sprintf(endpoint, "%s%s?status=%s","http://api.twitter.com/1/statuses",
                    "/update.xml", escaped_tweet);
            is_post = 1;
            break;

    }
    if(is_post){
        req_url = oauth_sign_url2(endpoint, &postarg, 
                OA_HMAC, NULL, consumer_key, consumer_secret, access_key, access_secret);

        reply = oauth_http_post(req_url, postarg);

    } else{
        req_url = oauth_sign_url2(endpoint, NULL, OA_HMAC, NULL,
                consumer_key, consumer_secret, access_key, access_secret);

        reply = oauth_http_get(req_url, postarg);
    }


    if (request->action != ACTION_UPDATE)
        parse_timeline(reply, request);

    if(reply)
        request->exit_code = 1;

    free(postarg);
    free(req_url);
    free(reply);
}


GList *get_home_timeline()
{
    request->action = ACTION_HOME_TIMELINE;
    send_request(request);
    return tweet_list;

}

int send_tweet(char *tweet)
{   
    request->tweet = tweet;
    request->action = ACTION_UPDATE;
    send_request(request);
    return request->exit_code;
}
