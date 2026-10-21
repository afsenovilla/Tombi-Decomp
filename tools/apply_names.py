#!/usr/bin/env python3
"""Applies the function names a user proposed in the graphics editor ("Sin nombre / Unnamed" node -> "Exportar nombres" ->
names_user.json, a list of {address, prog, old, new, comment}) with the same mechanics as the `names:` commits:

  - src/<old>.c -> src/<new>.c (git mv; SYSTEM overlays: src/<prog>/), the symbol inside it and in its callers;
  - X000 functions: the overlay twins src/x0NN/<old>.c and the rows of notes/functions_x0NN.csv whose `twin` is X000:<addr>;
  - the name column of notes/functions_<prog>.csv at the address, a row in notes/names_main0.csv / names_x000.csv
    (confidence `media`, the user's comment), a line under "# User names" in notes/names.md,
    the `animowner:aNN_<old>` label keys (tools/labels/labels.json);
  - every other mention of the old name in src/ include/ notes/ tools/ port2/ docs/ (MAIN0 / X000 names are unique across those
    trees; a SYSTEM overlay name such as func_800Exxxx may also be an X000 function, so those are replaced only inside src/<prog>/
    (plus src/wip/<prog>_*.c) and in notes/ docs/ lines that mention the overlay, and the remaining mentions are listed).
  - notes/unnamed.json is left alone: regenerate it with tools/unnamed.py after the renames.

Each new name must be a C identifier that is not generic (func_/FUN_/D_) and not already used by another function or symbol of
the same program (src files, function csvs, any identifier of the sources); offending rows are skipped with the reason.
Nothing is committed; review `git status` / `git diff` and run tools/build_full.py afterwards.

Usage: python3 tools/apply_names.py names_user.json [--dry]
  --dry   print what would change without touching any file
"""
import glob, json, os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import sysovl

IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]{0,63}$")
GENERIC = re.compile(r"^(?:FUN|func|D|DAT|PTR_DAT|PTR)_[0-9a-fA-F]{6,8}$")
KEYWORDS = set("auto break case char const continue default do double else enum extern float for goto if int long register return "
               "short signed sizeof static struct switch typedef union unsigned void volatile while asm inline".split())
HDR = re.compile(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s+(\w+)")
TEXT_EXT = (".c", ".h", ".s", ".inc", ".md", ".csv", ".txt", ".py", ".json", ".yaml", ".yml", ".cmake", ".sh", ".ld")
TREES = ("src", "include", "notes", "tools", "port2", "docs")
SYS_PROGS = set(sysovl.PROGS)


def rel(p):
    return os.path.relpath(p, ROOT).replace(os.sep, "/")


def read(p):
    with open(p, encoding="utf-8", errors="surrogateescape") as f:
        return f.read()


def func_header(path):
    try:
        with open(path, errors="replace") as f:
            return HDR.match(f.readline())
    except OSError:
        return None


def text_files(trees=TREES):
    out = []
    for t in trees:
        for dp, dn, fn in os.walk(os.path.join(ROOT, t)):
            dn[:] = [d for d in dn if d not in ("__pycache__", "psxemu_cache", "build")]
            for f in fn:
                if f.endswith(TEXT_EXT):
                    out.append(os.path.join(dp, f))
    return sorted(out)


def csv_rows(path):
    try:
        return read(path).splitlines()
    except OSError:
        return None


class Plan:
    """Collects moves and file edits (in memory) and applies them at the end, or only prints them (--dry)."""

    def __init__(self, dry):
        self.dry = dry
        self.moves = []             # (old path, new path)
        self.texts = {}             # path -> new content
        self.orig = {}
        self.log = []

    def edit(self, path, fn):
        if path not in self.orig:
            try:
                self.orig[path] = read(path)
            except OSError:
                return False
        cur = self.texts.get(path, self.orig[path])
        new = fn(cur)
        if new != cur:
            self.texts[path] = new
            return True
        return False

    def changed(self):
        return sorted(p for p, t in self.texts.items() if t != self.orig[p])

    def apply(self):
        for a, b in self.moves:
            subprocess.run(["git", "mv", a, b], check=True, cwd=ROOT)
            if a in self.texts:               # the edited content follows the moved file
                self.texts[b] = self.texts.pop(a)
                self.orig[b] = self.orig.pop(a)
        for p in self.changed():
            with open(p, "w", encoding="utf-8", errors="surrogateescape") as f:
                f.write(self.texts[p])


def load_rows(path):
    d = json.load(open(path, encoding="utf-8"))
    if isinstance(d, dict):
        d = d.get("names") or d.get("entries") or []
    rows = []
    for r in d:
        if not isinstance(r, dict) or not r.get("new"):
            continue
        rows.append(dict(address="%08x" % int(str(r.get("address", "0")), 16), prog=str(r.get("prog", "")).upper(), old=str(r.get("old", "")),
                         new=str(r["new"]).strip(), comment=str(r.get("comment", "")).strip()))
    return rows


def locate(row):
    """src file of the function: by name and the `// FUNC addr size prog` header."""
    prog, old, addr = row["prog"], row["old"], int(row["address"], 16)
    d = os.path.join(ROOT, "src", prog.lower()) if prog in SYS_PROGS else os.path.join(ROOT, "src")
    cands = [os.path.join(d, old + ".c")] if old else []
    cands += [f for f in glob.glob(os.path.join(d, "*.c")) if not f.endswith("_slpm80165.c")]
    for f in cands:
        m = func_header(f)
        if m and int(m.group(1), 16) == addr and m.group(3) == prog:
            return f
    return None


def scope_text_files(prog):
    """Files that may legitimately hold the program's identifiers (collision check)."""
    if prog in SYS_PROGS:
        return glob.glob(os.path.join(ROOT, "src", prog.lower(), "*.[ch]"))
    fs = glob.glob(os.path.join(ROOT, "src", "*.[ch]")) + glob.glob(os.path.join(ROOT, "src", "x0[0-9][0-9]", "*.[ch]"))
    fs += glob.glob(os.path.join(ROOT, "src", "data", "**", "*.[ch]"), recursive=True) + glob.glob(os.path.join(ROOT, "include", "**", "*.h"), recursive=True)
    return fs


def csv_names(prog):
    """{name: address} of the program's function csv(s)."""
    out = {}
    paths = ["functions_main0.csv", "functions_x000.csv"] if prog not in SYS_PROGS else ["functions_%s.csv" % prog.lower()]
    for p in paths:
        for l in csv_rows(os.path.join(ROOT, "notes", p)) or []:
            f = l.split(",")
            if len(f) >= 3 and re.fullmatch(r"[0-9a-f]{8}", f[0]):
                nm = f[1] if p == "functions_x000.csv" or p == "functions_main0.csv" else f[2]
                if nm:
                    out[nm] = int(f[0], 16)
    return out


def validate(rows):
    ok, bad = [], []
    seen_new = {}
    for r in rows:
        why = None
        new, prog = r["new"], r["prog"]
        if prog not in ("MAIN0", "X000") and prog not in SYS_PROGS:
            why = "unknown prog %r" % prog
        elif not IDENT.match(new):
            why = "not a C identifier"
        elif new in KEYWORDS:
            why = "C keyword"
        elif GENERIC.match(new):
            why = "generic name"
        elif new == r["old"]:
            why = "unchanged"
        elif (prog in SYS_PROGS and (new, prog) in seen_new) or (prog not in SYS_PROGS and (new, "MAIN0") in seen_new):
            why = "duplicate in the input"
        else:
            r["path"] = locate(r)
            if not r["path"]:
                why = "no src file with // FUNC %s ... %s" % (r["address"], prog)
            elif not r["old"]:
                r["old"] = os.path.basename(r["path"])[:-2]
            elif os.path.basename(r["path"])[:-2] != r["old"]:
                why = "the file of %s is %s, not %s" % (r["address"], rel(r["path"]), r["old"])
        if not why:
            d = os.path.dirname(r["path"])
            other = csv_names(prog).get(new)
            if os.path.exists(os.path.join(d, new + ".c")):
                why = "src/%s.c exists" % new
            elif other is not None and other != int(r["address"], 16):
                why = "name used by %08x in the function csv" % other
            else:
                rx = re.compile(r"\b%s\b" % re.escape(new))
                for f in scope_text_files(prog):
                    try:
                        if rx.search(read(f)):
                            why = "identifier already used in %s" % rel(f)
                            break
                    except OSError:
                        pass
        if why:
            bad.append((r, why))
        else:
            seen_new[(new, prog if prog in SYS_PROGS else "MAIN0")] = r
            ok.append(r)
    return ok, bad


def twin_overlays(addr):
    """[nn] of the area overlays whose functions_x0nn.csv row at addr is a twin of X000."""
    out = []
    for p in sorted(glob.glob(os.path.join(ROOT, "notes", "functions_x0[0-9][0-9].csv"))):
        nn = int(os.path.basename(p)[11:14])
        for l in csv_rows(p) or []:
            f = l.split(",")
            if len(f) >= 4 and f[0] == "%08x" % addr and f[3].strip() == "X000:%08x" % addr:
                out.append(nn)
    return out


def apply_row(r, plan, areas_of):
    addr, prog, old, new = int(r["address"], 16), r["prog"], r["old"], r["new"]
    rx = re.compile(r"\b%s\b" % re.escape(old))
    sub = lambda t: rx.sub(new, t)
    sysp = prog in SYS_PROGS
    touched = set()
    # 1. the file and its twins
    plan.moves.append((r["path"], os.path.join(os.path.dirname(r["path"]), new + ".c")))
    twins = twin_overlays(addr) if prog == "X000" else []
    for nn in twins:
        p = os.path.join(ROOT, "src", "x%03d" % nn, old + ".c")
        if os.path.exists(p):
            plan.moves.append((p, os.path.join(os.path.dirname(p), new + ".c")))
    # 2. symbol replacement
    if sysp:
        files = glob.glob(os.path.join(ROOT, "src", prog.lower(), "*.[ch]")) + glob.glob(os.path.join(ROOT, "src", "wip", prog.lower() + "_*.[ch]"))
    else:
        files = [f for f in text_files() if not any(rel(f).startswith("src/%s/" % s.lower()) for s in SYS_PROGS)
                 and not re.search(r"^notes/(functions|names)_[a-z0-9]+\.csv$", rel(f))
                 and not rel(f).startswith("src/x0") and rel(f) not in ("tools/labels/labels.json", "notes/unnamed.json")]
        for nn in twins:
            files += glob.glob(os.path.join(ROOT, "src", "x%03d" % nn, "*.[ch]")) + glob.glob(os.path.join(ROOT, "src", "data", "x%03d" % nn, "*.[ch]"))
    for f in files:
        if plan.edit(f, sub):
            touched.add(f)
    # 3. csv rows (by address)
    def csv_set(path, col, want_twin=None):
        def fn(t):
            lines = t.splitlines(True)
            for i, l in enumerate(lines):
                fl = l.rstrip("\n").split(",")
                if len(fl) > col and fl[0] == "%08x" % addr and (want_twin is None or (len(fl) >= 4 and fl[3].strip() == want_twin)):
                    fl[col] = new
                    lines[i] = ",".join(fl) + ("\n" if l.endswith("\n") else "")
            return "".join(lines)
        if plan.edit(path, fn):
            touched.add(path)
    if sysp:
        csv_set(os.path.join(ROOT, "notes", "functions_%s.csv" % prog.lower()), 2)
    else:
        csv_set(os.path.join(ROOT, "notes", "functions_%s.csv" % prog.lower()), 1)
        for nn in twins:
            csv_set(os.path.join(ROOT, "notes", "functions_x%03d.csv" % nn), 2, "X000:%08x" % addr)
        # names csv: upsert, sorted by address (as tools/apply_func_names.py)
        def upsert(t):
            lines = t.splitlines()
            hdr, body = lines[0], lines[1:]
            d, order = {}, []
            for l in body:
                a = l.split(",")[0]
                if a not in d:
                    order.append(a)
                d[a] = l
            a = "%08x" % addr
            if a not in d:
                order.append(a)
            d[a] = "%s,%s,media,%s" % (a, new, (r["comment"] or "user name (names_user.json)").replace(",", ";").replace("\n", " "))
            return "\n".join([hdr] + [d[x] for x in sorted(order)]) + "\n"
        p = os.path.join(ROOT, "notes", "names_%s.csv" % prog.lower())
        if plan.edit(p, upsert):
            touched.add(p)
        # labels: animowner:aNN_<old> keys
        def lab(t):
            for ar in sorted({0} | {a for nn in twins for a in areas_of(nn)}):
                t = t.replace('"animowner:a%02d_%s"' % (ar, old.lower()), '"animowner:a%02d_%s"' % (ar, new.lower()))
            return t
        p = os.path.join(ROOT, "tools", "labels", "labels.json")
        if plan.edit(p, lab):
            touched.add(p)
    # 4. notes/docs lines that mention a SYSTEM overlay name together with the overlay
    if sysp:
        pr = re.compile(re.escape(prog), re.I)
        def lines_with_prog(t):
            out = []
            for l in t.splitlines(True):
                out.append(sub(l) if rx.search(l) and pr.search(l) else l)
            return "".join(out)
        for f in text_files(("notes", "docs")):
            if re.search(r"^notes/(functions|names)_[a-z0-9]+\.csv$", rel(f)) or rel(f) == "notes/unnamed.json":
                continue
            if plan.edit(f, lines_with_prog):
                touched.add(f)
    # 5. names.md line
    def md(t):
        head = "# User names (names_user.json, tools/apply_names.py)"
        line = "- `%08x %s` (was %s; %s) - %s" % (addr, new, old, prog, r["comment"] or "no comment")
        if head not in t:
            t = t.rstrip("\n") + "\n\n" + head + "\n\n" + line + "\n"
        else:
            t = t.rstrip("\n") + "\n" + line + "\n"
        return t
    p = os.path.join(ROOT, "notes", "names.md")
    if plan.edit(p, md):
        touched.add(p)
    return touched, twins


def areas_of_overlay(nn):
    import objects as O
    return [a for a in O.SECTIONS if O.overlay_of(a) == nn]


def remaining(old, prog, plan):
    """Files that still mention the old name after the planned edits (SYSTEM overlay names: ambiguous with the X000 function of the
    same address, so mentions outside src/<prog>/ are reported, not changed)."""
    rx = re.compile(r"\b%s\b" % re.escape(old))
    out = []
    for f in text_files():
        if rel(f) == "notes/unnamed.json":
            continue
        try:
            if rx.search(plan.texts.get(f, plan.orig.get(f)) or read(f)):
                out.append(rel(f))
        except OSError:
            pass
    return out


def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    dry = "--dry" in argv
    args = [a for a in argv if not a.startswith("--")]
    if len(args) != 1:
        print(__doc__)
        return 2
    rows = load_rows(args[0])
    ok, bad = validate(rows)
    for r, why in bad:
        print("SKIP %s %s %s -> %s: %s" % (r["prog"], r["address"], r["old"] or "?", r["new"], why))
    plan = Plan(dry)
    n_files = set()
    for r in ok:
        touched, twins = apply_row(r, plan, areas_of_overlay)
        n_files |= touched
        print("%s %s %s %s -> %s  (%d file(s) edited%s)" % ("DRY " if dry else "OK  ", r["prog"], r["address"], r["old"], r["new"], len(touched),
                                                           ", twins x%s" % ",".join("%03d" % n for n in twins) if twins else ""))
        if r["prog"] in SYS_PROGS:
            left = remaining(r["old"], r["prog"], plan)
            if left:
                print("     still mentioned (maybe the X000 function of the same address, not changed): " + ", ".join(left[:12]) + (" ..." if len(left) > 12 else ""))
    if dry:
        for a, b in plan.moves:
            print("  git mv %s %s" % (rel(a), rel(b)))
        for p in plan.changed():
            print("  edit   %s" % rel(p))
    else:
        plan.apply()
    print("summary: %d name(s) applied, %d skipped, %d file(s) moved, %d file(s) edited%s" % (len(ok), len(bad), len(plan.moves), len(plan.changed()),
                                                                                                " (dry run, nothing written)" if dry else ""))
    return 0 if ok or not rows else 1


if __name__ == "__main__":
    sys.exit(main())
