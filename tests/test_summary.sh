#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-summary-test"
FILE="$TMP/summary_target.txt"
SIDECAR="$FILE.wmeta.json"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "summary me\n" > "$FILE"

echo "[test] summary writes sidecar"
$BIN summary "$FILE" "Lease renewal paperwork" > /tmp/bsql_summary.txt

grep -q "Summarized:" /tmp/bsql_summary.txt
grep -q "Sidecar:" /tmp/bsql_summary.txt
test -f "$SIDECAR"

grep -q '"summary":' "$SIDECAR"
grep -q 'Lease renewal paperwork' "$SIDECAR"

echo "[test] summary rejects missing file"
if $BIN summary "$TMP/not_here.txt" "ghost summary" >/tmp/bsql_bad_summary.txt 2>&1; then
    echo "expected missing summary target to fail"
    exit 1
fi

echo "[ok] Module 009 summary passed"
