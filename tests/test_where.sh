#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
LOG="$HOME/.local/share/bsql/logs/bsql.log"

echo "[test] where human output"
$BIN where > /tmp/bsql_where.txt

grep -q "Config:" /tmp/bsql_where.txt
grep -q "Data:" /tmp/bsql_where.txt
grep -q "Index:" /tmp/bsql_where.txt
grep -q "Logs:" /tmp/bsql_where.txt

echo "[test] where json output"
$BIN where --json > /tmp/bsql_where.json

grep -q '"tool": "bsql"' /tmp/bsql_where.json
grep -q '"command": "where"' /tmp/bsql_where.json

echo "[test] log exists"
test -f "$LOG"

echo "[test] log recorded"
grep -q '"command":"where"' "$LOG"

echo "[ok] Module 001 where passed"
