#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-sidecar-test"
FILE="$TMP/sidecar_target.txt"
SIDECAR="$FILE.wmeta.json"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "sidecar me\n" > "$FILE"

echo "[test] sidecar creates metadata file"
$BIN sidecar "$FILE" > /tmp/bsql_sidecar.txt

grep -q "Created sidecar:" /tmp/bsql_sidecar.txt
test -f "$SIDECAR"
grep -q '"tags": \[\]' "$SIDECAR"
grep -q '"note": ""' "$SIDECAR"
grep -q '"summary": ""' "$SIDECAR"

echo "[test] sidecar prints existing metadata file"
$BIN sidecar "$FILE" > /tmp/bsql_sidecar_existing.txt
grep -q "Sidecar exists:" /tmp/bsql_sidecar_existing.txt

echo "[test] sidecar rejects missing file"
if $BIN sidecar "$TMP/not_here.txt" >/tmp/bsql_bad_sidecar.txt 2>&1; then
    echo "expected missing sidecar target to fail"
    exit 1
fi

echo "[ok] Module 010 sidecar passed"
