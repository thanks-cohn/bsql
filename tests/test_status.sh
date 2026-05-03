#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-status-test"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "status me\n" > "$TMP/status_target.txt"

$BIN location "$TMP" >/tmp/bsql_status_location.txt
$BIN scan "$TMP" >/tmp/bsql_status_scan.txt

echo "[test] status prints engine state"
$BIN status > /tmp/bsql_status.txt

grep -q "BetterSQL status" /tmp/bsql_status.txt
grep -q "Locations:" /tmp/bsql_status.txt
grep -q "Index exists:" /tmp/bsql_status.txt
grep -q "Indexed files:" /tmp/bsql_status.txt
grep -q "Index size:" /tmp/bsql_status.txt
grep -q "Log exists:" /tmp/bsql_status.txt
grep -q "Paths" /tmp/bsql_status.txt

echo "[ok] Module 012 status passed"
