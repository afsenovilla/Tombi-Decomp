#!/usr/bin/env python3
"""Atomically pops the next line of a shared work queue (used when several agents work in parallel).
Usage: python3 tools/claim.py /path/to/queue.txt   -> prints the next line, or nothing when the queue is empty."""
import fcntl, os, sys

q = sys.argv[1]
with open(os.path.splitext(q)[0] + ".lock", "w") as lock:  # same lock file as the shell claim scripts
    fcntl.flock(lock, fcntl.LOCK_EX)
    lines = open(q).read().splitlines(True)
    if lines:
        open(q, "w").writelines(lines[1:])
        print(lines[0].rstrip("\n"))
