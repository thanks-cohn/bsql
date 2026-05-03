#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-search-test"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "hello\n" > "$TMP/red_graph_pdf.txt"
printf "world\n" > "$TMP/blue_notes.md"

$BIN scan "$TMP" >/tmp/bsql_search_scan.txt

echo "[test] search finds indexed file"
$BIN search "red" > /tmp/bsql_search.txt
grep -q "red_graph_pdf.txt" /tmp/bsql_search.txt
grep -q "Matches: 1" /tmp/bsql_search.txt

echo "[test] find alias works"
$BIN find "blue" > /tmp/bsql_find.txt
grep -q "blue_notes.md" /tmp/bsql_find.txt
grep -q "Matches: 1" /tmp/bsql_find.txt

echo "[test] search missing returns zero"
$BIN search "zzzz-not-here" > /tmp/bsql_search_zero.txt
grep -q "Matches: 0" /tmp/bsql_search_zero.txt

echo "[ok] Module 004 search/find passed"
