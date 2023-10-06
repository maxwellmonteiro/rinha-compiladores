CC = gcc
SOURCES_C = src/main.c src/interpreter.c src/term/term.c src/term/binary_term.c src/util/binding_list.c src/util/closure.c
INCLUDEDIR = ./include
LIBDIR = ./lib
CFLAGS_DEBUG = -g -Wl,-rpath -Wl,$(LIBDIR)
CFLAGS_RELEASE = -pipe -Wrestrict -Wall -O3 -Wl,-rpath -static -Wl,$(LIBDIR)
CFLAGS = $(CFLAGS_RELEASE)

all: rinha-compiladores

rinha-compiladores: $(SOURCES_C)
	$(CC) $(CFLAGS) $(SOURCES_C) -o $@ -ljansson -I$(INCLUDEDIR) -L$(LIBDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.c: %.h	

clean:
	rm -f *.o rinha-compiladores
