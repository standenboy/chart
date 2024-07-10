CC = cc

CFLAGS = 
PREFIX = /usr/local/
MAN = share/man
BIN = bin/
DEST = $(PREFIX)$(BIN)


chart : chart.c
	$(CC) chart.c -o chart $(CFLAGS)
install: chart
	install -m 755 chart $(DEST)
	cp chart.1 $(PREFIX)$(MAN)/man1/
uninstall:
	rm -rf $(DEST)/chart
clean: 
	rm -rf ./chart

