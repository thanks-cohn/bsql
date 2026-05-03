#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-missing-test"
FILE="$TMP/gone.txt"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "gone soon\n" > "$FILE"

$BIN scan "$TMP" >/tmp/bsql_missing_scan.txt
rm "$FILE"

echo "[test] missing finds deleted indexed file"
$BIN missing > /tmp/bsql_missing.txt

grep -q "BetterSQL missing" /tmp/bsql_missing.txt
grep -q "gone.txt" /tmp/bsql_missing.txt
grep -q "Checked: 1" /tmp/bsql_missing.txt
grep -q "Missing: 1" /tmp/bsql_missing.txt

echo "[ok] Module 015 missing passed"
