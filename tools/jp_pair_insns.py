#!/usr/bin/env python3
"""Pair unmatched JP (SLPS-01144) functions with PAL C files by instruction shape.

The callee pairer (tools/jp_pair_callees.py) misses functions whose JP form inlines a call, drops one or reads raw
globals for struct fields. This one compares opcode sequences instead: every function of build/slps01144/report.json
below 100 % (>= MINSIZE bytes) is disassembled from its target object, every PAL function compiled under build/base/
likewise, and the PAL function with the best difflib ratio of mnemonics (size within 0.6-1.6x, ratio >= MINRATIO)
is the candidate. Prints `prog:addr:size:palfile:0   # ratio palname` lines for the scratch sweep / tools/jpcheck.py.

  python3 tools/jp_pair_insns.py [MINSIZE=100] [MINRATIO=0.6] [--inline]

--inline also tries the two shapes the plain ratio misses: a JP function that inlines one of its PAL twin's callees
(the PAL sequence with that callee's body spliced in at its `jal`), and a JP function whose PAL twin inlines one of
the JP callees (the JP sequence with that callee's body spliced in). A pair found this way prints `# 0.93 +callee`
(PAL side inlined) or `# 0.93 jp+callee` (JP side inlined); it is reported only when it beats the plain best by 0.05.
"""
import difflib, glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def mnems(o):
    r = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", o], capture_output=True, text=True).stdout
    return [l.split("\t")[1].split()[0] for l in r.split("\n") if re.match(r"^\s+[0-9a-f]+:\t", l) and len(l.split("\t")) > 1]


def mnems_jal(o):
    """(mnemonics, {index: jal target name}) of the first function of an object."""
    r = subprocess.run(["mipsel-linux-gnu-objdump", "-dr", "--no-show-raw-insn", o], capture_output=True, text=True).stdout
    m, jal = [], {}
    for l in r.split("\n"):
        if re.match(r"^\s+[0-9a-f]+:\t", l) and len(l.split("\t")) > 1:
            m.append(l.split("\t")[1].split()[0])
        else:
            x = re.match(r"^\s+[0-9a-f]+: R_MIPS_26\s+(\S+)", l)
            if x and m and m[-1] == "jal" and x.group(1) != ".text": jal[len(m) - 1] = x.group(1)
    return m, jal


def body(m):
    """A callee's mnemonics without its frame set-up and return (what is left once it is inlined)."""
    k = [x for x in m if x not in ("jr",)]
    return k[2:-2] if len(k) > 8 else k


def spliced(m, jal, bodies):
    """One variant per callee with a known body: m with that call replaced by the callee body."""
    out = []
    for i, n in jal.items():
        if n in bodies: out.append((n, m[:i] + bodies[n] + m[i + 2:]))
    return out


def best_of(jm, cands, minratio, best):
    for tag, src, pm in cands:
        if not (0.6 <= len(pm) / len(jm) <= 1.6): continue
        sm = difflib.SequenceMatcher(None, jm, pm, autojunk=False)
        if sm.real_quick_ratio() < max(minratio, best[0]) or sm.quick_ratio() < max(minratio, best[0]): continue
        r = sm.ratio()
        if r > best[0]: best = (r, (tag, src))
    return best


def main():
    inline = "--inline" in sys.argv
    sys.argv = [a for a in sys.argv if a != "--inline"]
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
        m, jal = mnems_jal(o)
        if len(m) >= minsize // 8: pal.append((n, srcof[n], m, jal))
    palbody = {}
    if inline:
        for o in glob.glob(ROOT + "/build/base/*/*.o"):
            palbody.setdefault(os.path.splitext(os.path.basename(o))[0], body(mnems_jal(o)[0]))
        palv = [(n + " +" + c, src, v) for n, src, m, jal in pal for c, v in spliced(m, jal, palbody)]
        jpbody = {}
        for o in glob.glob(ROOT + "/build/slps01144/target/*/*.o"):
            jpbody.setdefault(os.path.splitext(os.path.basename(o))[0], body(mnems_jal(o)[0]))
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
            jm, jjal = mnems_jal(o)
            best = best_of(jm, [(n, src, pm) for n, src, pm, _ in pal], minratio, (0, None))
            if inline:
                plain = best[0]
                b2 = best_of(jm, palv, max(minratio, plain + 0.05), (0, None))
                for c, jv in spliced(jm, jjal, jpbody):
                    b3 = best_of(jv, [(n + " jp+" + c, src, pm) for n, src, pm, _ in pal], max(minratio, plain + 0.05), (0, None))
                    if b3[0] > b2[0]: b2 = b3
                if b2[1] is None: continue
                best = b2
            if best[1] and best[0] >= minratio:
                print("%s:%s:%d:%s:0   # %.2f %s" % (prog, a, sz, best[1][1], best[0], best[1][0]), flush=True)


if __name__ == "__main__":
    main()
