cflags := -std=c11 -Wpedantic -O0

prefix := /usr/local




cgen: cgen.c
	$(CC) $(cflags) cgen.c -o cgen

.PHONY: clean install uninstall

clean:
	rm cgen

install: cgen
	install cgen $(prefix)/bin

uninstall:
	rm $(prefix)/bin/cgen	
