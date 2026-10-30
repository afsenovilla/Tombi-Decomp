#!/usr/bin/env bash
# Publishes built files as GitHub Release assets (instead of committing them to dist/):
#   build-YYYYMMDD-<sha7>  one versioned release per commit; every exe workflow of that commit adds its files to it
#   latest                 rolling pre-release "Latest build", each asset replaced by its newest build
# Usage: publish-release.sh FILE...   (env: GH_TOKEN, GITHUB_REPOSITORY, GITHUB_SHA)
set -uo pipefail
[ $# -gt 0 ] || { echo "no files"; exit 1; }
R="$GITHUB_REPOSITORY"; S7="${GITHUB_SHA::7}"
# the tag date is the commit date, so every workflow of one commit lands in the same release
D=$(gh api "repos/$R/commits/$GITHUB_SHA" --jq '.commit.committer.date' | cut -c1-10 | tr -d -)
TAG="build-$D-$S7"
SUBJ=$(gh api "repos/$R/commits/$GITHUB_SHA" --jq '.commit.message' | head -n1)

ensure() {  # ensure TAG TITLE NOTESFILE PRERELEASE TARGET
  for i in 1 2 3 4 5; do
    gh release view "$1" -R "$R" >/dev/null 2>&1 && return 0
    gh release create "$1" -R "$R" --target "$5" --title "$2" --notes-file "$3" $4 && return 0
    sleep $((i * 5 + RANDOM % 7))   # lost a race with another workflow (or API hiccup): re-check
  done
  return 1
}
upload() {  # upload TAG FILE...
  t=$1; shift
  for i in 1 2 3 4 5; do gh release upload "$t" "$@" -R "$R" --clobber && return 0; sleep $((i * 5)); done
  return 1
}
notes() {  # notes TAG -> body: commits since the previous build-* release + asset list
  prev=$(gh release list -R "$R" --limit 50 --json tagName --jq '.[].tagName' | grep '^build-' | grep -vx "$1" | head -n1)
  echo "Build of $GITHUB_SHA"; echo
  if [ -n "$prev" ]; then
    echo "### Changes since $prev"
    gh api "repos/$R/compare/$prev...$GITHUB_SHA" --jq '.commits[] | "- " + (.commit.message | split("\n")[0]) + " (" + .sha[0:7] + ")"' | tail -n 100
  else
    echo "### Commit"; echo "- $SUBJ ($S7)"
  fi
  echo; echo "### Assets"
  gh release view "$1" -R "$R" --json assets --jq '.assets[].name' 2>/dev/null | sed 's/^/- /'
}

N=$(mktemp); notes "$TAG" > "$N"
ensure "$TAG" "Build $D $S7" "$N" "" "$GITHUB_SHA" || exit 1
upload "$TAG" "$@" || exit 1
notes "$TAG" > "$N"; gh release edit "$TAG" -R "$R" --notes-file "$N" || true

# rolling "latest": moved to this commit, assets replaced one by one (other tools' assets kept)
L=$(mktemp); { echo "Rolling pre-release: the newest build of each tool (versioned builds: the build-* releases)."
  echo "Last update: $TAG — $SUBJ"; } > "$L"
ensure latest "Latest build" "$L" --prerelease "$GITHUB_SHA" || exit 1
upload latest "$@" || exit 1
gh api -X PATCH "repos/$R/git/refs/tags/latest" -f sha="$GITHUB_SHA" -F force=true >/dev/null || true
{ cat "$L"; echo; echo "### Assets"; gh release view latest -R "$R" --json assets --jq '.assets[].name' | sed 's/^/- /'; } > "$N"
gh release edit latest -R "$R" --prerelease --notes-file "$N" || true
echo "published to $TAG and latest"
