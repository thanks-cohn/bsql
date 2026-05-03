#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-history-test"
FILE="$TMP/history_target.txt"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "history me\n" > "$FILE"

$BIN rebuild "$TMP" >/tmp/bsql_history_rebuild.txt

echo "[test] history shows indexed record"
$BIN history "$FILE" > /tmp/bsql_history.txt

grep -q "BetterSQL history" /tmp/bsql_history.txt
grep -q "Target:" /tmp/bsql_history.txt
grep -q "Current indexed record:" /tmp/bsql_history.txt
grep -q "history_target.txt" /tmp/bsql_history.txt
grep -q "History V0" /tmp/bsql_history.txt

echo "[test] history rejects unknown path"
if $BIN history "$TMP/not_here.txt" >/tmp/bsql_bad_history.txt 2>&1; then
    echo "expected unknown history target to fail"
    exit 1
fi

echo "[ok] Module 018 history passed"
