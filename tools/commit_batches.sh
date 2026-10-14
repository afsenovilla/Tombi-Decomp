#!/bin/sh
# usage: commit_batches.sh "<subject prefix>" [extra paths]  : commit changed src files in chunks of <=45 (+extra paths in the first)
cd "$(dirname "$0")/.." || exit 1
P=$1; shift
git diff --name-only -- src > /tmp/cb_list.txt
split -l 45 -d /tmp/cb_list.txt /tmp/cb_c_
i=0
for c in /tmp/cb_c_*; do
  n=$(wc -l < $c); X=""; [ $i = 0 ] && X="$*"
  [ -n "$X" ] && git add $X; git commit -q -m "$P ($n files, per-file linked object identical via tools/objcmp.py; full build counts + asm list unchanged)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_01VgmRdkLPfdCqeoJmcLwyi2" -- $(cat $c) $X
  i=$((i+1))
done
rm -f /tmp/cb_c_* /tmp/cb_list.txt
