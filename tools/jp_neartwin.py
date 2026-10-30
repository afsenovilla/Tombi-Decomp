#!/usr/bin/env python3
"""Near-twin pass for the JP retail build: PAL C compiled for JP (with scripted PAL->JP edits), installed when it matches.

For every JP function with a PAL twin that has C (config/slps01144/symbol_addrs_*.txt `PAL <mod> <addr>`, tools/units.py) and
no JP C yet, the PAL source is compiled with VERSION_SLPS_01144 (tools/jpcheck.py) in a few variants:
  raw | pad bits (`& 0x20` -> `& 0x2000`, 0xc00 -> 0xc) | no vibration check (SPAD.b1a4 / startPadVibration / GV2.bb0 = 0) | both.
A variant that diffs IDENTICAL against build/slps01144/target/ is installed as src/[x0nn/]func_<addr>_slps01144.c
(`// VERSION SLPS_01144` line + `#if defined(VERSION_SLPS_01144)`), and the JP symbol is renamed func_<addr>_slps01144.

  python3 tools/jp_neartwin.py [--install] [--limit N]     (needs a build_report --version slps01144 run for the targets)"""
import os, re, sys, glob, shutil, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT + "/tools")
import units as U
import jpcheck

PAD = [(r"&\s*0x(10|20|40|80)\b", lambda m: "& 0x%s00" % m.group(1)), (r"&\s*0x([0-9a-fA-F])00\b", lambda m: "& 0x%s" % m.group(1)),
       (r"0xc00\b", lambda m: "0xc"), (r"0x300\b", lambda m: "0x3")]


def pad(t):
    for rx, f in PAD: t = re.sub(rx, f, t)
    return t


def novib(t):
    out = []
    for l in t.split("\n"):
        if re.search(r"startPadVibration|\bb1a4\b|\.bb0\s*=\s*0\s*;", l):
            if re.match(r"\s*if\b.*\)\s*$", l) is None and not l.rstrip().endswith("{"): continue
            continue
        out.append(l)
    return "\n".join(out)


def demo(t): return t.replace("VERSION_SLPM_80165", "VERSION_SLPS_01144")  # the JP demo branch of the shared file is often the retail code


def dbl(t): return re.sub(r"s \+= (GAME_SELECTED_SECTION\.a|\*q);", r"s += \1 * 2;", demo(t))  # retail doubles the section index too


VARIANTS = [("demo2", dbl),("raw", lambda t: t), ("demo", demo), ("demo+pad", lambda t: pad(demo(t))), ("demo+novib", lambda t: novib(demo(t))), ("pad", pad), ("novib", novib), ("pad+novib", lambda t: pad(novib(t)))]


MOD = {}   # PAL src file -> PAL module of the function it holds


def candidates(minr=0.85):
    """Unmatched JP functions with no JP C and the closest matched PAL C function(s) by masked opcode skeleton (difflib)."""
    import json, difflib, collections, struct
    from xmatch import pal_funcs, mask, words
    from jp_globals import yaml_img
    pal = {(p.lower(), u.addr): u for p, us in U.units().items() for u in us if u.file}
    pool = [(m, n, a, s, mw, pal[(m, a)]) for m, n, a, s, mw, fb in pal_funcs(ROOT) if (m, a) in pal and pal[(m, a)].size == s and 24 <= s <= 1400]
    bylen = sorted(pool, key=lambda x: x[3])
    rep = json.load(open(ROOT + "/build/slps01144/report.json"))
    unmatched = set()
    for un in rep["units"]:
        pg = un["name"].split("/")[0]
        for f in un.get("functions", []):
            if float(f.get("fuzzy_match_percent") or 0) < 100: unmatched.add((pg, f["name"]))
    done = set()
    for f in glob.glob(ROOT + "/src/**/*_slps01144.c", recursive=True):
        m = re.search(r"func_([0-9A-F]{8})_slps01144", f)
        if m: done.add(int(m.group(1), 16))
    imgs, out = {}, []
    for sf in sorted(glob.glob(ROOT + "/asm/slps01144/*/nonmatchings/*/*.s")):
        prog, name = sf.split("/")[-4], os.path.basename(sf)[:-2]
        if (prog, name) not in unmatched: continue
        tx = open(sf).read()
        m, ms = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", tx), re.search(r"nonmatching \w+, 0x([0-9A-F]+)", tx)
        if not m or not ms: continue
        a, s = int(m.group(1), 16), int(ms.group(1), 16)
        if a in done or not 24 <= s <= 1400: continue
        if prog not in imgs: imgs[prog] = yaml_img("slps01144", prog)
        w = imgs[prog].words(a, s)
        if w is None: continue
        jm = [mask(x) for x in w]
        best = []
        lo = [x for x in bylen if 0.8 * s <= x[3] <= 1.25 * s]
        sm = difflib.SequenceMatcher(None, autojunk=False)
        sm.set_seq2(jm)
        for cand in lo:
            sm.set_seq1(list(cand[4]))
            if sm.real_quick_ratio() < minr or sm.quick_ratio() < minr: continue
            r = sm.ratio()
            if r >= minr: best.append((r, cand))
        best.sort(key=lambda x: -x[0])
        if best:
            for _, c in best[:3]: MOD[c[5].file] = c[0]
            out.append((prog, a, s, name, [c[5] for _, c in best[:3]]))
    return out


def build_text(t, name, new, prog, addr, size):
    head = [l for l in t.split("\n")[:12] if re.match(r"//\s*(FLAGS|CC|HEADERS|AS)\b", l)]
    body = "\n".join(l for l in t.split("\n") if not re.match(r"//\s*(FUNC|MATCHING|VERSION|FLAGS|CC|HEADERS|AS)\b", l))
    body = re.sub(r"\b%s\b" % re.escape(name), new, body)
    return "\n".join(["// near-twin of %s (PAL), tools/jp_neartwin.py" % name] + head + ["// VERSION SLPS_01144 %s %08X %d" % (prog, addr, size),
                     "#if defined(VERSION_SLPS_01144)", body.strip("\n"), "#endif", ""])


def main():
    install, lim = "--install" in sys.argv, (int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 10 ** 9)
    c = candidates()[:lim]
    print("candidates (PAL C of a different size):", len(c))
    won = []
    tmp = tempfile.mkdtemp()
    for prog, a, s, jname, us in c:
      for u in us:
        src = open(os.path.join(ROOT, u.file) if not os.path.isabs(u.file) else u.file, errors="replace").read()
        pname = os.path.basename(u.file)[:-2]
        new = "func_%08X_slps01144" % a
        if any(w[1] == a for w in won): break
        for vn, f in VARIANTS:
            txt = build_text(f(src), pname, new, prog, a, s)
            path = os.path.join(tmp, new + ".c")
            open(path, "w").write(txt)
            try: r = jpcheck.check(path, prog)
            except Exception as e: r = "ERR %s" % e
            if r == "IDENTICAL":
                print("MATCH %s %08X (%s) variant %s" % (prog, a, pname, vn)); won.append((prog, a, s, jname, new, txt)); break
    if install:
        for prog, a, s, jname, new, txt in won:
            d = ROOT + "/src" + ("" if prog == "slps01144" else "/x0" + prog[1:].rjust(2, "0")[-2:] if False else "")
            d = ROOT + "/src" + ("" if prog == "slps01144" else "/x%03d" % int(prog[1:]))
            os.makedirs(d, exist_ok=True)
            open("%s/%s.c" % (d, new), "w").write(txt)
            sf = ROOT + "/config/slps01144/symbol_addrs_%s.txt" % prog
            t = open(sf).read()
            if re.search(r"(?m)^\w+ = 0x%08X;" % a, t): t = re.sub(r"(?m)^\w+ = 0x%08X;" % a, "%s = 0x%08X;" % (new, a), t)
            else: t = t.rstrip("\n") + "\n%s = 0x%08X; // type:func size:0x%X\n" % (new, a, s)
            open(sf, "w").write(t)
        print("installed", len(won))
    print("matched:", len(won), "of", len(c))


if __name__ == "__main__":
    main()
