#!/bin/bash
# Compila un .c a ensamblador MIPS con el compilador Psy-Q bajo Wine.
# Uso: tools/psyq_cc.sh fichero.c [salida.s] [flags...]
# Requiere: wine (+ wine32), y el SDK local en $PSYQ (por defecto /opt/psyq/46/BIN).
# El SDK es de Sony: NO se sube al repo.
set -e
PSYQ=${PSYQ:-/opt/psyq/46/BIN}
export WINEDEBUG=-all WINEPREFIX=${WINEPREFIX:-/opt/wine} DISPLAY=
SRC=$1; OUT=${2:-${SRC%.c}.s}; shift 2 || true
wine "$PSYQ/CC1PSX.EXE" -quiet -O2 -G0 "$@" "$SRC" -o "$OUT"
