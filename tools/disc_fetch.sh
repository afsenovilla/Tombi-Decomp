#!/usr/bin/env bash
# Fetch and extract one demo / prototype disc for this session, from its GAME_FILES_URL_<NAME> environment variable:
#   bash tools/disc_fetch.sh DENGEKI_D5      (or PREPRE9, PREPRE8, HYPER12, YOIKO2, JP_DEMO, BETA980203, USA, SCES01330)
# -> game_<name>/fs (the files) and game_<name>/disc.bin (the image), in the repo root, ignored by git (never
# committed; /dev/shm was tried first and is wiped whenever the idle container restarts). Does nothing when the disc
# is already there. tools/event_icons.py and the code hunts read these paths.
set -eu
NAME="${1:?disc name, e.g. DENGEKI_D5}"; V="GAME_FILES_URL_$NAME"
[ -n "${!V:-}" ] || { echo "$V is not set in this environment"; exit 1; }
HERE="$(cd "$(dirname "$0")" && pwd)"
D="$(dirname "$HERE")/game_$(printf '%s' "$NAME" | tr 'A-Z' 'a-z')"
[ -d "$D/fs" ] && { echo "$D/fs is already there"; exit 0; }
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
bash "$HERE/ci_download.sh" "${!V}" "$T/dl" 10000000
KIND=""; [ "$NAME" = PREPRE8 ] && KIND=prepre8
bash "$HERE/disc_unpack.sh" "$T/dl" "$D/fs" $KIND
[ -s "$D/fs.img" ] && mv "$D/fs.img" "$D/disc.bin"
du -sh "$D"
