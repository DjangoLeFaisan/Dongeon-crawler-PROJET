# Makefile

CC      ?= gcc
CFLAGS  ?= -std=c17 -Wall -Wextra -g
CFLAGS  += -Isrc -Iinclude
LDFLAGS ?=


SRC  := $(wildcard src/*.c)
OBJ  := $(SRC:src/%.c=build/%.o)
BIN  := build/game

UNAME_S := $(shell uname -s)

# macOS (Homebrew + pkg-config)
ifeq ($(UNAME_S),Darwin)
    CFLAGS  += $(shell pkg-config --cflags raylib)
    LDFLAGS += $(shell pkg-config --libs raylib)
endif

# Windows (MSYS2 / MinGW64)
ifeq ($(OS),Windows_NT)
    CFLAGS  += $(shell pkg-config --cflags raylib)
    LDFLAGS += $(shell pkg-config --libs raylib)
endif

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build
