#!/usr/bin/env python3
"""Moves the big zero-initialised static arrays of a translation unit out of the program image (the 3DS build;
docs/PORT.md "Nintendo 3DS", "Lazy tables").

The 3DS loads code + data + bss into the title's memory (64 MB on an Old 3DS, for everything) and below the address
0x04000000: the widescreen generators' work tables (tens of MB of bss) do not fit there.  This writes copies of the
given sources in which every `static T name[...]...;` without an initialiser (not const) becomes a pointer to that
array type, `static T (*name__lz)[...]...;`, and `name` a macro for `(*name__lz)`, so the code reads the same (indexing,
sizeof, decay).  A function-local one gets its pointer at file scope just before the function and its macro for the
rest of the function (push_macro / pop_macro).  At the end of the unit, `port_lazy_ok()` (runtime/lazy_bss.h) places
them all in one block the first time it is called and says whether it could (runtime/lazy_bss.c, n3ds/main_3ds.c
decide); the code calls it before it touches any of them (the modules' entry points).  Statics with an initialiser,
const ones, struct bodies and anything with parentheses (function pointers, prototypes) are left as they are.

    lazy_bss.py OUTDIR MAIN.c [--transform FILE ...] [--copy FILE ...]

MAIN.c is the unit (written to OUTDIR with the registration appended); the --transform files (MAIN.c too when it is
listed) have their arrays moved; --copy files are written unchanged but for the includes.  In every written file an
`#include "x"` of a file not written to OUTDIR that is next to the source points there (an absolute path).
"""
import os, re, sys


def mask(src):
    """the text with comments, string / char literals and preprocessor lines blanked (same length, newlines kept)"""
    out = list(src); i = 0; n = len(src); bol = True
    while i < n:
        c = src[i]
        if bol and c in " \t":
            i += 1; continue
        if bol and c == "#":                       # a directive, with its continuation lines
            j = i
            while j < n:
                if src[j] == "\n" and src[j - 1] != "\\": break
                if src.startswith("/*", j):
                    k = src.find("*/", j + 2); k = n if k < 0 else k + 2
                    for q in range(j, k):
                        if out[q] != "\n": out[q] = " "
                    j = k; continue
                if out[j] != "\n": out[j] = " "
                j += 1
            i = j; bol = True; continue
        bol = False
        if src.startswith("//", i):
            j = src.find("\n", i); j = n if j < 0 else j
            for q in range(i, j): out[q] = " "
            i = j; continue
        if src.startswith("/*", i):
            j = src.find("*/", i + 2); j = n if j < 0 else j + 2
            for q in range(i, j):
                if out[q] != "\n": out[q] = " "
            i = j; continue
        if c in "\"'":
            j = i + 1
            while j < n and src[j] != c:
                j += 2 if src[j] == "\\" else 1
            for q in range(i + 1, min(j, n)):
                if out[q] != "\n": out[q] = " "
            i = j + 1; continue
        if c == "\n": bol = True
        i += 1
    return "".join(out)


IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")


def split_top(s, sep=","):
    parts, d, cur = [], 0, ""
    for ch in s:
        if ch in "[(": d += 1
        elif ch in "])": d -= 1
        if ch == sep and d == 0: parts.append(cur); cur = ""
        else: cur += ch
    parts.append(cur)
    return parts


def parse_static(stmt):
    """`static TYPE decl, decl, ...` (no ';'): (type, [(name, declarator, is_array)]) or None"""
    toks = stmt.split()
    if not toks or toks[0] != "static": return None
    if any(t in ("const", "inline", "extern", "typedef", "register", "__thread", "_Thread_local") for t in IDENT.findall(stmt)): return None
    if "(" in stmt or "=" in stmt or "{" in stmt or "}" in stmt or "[" not in stmt: return None
    body = stmt.strip()[len("static"):]
    decls = split_top(body)
    # the type: the first declarator's text before its identifier (the last identifier before '[', ',' or the end)
    first = decls[0]
    m = None
    for mm in IDENT.finditer(first):
        rest = first[mm.end():].lstrip()
        if rest == "" or rest.startswith("["): m = mm; break
    if not m: return None
    pre = first[:m.start()]
    # pointer stars belong to the declarator, the rest is the type
    k = len(pre.rstrip())
    while k > 0 and pre[k - 1] in "* \t": k -= 1
    typ = pre[:k].strip()
    if not typ or not IDENT.fullmatch(typ.split()[-1]): return None
    out = []
    for j, d in enumerate(decls):
        dd = d if j else first[k:]
        dd = dd.strip()
        mm = re.fullmatch(r"(\**)\s*([A-Za-z_][A-Za-z0-9_]*)\s*((?:\[[^\]]*\]\s*)*)", dd)
        if not mm: return None
        out.append((mm.group(2), mm.group(1) + "%s" + mm.group(3).strip(), bool(mm.group(3))))
    return typ, out


NOT_TYPE = {"return", "sizeof", "case", "else", "do", "goto", "typeof", "__typeof__", "_Alignof", "if", "while",
            "for", "switch", "default", "break", "continue"}


def declared_again(m, name, a, b, skip):
    """does `name` look declared (or used as a member) in m[a:b] anywhere but in [skip0, skip1)?  Conservative: a
    variable, parameter or member of that name would be turned into the moved array by the macro."""
    for mm in re.finditer(r"\b%s\b" % re.escape(name), m[a:b]):
        p = a + mm.start()
        if skip[0] <= p < skip[1]: continue
        j = p - 1
        while j >= 0 and m[j] in " \t\n": j -= 1
        if j >= 0 and (m[j] == "." or m[j - 1:j + 1] == "->"): return True
        k = j
        while k >= 0 and m[k] in "*& \t\n": k -= 1          # (pointer / reference stars)
        e = k + 1
        while k >= 0 and (m[k].isalnum() or m[k] == "_"): k -= 1
        w = m[k + 1:e]
        if w and IDENT.fullmatch(w) and w not in NOT_TYPE: return True     # `type name`, `type *name`
        if j >= 0 and m[j] == ",":
            # `type a, name`: the statement's first two words a declaration's
            q = j
            while q >= 0 and m[q] not in ";{}(": q -= 1
            st = m[q + 1:p].split()
            if len(st) >= 2 and IDENT.fullmatch(st[0]) and st[0] not in NOT_TYPE and IDENT.fullmatch(st[1].lstrip("*")): return True
    return False


class Unit:
    def __init__(self, unit):
        self.ptrs = []          # pointer names, for the registration
        self.n = 0
        self.unit = unit
        self.kept = []

    def scan(self, src):
        """the candidate statements: (s0, end, depth, func_start, func_end, typ, decls)"""
        m = mask(src)
        found = []; opens = []
        depth = 0; stmt_start = 0; top_boundary = 0; func_start = 0; func_idx = []
        in_dir = False; line_start = 0
        n = len(m)
        for i in range(n):
            c = m[i]
            if c == "\n":
                ln = src[line_start:i]
                if ln.lstrip().startswith("#"): in_dir = True
                if in_dir and not ln.endswith("\\"):
                    in_dir = False
                    if depth == 0: top_boundary = i + 1; stmt_start = i + 1
                line_start = i + 1
                continue
            if c == "{":
                if depth == 0: func_start = top_boundary; func_idx = []
                depth += 1; stmt_start = i + 1
            elif c == "}":
                depth -= 1; stmt_start = i + 1
                if depth == 0:
                    top_boundary = i + 1
                    for f in func_idx: f[4] = i + 1
                    func_idx = []
            elif c == ";":
                stmt = m[stmt_start:i]
                p = parse_static(stmt)
                if p and any(d[2] for d in p[1]):
                    s0 = stmt_start + len(stmt) - len(stmt.lstrip())
                    f = [s0, i + 1, depth, func_start, None, p[0], p[1]]
                    found.append(f)
                    if depth: func_idx.append(f)
                stmt_start = i + 1
                if depth == 0: top_boundary = i + 1
        return m, found

    def transform_all(self, srcs, tr):
        """srcs: {file: text}; the files in tr have their arrays moved; returns {file: new text}"""
        masked = {}; cands = {}
        for f in srcs:
            if f in tr: masked[f], cands[f] = self.scan(srcs[f])
        out = dict(srcs)
        for f in cands:
            m = masked[f]; src = srcs[f]; edits = []
            for s0, end, depth, fstart, fend, typ, decls in cands[f]:
                arrays = []
                for d in decls:
                    if not d[2]: continue
                    nm = d[0]
                    if depth == 0:
                        bad = declared_again(m, nm, 0, len(m), (s0, end)) or any(
                            declared_again(masked.get(g) or mask(srcs[g]), nm, 0, len(srcs[g]), (0, 0)) for g in srcs if g != f)
                    else:
                        bad = declared_again(m, nm, end, fend, (0, 0))
                    if bad: self.kept.append(nm)
                    else: arrays.append(d)
                if not arrays: continue
                keep = [d[1] % d[0] for d in decls if d not in arrays]
                text = ("static %s %s;" % (typ, ", ".join(keep))) if keep else ""
                pops = []
                for nm, decl, _ in arrays:
                    if depth == 0:
                        ptr = "%s__lz" % nm
                        text += "\nstatic %s %s;\n#define %s (*%s)\n" % (typ, decl % ("(*%s)" % ptr), nm, ptr)
                    else:
                        self.n += 1; ptr = "port_lz_%s_%d" % (self.unit, self.n)
                        edits.append((fstart, fstart, "\nstatic %s %s;\n" % (typ, decl % ("(*%s)" % ptr))))
                        text += '\n#pragma push_macro("%s")\n#undef %s\n#define %s (*%s)\n' % (nm, nm, nm, ptr)
                        pops.append(nm)
                    self.ptrs.append(ptr)
                if pops:
                    edits.append((fend, fend, "".join('\n#pragma pop_macro("%s")' % nm for nm in reversed(pops)) + "\n"))
                edits.append((s0, end, text))
            for a, b, t in sorted(edits, key=lambda e: (e[0], e[1]), reverse=True):
                src = src[:a] + t + src[b:]
            out[f] = src
        return out

    def registration(self):
        if not self.ptrs: return "\nstatic int port_lazy_ok(void) { return 1; }\n"
        al = lambda p: "((sizeof *%s + 15) & ~(size_t)15)" % p
        s = ["", "/* ---- lazy_bss.py: the unit's moved arrays, in one block placed at the first call */",
             "#include <stddef.h>", "extern void *port_lazy_alloc(const char *what, size_t n);",
             "static int port_lazy_ok(void)", "{", "    static int st;      /* 0: not tried, 1: placed, -1: no memory */",
             "    if (!st) {", "        size_t n = 0; char *b;"]
        for p in self.ptrs: s.append("        n += %s;" % al(p))
        s.append('        b = (char *)port_lazy_alloc("%s", n);' % self.unit)
        s.append("        if (!b) st = -1;")
        s.append("        else {")
        s.append("            st = 1;")
        for p in self.ptrs: s.append("            %s = (void *)b; b += %s;" % (p, al(p)))
        s += ["        }", "    }", "    return st > 0;", "}", ""]
        return "\n".join(s)


def main():
    a = sys.argv[1:]
    out, main_c = a[0], a[1]; tr, cp = [], []
    cur = None
    for x in a[2:]:
        if x == "--transform": cur = tr
        elif x == "--copy": cur = cp
        else: cur.append(x)
    files = []
    for f in [main_c] + tr + cp:
        if f not in files: files.append(f)
    names = {os.path.basename(f) for f in files}
    unit = re.sub(r"\W", "_", os.path.splitext(os.path.basename(main_c))[0])
    u = Unit(unit)
    os.makedirs(out, exist_ok=True)
    srcs = {f: open(f).read() for f in files}
    srcs = u.transform_all(srcs, tr)
    texts = {}
    for f in files:
        src = srcs[f]
        d = os.path.dirname(os.path.abspath(f))
        def inc(mm):
            nm = mm.group(2)
            if os.path.basename(nm) in names and os.path.dirname(nm) == "": return mm.group(0)
            q = os.path.normpath(os.path.join(d, nm))
            return '%s"%s"' % (mm.group(1), q) if os.path.exists(q) else mm.group(0)   # (else from the -I paths)
        src = re.sub(r'(^[ \t]*#[ \t]*include[ \t]*)"([^"]+)"', inc, src, flags=re.M)
        texts[f] = src
    texts[main_c] += u.registration()
    for f, t in texts.items():
        p = os.path.join(out, os.path.basename(f))
        old = open(p).read() if os.path.exists(p) else None
        if old != t: open(p, "w").write(t)      # (unchanged files keep their time: no rebuild)
    print("lazy_bss: %s: %d arrays moved, %d kept (a name declared again: %s)" % (unit, len(u.ptrs), len(u.kept), " ".join(sorted(set(u.kept)))))


main()
