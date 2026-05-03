CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
SRC=src/main.c src/where.c src/log.c
OUT=build/bsql

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: all
	bash tests/test_where.sh

install: all
	mkdir -p ~/.local/bin
	cp $(OUT) ~/.local/bin/bsql
	@echo "Installed bsql to ~/.local/bin/bsql"

clean:
	rm -rf build
