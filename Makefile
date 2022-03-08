# Installation directories following GNU conventions
prefix ?= /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
sbindir = $(exec_prefix)/sbin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
includedir = $(prefix)/include
mandir = $(datarootdir)/man

BIN=bin
OBJ=obj
SRC=src

OBJ_MODS=obj/modules
SRC_MODS=src/modules

CC ?= gcc
CFLAGS ?= -Wextra -Wall -iquote$(SRC) -DLOG_USE_COLOR=1 -I../../src/. -luv -lcurl

.PHONY: all install uninstall clean

EXES = bmenu test1 test2

all: $(EXES)


test2: $(OBJ)/tio.o $(OBJ)/menu.o $(OBJ)/test2.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ 

test1: $(OBJ)/tio.o $(OBJ)/menu.o $(OBJ)/test1.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ 

bmenu: $(OBJ)/tio.o $(OBJ)/menu.o $(OBJ)/main.o | $(BIN)
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^ 

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BIN):
	mkdir -p $(BIN)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(BIN)
	rm -rf $(OBJ)

install:
	install -d $(DESTDIR)$(bindir)
	cd $(BIN) && install $(EXES) $(DESTDIR)$(bindir)

uninstall:
	for exe in $(EXES); do rm $(DESTDIR)$(bindir)/$$exe; done

