# This program is free software; you can redistribute
# it and/or modify it under the terms of the GNU GPL

#### Start of system configuration section. ####
srcdir = .

SHELL = /bin/sh

CC = gcc -O
C++ = g++ -O
YACC = bison -y
INSTALL = /usr/local/bin/install -c
INSTALLDATA = /usr/local/bin/install -c -m 644

DEFS =  -DSIGTYPE=int -DDIRENT -DSTRSTR_MISSING \
        -DVPRINTF_MISSING -DBSD42 -D_GNU_SOURCE

WARNING = -Wall
CDEBUG = -g
LIBS = -L/usr/lib -L/usr/lib/x86_64-linux-gnu -L /lib
CFLAGS =   $(DEFS) $(LIBS) $(CDEBUG) -I. -I$(srcdir) -fPIC
CPPFLAGS = $(CDEBUG) -I.  $(WARNING) 
LDFLAGS = -lpthread -lmysqlclient -lz -lm -lrt -ldl

binaries := cfgana
objects := $(patsubst %.c,%.o,$(wildcard *.c))
sources := $(wildcard *.c) + $(wildcard *.cpp)

all: $(binaries)

cfgana:  main.o util.o db.o
	$(CC) -Wall $(LDFLAGS) -o $@ $?

.c:
	$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -o $@ $<

.cpp:
	$(C++)  $(CPPFLAGS) -o $@ $<

clean:
	rm -f *.o $(binaries) core core.* *.a

distclean: clean
	rm -f TAGS Makefile config.status



