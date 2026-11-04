#!/usr/bin/env bash
# Turn a game-files download (tools/ci_download.sh) into the disc's file tree, whatever its shape: a raw disc image
# (.bin of 2352-byte sectors or a 2048-byte ISO), a zip / 7z holding the image (in any folder, with .cue / .txt beside
# it), or a zip of the already extracted files. The image, when there is one, is kept beside the tree as OUT_DIR.img.
# PrePre 8 (PCPX-96084) needs tools/prepre8.py extract instead of isoext.py: pass prepre8 as the third argument.
# Usage: tools/disc_unpack.sh DOWNLOAD OUT_DIR [prepre8]   -> OUT_DIR/ (the files), OUT_DIR.img (the image, if any)
set -eu
DL="$1"; OUT="$2"; KIND="${3:-}"
HERE="$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$OUT"
X="$(mktemp -d)"
if unzip -tq "$DL" >/dev/null 2>&1; then unzip -o -q "$DL" -d "$X"
elif 7z t "$DL" >/dev/null 2>&1; then 7z x -y -bd -o"$X" "$DL" >/dev/null
else cp "$DL" "$X/disc.bin"; fi
chmod -R u+rwX "$X"            # some archives carry read-only folders
IMG="$(find "$X" -type f \( -iname '*.bin' -o -iname '*.iso' -o -iname '*.img' \) -size +10M -printf '%s %p\n' | sort -n | tail -1 | cut -d' ' -f2-)"
if [ -n "$IMG" ]; then
  if [ "$KIND" = prepre8 ]; then python3 -I "$HERE/prepre8.py" extract "$IMG" "$OUT"
  else python3 -I "$HERE/isoext.py" "$IMG" "$OUT"; fi
  mv "$IMG" "$OUT.img"
else                            # a zip of the extracted files: the folder that holds them (SYSTEM/ or an .EXE)
  ROOT="$(find "$X" -type d -iname SYSTEM -printf '%h\n' | head -1)"
  [ -n "$ROOT" ] || ROOT="$(find "$X" -type f -iname '*.EXE' -printf '%h\n' | head -1)"
  [ -n "$ROOT" ] || ROOT="$X"
  cp -r "$ROOT"/. "$OUT/"
fi
rm -rf "$X"
echo "$OUT: $(find "$OUT" -type f | wc -l) files$( [ -s "$OUT.img" ] && echo ", image $OUT.img")"
