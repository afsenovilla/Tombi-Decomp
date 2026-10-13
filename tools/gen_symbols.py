#!/usr/bin/env python3
"""Generates the splat symbol files (config/symbol_addrs_<prog>.txt) from the unit list of tools/units.py:
  - one sized symbol per src file with a `// MATCHING` marker, named after the C (or asm) function it defines,
    so objdiff pairs the retail function with ours by name; when the compiled object of that file exists in
    build/base/<prog>/<stem>.o and holds several functions (a file matched as a block), each inner function gets
    its own symbol too, so splat cuts the retail code exactly where our object does;
  - one sized symbol per csv row (or remainder of a row) that no file covers, with the csv name.
Library and twin units get symbols too (splat needs them to disassemble the code sanely); only the counting
differs.  Ghidra fragments (zero first word, shorter than 8 bytes) are already merged away by units.py: with a
symbol there splat emits the rest of the program as .word.

Usage: python3 tools/gen_symbols.py"""
import os, re, subprocess, sys
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(R, "tools"))
import units as U

NM = os.environ.get("MIPS_NM", "mipsel-linux-gnu-nm")


def inner_functions(obj):
    """[(offset, size, name)] of the function symbols of a compiled object, from nm."""
    try:
        out = subprocess.run([NM, "-S", "--defined-only", obj], capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError):
        return []
    fs = []
    for line in out.splitlines():
        m = re.match(r"([0-9a-f]+)\s+([0-9a-f]+)\s+[Tt]\s+(\S+)", line)
        if m and m.group(3) not in ("gcc2_compiled.", "__gnu_compiled_c"):
            fs.append((int(m.group(1), 16), int(m.group(2), 16), m.group(3)))
    return sorted(fs)


def main():
    ignored = {}   # config/symbol_ignore.txt: constants splat must not symbolize (see the file)
    for line in open(os.path.join(R, "config", "symbol_ignore.txt")):
        m = re.match(r"\s*(\w+)\s+0x([0-9A-Fa-f]+)", line)
        if m: ignored.setdefault(m.group(1).upper(), []).append(int(m.group(2), 16))
    for prog, us in U.units().items():
        syms = []  # (addr, name, size)
        for u in us:
            if u.file:
                name = U.defined_name(u.file, u.name)
                obj = os.path.join(R, "build", "base", prog.lower(), u.name + ".o")
                inner = inner_functions(obj) if os.path.exists(obj) else []
                if len(inner) > 1 and inner[-1][0] + inner[-1][1] <= u.size:
                    for off, size, nm in inner:
                        syms.append((u.addr + off, nm, size))
                    continue
                syms.append((u.addr, name, u.size))
            else:
                syms.append((u.addr, u.name, u.size))
        seen = {}
        with open(os.path.join(R, "config", "symbol_addrs_%s.txt" % prog.lower()), "w") as out:
            for a, n, size in sorted(syms):
                if n in seen: n = "%s_%08X" % (n, a)     # splat needs unique names
                seen[n] = a
                out.write("%s = 0x%08X; // type:func size:0x%X\n" % (n, a, size))
            for a in ignored.get(prog, []):
                out.write("D_%08X = 0x%08X; // ignore:true\n" % (a, a))
        print(prog, len(syms), "symbols")


if __name__ == "__main__":
    main()
