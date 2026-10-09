#!/usr/bin/env python3
"""Atomically pops the next line of a shared work queue (used when several agents work in parallel).
Usage: python3 tools/claim.py /path/to/queue.txt   -> prints the next line, or nothing when the queue is empty."""
import fcntl, sys

q = sys.argv[1]
with open(q + ".lock", "w") as lock:
    fcntl.flock(lock, fcntl.LOCK_EX)
    lines = open(q).read().splitlines(True)
    if lines:
        open(q, "w").writelines(lines[1:])
        print(lines[0].rstrip("\n"))
