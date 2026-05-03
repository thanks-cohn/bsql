#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-clean-test"
INDEX="$HOME/.local/share/bsql/index/bsql.index.jsonl"
SIDECAR="$TMP/keep.txt.wmeta.json"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "clean me\n" > "$TMP/keep.txt"
$BIN sidecar "$TMP/keep.txt" >/tmp/bsql_clean_sidecar.txt
$BIN scan "$TMP" >/tmp/bsql_clean_scan.txt

test -f "$INDEX"
test -f "$SIDECAR"

echo "[test] clean removes index only"
$BIN clean > /tmp/bsql_clean.txt

grep -q "BetterSQL clean" /tmp/bsql_clean.txt
grep -q "Sidecars are never touched" /tmp/bsql_clean.txt
grep -q "Clean complete" /tmp/bsql_clean.txt

test ! -f "$INDEX"
test -f "$SIDECAR"
test -f "$TMP/keep.txt"

echo "[ok] Module 013 clean passed"
