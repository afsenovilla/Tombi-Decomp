#!/usr/bin/env python3
"""Data sections for the objdiff report (tools/build_report.py), so decomp.dev can show a data bar.

objdiff counts data per *target* section (kind data/bss): total_data += section size, matched_data += size only when
the section matches 100 %.  A target data section matches when every sized symbol in it matches the base symbol of
the same name: bytes equal and relocations equal.  So:

  - claimed data: a C object's data sections that tools/build_full.py would place at their retail address (string
    literals, jump tables, static tables reached through the section symbol; same analysis, build_full.analyse) are
    given to the unit on both sides:
      target: the retail bytes at that address, as a section of the same name, with one symbol D_<addr>;
      base:   the compiled section with its R_MIPS_32 relocations *applied* (addresses: the C object's own
              placed sections, or the extern symbol address inferred from the retail code like build_full does)
              and then dropped, plus the same symbol D_<addr>.
    objdiff then compares the final bytes of the C data with the retail bytes: matched only when they are equal.
  - all other bytes of each program that are not code (every unit of tools/units.py is code; everything else in the
    retail file: MAIN0 .rodata/.data, jump tables and tables inside the overlays, padding) go into one target-only
    unit per program, `<prog>/data`, which has no base and so counts as unmatched.
  So total_data = the whole non-code part of every reported program, matched_data = the bytes C really reproduces.
  .bss is not counted (it is not in the retail files).
"""
import os, shutil, struct, subprocess, tempfile

import build_full as BF

DATA_SECS = (".rodata", ".rdata", ".data", ".sdata")


def claim(src, obj, addr, size, game, vram, hdr):
    """Runs build_full's placement analysis on a compiled unit. Returns (elf, info) or None when it cannot be placed.
    info: {sec: address} of its data sections placed at a retail address, plus the resolved extern symbols."""
    u = dict(src=src, obj=obj, addr=addr, size=size)
    import io, contextlib
    with contextlib.redirect_stdout(io.StringIO()):
        ok = BF.analyse(u, game, vram, hdr)
    if not ok: return None
    place = {s: a for s, a in u["place"].items() if s in DATA_SECS and u["elf"].size(s)
             and u["elf"].sh[u["elf"].idx[s]][1] != 8}
    if not place: return None
    syms = {n: next(iter(v)) for n, v in u["syms"].items() if len(v) == 1}
    return dict(place=place, all_place=u["place"], syms=syms)


def patch_base(obj, out, info):
    """Copy of the base object with the placed data sections relocated (R_MIPS_32 applied, REL entries dropped) and a
    symbol D_<addr> covering each. Returns {sec: (addr, size)}."""
    e = BF.Elf(obj)
    res = {}
    for sec, a in info["place"].items():
        idx = e.idx[sec]
        s = e.sh[idx]
        rel = e.relocs(sec)
        done = True
        for off, ty, sym in rel:
            if ty != 2: done = False; continue
            if sym["shndx"] == 0:
                base = info["syms"].get(sym["name"])
            elif sym["shndx"] < len(e.names) and e.names[sym["shndx"]] in info["all_place"]:
                base = info["all_place"][e.names[sym["shndx"]]] + (sym["value"] if sym["type"] != 3 else 0)
            else:
                base = None
            if base is None: base = info["syms"].get(sym["name"])
            if base is None: done = False; continue
            v = (BF.W(e.d, s[4] + off) + base) & 0xFFFFFFFF
            struct.pack_into("<I", e.d, s[4] + off, v)
        if done:  # every relocation applied: drop the REL section's entries
            for i, r in enumerate(e.sh):
                if r[1] == 9 and r[7] == idx:
                    r[5] = 0
                    struct.pack_into("<10I", e.d, e.shoff + 40 * i, *r)
        res[sec] = (a, s[5])
    e.save(out)
    args = ["mipsel-linux-gnu-objcopy"]
    for sec, (a, n) in res.items():
        args += ["--add-symbol", "D_%08X=%s:0,global,object" % (a, sec)]
    subprocess.run(args + [out], check=True, capture_output=True)
    e = BF.Elf(out)  # objcopy cannot set a size
    for y in e.syms:
        for sec, (a, n) in res.items():
            if y["name"] == "D_%08X" % a:
                struct.pack_into("<I", e.d, y["off"] + 8, n)
    e.save(out)
    return res


def target_asm(claims, game, vram, hdr, tmpdir):
    """Assembly appended to a unit's target .s: the retail bytes of each claimed section."""
    out = []
    for sec, (a, n) in sorted(claims.items()):
        b = os.path.join(tmpdir, "D_%08X.bin" % a)
        open(b, "wb").write(game[hdr + a - vram:hdr + a - vram + n])
        out.append('\n.section %s, "a"\n.globl D_%08X\n.type D_%08X, @object\n.size D_%08X, %d\nD_%08X:\n.incbin "%s"\n'
                   % (sec, a, a, a, n, a, b))
    return "".join(out)


def rest_object(out, game, vram, hdr, code, claimed, AS):
    """Target-only object of the non-code bytes nobody claims: one .data section. Returns its size."""
    end = vram + len(game) - hdr
    taken = sorted(code + claimed)
    gaps, pos = [], vram
    for a, b in taken:
        if a > pos: gaps.append((pos, a))
        pos = max(pos, b)
    if pos < end: gaps.append((pos, end))
    blob = b"".join(game[hdr + a - vram:hdr + b - vram] for a, b in gaps)
    if not blob: return 0
    d = tempfile.mkdtemp()
    try:
        open(d + "/d.bin", "wb").write(blob)
        open(d + "/d.s", "w").write('.section .data, "aw"\n.incbin "%s/d.bin"\n' % d)
        os.makedirs(os.path.dirname(out), exist_ok=True)
        subprocess.run(AS + [d + "/d.s", "-o", out], check=True, capture_output=True)
    finally:
        shutil.rmtree(d, ignore_errors=True)
    return len(blob)
