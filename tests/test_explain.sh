#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-explain-test"

rm -rf "$TMP"
mkdir -p "$TMP/red-folder"

printf "explain test\n" > "$TMP/red-folder/red_graph.txt"
printf "other\n" > "$TMP/blue_notes.md"

$BIN scan "$TMP" >/tmp/bsql_explain_scan.txt

echo "[test] explain shows match reason"
$BIN explain "red" > /tmp/bsql_explain.txt

grep -q "BetterSQL explain: red" /tmp/bsql_explain.txt
grep -q "red_graph.txt" /tmp/bsql_explain.txt
grep -q "Matched fields:" /tmp/bsql_explain.txt
grep -q "path:" /tmp/bsql_explain.txt
grep -q "name:" /tmp/bsql_explain.txt
grep -q "Explained matches:" /tmp/bsql_explain.txt

echo "[test] explain zero matches"
$BIN explain "zzzz-not-here" > /tmp/bsql_explain_zero.txt
grep -q "Explained matches: 0" /tmp/bsql_explain_zero.txt

echo "[ok] Module 006 explain passed"
