#!/usr/bin/env bash
# CI helper (.github/workflows/report.yml): download a game-files URL (Google Drive or direct) with retries and,
# when the result is neither a zip nor a disc image, print what came back so the failure is diagnosable
# (Drive answers an HTML page when a file is rate-limited, needs a virus-scan confirmation or is not shared).
# Usage: tools/ci_download.sh URL OUT MIN_BYTES
set -u
URL="$1"; OUT="$2"; MIN="${3:-10000000}"
ok() { unzip -tq "$OUT" >/dev/null 2>&1 || [ "$(stat -c%s "$OUT" 2>/dev/null || echo 0)" -gt "$MIN" ]; }
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
  echo "::warning::attempt $i: got $(stat -c%s "$OUT" 2>/dev/null || echo 0) bytes, $(file -b "$OUT" 2>/dev/null | cut -c1-60)"
  sed -e 's/<[^>]*>/ /g' "$OUT" 2>/dev/null | tr -s ' \n' ' ' | cut -c1-400; echo
  sleep $((20 * i))
done
exit 1
