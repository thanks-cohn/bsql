#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
LOC="$HOME/.config/bsql/locations.tsv"
TMP="/tmp/bsql-location-test"

mkdir -p "$TMP"

echo "[test] location registers folder"
$BIN location "$TMP" > /tmp/bsql_location.txt

grep -q "Registered location:" /tmp/bsql_location.txt
test -f "$LOC"
grep -q "$TMP" "$LOC"

echo "[test] location rejects missing folder"
if $BIN location "/tmp/bsql-does-not-exist-xyz" >/tmp/bsql_bad_location.txt 2>&1; then
    echo "expected missing location to fail"
    exit 1
fi

echo "[ok] Module 002 location passed"
