CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
SRC=src/main.c src/where.c src/log.c src/location.c src/scan.c src/search.c src/meta.c src/explain.c src/tag.c src/note.c src/summary.c src/sidecar.c src/compile.c src/status.c src/clean.c src/logs.c src/missing.c src/rebuild.c src/doctor.c src/history.c
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
	bash tests/test_note.sh
	bash tests/test_summary.sh
	bash tests/test_sidecar.sh
	bash tests/test_compile.sh
	bash tests/test_status.sh
	bash tests/test_clean.sh
	bash tests/test_logs.sh
	bash tests/test_missing.sh
	bash tests/test_rebuild.sh
	bash tests/test_doctor.sh
	bash tests/test_history.sh

install: all
	mkdir -p ~/.local/bin
	cp $(OUT) ~/.local/bin/bsql
	@echo "Installed bsql to ~/.local/bin/bsql"

clean:
	rm -rf build
