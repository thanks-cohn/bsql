#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-meta-test"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "meta test\n" > "$TMP/meta_target.txt"

$BIN scan "$TMP" >/tmp/bsql_meta_scan.txt

echo "[test] meta finds indexed file"
$BIN meta "$TMP/meta_target.txt" > /tmp/bsql_meta.txt

grep -q "meta_target.txt" /tmp/bsql_meta.txt
grep -q '"path":' /tmp/bsql_meta.txt
grep -q '"name":' /tmp/bsql_meta.txt
grep -q '"kind":"text"' /tmp/bsql_meta.txt

echo "[test] meta rejects missing file"
if $BIN meta "$TMP/not_here.txt" >/tmp/bsql_bad_meta.txt 2>&1; then
    echo "expected missing meta target to fail"
    exit 1
fi

echo "[ok] Module 005 meta passed"
