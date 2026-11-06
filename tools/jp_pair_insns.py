#!/usr/bin/env python3
"""Pair unmatched JP (SLPS-01144) functions with PAL C files by instruction shape.

The callee pairer (tools/jp_pair_callees.py) misses functions whose JP form inlines a call, drops one or reads raw
globals for struct fields. This one compares opcode sequences instead: every function of build/slps01144/report.json
below 100 % (>= MINSIZE bytes) is disassembled from its target object, every PAL function compiled under build/base/
likewise, and the PAL function with the best difflib ratio of mnemonics (size within 0.6-1.6x, ratio >= MINRATIO)
is the candidate. Prints `prog:addr:size:palfile:0   # ratio palname` lines for the scratch sweep / tools/jpcheck.py.

  python3 tools/jp_pair_insns.py [MINSIZE=100] [MINRATIO=0.6]
"""
import difflib, glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def mnems(o):
    r = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", o], capture_output=True, text=True).stdout
    return [l.split("\t")[1].split()[0] for l in r.split("\n") if re.match(r"^\s+[0-9a-f]+:\t", l) and len(l.split("\t")) > 1]


def main():
    minsize = int(sys.argv[1]) if len(sys.argv) > 1 else 100
    minratio = float(sys.argv[2]) if len(sys.argv) > 2 else 0.6
    have = set()
    srcof = {}
    for f in glob.glob(ROOT + "/src/**/*.c", recursive=True):
        s = open(f, errors="replace").read()
        for m in re.findall(r"^// VERSION SLPS_01144 (\S+) ([0-9A-Fa-f]{8})", s, re.M): have.add(m)
        if "VERSION SLPS_01144" in s: continue
        srcof.setdefault(os.path.splitext(os.path.basename(f))[0], os.path.relpath(f, ROOT))
    pal = []
    for o in glob.glob(ROOT + "/build/base/*/*.o"):
        n = os.path.splitext(os.path.basename(o))[0]
        if n not in srcof: continue
        m = mnems(o)
        if len(m) >= minsize // 8: pal.append((n, srcof[n], m))
    d = json.load(open(ROOT + "/build/slps01144/report.json"))
    for u in d["units"]:
        for f in u.get("functions", []):
            sz = int(f["size"])
            if f.get("fuzzy_match_percent", 0) >= 100 or sz < minsize: continue
            prog = u["name"].split("/")[0]
            a = re.sub(r"func_(\w{8}).*", r"\1", f["name"])
            if (prog, a) in have: continue
            o = "%s/build/slps01144/target/%s/%s.o" % (ROOT, prog, f["name"])
            if not os.path.exists(o): continue
            jm = mnems(o)
            best = (0, None)
            for n, src, pm in pal:
                if not (0.6 <= len(pm) / len(jm) <= 1.6): continue
                sm = difflib.SequenceMatcher(None, jm, pm, autojunk=False)
                if sm.real_quick_ratio() < max(minratio, best[0]) or sm.quick_ratio() < max(minratio, best[0]): continue
                r = sm.ratio()
                if r > best[0]: best = (r, (n, src))
            if best[1] and best[0] >= minratio:
                print("%s:%s:%d:%s:0   # %.2f %s" % (prog, a, sz, best[1][1], best[0], best[1][0]), flush=True)


if __name__ == "__main__":
    main()
