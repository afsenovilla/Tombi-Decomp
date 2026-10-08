#!/usr/bin/env python3
"""Tries to match functions WITHOUT using AI: turns Ghidra's decompilation into compilable C,
compiles it with several flag combinations and verifies with matchcheck. CPU only.
Usage: tools/automatch.py [--max-size N] [--limit N] [--work DIR]
The ones that match are copied to src/<Name>.c with the // MATCHING marker.
"""
import csv, os, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
args = sys.argv[1:]
def opt(n, d):
    return args[args.index(n) + 1] if n in args else d
MAXSZ = int(opt("--max-size", 400)); LIMIT = int(opt("--limit", 100000))
WORK = opt("--work", "/opt/psyq/auto"); BATCH = 60
VARIANTS = ["-O2 -G0", "-O1 -G0", "-O2 -G8"]

PRELUDE = """#include "TOBJ.H"
typedef unsigned char undefined, undefined1, byte, uchar;
typedef unsigned short undefined2, ushort;
typedef unsigned int undefined4, uint;
typedef unsigned long ulong;
typedef int bool;
typedef void code(void);
"""

def decomp_blocks():
    out = {}
    for prog, fn in (("MAIN0", "MAIN0_EXE"), ("X000", "X000_BIN")):
        t = open("%s/game/out/%s_decomp.c" % (ROOT, fn), errors="replace").read()
        for m in re.finditer(r"// ==== (\S+) @ ([0-9a-f]{8}) size=(\d+)\n(.*?)(?=\n// ==== |\Z)", t, re.S):
            out[m.group(2)] = (prog, m.group(1), int(m.group(3)), m.group(4).strip())
    return out

def make_c(addr, prog, size, body, flags):
    ids = set(re.findall(r"\b(DAT_[0-9a-f]+|PTR_\w+|FUN_[0-9a-f]+|thunk_FUN_[0-9a-f]+|s_\w+_[0-9a-f]+|_DAT_[0-9a-f]+)\b", body))
    decl = []
    for i in sorted(ids):
        decl.append("extern int %s();" % i if i.startswith(("FUN_", "thunk_")) else "extern int %s;" % i)
    own = re.match(r"\S[^\n(]*?\b(\w+)\(", body)
    for d in list(decl):
        if own and (" %s();" % own.group(1)) in d: decl.remove(d)
    hdr = "// FUNC %s %d %s\n// FLAGS %s\n" % (addr, size, prog, flags)
    return hdr + PRELUDE + "\n".join(decl) + "\n\n" + body + "\n"

def main():
    todo = [r for r in csv.DictReader(open(ROOT + "/notes/todo_match.csv"))
            if int(r["size"]) <= MAXSZ]
    have = set()
    for dp, _, fs in os.walk(ROOT + "/src"):
        for f in fs:
            m = re.search(r"//\s*FUNC\s+([0-9a-f]+)", open(os.path.join(dp, f), errors="replace").read())
            if m and "wip" not in dp: have.add(m.group(1))
    blocks = decomp_blocks()
    todo = [r for r in todo if r["address"] not in have and r["address"] in blocks][:LIMIT]
    shutil.rmtree(WORK, ignore_errors=True); os.makedirs(WORK)
    won = 0
    for b in range(0, len(todo), BATCH // len(VARIANTS)):
        chunk = todo[b:b + BATCH // len(VARIANTS)]
        files = []
        for r in chunk:
            prog, name, size, body = blocks[r["address"]]
            body = re.sub(r"^(\s*)\bundefined\b\s+", r"\1undefined1 ", body)
            for k, fl in enumerate(VARIANTS):
                p = "%s/%s_v%d.c" % (WORK, r["address"], k)
                open(p, "w").write(make_c(r["address"], prog, size, body, fl)); files.append(p)
        env = dict(os.environ, WORK="/opt/psyq/w_auto")
        res = subprocess.run([sys.executable, ROOT + "/tools/matchcheck.py"] + files, env=env,
                             capture_output=True, text=True, timeout=1800)
        seen = set()
        for line in res.stdout.splitlines():
            m = re.match(r"MATCH\s+(\S+)_v(\d)\.c\s+([0-9a-f]+)", line)
            if m and m.group(3) not in seen:
                seen.add(m.group(3)); addr = m.group(3)
                row = next(r for r in chunk if r["address"] == addr)
                name = row["name"]; dst = "%s/src/%s.c" % (ROOT, name)
                shutil.copy("%s/%s_v%s.c" % (WORK, addr, m.group(2)), dst)
                subprocess.run([sys.executable, ROOT + "/tools/matchcheck.py", "--mark", dst],
                               env=env, capture_output=True)
                won += 1; print("AUTO-MATCH", name, addr, flush=True)
        print("batch %d/%d: %d matches in total" % (b // (BATCH // len(VARIANTS)) + 1,
              -(-len(todo) // (BATCH // len(VARIANTS))), won), flush=True)

main()
