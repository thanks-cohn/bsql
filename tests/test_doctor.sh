#!/usr/bin/env bash
set -euo pipefail

BIN="./build/bsql"
TMP="/tmp/bsql-doctor-test"

rm -rf "$TMP"
mkdir -p "$TMP"

printf "doctor me\n" > "$TMP/doctor_target.txt"

$BIN location "$TMP" >/tmp/bsql_doctor_location.txt
$BIN scan "$TMP" >/tmp/bsql_doctor_scan.txt

echo "[test] doctor prints health report"
$BIN doctor > /tmp/bsql_doctor.txt

grep -q "BetterSQL doctor" /tmp/bsql_doctor.txt
grep -q "locations file" /tmp/bsql_doctor.txt
grep -q "index" /tmp/bsql_doctor.txt
grep -q "log" /tmp/bsql_doctor.txt
grep -q "Issues:" /tmp/bsql_doctor.txt
grep -q "Diagnosis:" /tmp/bsql_doctor.txt

echo "[test] doctor detects missing indexed file"
rm "$TMP/doctor_target.txt"

$BIN doctor > /tmp/bsql_doctor_missing.txt

grep -q "indexed files missing from disk" /tmp/bsql_doctor_missing.txt
grep -q "Diagnosis: attention needed" /tmp/bsql_doctor_missing.txt

echo "[ok] Module 017 doctor passed"
