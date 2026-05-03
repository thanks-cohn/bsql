#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"

$BIN where >/tmp/bsql_logs_seed.txt

echo "[test] logs prints recent activity"
$BIN logs > /tmp/bsql_logs.txt

grep -q "BetterSQL logs" /tmp/bsql_logs.txt
grep -q "Showing last" /tmp/bsql_logs.txt
grep -q '"tool":"bsql"' /tmp/bsql_logs.txt
grep -q '"command":"where"' /tmp/bsql_logs.txt

echo "[ok] Module 014 logs passed"
