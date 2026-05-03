#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-scan-test"
INDEX="$HOME/.local/share/bsql/index/bsql.index.jsonl"

rm -rf "$TMP"
mkdir -p "$TMP/sub"

printf "hello better sql\n" > "$TMP/alpha.txt"
printf "# notes\n" > "$TMP/sub/notes.md"

echo "[test] scan indexes folder"
$BIN scan "$TMP" > /tmp/bsql_scan.txt

grep -q "Scanned:" /tmp/bsql_scan.txt
grep -q "Files indexed:" /tmp/bsql_scan.txt
test -f "$INDEX"

grep -q "alpha.txt" "$INDEX"
grep -q "notes.md" "$INDEX"

echo "[test] scan rejects missing folder"
if $BIN scan "/tmp/bsql-scan-missing-xyz" >/tmp/bsql_bad_scan.txt 2>&1; then
    echo "expected missing scan target to fail"
    exit 1
fi

echo "[ok] Module 003 scan passed"
