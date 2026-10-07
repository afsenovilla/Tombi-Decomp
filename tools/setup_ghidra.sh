#!/usr/bin/env bash
# Instala Ghidra 12.1.4 y el plugin ghidra_psx_ldr (zip de la extension para 12.1.4) en /opt/tools.
# Uso: tools/setup_ghidra.sh /ruta/ghidra_12.1.4_PUBLIC_20261004_ghidra_psx_ldr.zip
set -euo pipefail
PLUGIN_ZIP="${1:?falta el zip del plugin ghidra_psx_ldr}"
DEST="${DEST:-/opt/tools}"
mkdir -p "$DEST"
if [ ! -d "$DEST/ghidra_12.1.4_PUBLIC" ]; then
  curl -L -o "$DEST/ghidra.zip" \
    "https://github.com/NationalSecurityAgency/ghidra/releases/download/Ghidra_12.1.4_build/ghidra_12.1.4_PUBLIC_20260921.zip"
  unzip -q "$DEST/ghidra.zip" -d "$DEST" && rm "$DEST/ghidra.zip"
fi
TMP="$(mktemp -d)"; unzip -q "$PLUGIN_ZIP" -d "$TMP"
rm -rf "$DEST/ghidra_12.1.4_PUBLIC/Ghidra/Extensions/ghidra_psx_ldr"
cp -r "$TMP/ghidra_psx_ldr" "$DEST/ghidra_12.1.4_PUBLIC/Ghidra/Extensions/"
rm -rf "$TMP"
echo "Ghidra listo en $DEST/ghidra_12.1.4_PUBLIC (necesita JDK 21)"
