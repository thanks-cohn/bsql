#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-compile-test"
FILE="$TMP/compile_target.txt"
INDEX="$HOME/.local/share/bsql/index/bsql.index.jsonl"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "compile me\n" > "$FILE"

$BIN sidecar "$FILE" >/tmp/bsql_compile_sidecar.txt
$BIN tag "$FILE" alpha beta "red sunset" >/tmp/bsql_compile_tag.txt
$BIN summary "$FILE" "Lease renewal paperwork" >/tmp/bsql_compile_summary.txt

echo "[test] compile merges sidecar memory into index"
$BIN compile "$TMP" > /tmp/bsql_compile.txt

grep -q "Compiled:" /tmp/bsql_compile.txt
test -f "$INDEX"

grep -q "compile_target.txt" "$INDEX"
grep -q "Lease renewal paperwork" "$INDEX"

echo "[test] compiled index is searchable by sidecar summary"
$BIN search "Lease renewal" > /tmp/bsql_compile_search.txt
grep -q "compile_target.txt" /tmp/bsql_compile_search.txt
grep -q "Matches: 1" /tmp/bsql_compile_search.txt

echo "[test] compile rejects missing folder"
if $BIN compile "/tmp/bsql-compile-missing-xyz" >/tmp/bsql_bad_compile.txt 2>&1; then
    echo "expected missing compile target to fail"
    exit 1
fi

echo "[ok] Module 011 compile passed"
