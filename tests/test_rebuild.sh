#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-rebuild-test"
FILE="$TMP/rebuild_target.txt"
INDEX="$HOME/.local/share/bsql/index/bsql.index.jsonl"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "rebuild me\n" > "$FILE"

$BIN tag "$FILE" rebuild-tag "sidecar memory" >/tmp/bsql_rebuild_tag.txt

echo "[test] rebuild compiles files and sidecars"
$BIN rebuild "$TMP" > /tmp/bsql_rebuild.txt

grep -q "BetterSQL rebuild" /tmp/bsql_rebuild.txt
grep -q "Rebuild complete" /tmp/bsql_rebuild.txt
test -f "$INDEX"

grep -q "rebuild_target.txt" "$INDEX"
grep -q "rebuild-tag" "$INDEX"
grep -q "sidecar memory" "$INDEX"

echo "[test] rebuild output is searchable"
$BIN search "rebuild-tag" > /tmp/bsql_rebuild_search.txt
grep -q "rebuild_target.txt" /tmp/bsql_rebuild_search.txt
grep -q "Matches: 1" /tmp/bsql_rebuild_search.txt

echo "[test] rebuild rejects missing folder"
if $BIN rebuild "/tmp/bsql-rebuild-missing-xyz" >/tmp/bsql_bad_rebuild.txt 2>&1; then
    echo "expected missing rebuild target to fail"
    exit 1
fi

echo "[ok] Module 016 rebuild passed"
