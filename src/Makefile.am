bin_PROGRAMS = vitweet
vitweet_SOURCES = \
				  main.c \
				  tweet-input.c tweet-input.h  \
				  vitweet-request-key-dialog.c vitweet-request-key-dialog.h
vitweet_CFLAGS =  $(GTK_CFLAGS) 

vitweet_CPPFLAGS = -I$(top_srcdir)/common 
vitweet_LDADD = \
				../common/libmicro.a \
				$(GTK_LIBS) \
				$(LIBOAUTH_LIBS) \
				$(XML_LIBS)


