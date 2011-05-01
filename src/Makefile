CFLAGS = -g -O0
CFLAGS = -Wall -g
LDFLAGS = -loauth

all: vitweet

check: all
	./vitweet | grep "My first tweet from my own app"
	@echo "*** ALL TESTS PASSED ***"

install:
	install -d $(DESTDIR)$(bindir)
	install -m 0755 vitweet $(DESTDIR)$(bindir)

uninstall:
	-rm $(DESTDIR)$(bindir)/vitweet

vitweet: main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ main.c $(LDFLAGS)

clean:
	-rm vitweet

.PHONY: all clean check install
