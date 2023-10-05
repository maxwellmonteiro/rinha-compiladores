CC = gcc
SOURCES_C = src/main.c src/transpiler.c src/term/term.c src/term/binary_term.c src/util/context.c src/util/fn_list.c src/util/callee_list.c src/util/var_list.c src/util/binding_list.c src/util/clojure.c src/util/scope.c
INCLUDEDIR = ./include
LIBDIR = ./lib
CFLAGS_DEBUG = -g -Wl,-rpath -Wl,$(LIBDIR)
CFLAGS_RELEASE = -pipe -Wrestrict -Wall -O3 -Wl,-rpath -static -Wl,$(LIBDIR)
CFLAGS = $(CFLAGS_DEBUG)

all: rinha-compiladores

rinha-compiladores: $(SOURCES_C)
	$(CC) $(CFLAGS) $(SOURCES_C) -o $@ -ljansson -I$(INCLUDEDIR) -L$(LIBDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.c: %.h	

clean:
	rm -f *.o rinha-compiladores
