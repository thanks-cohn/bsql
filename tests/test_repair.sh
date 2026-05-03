#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
GOOD="/tmp/bsql-repair-good"
DEAD="/tmp/bsql-repair-dead"
LOC="$HOME/.config/bsql/locations.tsv"

mkdir -p "$GOOD"
rm -rf "$DEAD"

cp "$LOC" /tmp/bsql_locations_backup.tsv 2>/dev/null || true

cat > "$LOC" <<EOF2
$GOOD
$GOOD
$DEAD

EOF2

echo "[test] repair cleans locations file"
$BIN repair > /tmp/bsql_repair.txt

grep -q "BetterSQL repair" /tmp/bsql_repair.txt
grep -q "Kept locations: 1" /tmp/bsql_repair.txt
grep -q "Removed dead locations: 1" /tmp/bsql_repair.txt
grep -q "Removed duplicate locations: 1" /tmp/bsql_repair.txt
grep -q "Real files, sidecars, and index are untouched" /tmp/bsql_repair.txt

LINES="$(wc -l < "$LOC")"
if [ "$LINES" -ne 1 ]; then
    echo "expected exactly one repaired location"
    exit 1
fi

grep -q "$GOOD" "$LOC"

if [ -f /tmp/bsql_locations_backup.tsv ]; then
    cp /tmp/bsql_locations_backup.tsv "$LOC"
fi

echo "[ok] Module 021 repair passed"
