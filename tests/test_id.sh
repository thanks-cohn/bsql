#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-id-test"
FILE="$TMP/id_target.txt"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "id me\n" > "$FILE"

echo "[test] id prints deterministic identifier"
$BIN id "$FILE" > /tmp/bsql_id.txt
$BIN id "$FILE" > /tmp/bsql_id_again.txt

grep -q "BetterSQL id" /tmp/bsql_id.txt
grep -q "Path:" /tmp/bsql_id.txt
grep -q "ID: bsql_" /tmp/bsql_id.txt
grep -q "ID V0" /tmp/bsql_id.txt

ID1="$(grep '^ID:' /tmp/bsql_id.txt)"
ID2="$(grep '^ID:' /tmp/bsql_id_again.txt)"

if [ "$ID1" != "$ID2" ]; then
    echo "expected deterministic ID"
    exit 1
fi

echo "[ok] Module 019 id passed"
