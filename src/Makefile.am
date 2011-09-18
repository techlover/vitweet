bin_PROGRAMS = vitweet
vitweet_SOURCES = \
				  main.c \
				  tweet-input.c tweet-input.h  \
				  vitweet-request-key-dialog.c vitweet-request-key-dialog.h

vitweet_CFLAGS =  $(GTK_CFLAGS) \
				  $(WEBKIT_CFLAGS) 


vitweet_CPPFLAGS = -I$(top_srcdir)/include 
vitweet_LDADD = \
				../libgsocial/libgsocial.la \
				$(GTK_LIBS) \
				$(WEBKIT_LIBS) \
				$(LIBOAUTH_LIBS) \
				$(XML_LIBS) 


