#!/usr/bin/env bash
# Download a game-files URL (a GAME_FILES_URL* secret / variable) with retries, for CI and the session bootstrap.
# Accepts a direct URL, a Dropbox share link, a Google Drive share link or bare Drive id, and a public MEGA link with its
# key (https://mega.nz/file/ID#KEY, fetched with megatools' megadl, installed on the fly when missing). When what comes
# back is neither a zip / 7z nor at least MIN_BYTES, prints what it is (Drive answers an HTML page when a file is
# rate-limited, over quota, needs a virus-scan confirmation or is not shared) so the failure is diagnosable.
# Usage: tools/ci_download.sh URL OUT [MIN_BYTES=10000000]
set -u
URL="$1"; OUT="$2"; MIN="${3:-10000000}"
URL="$(printf '%s' "$URL" | tr -d '\r\n"'"'"' ')"
URL="$(printf '%s' "$URL" | sed -E 's#(dropbox\.com/.*[?&])dl=0#\1dl=1#')"
ID="$(printf '%s' "$URL" | sed -nE 's#.*drive\.google\.com/(file/d/|open\?id=|uc\?.*id=)([A-Za-z0-9_-]+).*#\2#p')"
if [ -z "$ID" ] && ! printf '%s' "$URL" | grep -q '://'; then ID="$URL"; fi
if [ -n "$ID" ]; then URL="https://drive.usercontent.google.com/download?id=$ID&export=download&confirm=t"; fi
ok() { unzip -tq "$OUT" >/dev/null 2>&1 || 7z t "$OUT" >/dev/null 2>&1 || [ "$(stat -c%s "$OUT" 2>/dev/null || echo 0)" -gt "$MIN" ]; }
if printf '%s' "$URL" | grep -q 'mega\.nz/'; then
  if ! command -v megadl >/dev/null; then
    (command -v sudo >/dev/null && SUDO=sudo || SUDO=""; $SUDO apt-get -qq update && $SUDO apt-get -qq install -y megatools) >/dev/null 2>&1 \
      || { echo "::error::megatools (megadl) is needed for a MEGA link and could not be installed"; exit 1; }
  fi
  for i in 1 2 3; do
    rm -f "$OUT"; megadl --no-progress --path "$OUT" "$URL" && ok && exit 0
    echo "::warning::attempt $i: megadl got $(stat -c%s "$OUT" 2>/dev/null || echo 0) bytes"; sleep $((20 * i))
  done
  exit 1
fi
for i in 1 2 3 4; do
  rm -f "$OUT"
  curl -fsSL --retry 2 -A "Mozilla/5.0" -c /tmp/ci_cookies -b /tmp/ci_cookies -o "$OUT" "$URL" || true
  # Drive "can't scan for viruses" page: follow its confirmation form
  if ! ok && grep -q 'name="confirm"' "$OUT" 2>/dev/null; then
    id=$(sed -nE 's/.*name="id" value="([^"]+)".*/\1/p' "$OUT" | head -1)
    uuid=$(sed -nE 's/.*name="uuid" value="([^"]+)".*/\1/p' "$OUT" | head -1)
    if [ -n "$id" ]; then
      curl -fsSL -A "Mozilla/5.0" -c /tmp/ci_cookies -b /tmp/ci_cookies -o "$OUT" \
        "https://drive.usercontent.google.com/download?id=$id&export=download&confirm=t&uuid=$uuid" || true
    fi
  fi
  ok && exit 0
  if grep -q 'Quota exceeded' "$OUT" 2>/dev/null; then echo "::error::Google Drive: download quota exceeded for this file (try later, or share it elsewhere, e.g. a MEGA link)"; exit 1; fi
  echo "::warning::attempt $i: got $(stat -c%s "$OUT" 2>/dev/null || echo 0) bytes, $(file -b "$OUT" 2>/dev/null | cut -c1-60)"
  sed -e 's/<[^>]*>/ /g' "$OUT" 2>/dev/null | tr -s ' \n' ' ' | cut -c1-400; echo
  sleep $((20 * i))
done
exit 1
