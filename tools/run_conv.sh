#!/bin/sh
# usage: run_conv.sh <conv script> files...  : convert, check each file's object, retry failures in --direct mode, revert the rest
cd "$(dirname "$0")/.." || exit 1
S=$1; shift
python3 $S "$@" > /dev/null
ch=$(git diff --name-only -- src)
[ -z "$ch" ] && { echo none; exit 0; }
bad=$(python3 tools/objcmp.py $ch | grep '^DIFF' | cut -d' ' -f2)
if [ -n "$bad" ]; then
  git checkout -- $bad
  python3 $S --direct $bad > /dev/null
  ch2=$(git diff --name-only -- src $bad)
  bad2=$(python3 tools/objcmp.py $(git diff --name-only -- $bad) | grep '^DIFF' | cut -d' ' -f2)
  [ -n "$bad2" ] && git checkout -- $bad2
fi
echo "converted: $(git diff --name-only -- src | wc -l)"
