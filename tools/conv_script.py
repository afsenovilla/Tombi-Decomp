#!/usr/bin/env python3
"""SCRIPT_CTX raw-offset accesses -> ScriptCtx fields (include/scriptctx.h). Tries direct field form, then cast form;
keeps a file only if tools/objcmp.py says the linked object is unchanged. Usage: conv_script.py files..."""
import re, sys, os, subprocess, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools")); os.chdir(ROOT)
import objcmp
T = {"char": ("uchar", 1), "unsigned char": ("uchar", 1), "short": ("short", 2), "unsigned short": ("ushort", 2), "int": ("int", 4), "unsigned int": ("uint", 4)}
FIELD = {0x88: ("state", "uchar"), 0x89: ("cmpFlag", "uchar"), 0x8a: ("pc", "ushort"), 0x8c: ("sp", "ushort")}

def field(off):
    if off in FIELD: return FIELD[off]
    if 0x1190 <= off < 0x11b0 and off % 4 == 0: return ("arg[%d]" % ((off - 0x1190) // 4), "int")
    if 0x1090 <= off < 0x1190 and off % 4 == 0: return ("vars[%d]" % ((off - 0x1090) // 4), "int")
    return None

def conv_text(s, direct):
    vs = set(re.findall(r"\b(\w+) = SCRIPT_CTX;", s))
    if not vs: return None
    for v in vs:  # declarations
        s = re.sub(r"\b(?:unsigned char|char) \*%s\b" % v, "ScriptCtx *" + v, s)
    for v in vs:
        def mk(m):
            ct = m.group(1).strip(); off = int(m.group(2), 0)
            f = field(off)
            if f is None or ct not in T: return m.group(0)
            name, ft = f
            if T[ct][0] == ft: return "%s->%s" % (v, name)
            return "(*(%s*)&%s->%s)" % (ct, v, name)
        s = re.sub(r"\*\(([\w ]+?) ?\*\)\(%s \+ (0x[0-9a-fA-F]+|\d+)\)" % v, mk, s)
        s = re.sub(r"\(\*\(([\w ]+?) ?\*\)\(%s \+ (0x[0-9a-fA-F]+|\d+)\)\)" % v, lambda m: "(" + mk(m)[0:] + ")" if False else mk(m), s)
        s = re.sub(r"\b%s\[(0x[0-9a-fA-F]+)\]" % v, lambda m: ("%s->%s" % (v, field(int(m.group(1), 0))[0])) if field(int(m.group(1), 0)) and field(int(m.group(1), 0))[1] == "uchar" else m.group(0), s)
        s = re.sub(r"\*\(int \*\)\((\w+) \* 4 \+ \(int\)%s \+ 0x1090\)" % v, lambda m: "%s->vars[%s]" % (v, m.group(1)), s)
    s = re.sub(r"extern (?:unsigned char|char) \*SCRIPT_CTX;\n", "", s)
    if '#include "scriptctx.h"' not in s:
        ls = s.split("\n"); i = 0
        while i < len(ls) and ls[i].startswith("//"): i += 1
        ls.insert(i, '#include "scriptctx.h"'); s = "\n".join(ls)
    return s

if __name__ == "__main__":
    tmp = tempfile.mkdtemp(); inc = os.path.join(tmp, "inc"); objcmp.mkinc(inc, None); objcmp.init()
    for f in sys.argv[1:]:
        orig = open(f, errors="surrogateescape").read()
        if "SCRIPT_CTX" not in orig or "scriptctx.h" in orig and "extern" not in orig: continue
        new = conv_text(orig, False)
        if new is None or new == orig: continue
        open(f, "w", errors="surrogateescape").write(new)
        r = objcmp.job((f, inc, tmp))
        if r is None: print("conv", f)
        else:
            open(f, "w", errors="surrogateescape").write(orig); print("FAIL", f)
