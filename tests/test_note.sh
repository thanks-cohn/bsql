#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-note-test"
FILE="$TMP/note_target.txt"
SIDECAR="$FILE.wmeta.json"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "note me\n" > "$FILE"

echo "[test] note writes sidecar"
$BIN note "$FILE" "important rent document" > /tmp/bsql_note.txt

grep -q "Noted:" /tmp/bsql_note.txt
grep -q "Sidecar:" /tmp/bsql_note.txt
test -f "$SIDECAR"

grep -q '"note":' "$SIDECAR"
grep -q 'important rent document' "$SIDECAR"

echo "[test] note rejects missing file"
if $BIN note "$TMP/not_here.txt" "ghost note" >/tmp/bsql_bad_note.txt 2>&1; then
    echo "expected missing note target to fail"
    exit 1
fi

echo "[ok] Module 008 note passed"
