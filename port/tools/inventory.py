#!/usr/bin/env python3
"""Port inventory: which Psy-Q / hardware functions the game C calls (and how often), inline asm,
hard-coded addresses, per-program file counts.  Usage: python3 port/tools/inventory.py [--md]"""
import os, re, sys, collections
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from units import units

U = units()
lib_names, game_files = set(), collections.defaultdict(list)
lib_files = set()
for prog, us in U.items():
    for u in us:
        if u.lib:
            if prog == "MAIN0": lib_names.add(u.name)
            if u.file: lib_files.add(u.file)
        elif u.file:
            game_files[prog].append(u.file)


def strip_comments(s):
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.S)
    return re.sub(r"//[^\n]*", " ", s)


calls = collections.Counter(); callers = collections.defaultdict(set)
asm = []; addr = collections.Counter(); scratch = 0; funcptr_calls = 0
seen = set()
for prog, files in game_files.items():
    for f in files:
        if f in seen or f in lib_files: continue
        seen.add(f)
        txt = strip_comments(open(os.path.join(ROOT, f), errors="replace").read())
        for m in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", txt):
            if m.group(1) in lib_names:
                calls[m.group(1)] += 1; callers[m.group(1)].add(prog)
        if re.search(r"__asm__|\basm\s*\(|\bgte_\w+\(", txt):
            asm.append(f)
        for m in re.finditer(r"0x(80[0-9A-Fa-f]{6}|1[Ff]80[0-9A-Fa-f]{4})\b", txt):
            v = m.group(1).upper()
            if v.startswith("1F80"): scratch += 1
            else: addr[v[:4]] += 1
        funcptr_calls += len(re.findall(r"\(\s*\*\s*[\w\.\->\[\]]+\s*\)\s*\(", txt))

md = "--md" in sys.argv
print("game C files: " + ", ".join("%s %d" % (p, len(v)) for p, v in game_files.items()))
print("distinct game C files scanned: %d; Psy-Q C files (replaced in the port): %d" % (len(seen), len(lib_files)))
print("Psy-Q functions in MAIN0: %d; called directly from game C: %d" % (len(lib_names), len(calls)))
print("files with inline asm / gte macros: %d" % len(asm))
print("hard-coded 0x80xxxxxx literals: %d (by 64K page: %s); 0x1F80xxxx scratchpad literals: %d"
      % (sum(addr.values()), ", ".join("%s:%d" % kv for kv in addr.most_common(8)), scratch))
print("explicit calls through function pointers: %d" % funcptr_calls)
print()
for n, c in sorted(calls.items(), key=lambda kv: (-kv[1], kv[0])):
    print(("| `%s` | %d | %s |" if md else "%-28s %5d  %s") % (n, c, " ".join(sorted(callers[n]))))
print()
for f in asm: print("asm:", f)
