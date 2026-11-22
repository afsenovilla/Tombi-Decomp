#!/usr/bin/env python3
"""Proves that a readability pass over the port's C code (port2/runtime, port2/shims) changed nothing the compiler sees.

Two checks, run on every C / header / .inc file that differs between a base commit and the working tree:

  tokens   The files are split into C tokens with comments dropped, line splices joined, adjacent string literals
           merged (by their bytes) and preprocessor directives kept as lines.  Equal token lists: only layout and
           comments changed.  Otherwise the only difference accepted is a renamed identifier, under these rules:
             - the new name does not occur anywhere in the base file and is not a macro of the build;
             - two different old names never get the same new name;
             - an old name renamed at file scope (outside every function body) is renamed everywhere in the file;
               an old name renamed inside a function body is renamed everywhere in that body;
             - names with external linkage keep their names (checked on the i386 objects);
             - struct members keep their names (a member access .x / ->x is never renamed);
             - no renames in headers (.h: shared with other files), nor in code under #if blocks for builds this
               container cannot compile (__3DS__, __ANDROID__, __arm__, __wasm__, _WIN32, the USA / JP versions),
               nor in files only those builds compile.
  objects  Every port2rt source of the i386 build (build/port2/compile_commands.json) that is, or includes, a
           changed file is compiled from the base tree and from the working tree with the build's flags (no debug
           info, a fixed __DATE__ / __TIME__): the code, data and relocations must be equal (renamed static symbols
           are matched through the renames the token check found).

  python3 tools/same_code.py [--base REF] [--no-objects] [paths...]      (default base: origin/master)
Exit status 0 when every file passes.
"""
import argparse, os, re, subprocess, sys, tempfile, json, shutil

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIRS = ["port2/runtime", "port2/shims"]
EXTS = (".c", ".h", ".inc")
UNTESTED_GUARDS = ("__3DS__", "__ANDROID__", "ANDROID", "__arm__", "__wasm__", "_WIN32", "__aarch64__", "_MSC_VER",
                   "PORT_VERSION_USA", "PORT_VERSION_JP")
# files only the 3DS / Windows / Android / wasm builds compile (the i386 build never sees them)
UNTESTED_FILES = ("port2/shims/gpu_pica.c", "port2/shims/gpu_pica.h", "port2/runtime/host_win32.c",
                  "port2/runtime/lowmem_arm.inc")
# macros of other platforms' headers a rename must not hit (the i386 build's own macros are read from gcc -dM)
FOREIGN_MACROS = {"near", "far", "small", "min", "max", "IN", "OUT", "OPTIONAL", "interface", "ERROR", "DELETE",
                  "TRUE", "FALSE", "CONST", "VOID", "BOOL", "BYTE", "WORD", "DWORD", "LONG", "PASCAL", "CALLBACK",
                  "WINAPI", "APIENTRY", "RGB", "NULL", "errno", "stdin", "stdout", "stderr", "assert", "offsetof",
                  "u8", "u16", "u32", "s8", "s16", "s32", "u64", "s64", "vu32", "vs32", "Result", "Handle", "BIT"}


def git(*a, check=True):
    return subprocess.run(["git", *a], cwd=ROOT, capture_output=True, text=True, check=check).stdout


def git_bytes(*a):
    r = subprocess.run(["git", *a], cwd=ROOT, capture_output=True)
    return r.stdout if r.returncode == 0 else None


# --- the tokenizer -----------------------------------------------------------------------------------------------

PUNCT = sorted("""... <<= >>= -> ++ -- << >> <= >= == != && || *= /= %= += -= &= ^= |= ## :: [ ] ( ) { } . & * + - ~ ! / %
< > ^ | ? : ; = , #""".split(), key=len, reverse=True)
IDENT = re.compile(r"[A-Za-z_]\w*")
NUMBER = re.compile(r"\.?\d(?:[eEpP][+-]|[\w.])*")
ESCAPES = {"n": 10, "t": 9, "r": 13, "a": 7, "b": 8, "f": 12, "v": 11, "\\": 92, "'": 39, '"': 34, "?": 63, "0": 0}


def c_string_bytes(body):
    """The values a string literal's body stands for (escapes resolved), so "ab" "c" == "abc"."""
    out, i = [], 0
    while i < len(body):
        ch = body[i]
        if ch != "\\":
            out.append(ord(ch)); i += 1; continue
        n = body[i + 1]
        if n == "x":
            m = re.match(r"[0-9a-fA-F]+", body[i + 2:]); out.append(int(m.group(0), 16)); i += 2 + len(m.group(0))
        elif n in "01234567":
            m = re.match(r"[0-7]{1,3}", body[i + 1:]); out.append(int(m.group(0), 8)); i += 1 + len(m.group(0))
        else:
            out.append(ESCAPES.get(n, ord(n))); i += 2
    return tuple(out)


class Tok:
    __slots__ = ("kind", "text", "line", "depth", "func", "guard")

    def __init__(self, kind, text, line):
        self.kind, self.text, self.line, self.depth, self.func, self.guard = kind, text, line, 0, -1, False

    def key(self):
        return (self.kind, self.text)


def tokenize(src):
    """C tokens of a source: (kind, text) with kind id / num / str / chr / punct / pp (directive start) / eol."""
    src = src.replace("\r\n", "\n")
    toks, i, line, n = [], 0, 1, len(src)
    in_dir = False            # inside a preprocessor directive (ends at a newline)
    at_line_start = True
    define_name = False       # the token after "#define": a following "(" without space makes a function-like macro
    while i < n:
        c = src[i]
        if c == "\\" and src.startswith("\\\n", i):              # line splice
            i += 2; line += 1; continue
        if c == "\n":
            if in_dir:
                toks.append(Tok("eol", "", line)); in_dir = False
            i += 1; line += 1; at_line_start = True; continue
        if c in " \t\f\v":
            i += 1; continue
        if src.startswith("//", i):
            while i < n and src[i] != "\n":
                if src.startswith("\\\n", i):
                    i += 2; line += 1; continue
                i += 1
            continue
        if src.startswith("/*", i):
            j = src.index("*/", i + 2); line += src.count("\n", i, j); i = j + 2; continue
        if c == "#" and at_line_start and not in_dir:
            toks.append(Tok("pp", "#", line)); in_dir = True; at_line_start = False; i += 1
            m = re.compile(r"[ \t]*(\w+)").match(src, i)
            if m:
                toks.append(Tok("id", m.group(1), line)); i = m.end()
                define_name = m.group(1) == "define"
            continue
        at_line_start = False
        if c == '"' or (c in "LuU8" and re.match(r'(?:u8|[LuU])"', src[i:i + 3])):
            pre = re.match(r'(?:u8|[LuU])?', src[i:]).group(0); j = i + len(pre) + 1
            while src[j] != '"':
                j += 2 if src[j] == "\\" else 1
            body = src[i + len(pre) + 1:j]
            if in_dir and toks and toks[-1].text == "include":
                toks.append(Tok("str", ("include", body), line))
            else:
                val = c_string_bytes(body)
                if toks and toks[-1].kind == "str" and toks[-1].text[0] == pre:  # adjacent literals: one string
                    prev = toks[-1]; prev.text = (pre, prev.text[1] + val)
                else:
                    toks.append(Tok("str", (pre, val), line))
            i = j + 1; continue
        if c == "'" or (c in "LuU" and src[i + 1:i + 2] == "'"):
            j = i + (2 if c != "'" else 1)
            while src[j] != "'":
                j += 2 if src[j] == "\\" else 1
            toks.append(Tok("chr", src[i:j + 1], line)); i = j + 1; continue
        if c.isdigit() or (c == "." and i + 1 < n and src[i + 1].isdigit()):
            m = NUMBER.match(src, i); toks.append(Tok("num", m.group(0), line)); i = m.end(); continue
        m = IDENT.match(src, i)
        if m:
            toks.append(Tok("id", m.group(0), line)); i = m.end()
            if define_name:
                define_name = False
                if src[i:i + 1] == "(":
                    toks.append(Tok("punct", "<fnmacro>", line))
            continue
        if in_dir and c == "<" and toks and toks[-1].text == "include":
            j = src.index(">", i); toks.append(Tok("str", ("<>", src[i:j + 1]), line)); i = j + 1; continue
        for p in PUNCT:
            if src.startswith(p, i):
                toks.append(Tok("punct", p, line)); i += len(p); break
        else:
            raise ValueError(f"line {line}: unexpected character {c!r}")
    if in_dir:
        toks.append(Tok("eol", "", line))
    annotate(toks)
    return toks


def annotate(toks):
    """Brace depth, the index of the function body each token is in (-1: file scope), and whether it is under an
    #if for a build this container cannot compile."""
    depth, func, nfunc, stack, k = 0, -1, 0, [], 0
    while k < len(toks):
        t = toks[k]
        if t.kind == "pp":
            d = toks[k + 1].text if k + 1 < len(toks) else ""
            end = k
            while toks[end].kind != "eol":
                end += 1
            cond = " ".join(str(x.text) for x in toks[k + 2:end])
            hit = any(re.search(r"\b%s\b" % re.escape(g), cond) for g in UNTESTED_GUARDS)
            if d in ("if", "ifdef", "ifndef"):
                stack.append(hit)
            elif d in ("elif", "else"):
                if stack:
                    # the #else of an "#if __3DS__" is code the i386 build does compile, but both branches count as
                    # untested: the check stays on the safe side
                    stack[-1] = stack[-1] or hit
            elif d == "endif" and stack:
                stack.pop()
            for x in toks[k:end + 1]:
                x.depth, x.func, x.guard = depth, func, any(stack)
            k = end + 1; continue
        if t.text == "{":
            if depth == 0:
                func = nfunc; nfunc += 1
            depth += 1
        t.depth, t.func, t.guard = depth, func, any(stack)
        if t.text == "}":
            depth = max(0, depth - 1)
            if depth == 0:
                func = -1
        k += 1


# --- the token check ---------------------------------------------------------------------------------------------

def macro_names(path):
    """Macros the i386 build defines for a source (compiler, system and project headers)."""
    cmd = compile_command(path)
    if not cmd:
        return set()
    args = [a for a in cmd["arguments"] if a not in ("-c",)]
    out_i = args.index("-o"); del args[out_i:out_i + 2]
    r = subprocess.run(args + ["-dM", "-E", "-o", "-"], cwd=cmd["directory"], capture_output=True, text=True)
    return set(re.findall(r"^#define (\w+)", r.stdout, re.M))


_ccmds = None


def compile_commands():
    global _ccmds
    if _ccmds is None:
        p = os.path.join(ROOT, "build/port2/compile_commands.json")
        main_root = ROOT
        if not os.path.exists(p):     # a git worktree: the build of the main checkout, with this tree's sources
            first = git("worktree", "list", "--porcelain").splitlines()[0]
            main_root = first.split(" ", 1)[1]
            p = os.path.join(main_root, "build/port2/compile_commands.json")
        if not os.path.exists(p):
            sys.exit("build/port2/compile_commands.json missing: cmake -S port2 -B build/port2 "
                     "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
        _ccmds = []
        for c in json.load(open(p)):
            if "/port2/runtime/" in c["file"] or "/port2/shims/" in c["file"]:
                c["arguments"] = c.get("arguments") or c["command"].split()
                if main_root != ROOT:
                    mp, wp = main_root + "/port2/", ROOT + "/port2/"
                    c["file"] = c["file"].replace(mp, wp)
                    c["arguments"] = [x.replace(mp, wp) for x in c["arguments"]]
                _ccmds.append(c)
    return _ccmds


def compile_command(path):
    full = os.path.join(ROOT, path)
    for c in compile_commands():
        if c["file"] == full:
            return c
    return None


def check_tokens(path, old_src, new_src, macros):
    """Returns (problems, renames {old: new})."""
    try:
        a, b = tokenize(old_src), tokenize(new_src)
    except Exception as e:  # noqa: BLE001
        return [f"cannot tokenize: {e}"], {}
    if [t.key() for t in a] == [t.key() for t in b]:
        return [], {}
    if len(a) != len(b):
        import difflib
        sm = difflib.SequenceMatcher(None, [t.key() for t in a], [t.key() for t in b], autojunk=False)
        for op, i1, i2, j1, j2 in sm.get_opcodes():
            if op != "equal":
                at = a[i1].line if i1 < len(a) else a[-1].line
                bt = b[j1].line if j1 < len(b) else b[-1].line
                return [f"tokens added or removed near line {at} (new line {bt}): "
                        f"{' '.join(str(t.text) for t in a[i1:i2][:8])!r} -> "
                        f"{' '.join(str(t.text) for t in b[j1:j2][:8])!r}"], {}
    problems, renames, by_new = [], {}, {}
    old_ids = {t.text for t in a if t.kind == "id"}
    scopes = {}   # (old, func) -> new ; func -1 = file scope
    for k, (ta, tb) in enumerate(zip(a, b)):
        member = k > 0 and a[k - 1].text in (".", "->")       # a struct member: a name space of its own
        if ta.key() == tb.key():
            if ta.kind == "id" and not member:
                scopes.setdefault((ta.text, ta.func), set()).add(ta.text)
            continue
        if ta.kind != "id" or tb.kind != "id":
            problems.append(f"line {ta.line} (new {tb.line}): {ta.text!r} became {tb.text!r}"); continue
        if member:
            problems.append(f"line {ta.line}: struct member {ta.text} renamed to {tb.text} (members keep their names)")
            continue
        if path.endswith(".h"):
            problems.append(f"line {ta.line}: rename {ta.text} -> {tb.text} in a header"); continue
        if ta.guard or path in UNTESTED_FILES:
            problems.append(f"line {ta.line}: rename {ta.text} -> {tb.text} in code this container cannot compile"); continue
        if tb.text in old_ids:
            problems.append(f"line {ta.line}: new name {tb.text} already occurs in the file"); continue
        if tb.text in macros or tb.text in FOREIGN_MACROS:
            problems.append(f"line {ta.line}: new name {tb.text} is a macro"); continue
        if by_new.setdefault(tb.text, ta.text) != ta.text:
            problems.append(f"line {ta.line}: {ta.text} and {by_new[tb.text]} both renamed to {tb.text}"); continue
        renames.setdefault(ta.text, set()).add(tb.text)
        scopes.setdefault((ta.text, ta.func), set()).add(tb.text)
    for (old, func), news in scopes.items():
        if old in renames and len(news) > 1:
            where = "at file scope" if func < 0 else f"in function body #{func}"
            problems.append(f"{old} renamed only in part {where}: {sorted(news)}")
    file_scope = {old for (old, func), news in scopes.items() if func < 0 and old in renames and news != {old}}
    for old in file_scope:
        left = {f for (o, f), news in scopes.items() if o == old and old in news}
        if left:
            problems.append(f"{old} (file scope) renamed, but kept in function bodies {sorted(left)}")
    return problems, {k: sorted(v) for k, v in renames.items()}


# --- the object check --------------------------------------------------------------------------------------------

def includes_of(path, src):
    return re.findall(r'^\s*#\s*include\s+"([^"]+)"', src, re.M)


def sources_using(changed, base):
    """The i386 build's port2rt sources that are, or (transitively) include, one of the changed files, with the files
    each one reads."""
    out = []
    for c in compile_commands():
        rel = os.path.relpath(c["file"], ROOT)
        seen, todo = set(), [rel]
        while todo:
            p = todo.pop()
            if p in seen:
                continue
            seen.add(p)
            full = os.path.join(ROOT, p)
            if not os.path.exists(full):
                continue
            for inc in includes_of(p, open(full, errors="replace").read()):
                for cand in (os.path.normpath(os.path.join(os.path.dirname(p), inc)), "port2/include/" + inc):
                    if os.path.exists(os.path.join(ROOT, cand)):
                        todo.append(cand); break
        if seen & set(changed):
            out.append((rel, seen))
    return out


def object_fingerprint(obj, renames):
    """Section contents (no debug info), relocations and symbols of an object file, renames applied."""
    def run(*a):
        return subprocess.run(a, capture_output=True, text=True, check=True).stdout
    s = run("objdump", "-s", obj)
    keep, cur = [], None
    for ln in s.splitlines():
        m = re.match(r"Contents of section (\S+):", ln)
        if m:
            cur = m.group(1)
        if cur and (cur.startswith(".debug") or cur in (".comment", ".note.GNU-stack")):
            continue
        keep.append(ln)
    rel = run("objdump", "-r", obj)
    sym = "\n".join(l for l in run("objdump", "-t", obj).splitlines()[2:] if ".debug" not in l)
    names = rel + "\n" + sym                 # renames only touch symbol names, never the section bytes
    for old, news in renames.items():
        for new in news:
            names = re.sub(r"\b%s\b" % re.escape(new), old, names)
    text = ("\n".join(keep) + names).replace(obj, "OBJ")
    return text


def compile_one(cmd, src_root, out):
    args = list(cmd["arguments"])
    i = args.index("-o"); args[i + 1] = out
    full = cmd["file"]
    args[args.index(full)] = os.path.join(src_root, os.path.relpath(full, ROOT))
    args = [x for x in args if not x.startswith("-g")] + ["-g0", f"-ffile-prefix-map={src_root}={ROOT}"]
    env = dict(os.environ, SOURCE_DATE_EPOCH="1700000000")
    r = subprocess.run(args, cwd=cmd["directory"], capture_output=True, text=True, env=env)
    return r.returncode, r.stderr


def check_objects(changed, base, renames_by_file):
    srcs = sources_using(changed, base)
    if not srcs:
        return []
    problems = []
    with tempfile.TemporaryDirectory() as tmp:
        broot = os.path.join(tmp, "base")
        os.makedirs(broot)
        arch = subprocess.run(["git", "archive", base, "port2"], cwd=ROOT, capture_output=True, check=True).stdout
        subprocess.run(["tar", "-x", "-C", broot], input=arch, check=True)
        for rel, reads in srcs:
            renames = {}                      # the renames of the files this source reads
            for f in reads:
                for k, v in renames_by_file.get(f, {}).items():
                    renames.setdefault(k, set()).update(v)
            cmd = compile_command(rel)
            oa, ob = os.path.join(tmp, "a.o"), os.path.join(tmp, "b.o")
            ra, ea = compile_one(cmd, broot, oa)
            rb, eb = compile_one(cmd, ROOT, ob)
            if ra or rb:
                problems.append(f"{rel}: does not compile ({'base' if ra else 'new'}): {(ea or eb)[:600]}")
                continue
            syms = subprocess.run(["objdump", "-t", oa], capture_output=True, text=True).stdout
            globs = {ln.split()[-1] for ln in syms.splitlines() if re.match(r"^[0-9a-f]+ g", ln)}
            if globs & set(renames):
                problems.append(f"{rel}: renamed names with external linkage: {sorted(globs & set(renames))}")
            elif object_fingerprint(oa, {}) != object_fingerprint(ob, renames):
                problems.append(f"{rel}: the compiled object differs")
            else:
                print(f"  same object: {rel}")
    return problems


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--base", default="origin/master")
    ap.add_argument("--no-objects", action="store_true", help="the token check only")
    ap.add_argument("paths", nargs="*")
    o = ap.parse_args()
    paths = o.paths or DIRS
    diff = git("diff", "--name-only", o.base, "--", *paths).split()
    untracked = git("ls-files", "--others", "--exclude-standard", "--", *paths).split()
    changed = [p for p in diff + untracked if p.endswith(EXTS)]
    if not changed:
        print("no changed C files"); return 0
    bad, renames_by_file = 0, {}
    for p in changed:
        old = git_bytes("show", f"{o.base}:{p}")
        if old is None:
            print(f"NEW   {p} (not in {o.base}: not checked)"); bad += 1; continue
        if not os.path.exists(os.path.join(ROOT, p)):
            print(f"GONE  {p}"); bad += 1; continue
        new = open(os.path.join(ROOT, p), "rb").read()
        macros = macro_names(p) if p.endswith(".c") else set()
        if not macros and not p.endswith(".h"):
            for c in compile_commands():          # an .inc: the macros of a source that includes it
                if os.path.basename(p) in open(c["file"], errors="replace").read():
                    macros = macro_names(os.path.relpath(c["file"], ROOT)); break
        problems, renames = check_tokens(p, old.decode("utf-8", "replace"), new.decode("utf-8", "replace"), macros)
        if problems:
            bad += 1
            print(f"FAIL  {p}")
            for x in problems[:20]:
                print(f"        {x}")
        else:
            renames_by_file[p] = renames
            note = f" ({len(renames)} renames: " + ", ".join(f"{k}->{'/'.join(v)}" for k, v in
                                                            sorted(renames.items())[:12]) + ")" if renames else ""
            print(f"OK    {p}{note}")
    if not o.no_objects:
        print("objects (i386 build):")
        for x in check_objects(changed, o.base, renames_by_file):
            bad += 1
            print(f"FAIL  {x}")
    print("all the same" if not bad else f"{bad} problem(s)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
