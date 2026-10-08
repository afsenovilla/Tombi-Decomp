#!/bin/bash
# Compiles a .c file to MIPS assembly with the Psy-Q compiler under Wine.
# Usage: tools/psyq_cc.sh file.c [output.s] [flags...]
# Requires: wine (+ wine32), and the local SDK in $PSYQ (default /opt/psyq/46/BIN).
# The SDK belongs to Sony: it is NOT committed to the repo.
set -e
PSYQ=${PSYQ:-/opt/psyq/46/BIN}
export WINEDEBUG=-all WINEPREFIX=${WINEPREFIX:-/opt/wine} DISPLAY=
SRC=$1; OUT=${2:-${SRC%.c}.s}; shift 2 || true
wine "$PSYQ/CC1PSX.EXE" -quiet -O2 -G0 "$@" "$SRC" -o "$OUT"
