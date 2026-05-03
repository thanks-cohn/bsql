#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-context-test"
FILE="$TMP/context_target.txt"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "context me\n" > "$FILE"

$BIN tag "$FILE" context-tag "full view" >/tmp/bsql_context_tag.txt
$BIN rebuild "$TMP" >/tmp/bsql_context_rebuild.txt

echo "[test] context prints full object view"
$BIN context "$FILE" > /tmp/bsql_context.txt

grep -q "BetterSQL context" /tmp/bsql_context.txt
grep -q "Path:" /tmp/bsql_context.txt
grep -q "Exists: yes" /tmp/bsql_context.txt
grep -q "ID: bsql_" /tmp/bsql_context.txt
grep -q "Indexed record:" /tmp/bsql_context.txt
grep -q "context_target.txt" /tmp/bsql_context.txt
grep -q "Sidecar:" /tmp/bsql_context.txt
grep -q "context-tag" /tmp/bsql_context.txt
grep -q "History: V0" /tmp/bsql_context.txt

echo "[ok] Module 020 context passed"
