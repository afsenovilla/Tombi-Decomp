#!/usr/bin/env bash
# Installs Ghidra 12.1.4 and the ghidra_psx_ldr plugin (extension zip for 12.1.4) in /opt/tools.
# Usage: tools/setup_ghidra.sh /path/ghidra_12.1.4_PUBLIC_20261004_ghidra_psx_ldr.zip
set -euo pipefail
PLUGIN_ZIP="${1:?missing the ghidra_psx_ldr plugin zip}"
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
echo "Ghidra ready in $DEST/ghidra_12.1.4_PUBLIC (requires JDK 21)"
