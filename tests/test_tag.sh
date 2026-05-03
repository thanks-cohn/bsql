#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-tag-test"
FILE="$TMP/tag_target.txt"
SIDECAR="$FILE.wmeta.json"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "tag me\n" > "$FILE"

echo "[test] tag writes sidecar"
$BIN tag "$FILE" dog grass "red sunset" > /tmp/bsql_tag.txt

grep -q "Tagged:" /tmp/bsql_tag.txt
grep -q "Sidecar:" /tmp/bsql_tag.txt
test -f "$SIDECAR"

grep -q '"tags":' "$SIDECAR"
grep -q '"dog"' "$SIDECAR"
grep -q '"grass"' "$SIDECAR"
grep -q '"red sunset"' "$SIDECAR"

echo "[test] tag rejects missing file"
if $BIN tag "$TMP/not_here.txt" ghost >/tmp/bsql_bad_tag.txt 2>&1; then
    echo "expected missing tag target to fail"
    exit 1
fi

echo "[ok] Module 007 tag passed"
