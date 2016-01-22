PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin
INSTALL_LIB=$(PREFIX)/lib
INSTALL_INCLUDE=$(PREFIX)/include/seed

CC=gcc
INSTALL=install
UNINSTALL=rm -rf

OPTIMIZATION?=-O2

STD=-std=c99 -pedantic
WARN=-Wall -W
OPT=$(OPTIMIZATION)

CFLAGS=$(STD) $(WARN) $(OPT)
LIBS=-lpthread

SEED_CC=$(CC) $(CFLAGS)
SEED_INSTALL=$(INSTALL)

SEED_LIB=libseed.so

all: $(SEED_LIB)

.PHONY: all

$(SEED_LIB): *.c
	$(SEED_CC) -shared -fPIC -DPIC -o $@ $^

clean:
	rm -rf $(SEED_LIB)

.PHONY: clean

install: all
	@mkdir -p $(INSTALL_LIB)
	$(SEED_INSTALL) $(SEED_LIB) $(INSTALL_LIB)
	@mkdir -p $(INSTALL_INCLUDE)
	$(SEED_INSTALL) *.h $(INSTALL_INCLUDE)

uninstall:
	$(UNINSTALL) $(INSTALL_LIB)/$(SEED_LIB)
	$(UNINSTALL) $(INSTALL_INCLUDE)

.PHONY: uninstall
