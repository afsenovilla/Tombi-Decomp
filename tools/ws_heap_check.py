#!/usr/bin/env python3
"""Checks the widescreen picture strips' heap tables (port2/runtime/ws_heap.h, docs/PORT.md "Widescreen tables on
the heap"): every function of ws_backdrop.c (with its ws_backdrop_*.inc) that touches a heap table must be reached
only from entry points whose list (ws_need_ENTRY, at the end of the includes in ws_backdrop.c) asks for that table.
A table missing from a list is a NULL pointer at run time, in that room only.

    python3 tools/ws_heap_check.py            prints the entry points and their tables; exit 1 on a missing one
    python3 tools/ws_heap_check.py --lists    prints the lists the call graph asks for (C, to paste)

The call graph is read from the source text: a function counts as called where its name appears in a body (a call
or a function pointer taken), so the check errs on the side of asking for more.  An entry point is a function that
starts with `if (!ws_need_X()) return`; the closure of an entry stops at other entry points (they ask for their own).
Functions outside every entry's closure that read a heap table directly are allowed only in ROOTS below (they run
after an entry point has asked for the tables, in the same call)."""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RT = os.path.join(ROOT, "port2", "runtime")
# reached outside an entry point's closure, after the tables were asked for: port_ws_backdrop's body (the research
# dump after a build, the room's picture once grab has asked for it), the rooms' `is` tests (after grab), the
# standalone main's file reading
ROOTS = {"backdrop", "port_ws_backdrop", "main", "tw_main", "im_main", "port_ws_backdrop_over", "port_ws_backdrop_fix",
         "port_ws_island_fg", "v_cherry", "v_not_cherry"}


def load(p):
    out = []
    for l in open(p):
        m = re.match(r'\s*#include\s+"(ws_backdrop[^"]*\.inc)"', l)
        out.append(load(os.path.join(RT, m.group(1))) if m else l)
    return "".join(out)


def strip(t):
    t = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), t, flags=re.S)
    t = re.sub(r"//[^\n]*", "", t)
    t = re.sub(r'"(\\.|[^"\\])*"', '""', t)
    t = re.sub(r"'(\\.|[^'\\])*'", "0", t)
    return re.sub(r"^\s*#[^\n]*", "", t, flags=re.M)


def functions(t):
    funcs = {}; depth = 0; cur = None; body = []; last = None; paren = 0; sig = None
    toks = re.findall(r"[A-Za-z_]\w*|\{|\}|\(|\)|;|=", t)
    for k, s in enumerate(toks):
        if depth == 0:
            if s == "(":
                if paren == 0: sig = last
                paren += 1
            elif s == ")": paren -= 1
            elif s in (";", "="):
                if paren == 0: sig = None
            elif s == "{":
                cur = sig if paren == 0 and sig and toks[k - 1] == ")" else None
                body = []; depth = 1
            elif re.match(r"\w", s): last = s
        else:
            if s == "{": depth += 1
            elif s == "}":
                depth -= 1
                if depth == 0:
                    if cur: funcs.setdefault(cur, []).extend(body)
                    cur = None; sig = None
            elif cur and re.match(r"\w", s): body.append(s)
    return funcs


def main():
    raw = load(os.path.join(RT, "ws_backdrop.c"))
    t = strip(raw)
    funcs = functions(t)
    lists = {}                                   # entry list name -> tables
    for m in re.finditer(r"static int ws_need_(\w+)\(void\)\s*\{(.*?)\n\}", raw, flags=re.S):
        lists[m.group(1)] = set(x.split(".")[0] for x in re.findall(r"WS_T\(\s*([\w.]+)\s*,", m.group(2)))
    heap = set().union(*lists.values())
    entry_of = {}                                # function -> its list
    for f, body in funcs.items():
        for i, w in enumerate(body):
            if w.startswith("ws_need_") and w[8:] in lists and not f.startswith("ws_need_"):
                entry_of.setdefault(f, set()).add(w[8:])
    calls = {f: set(b for b in body if b in funcs and b != f) for f, body in funcs.items()}
    refs = {f: set(b for b in body if b in heap) for f, body in funcs.items()}
    bad = 0; reached = set()
    for e in sorted(entry_of):
        have = set().union(*(lists[x] for x in entry_of[e]))
        seen, st = set(), [e]
        while st:
            x = st.pop()
            if x in seen: continue
            seen.add(x)
            st += [c for c in calls.get(x, ()) if c not in entry_of]
        reached |= seen
        want = set().union(*(refs.get(x, set()) for x in seen))
        miss = want - have
        if "--lists" in sys.argv:
            print("%s: %s" % (e, " ".join(sorted(want))))
        else:
            print("%-14s %3d tables%s" % (e, len(want), "  MISSING: " + " ".join(sorted(miss)) if miss else ""))
        if miss:
            for x in sorted(seen):
                if refs.get(x, set()) & miss: print("    %s uses %s" % (x, " ".join(sorted(refs[x] & miss))))
            bad = 1
    live, st = set(), [f for f in funcs if f.startswith("port_ws_") or f == "main"]   # (dead helpers are left out)
    while st:
        x = st.pop()
        if x not in live: live.add(x); st += list(calls.get(x, ()))
    for f in sorted(funcs):
        if f in live and f not in reached and f not in ROOTS and not f.startswith("ws_need_") and refs.get(f):
            print("%s uses %s outside every entry point" % (f, " ".join(sorted(refs[f]))))
            bad = 1
    print("heap tables: %d, entry points: %d: %s" % (len(heap), len(entry_of), "MISSING TABLES" if bad else "ok"))
    return bad


if __name__ == "__main__":
    sys.exit(main())
