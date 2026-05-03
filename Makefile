CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
SRC=src/main.c src/where.c src/log.c src/location.c src/scan.c src/search.c src/meta.c src/explain.c src/tag.c
OUT=build/bsql

all:
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: all
	bash tests/test_where.sh
	bash tests/test_location.sh
	bash tests/test_scan.sh
	bash tests/test_search.sh
	bash tests/test_meta.sh
	bash tests/test_explain.sh
	bash tests/test_tag.sh

install: all
	mkdir -p ~/.local/bin
	cp $(OUT) ~/.local/bin/bsql
	@echo "Installed bsql to ~/.local/bin/bsql"

clean:
	rm -rf build
