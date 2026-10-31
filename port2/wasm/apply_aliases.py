#!/usr/bin/env python3
"""apply_aliases.py MERGED.ll SYMS.ld OUT.ll: the linker-script aliases of gen2.py (`a = b;`, `a = region + 0x10;`) on the
one-module IR of the wasm32 build (wasm-ld takes no linker script).  A name whose chain ends at a global variable
defined in the module becomes a GlobalAlias at that offset; any other (functions, libc names) is renamed to its
target at every use and its declaration dropped (calls keep their own function type: the wasm backend's
FixFunctionBitcasts adapts arity as the i386 ABI tolerated it)."""
import re, sys
ll_path, ld_path, out_path = sys.argv[1:4]
amap = {}
for l in open(ld_path):
    m = re.match(r"\s*([A-Za-z_][\w.$]*)\s*=\s*([A-Za-z_][\w.$]*)\s*(?:\+\s*(0x[0-9A-Fa-f]+|\d+))?\s*;", l)
    if m: amap[m.group(1)] = (m.group(2), int(m.group(3), 0) if m.group(3) else 0)
# the data image's word for the game's main (0x800163F4) points to runtime/main.c's main(argc, argv), which clang
# names __main_argc_argv on wasm32
amap.setdefault("main", ("__main_argc_argv", 0))
def resolve(n):
    off, seen = 0, set()
    while n in amap and n not in seen:
        seen.add(n); t, o = amap[n]; off += o; n = t
    return n, off
txt = open(ll_path).read()
lines = txt.split("\n")
# what the module defines / declares
gdef, fdef, decl_line = set(), set(), {}
for i, l in enumerate(lines):
    m = re.match(r'@"?([^"\s=]+)"? = (.*)', l)
    if m:
        rest = m.group(2)
        if re.match(r"(?:\w+ )*external ", rest): decl_line[m.group(1)] = i
        elif " alias " in " " + rest: gdef.add(m.group(1))
        else: gdef.add(m.group(1))
        continue
    m = re.match(r'define [^@]*@"?([^"(\s]+)"?\(', l)
    if m: fdef.add(m.group(1)); continue
    m = re.match(r'declare [^@]*@"?([^"(\s]+)"?\(', l)
    if m: decl_line[m.group(1)] = i
rename, aliases, drop = {}, [], set()
for n in amap:
    if n in gdef or n in fdef: continue          # the module has its own definition (it wins, as with ld)
    if n not in decl_line: continue              # unreferenced
    base, off = resolve(n)
    if base in gdef:
        aliases.append("@%s = alias i8, getelementptr (i8, ptr @%s, i32 %d)" % (n, base, off) if off else "@%s = alias i8, ptr @%s" % (n, base))
        drop.add(decl_line[n])
    elif off == 0:
        rename[n] = base; drop.add(decl_line[n])
    else:
        sys.stderr.write("apply_aliases: %s = %s + %d: base not defined in the module\n" % (n, base, off))
# a rename target that is not declared anywhere (a libc name nobody else calls): declare it as the dropped one was
need = {}
for n, t in rename.items():
    if t not in decl_line and t not in fdef and t not in gdef: need.setdefault(t, decl_line[n])
out = [l for i, l in enumerate(lines) if i not in drop]
for t, i in need.items():
    out.append(re.sub(r'@"?[^"(\s]+"?(?=\()', "@" + t, lines[i], count=1) if lines[i].startswith("declare") else
               re.sub(r'^@"?[^"\s=]+"?', "@" + t, lines[i]))
out += aliases
pat = re.compile(r'@([A-Za-z_][\w$]*)(?![\w.$])')
res = pat.sub(lambda m: "@" + rename[m.group(1)] if m.group(1) in rename else m.group(0), "\n".join(out))
open(out_path, "w").write(res)
print("apply_aliases: %d renames, %d data aliases, %d new declarations" % (len(rename), len(aliases), len(need)))
