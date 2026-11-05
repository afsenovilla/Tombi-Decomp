#!/usr/bin/env python3
"""Pair unmatched JP (SLPS-01144) functions with PAL C files by callee names.

For every function of build/slps01144/report.json below 100 % (>= MINSIZE bytes) the jal targets of its target object
are translated to PAL names through config/slps01144/symbol_addrs_*.txt (`PAL <prog> 0x...` comments, or a PAL name
used as the JP name); a PAL source without a JP branch whose called names cover >= 60 % of that set (at least two) and
whose `// FUNC` size is within 0.8-1.25x is a candidate. Prints `prog:addr:size:palfile:0` lines for the scratch
sweep / tools/jpcheck.py. Found the contact / attack twins of 2026-11-05 (docs/JP.md).

  python3 tools/jp_pair_callees.py [MINSIZE=200]
"""
import glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    minsize = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    palname = {}
    for f in glob.glob(ROOT + "/config/symbol_addrs_*.txt"):
        for l in open(f):
            m = re.match(r"(\w+)\s*=\s*0x([0-9A-Fa-f]+);", l)
            if m: palname[int(m.group(2), 16)] = m.group(1)
    jp2pal = {}
    for f in glob.glob(ROOT + "/config/slps01144/symbol_addrs_*.txt"):
        for l in open(f):
            m = re.match(r"(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)", l)
            if not m: continue
            n, c = m.group(1), m.group(3)
            pm = re.search(r"PAL \w+ 0x([0-9A-Fa-f]+)", c)
            if pm and int(pm.group(1), 16) in palname: jp2pal[n] = palname[int(pm.group(1), 16)]
            elif not n.startswith("func_"): jp2pal[n] = n
    d = json.load(open(ROOT + "/build/slps01144/report.json"))
    jpf = []
    for u in d["units"]:
        for f in u.get("functions", []):
            sz = int(f["size"])
            if f.get("fuzzy_match_percent", 0) >= 100 or sz < minsize: continue
            prog = u["name"].split("/")[0]
            o = "%s/build/slps01144/target/%s/%s.o" % (ROOT, prog, f["name"])
            if not os.path.exists(o): continue
            r = subprocess.run(["mipsel-linux-gnu-objdump", "-r", o], capture_output=True, text=True).stdout
            cs = set(re.findall(r"R_MIPS_26\s+(\S+)", r)) - {".text"}
            pal = {jp2pal[c] for c in cs if c in jp2pal}
            if len(pal) >= 2: jpf.append((sz, prog, f["name"], pal))
    pf = []
    for f in glob.glob(ROOT + "/src/**/*.c", recursive=True):
        s = open(f, errors="replace").read()
        if "VERSION SLPS_01144" in s: continue
        m = re.search(r"// FUNC ([0-9a-f]+) (\d+) (\w+)", s)
        if not m: continue
        pf.append((int(m.group(2)), os.path.relpath(f, ROOT), set(re.findall(r"\b([A-Za-z_]\w*)\s*\(", s))))
    out = []
    for sz, prog, name, pal in jpf:
        best = None
        for psz, pfile, calls in pf:
            if not (0.8 <= psz / sz <= 1.25): continue
            inter = len(pal & calls)
            if inter >= 2 and inter / len(pal) >= 0.6:
                k = (inter / len(pal), -abs(psz - sz))
                if best is None or k > best[0]: best = (k, pfile)
        if best: out.append((sz, prog, name, best[1], best[0][0]))
    out.sort(key=lambda r: -r[0])
    for sz, prog, name, pfile, sc in out:
        a = re.sub(r"func_(\w{8}).*", r"\1", name)
        print("%s:%s:%d:%s:0   # %.2f" % (prog, a, sz, pfile, sc))


if __name__ == "__main__":
    main()
