#!/usr/bin/env python3
"""Shares the src/ tree between the game versions (tools/versions.py), like psx_tomba's version_share.py.

The C keeps the PAL (SCES-01331, reference) names and layout. A function of another version is built by:
  - the PAL file unchanged, when the PAL C matches it (pairing: config/<ver>/symbol_addrs_<prog>.txt `PAL` comments
    and config/<ver>/reuse.txt, see tools/build_report.py jpdemo_sources);
  - otherwise the PAL file of its PAL counterpart, with a version branch and a `// VERSION` line:

        // FUNC ... / // MATCHING ...                 (PAL header, unchanged)
        // VERSION SLPS_01144 x09 8011F5AC 176         (version prog, address, size this file builds)
        // FLAGS.SLPS_01144 -O2 -G0 -fno-...           (optional: compiler options of the version branch)
        #if defined(VERSION_SLPS_01144)
        <version C>   or   #include "include_asm.h" / INCLUDE_ASM_V("x09/nonmatchings/x09", name);
        #elif defined(VERSION_SLPM_80165)
        ...
        #else
        <PAL C>
        #endif

  - or, for a function with no PAL file (version-only code, or a PAL file another function of the version still
    matches unchanged), a guarded file next to the PAL ones: src/[x0nn/]<name>_<serial>.c, whose whole body is inside
    `#if defined(VERSION_<SERIAL>)` (no FUNC/MATCHING marker, so the PAL build and progress ignore it).
Version-only names (func_/D_<version address> with no PAL meaning) carry the serial: func_8011F5AC_slps01144.

Commands:
  python3 tools/version_share.py fold <version> <file.c>...   folds standalone version C files into the shared tree
        (header `// VERSION <SERIAL> <prog> <addr> <size>`; the old `// JP` / `// JPDEMO` headers are accepted) and
        deletes them. Write new version C that way (e.g. in src/wip/), then fold it.
  python3 tools/version_share.py asm <version>                 adds INCLUDE_ASM branches to the PAL files whose
        version counterpart the PAL C does not match (needs build/<version>/report.json from tools/build_report.py
        --version <version>), so every paired function says explicitly what builds it in that version.
  add --dry-run to print what would change.
"""
import os, re, sys, json

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from versions import VERSIONS, canon, pal_src_dir

DRY = "--dry-run" in sys.argv
DIRECTIVES = ("FLAGS", "CC", "HEADERS", "AS")
_HDR = re.compile(r"^//\s*(?:VERSION\s+(\w+)|(JPDEMO|JP))\s+(\w+)\s+([0-9A-Fa-f]{8})\s+(\d+)\s*$")


def split_header(text):
    """(header lines, body text): the leading `//` comment lines of a file, and the rest."""
    lines = text.split("\n")
    i = 0
    while i < len(lines) and lines[i].startswith("//"): i += 1
    return lines[:i], "\n".join(lines[i:])


def parse_branches(body):
    """[(serial, text)], pal_text for a body written by this tool; ([], body) for a plain one."""
    lines = body.strip("\n").split("\n")
    if not lines or not re.match(r"#if defined\(VERSION_\w+\)$", lines[0]) or lines[-1] != "#endif":
        return [], body
    out, cur, buf, depth = [], re.match(r"#if defined\(VERSION_(\w+)\)", lines[0]).group(1), [], 0
    for l in lines[1:-1]:
        s = l.strip()
        if depth == 0 and (re.match(r"#elif defined\(VERSION_\w+\)$", s) or s == "#else"):
            out.append((cur, "\n".join(buf))); buf = []
            cur = None if s == "#else" else re.match(r"#elif defined\(VERSION_(\w+)\)", s).group(1)
            continue
        if re.match(r"#\s*if", s): depth += 1
        elif re.match(r"#\s*endif", s): depth -= 1
        buf.append(l)
    if cur is not None:  # no #else: a version-only file
        out.append((cur, "\n".join(buf)))
        return out, None
    return out, "\n".join(buf)


def join_branches(branches, pal):
    t = []
    for i, (ser, text) in enumerate(branches):
        t.append(("#if" if i == 0 else "#elif") + " defined(VERSION_%s)" % ser)
        t.append(text.strip("\n"))
    if pal is not None:
        t.append("#else")
        t.append(pal.strip("\n"))
    t.append("#endif")
    return "\n".join(t) + "\n"


def directives(lines):
    out = {}
    for l in lines:
        m = re.match(r"//\s*(%s)\s+(.*)$" % "|".join(DIRECTIVES), l)
        if m: out.setdefault(m.group(1), m.group(2).strip())
    return out


_KNOWN = {}


def pal_known():
    """func_/D_<address> names the PAL sources and notes already use (kept as they are in version C)."""
    if _KNOWN: return _KNOWN
    rx = re.compile(r"\b(?:func|D)_[0-9A-F]{8}\b")
    for dp, dirs, fs in os.walk(os.path.join(ROOT, "src")):
        for fn in fs:
            if fn.endswith((".c", ".h")):
                t = open(os.path.join(dp, fn), errors="replace").read()
                if "#if defined(VERSION_" in t:  # only the PAL part of a shared file
                    br, pal = parse_branches(split_header(t)[1])
                    t = pal or ""
                _KNOWN.update(dict.fromkeys(rx.findall(t)))
    nd = os.path.join(ROOT, "notes")
    for fn in os.listdir(nd):
        if fn.endswith(".csv"): _KNOWN.update(dict.fromkeys(rx.findall(open(os.path.join(nd, fn), errors="replace").read())))
    _KNOWN["-"] = None
    return _KNOWN


def suffix_names(text, ver):
    """Version-only names get the serial: func_/D_<address> unknown to PAL (scratchpad D_1F80xxxx stay literal)."""
    known = pal_known()
    return re.sub(r"\b((?:func|D)_([0-9A-F]{8}))\b(?!_)",
                  lambda m: m.group(1) if m.group(1) in known or m.group(2).startswith("1F80") else m.group(1) + "_" + ver,
                  text)


class Share:
    def __init__(self, ver):
        os.chdir(ROOT)
        self.ver, self.serial = ver, VERSIONS[ver]["serial"]
        import build_report as BR
        self.BR = BR
        self.progs = [p for p in VERSIONS[ver]["progs"] if os.path.exists("config/%s/%s.yaml" % (ver, p))]
        self.sources = BR.jpdemo_sources(self.progs, ver)  # (prog, addr) -> (path, name)
        import units as U
        self.pal = {(p.lower(), u.addr): u for p, us in U.units().items() for u in us if u.file}
        self.pairs = {}  # (prog, addr) -> (PAL module, PAL addr) from the symbol table comments
        for prog in self.progs:
            for l in open("config/%s/symbol_addrs_%s.txt" % (ver, prog)):
                m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+); // type:func size:0x([0-9A-Fa-f]+)\s+PAL (\w+) 0x([0-9A-Fa-f]+)", l)
                if m: self.pairs[(prog, int(m.group(2), 16))] = (m.group(4).lower(), int(m.group(5), 16), m.group(1), int(m.group(3), 16))
        if os.path.exists("config/%s/reuse.txt" % ver):
            for l in open("config/%s/reuse.txt" % ver):
                f = l.split("#")[0].split()
                if len(f) == 4:
                    k = (f[0], int(f[1], 16))
                    old = self.pairs.get(k)
                    self.pairs[k] = (f[2].lower(), int(f[3], 16), old[2] if old else "func_%s_%s" % (f[1], ver), old[3] if old else 0)

    def write(self, path, text):
        print(("would write " if DRY else "wrote ") + os.path.relpath(path, ROOT))
        if not DRY: open(path, "w").write(text)

    def users(self, path, key):
        """Other functions of this version built from the file `path` (they must keep its plain PAL C)."""
        return [k for k, (p, _) in self.sources.items() if os.path.abspath(p) == os.path.abspath(path) and k != key]

    def add_branch(self, path, key, size, body, dirs):
        """Adds a version branch for `key` to the shared PAL file `path`. False when it cannot (another branch)."""
        hdr, rest = split_header(open(path, errors="replace").read())
        branches, pal = parse_branches(rest)
        vline = "// VERSION %s %s %08X %d" % (self.serial, key[0], key[1], size)
        mine = [t for s, t in branches if s == self.serial]
        if mine and mine[0].strip("\n") != body.strip("\n"):
            return False
        if not mine:
            branches.append((self.serial, body))
        if not mine and dirs is not None:
            # version branch compiler options differ from the PAL ones: per-version directive lines
            pd = directives(hdr)
            for d in DIRECTIVES:
                if dirs.get(d) != pd.get(d) and not any(l.startswith("// %s.%s " % (d, self.serial)) for l in hdr):
                    hdr.append("// %s.%s %s" % (d, self.serial, dirs.get(d, "default")))
        if vline not in hdr:
            hdr.insert(max([i + 1 for i, l in enumerate(hdr) if re.match(r"//\s*(FUNC|MATCHING|VERSION)\s", l)] or [len(hdr)]), vline)
        self.write(path, "\n".join(hdr) + "\n" + join_branches(branches, pal))
        self.sources[key] = (path, None)
        return True

    def fold(self, vfile):
        text = open(vfile, errors="replace").read()
        lines = text.split("\n")
        m = _HDR.match(lines[0])
        if not m: sys.exit("%s: no `// VERSION <SERIAL> <prog> <addr> <size>` header" % vfile)
        prog, addr, size = m.group(3), int(m.group(4), 16), int(m.group(5))
        key = (prog, addr)
        body_lines = [l for l in lines[1:] if not re.match(r"//\s*(%s)\s" % "|".join(DIRECTIVES), l)]
        dirs = directives(lines[1:])
        body = suffix_names("\n".join(body_lines).strip("\n"), self.ver) + "\n"
        # PAL counterpart: the comment `... version of PAL <module> <address>`, else the symbol table / reuse pairing
        cm = re.search(r"version of PAL (\w+) ([0-9A-Fa-f]{8})", text)
        pk = (cm.group(1).lower(), int(cm.group(2), 16)) if cm else (self.pairs.get(key) or (None, None))[:2]
        u = self.pal.get(pk)
        if u and not self.users(u.file, key) and self.add_branch(u.file, key, size, body, dirs):
            how = "folded into " + os.path.relpath(u.file, ROOT)
        else:
            path = self.version_only(key, size, body, dirs, os.path.basename(vfile)[:-2])
            how = "version-only " + os.path.relpath(path, ROOT)
        print("%s: %s" % (os.path.relpath(vfile, ROOT), how))
        if not DRY: os.unlink(vfile)

    def version_only(self, key, size, body, dirs, name):
        prog, addr = key
        name = suffix_names(name, self.ver)
        # the function itself is version-only even when its address happens to be a PAL name too
        body = re.sub(r"\bfunc_%08X\b(?!_)" % addr, "func_%08X_%s" % (addr, self.ver), body)
        if re.fullmatch(r"func_[0-9A-F]{8}", name): name = "func_%08X" % addr
        if not name.endswith("_" + self.ver): name += "_" + self.ver
        d = os.path.join(ROOT, pal_src_dir(self.ver, prog))
        path = os.path.join(d, name + ".c")
        if os.path.exists(path): path = os.path.join(d, "%s_%08X.c" % (name, addr))
        hdr = ["// VERSION %s %s %08X %d" % (self.serial, prog, addr, size)]
        hdr += ["// %s.%s %s" % (k, self.serial, v) for k, v in sorted(dirs.items())]
        self.write(path, "\n".join(hdr) + "\n" + join_branches([(self.serial, body)], None))
        self.sources[key] = (path, None)
        return path

    def asm(self):
        rep = json.load(open("build/%s/report.json" % self.ver))
        matched, addr_of = set(), {}
        names = {}
        for (prog, a), v in self.pairs.items(): names["%s/%s" % (prog, v[2])] = a
        for u in rep["units"]:
            m = u.get("measures", {})
            prog, nm = u["name"].split("/", 1)
            a = names.get(u["name"])
            if a is None:
                am = re.match(r"func_([0-9A-F]{8})", nm)
                if not am: continue
                a = int(am.group(1), 16)
            addr_of[(prog, a)] = u["name"]
            if int(m.get("total_code", 0) or 0) and m.get("matched_code") == m.get("total_code"): matched.add((prog, a))
            # data the PAL C already reproduces for this version keeps the file plain (the report counts it)
            elif int(m.get("matched_data", 0) or 0): matched.add((prog, a))
        per_file = {}
        for key, (mod, pa, nm, sz) in sorted(self.pairs.items()):
            u = self.pal.get((mod, pa))
            if not u or key not in addr_of: continue
            per_file.setdefault(u.file, []).append(key)
        for k, (p, _) in self.sources.items():  # functions already built from a file (keep it plain if they match)
            per_file.setdefault(p, [])
        n = 0
        for path, keys in sorted(per_file.items()):
            users = set(keys) | {k for k, (p, _) in self.sources.items() if p == path}
            if any(k in matched for k in users): continue
            t = open(path, errors="replace").read()
            if re.search(r"^#(if|elif) defined\(VERSION_%s\)" % self.serial, t, re.M): continue
            for key in sorted(set(keys)):
                prog = key[0]
                unit = addr_of[key].split("/", 1)[1]
                body = '#include "include_asm.h"\nINCLUDE_ASM_V("%s/nonmatchings/%s", %s);\n' % (prog, prog, unit)
                size = self.pairs[key][3]
                if self.add_branch(path, key, size, body, None):
                    n += 1
                    break  # one branch per version and file; the other keys keep their own asm (no base) anyway
        print("%s: %d INCLUDE_ASM branches added" % (self.ver, n))


def main():
    args = [a for a in sys.argv[1:] if a != "--dry-run"]
    if len(args) < 2 or args[0] not in ("fold", "asm"): sys.exit(__doc__)
    s = Share(canon(args[1]))
    if args[0] == "fold":
        for f in args[2:]: s.fold(f)
    else:
        s.asm()


if __name__ == "__main__":
    main()
