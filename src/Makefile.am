bin_PROGRAMS = vitweet

vitweet_SOURCES = \
				  vitweet.c \
				  vitweet-tweet-input-gtk.c \
				  vitweet-tweet-input-gtk.h  \
				  vitweet-tweet-input.c \
				  vitweet-tweet-input.h  \
				  vitweet-request-key-dialog.c \
				  vitweet-request-key-dialog.h \
				  vitweet-webkit-tweet-column-gtk.c \
				  vitweet-webkit-tweet-column-gtk.h


vitweet_CFLAGS =  $(GTK_CFLAGS) \
				  $(WEBKIT_CFLAGS) 


vitweet_CPPFLAGS = -I$(top_srcdir)/include 
vitweet_LDADD = \
				../libgsocial/libgsocial.la \
				$(GTK_LIBS) \
				$(WEBKIT_LIBS) \
				$(LIBOAUTH_LIBS) \
				$(XML_LIBS) 


