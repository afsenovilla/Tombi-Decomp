#!/usr/bin/env python3
"""Near-miss matcher for the Japanese retail build: PAL C compiled for JP, numeric literals shifted until it matches.

For each unmatched JP function paired with a PAL function that has C (the closest PAL C function by masked words,
or the symbol-table `PAL` pair), the PAL C is compiled with VERSION_SLPS_01144 (tools/jpcheck.py) and diffed against
the splat target. When the only differences are immediates of otherwise identical instructions (struct offsets,
constants, pad bits), each numeric literal of the C equal to an old immediate is replaced by the new one, greedily
(an edit is kept when the diff shrinks), until the object is identical. A match is installed as the
`#if defined(VERSION_SLPS_01144)` branch of the shared PAL file (replacing its INCLUDE_ASM_V branch) or, when the
file already builds another JP function, as src/[x0nn/]<name>_slps01144.c. The residual diff of the others is
printed so they can be finished by hand.

  python3 tools/jp_autofix.py [--install] [--min 0.85] [--limit N] [--only ADDR[,ADDR]] [--show] [--emit DIR]
  python3 tools/jp_autofix.py --install-file FILE.c...    (hand-edited `--emit` files: checked, installed, deleted)
  python3 tools/jp_autofix.py --pair prog:addr:size:palfile ... [--show] [--emit DIR] [--install]   (explicit pairs)
Needs: build/slps01144/report.json and targets (tools/build_report.py --version slps01144), the PAL game files and
config/symbol_addrs_*.txt (tools/gen_symbols.py)."""
import difflib, glob, json, os, re, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT + "/tools")
os.chdir(ROOT)
import jpcheck
import version_share as VS
import units as U
from xmatch import pal_funcs, mask
from jp_globals import yaml_img

VER, SERIAL = "slps01144", "SLPS_01144"


def jp_names():
    """(prog, addr) -> (JP symbol name, size, PAL module, PAL addr or None)."""
    out = {}
    for sf in glob.glob("config/%s/symbol_addrs_*.txt" % VER):
        prog = os.path.basename(sf)[len("symbol_addrs_"):-4]
        for l in open(sf):
            m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)(?:\s+PAL (\w+) 0x([0-9A-Fa-f]+))?", l)
            if m:
                out[(prog, int(m.group(2), 16))] = (m.group(1), int(m.group(3), 16), m.group(4) and m.group(4).lower(), m.group(5) and int(m.group(5), 16))
    return out


def unmatched():
    rep = json.load(open("build/%s/report.json" % VER))
    out = set()
    for un in rep["units"]:
        pg = un["name"].split("/")[0]
        for f in un.get("functions", []):
            if float(f.get("fuzzy_match_percent") or 0) < 100: out.add((pg, f["name"]))
    return out


def candidates(minr):
    """[(ratio, prog, addr, size, jname, PAL unit)] for the unmatched JP functions, best PAL C function by masked words."""
    pal = {(p.lower(), u.addr): u for p, us in U.units().items() for u in us if u.file}
    pool = [(m, n, a, s, mw, pal[(m, a)]) for m, n, a, s, mw, fb in pal_funcs(ROOT) if (m, a) in pal and pal[(m, a)].size == s and 24 <= s <= 4000]
    bylen = sorted(pool, key=lambda x: x[3])
    um, names, imgs, rows = unmatched(), jp_names(), {}, []
    for sf in sorted(glob.glob("asm/%s/*/nonmatchings/*/*.s" % VER)):
        prog, name = sf.split("/")[-4], os.path.basename(sf)[:-2]
        if (prog, name) not in um: continue
        tx = open(sf).read()
        m, ms = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", tx), re.search(r"nonmatching \w+, 0x([0-9A-F]+)", tx)
        if not m or not ms: continue
        a, s = int(m.group(1), 16), int(ms.group(1), 16)
        if not 24 <= s <= 4000: continue
        if prog not in imgs: imgs[prog] = yaml_img(VER, prog)
        w = imgs[prog].words(a, s)
        if w is None: continue
        jm = [mask(x) for x in w]
        sm = difflib.SequenceMatcher(None, autojunk=False)
        sm.set_seq2(jm)
        best = (0, None)
        for cand in [x for x in bylen if 0.7 * s <= x[3] <= 1.4 * s]:
            sm.set_seq1(list(cand[4]))
            if sm.real_quick_ratio() < minr or sm.quick_ratio() < minr: continue
            r = sm.ratio()
            if r > best[0]: best = (r, cand)
        # the symbol-table pair wins when it has C (the masked ratio is only a tie-break)
        jn = names.get((prog, a))
        if jn and jn[2] and (jn[2], jn[3]) in pal and best[0] < 0.999:
            best = (max(best[0], minr), pool_entry(pool, jn[2], jn[3]) or best[1])
        if best[1] is None: continue
        rows.append((best[0], prog, a, s, name, best[1][5]))
    rows.sort(key=lambda r: -r[0])
    return rows


def pool_entry(pool, mod, addr):
    for x in pool:
        if x[0] == mod and x[2] == addr: return x
    return None


def pal_text(path):
    """(header lines, PAL body) of a shared file (the `#else` branch when it has version branches)."""
    hdr, rest = VS.split_header(open(path, errors="replace").read())
    branches, pal = VS.parse_branches(rest)
    return hdr, (pal if pal is not None else rest)


def jp_source(hdr, body, palname, jname, prog, addr, size):
    head = [l for l in hdr if re.match(r"//\s*(FLAGS|CC|HEADERS|AS|ASPSX)\b", l)]
    body = re.sub(r"\b%s\b" % re.escape(palname), jname, body)
    return "\n".join(["// VERSION %s %s %08X %d" % (SERIAL, prog, addr, size)] + head +
                     ["#if defined(VERSION_%s)" % SERIAL, body.strip("\n"), "#endif", ""])


def diff_lines(r):
    return [l for l in r.split("\n") if l[:1] in "-+" and not l.startswith(("---", "+++"))]


NUM = r"-?(?:0x[0-9a-fA-F]+|\d+)"


def imm_pairs(r):
    """[(old, new)] immediates of same-mnemonic lines that differ only in one number; None when other lines differ."""
    out = []
    minus, plus = [], []
    for l in r.split("\n"):
        if l.startswith(("---", "+++", "@@")): continue
        if l.startswith("-"): minus.append(l[1:])
        elif l.startswith("+"): plus.append(l[1:])
    if len(minus) != len(plus): return None
    for a, b in zip(minus, plus):
        if a.startswith("   R_MIPS") or b.startswith("   R_MIPS"): return None
        ta, tb = re.findall(NUM + r"|[^\s,()]+", a), re.findall(NUM + r"|[^\s,()]+", b)
        if len(ta) != len(tb): return None
        d = [(x, y) for x, y in zip(ta, tb) if x != y]
        if len(d) != 1 or not re.fullmatch(NUM, d[0][0]) or not re.fullmatch(NUM, d[0][1]): return None
        out.append((int(d[0][0], 0), int(d[0][1], 0)))
    return out


def literal_spots(text, val):
    """Spans of the numeric literals of the C body equal to val (hex or decimal, outside comments and strings)."""
    spots = []
    for m in re.finditer(r"0x[0-9a-fA-F]+|\b\d+\b", text):
        line_start = text.rfind("\n", 0, m.start()) + 1
        if "//" in text[line_start:m.start()]: continue
        try: v = int(m.group(0), 0)
        except ValueError: continue
        if v == val or (val < 0 and v == val & 0xffff) or (val < 0 and v == -val): spots.append((m.start(), m.end(), v))
    return spots


def fmt_like(old_tok, new_val):
    if old_tok.lower().startswith("0x"): return "0x%x" % (new_val & 0xffffffff if new_val < 0 else new_val)
    return str(new_val)


def check(path, prog, strict=False):
    """jpcheck --norm; strict keeps the branch targets (the greedy edits count diff lines, so they use the loose form,
    and a match is confirmed in strict form before it is installed)."""
    try:
        jpcheck.STRICT = strict
        return jpcheck.check(path, prog, norm=True)
    except Exception as e: return "ERROR %s" % e
    finally: jpcheck.STRICT = False


def all_literals(text):
    spots = []
    for m in re.finditer(r"0x[0-9a-fA-F]+|\b\d+\b", text):
        line_start = text.rfind("\n", 0, m.start()) + 1
        if "//" in text[line_start:m.start()] or text[line_start:].startswith("#"): continue
        try: v = int(m.group(0), 0)
        except ValueError: continue
        spots.append((m.start(), m.end(), v))
    return spots


def try_fix(path, prog, text, maxrounds=60):
    """Greedy literal shifting on the JP source text; returns (final text, result string)."""
    open(path, "w").write(text)
    r = check(path, prog)
    if r == "IDENTICAL": return text, r
    best = len(diff_lines(r))
    for _ in range(maxrounds):
        pairs = imm_pairs(r)
        if not pairs: break
        improved = False
        for old, new in dict.fromkeys(pairs):
            delta = new - old
            # literals equal to the old immediate first; then any literal shifted by the delta (struct pads, offsets
            # written as differences), kept only when the diff shrinks
            spots = literal_spots(text, old) + [s for s in all_literals(text) if s[2] != old and 0 < s[2] < 0x10000]
            for s, e, v in spots:
                tok = text[s:e]
                nv = v + delta if v == old else v + delta
                if nv < 0 and not tok.startswith("-"): continue
                cand = text[:s] + fmt_like(tok, nv) + text[e:]
                open(path, "w").write(cand)
                r2 = check(path, prog)
                n = len(diff_lines(r2)) if r2 != "IDENTICAL" else 0
                if n < best and not r2.startswith("ERROR"):
                    text, r, best, improved = cand, r2, n, True
                    if r == "IDENTICAL": return text, r
                    break
            if improved: break
        if not improved: break
    # generic edits, each kept only when the diff shrinks: pad bits (0x10..0xf0 -> 0x1000..0xf000, JP pad masks),
    # one statement deleted (stores, calls and flags the JP build lacks), two adjacent statements swapped
    if r != "IDENTICAL":
        t0, r0, b0 = text, r, best
        text, r, best = generic_fix(path, prog, text, r, best)
        if r != "IDENTICAL":  # the other order of edits suits other functions
            t2, r2, b2 = generic_fix(path, prog, t0, r0, b0, delete_first=True)
            if b2 < best: text, r, best = t2, r2, b2
    open(path, "w").write(text)
    return text, r


STMT = re.compile(r"^(\s+)(?!(?:if|else|for|while|switch|case|default|do|return|break|continue|goto|\{|\}|#)\b)[^\n{}]*;\s*(//.*)?$")


def statements(text):
    """Line indexes of the simple statement lines of the body (one per line: stores, calls, assignments)."""
    L = text.split("\n")
    return [i for i, l in enumerate(L) if STMT.match(l) and not l.lstrip().startswith(("extern", "typedef", "static", "int ", "short ", "char ", "unsigned ", "void ", "TObj ", "//"))]


def generic_fix(path, prog, text, r, best, budget=200, delete_first=False):
    """Edits kept only when the diff shrinks, in order: pad bits, immediates, a statement moved up to three statements
    away, one statement deleted (stores, calls and flags the JP build lacks)."""
    def run(cand):
        open(path, "w").write(cand)
        r2 = check(path, prog)
        return r2, (0 if r2 == "IDENTICAL" else (10 ** 6 if r2.startswith("ERROR") else len(diff_lines(r2))))
    n = 0
    while n < budget:
        improved = False
        cands = []
        for m in re.finditer(r"(&\s*)0x([1-9a-fA-F][0-9a-fA-F]?)\b(?!00)", text):
            cands.append(text[:m.start(2)] + "%s00" % m.group(2) + text[m.end(2):])
        for old, new in dict.fromkeys(imm_pairs(r) or []):
            delta = new - old
            for s, e, v in literal_spots(text, old) + [s for s in all_literals(text) if s[2] != old and 0 < s[2] < 0x10000]:
                nv = v + delta
                if nv < 0 and not text[s:e].startswith("-"): continue
                cands.append(text[:s] + fmt_like(text[s:e], nv) + text[e:])
        L = text.split("\n")
        st = statements(text)
        for j, i in enumerate(st):
            for d in (1, -1, 2, -2, 3, -3):
                if not 0 <= j + d < len(st): continue
                M = L[:]; line = M.pop(i)
                M.insert(st[j + d] if d < 0 else st[j + d], line)
                cands.append("\n".join(M))
        # statements the JP build is known to lack first (vibration, shock flags, player ext bytes 0xa0.., load lists)
        hot = re.compile(r"Vibration|\bbb0\b|b1a4|1f8001a4|0xa0\b|0xbe\b|0xbf\b|0xcd\b|0xce\b|0xd1\b|0xd5\b|0xd6\b|0xd7\b|GAME_PLAYER_MODE|queueSystemLoadList|FUN_8001f620|DAT_8009c933|\bba0\b|\bbbe\b|\bbcd\b|\bbce\b")
        st2 = sorted(st, key=lambda i: 0 if hot.search(L[i]) else 1)
        dels = ["\n".join(L[:i] + L[i + 1:]) for i in st2]
        cands = (dels + cands) if delete_first else (cands + dels)
        for cand in cands:
            if cand == text: continue
            r2, k = run(cand); n += 1
            if k < best:
                text, r, best, improved = cand, r2, k, True
                break
            if n >= budget: break
        if not improved or r == "IDENTICAL": break
    return text, r, best


def install(prog, addr, size, jname, palfile, text, sources):
    """The JP C into the shared file's SLPS_01144 branch (replacing INCLUDE_ASM) or a JP-only file."""
    body = text.split("#if defined(VERSION_%s)\n" % SERIAL, 1)[1].rsplit("#endif", 1)[0]
    hdr, rest = VS.split_header(open(palfile, errors="replace").read())
    branches, pal = VS.parse_branches(rest)
    mine = [t for s, t in branches if s == SERIAL]
    others = [k for k, (p, _) in sources.items() if os.path.abspath(p) == os.path.abspath(palfile) and k != (prog, addr)]
    if pal is not None and not others and (not mine or "INCLUDE_ASM" in mine[0]):
        branches = [(s, body.strip("\n")) if s == SERIAL else (s, t) for s, t in branches] if mine else branches + [(SERIAL, body.strip("\n"))]
        vline = "// VERSION %s %s %08X %d" % (SERIAL, prog, addr, size)
        if vline not in hdr:
            hdr.insert(max([i + 1 for i, l in enumerate(hdr) if re.match(r"//\s*(FUNC|MATCHING|VERSION)\s", l)] or [len(hdr)]), vline)
        open(palfile, "w").write("\n".join(hdr) + "\n" + VS.join_branches(branches, pal))
        return palfile
    from versions import pal_src_dir
    d = pal_src_dir(VER, prog)
    name = jname if jname.endswith("_" + VER) else jname + "_" + VER
    out = "%s/%s.c" % (d, name)
    open(out, "w").write("// near-twin of PAL %s (tools/jp_autofix.py)\n" % os.path.basename(palfile)[:-2] + text)
    return out


def install_files(files, sources, force=False):
    """`--install-file F...`: standalone JP sources (`// VERSION SLPS_01144 <prog> <addr> <size>` header, edited by hand
    from an `--emit` file) are checked and installed like a match."""
    names, n = jp_names(), 0
    for f in files:
        text = open(f).read()
        m = re.search(r"//\s*VERSION\s+%s\s+(\w+)\s+([0-9A-Fa-f]{8})\s+(\d+)" % SERIAL, text)
        pm = re.search(r"// near-twin of PAL (\S+)", text)
        if not m or not pm: print("skip (no VERSION / near-twin header):", f); continue
        prog, addr, size = m.group(1), int(m.group(2), 16), int(m.group(3))
        jname = names.get((prog, addr), ("func_%08X_%s" % (addr, VER),))[0]
        u = next((u for p, us in U.units().items() for u in us if u.file and os.path.basename(u.file)[:-2] == pm.group(1)), None)
        if u is None: print("skip (PAL file not found):", f); continue
        path = os.path.join(tempfile.mkdtemp(), jname + ".c")
        body = "\n".join(l for l in text.split("\n") if not l.startswith("// near-twin"))
        open(path, "w").write(body)
        r = check(path, prog, strict=True)
        print("%s %s %08X %s" % ("MATCH" if r == "IDENTICAL" else "DIFF %d" % len(diff_lines(r)), prog, addr, jname))
        if r == "IDENTICAL" or force:
            print("   installed", install(prog, addr, size, jname, u.file, body, sources)); os.unlink(f); n += 1
        else: print("\n".join("   " + l for l in r.split("\n")[:40]))
    print("installed %d" % n)


def main():
    argv = sys.argv[1:]
    if "--install-file" in argv:
        import build_report as BR
        progs = sorted({os.path.basename(f)[len("symbol_addrs_"):-4] for f in glob.glob("config/%s/symbol_addrs_*.txt" % VER)} - {"manual"})
        return install_files([a for a in argv[argv.index("--install-file") + 1:] if not a.startswith("--")], BR.jpdemo_sources(progs, VER), "--force" in argv)
    emit = argv[argv.index("--emit") + 1] if "--emit" in argv else None
    minr = float(argv[argv.index("--min") + 1]) if "--min" in argv else 0.85
    lim = int(argv[argv.index("--limit") + 1]) if "--limit" in argv else 10 ** 9
    only = set(int(x, 16) for x in argv[argv.index("--only") + 1].split(",")) if "--only" in argv else None
    do_install, show = "--install" in argv, "--show" in argv
    import build_report as BR
    progs = sorted({os.path.basename(f)[len("symbol_addrs_"):-4] for f in glob.glob("config/%s/symbol_addrs_*.txt" % VER)} - {"manual"})
    sources = BR.jpdemo_sources(progs, VER)
    names = jp_names()
    tmp = tempfile.mkdtemp()
    done = []
    if "--pair" in argv:  # --pair prog:addr:size:palfile [...]: explicit pairs, no candidate search
        class _U: pass
        cands = []
        for a in argv[argv.index("--pair") + 1:]:
            if a.startswith("--"): break
            prog, addr, size, pf = a.split(":")
            u = _U(); u.file = os.path.abspath(pf)
            cands.append((1.0, prog, int(addr, 16), int(size), "func_%08X" % int(addr, 16), u))
    else:
        cands = candidates(minr)[:lim]
    for ratio, prog, addr, size, name, u in cands:
        if only and addr not in only: continue
        jname = names.get((prog, addr), (name,))[0]
        hdr, body = pal_text(u.file)
        if "INCLUDE_ASM" in body: continue
        src = sources.get((prog, addr))
        if src and "INCLUDE_ASM" not in open(src[0], errors="replace").read().split("#else")[0]: continue  # JP C exists (stale report)
        palname = os.path.basename(u.file)[:-2]
        text = jp_source(hdr, body, palname, jname, prog, addr, size)
        # the checker finds the target by the file name (JP symbol, with or without the version suffix)
        path = os.path.join(tmp, jname + ".c")
        text, r = try_fix(path, prog, text)
        if r == "IDENTICAL":
            open(path, "w").write(text)
            r = check(path, prog, strict=True)
            if r != "IDENTICAL": r = "BRANCH " + r  # same instructions, a branch goes elsewhere
        tag = "MATCH" if r == "IDENTICAL" else ("ERROR" if r.startswith("ERROR") else ("BRANCH %d" % len(diff_lines(r)) if r.startswith("BRANCH") else "DIFF %d" % len(diff_lines(r))))
        print("%s %.3f %s %08X %5d %-40s <- %s" % (tag, ratio, prog, addr, size, jname, os.path.relpath(u.file)))
        if r == "IDENTICAL":
            done.append((prog, addr, size, jname, u.file, text))
            if do_install: print("   installed", install(prog, addr, size, jname, u.file, text, sources))
        elif show:
            print("\n".join("   " + l for l in r.split("\n")[:60]))
        if emit:  # the best attempt (or the match, when --install was not given) for hand edits / --install-file
            os.makedirs(emit, exist_ok=True)
            open(os.path.join(emit, jname + ".c"), "w").write("// near-twin of PAL %s (tools/jp_autofix.py --emit; edit, then --install-file)\n" % palname + text)
    print("matched %d" % len(done))


if __name__ == "__main__":
    main()
