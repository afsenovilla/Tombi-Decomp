#!/usr/bin/env bash
# Fetch and extract one demo / prototype disc for this session, from its GAME_FILES_URL_<NAME> environment variable:
#   bash tools/disc_fetch.sh DENGEKI_D5      (or PREPRE9, PREPRE8, JP_DEMO, BETA980203, USA, SCES01330)
# -> /dev/shm/<name>/fs (the files) and /dev/shm/<name>/disc.bin (the image). RAM only, gone with the container, never
# committed. Does nothing when the disc is already there. tools/event_icons.py and the code hunts read these paths.
set -eu
NAME="${1:?disc name, e.g. DENGEKI_D5}"; V="GAME_FILES_URL_$NAME"
[ -n "${!V:-}" ] || { echo "$V is not set in this environment"; exit 1; }
D="/dev/shm/$(printf '%s' "$NAME" | tr 'A-Z' 'a-z')"
[ -d "$D/fs" ] && { echo "$D/fs is already there"; exit 0; }
HERE="$(cd "$(dirname "$0")" && pwd)"
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
bash "$HERE/ci_download.sh" "${!V}" "$T/dl" 10000000
KIND=""; [ "$NAME" = PREPRE8 ] && KIND=prepre8
bash "$HERE/disc_unpack.sh" "$T/dl" "$D/fs" $KIND
[ -s "$D/fs.img" ] && mv "$D/fs.img" "$D/disc.bin"
du -sh "$D"
