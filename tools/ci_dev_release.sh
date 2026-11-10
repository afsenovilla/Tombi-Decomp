#!/bin/bash
# Puts one asset into the rolling pre-release tagged "dev" (created when missing, never deleted: the Windows exe and the
# Android APK come from different workflows and share it) and keeps one line per asset in its notes.
#   tools/ci_dev_release.sh FILE LABEL        (GH_TOKEN, R = owner/repo, GITHUB_SHA set by the workflow)
set -e
FILE=$1; LABEL=$2; NAME=$(basename "$FILE")
SHA7=${GITHUB_SHA::7}; DAY=$(date -u +%Y-%m-%d)
SUBJECT=$(gh api "repos/$R/commits/$GITHUB_SHA" --jq .commit.message | head -n1)
HEAD_NOTE="Automatic builds of master, replaced after every green build (not a release: the latest versioned release is the stable one). Unpack the zip and run tombi-port.exe; install the APK over the previous dev build."
if ! gh release view dev -R "$R" >/dev/null 2>&1; then
  gh release create dev -R "$R" --prerelease --latest=false --target "$GITHUB_SHA" --title "dev build" --notes "$HEAD_NOTE"
fi
gh release upload dev -R "$R" --clobber "$FILE"
LINE="- $NAME ($LABEL): master $SHA7, $DAY: $SUBJECT"
BODY=$(gh release view dev -R "$R" --json body --jq .body | grep "^- " | grep -v "^- $NAME (" || true)
printf '%s\n\n%s\n%s\n' "$HEAD_NOTE" "$BODY" "$LINE" | sed '/^$/N;/^\n$/D' > "$RUNNER_TEMP/dev_notes.md"
gh release edit dev -R "$R" --prerelease --latest=false --title "dev build ($DAY, $SHA7)" --notes-file "$RUNNER_TEMP/dev_notes.md"
echo "https://github.com/$R/releases/download/dev/$NAME"
