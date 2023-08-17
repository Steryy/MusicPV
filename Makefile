CC=gcc
CFLAG_GLIB=`pkg-config --cflags  gio-2.0 gio-unix-2.0 glib-2.0`
CLIBS_GLIB=`pkg-config  --libs gio-2.0 gio-unix-2.0 glib-2.0`
CFLAGS=-lncurses  -pthread -D_FILE_OFFSET_BITS=64
# `pkg-config --cflags `
CLIBS=`pkg-config --libs mpv  gio-2.0 gio-unix-2.0 glib-2.0`

musicpv: mpvValues.o mpris.o prompt.o helpers.o command.o file.o init.o musicpv.o input.o
	$(CC)   mpris.o init.o mpvValues.o  prompt.o helpers.o command.o file.o  musicpv.o input.o -o musicpv $(CFLAGS) $(CLIBS)   $(CFLAG_GLIB) $(CLIBS_GLIB)



input.o: input.c input.h
	$(CC) -c input.c
musicpv.o: musicpv.c
	$(CC)  -c musicpv.c
init.o: init.c init.h
	$(CC) -c init.c

mpris.o: mpris.c mpris.h
	$(CC) -c mpris.c $(CFLAG_GLIB) $(CLIBS_GLIB)

file.o: commands/file.c commands/file.h
	$(CC) -c commands/file.c
command.o: command.c command.h commands/sort.c
	$(CC) -c command.c
helpers.o:helpers.c helpers.h
	$(CC) -c helpers.c
prompt.o: prompt.h prompt.c
	$(CC) -c prompt.c

mpvValues.o: mpvValues.c mpvValues.h
	$(CC) -c mpvValues.c


clean:
	rm ./*.o

install:
	 cc  input.c  musicpv.c commands/file.c helpers.c  init.c command.c prompt.c  -o musicpv.so  `pkg-config --libs --cflags mpv` -lncurses -pthread
