PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin
INSTALL_LIB=$(PREFIX)/lib
INSTALL_INCLUDE=$(PREFIX)/include/seed

CC=gcc
INSTALL=install

OPTIMIZATION?=-O2

STD=-std=c99 -pedantic
WARN=-Wall -W
OPT=$(OPTIMIZATION)

CFLAGS=$(STD) $(WARN) $(OPT)
LIBS=-lpthread

SEED_CC=$(CC) $(CFLAGS)
SEED_INSTALL=$(INSTALL)

SEED_LIB_NAME=libseed.so

all: $(SEED_LIB_NAME)
	@echo "Oh, GOOD CODER,"
	@echo "your presence here somehow soothes."
	@echo ""

.PHONY: all

$(SEED_LIB_NAME): *.c
	$(SEED_CC) -shared -fPIC -DPIC -o $@ $^

install: all
	@mkdir -p $(INSTALL_LIB)
	$(SEED_INSTALL) $(SEED_LIB_NAME) $(INSTALL_LIB)
	@mkdir -p $(INSTALL_INCLUDE)
	$(SEED_INSTALL) *.h $(INSTALL_INCLUDE)
