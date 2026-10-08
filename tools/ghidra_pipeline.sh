#!/usr/bin/env bash
# Ghidra pipeline in headless mode (no GUI).
#   tools/ghidra_pipeline.sh import   imports and analyzes MAIN0.EXE and X000.BIN (only once)
#   tools/ghidra_pipeline.sh apply    creates functions, applies names and structures and exports the decompilation
#   tools/ghidra_pipeline.sh all      import + apply
# Variables: GHIDRA (installation), PROJ (project folder), OUT (decompilation output)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
GHIDRA="${GHIDRA:-/opt/tools/ghidra_12.1.4_PUBLIC}"
PROJ="${PROJ:-/opt/gproj}"
OUT="${OUT:-$ROOT/game/out}"
RUN="$GHIDRA/support/analyzeHeadless $PROJ tombi"
SCRIPTS="$ROOT/ghidra/scripts"

do_import() {
  mkdir -p "$PROJ"
  $RUN -import "$ROOT/game/MAIN0.EXE" -overwrite -analysisTimeoutPerFile 900
  $RUN -import "$ROOT/game/AREA00/X000.BIN" -overwrite -loader BinaryLoader \
       -loader-baseAddr 800e8028 -processor "MIPS:LE:32:default" -analysisTimeoutPerFile 900
}

do_apply() {
  mkdir -p "$OUT"
  $RUN -process MAIN0.EXE -noanalysis -scriptPath "$SCRIPTS" \
       -postScript CreateFunctions.java "$ROOT/notes/main0_missing_functions.txt" \
       -postScript ImportNames.java "$ROOT/notes/names_main0.csv" \
       -postScript DefineObj.java \
       -postScript ApplyObj.java \
       -postScript ExportAll.java "$OUT"
  $RUN -process X000.BIN -noanalysis -scriptPath "$SCRIPTS" \
       -postScript ImportNames.java "$ROOT/notes/names_x000.csv" \
       -postScript DefineObj.java \
       -postScript ApplyObj.java \
       -postScript ExportAll.java "$OUT"
}

case "${1:-all}" in
  import) do_import ;;
  apply)  do_apply ;;
  all)    do_import; do_apply ;;
  *) echo "usage: $0 import|apply|all"; exit 1 ;;
esac
