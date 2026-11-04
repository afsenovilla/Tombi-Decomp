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
# $DISC_CACHE (a folder; on Actions set by .github/actions/disc-cache): a copy of every download, named by the URL's
# hash, so the same disc is fetched once. Hard links when the cache and OUT share a file system, a copy otherwise.
CACHED=""
if [ -n "${DISC_CACHE:-}" ]; then
  mkdir -p "$DISC_CACHE"; CACHED="$DISC_CACHE/$(printf '%s' "$URL" | sha256sum | cut -c1-32).dl"
  if [ -s "$CACHED" ]; then rm -f "$OUT"; ln "$CACHED" "$OUT" 2>/dev/null || cp "$CACHED" "$OUT"; ok && { echo "disc from cache"; exit 0; }; rm -f "$OUT"; fi
fi
keep() { [ -n "$CACHED" ] && { ln "$OUT" "$CACHED" 2>/dev/null || cp "$OUT" "$CACHED"; }; exit 0; }
if printf '%s' "$URL" | grep -q 'mega\.nz/'; then
  # tools/mega_dl.py (MEGA API + AES-CTR in Python, works through an HTTPS proxy) first; megadl as the fallback
  HERE="$(cd "$(dirname "$0")" && pwd)"
  python3 -c "import cryptography" 2>/dev/null || python3 -m pip install -q cryptography >/dev/null 2>&1 || true
  for i in 1 2 3; do
    rm -f "$OUT"
    if python3 -I "$HERE/mega_dl.py" "$URL" "$OUT" 2>"$OUT.err"; then ok && rm -f "$OUT.err" && keep; fi
    if grep -q "509\|Bandwidth Limit" "$OUT.err"; then echo "::error::MEGA: transfer quota of this address exhausted for now (HTTP 509); try again in a few hours"; cat "$OUT.err" | tail -2; rm -f "$OUT.err"; exit 1; fi
    tail -2 "$OUT.err"
    if command -v megadl >/dev/null || { (command -v sudo >/dev/null && SUDO=sudo || SUDO=""; $SUDO apt-get -qq update && $SUDO apt-get -qq install -y megatools) >/dev/null 2>&1; }; then
      rm -f "$OUT"; timeout 1800 megadl --no-progress --path "$OUT" "$URL" && ok && rm -f "$OUT.err" && keep
    fi
    echo "::warning::attempt $i: MEGA download got $(stat -c%s "$OUT" 2>/dev/null || echo 0) bytes"; sleep $((20 * i))
  done
  rm -f "$OUT.err"; exit 1
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
  ok && keep
  if grep -q 'Quota exceeded' "$OUT" 2>/dev/null; then echo "::error::Google Drive: download quota exceeded for this file (try later, or share it elsewhere, e.g. a MEGA link)"; exit 1; fi
  echo "::warning::attempt $i: got $(stat -c%s "$OUT" 2>/dev/null || echo 0) bytes, $(file -b "$OUT" 2>/dev/null | cut -c1-60)"
  sed -e 's/<[^>]*>/ /g' "$OUT" 2>/dev/null | tr -s ' \n' ' ' | cut -c1-400; echo
  sleep $((20 * i))
done
exit 1
