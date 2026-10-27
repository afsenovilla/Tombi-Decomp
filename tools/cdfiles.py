#!/usr/bin/env python3
"""The file table inside the MAINn executables (EXE/MAIN0..8.EXE, game/MAIN0.EXE): dump, check, update. Format: notes/cdfiles.md.

The game does not read the ISO directory: it seeks to (LBA, size) taken from an embedded table of 8-byte entries,
`CdlLOC(min, sec, frame, 0)` in BCD (LBA + 150) followed by a u32 size (bytes; for MOVIE/*.STR sectors * 2336).
The table starts at file offset 0x6E6F0 (vaddr 0x8007DEF0) in every MAINn; each executable has its own order and count.
SCES_013.31 has no table (it finds EXE/MAINn.EXE by name).

  tools/cdfiles.py dump EXE [--ref IMAGE]
        prints index, address, LBA, size and the resolved file name of every entry
  tools/cdfiles.py check IMAGE EXE... [--ref IMAGE] [--names EXE|DIR]
        verifies that every entry equals the LBA/size of that file in IMAGE (a raw .bin or a layout .json); exit 1 if not
  tools/cdfiles.py update EXE LAYOUT [-o OUT] [--ref IMAGE] [--names EXE]
        rewrites the LBA/size of every entry from LAYOUT (image or .json {"path": [lba, size]}); in place without -o

Names are resolved by matching entries against the layout of the original image (--ref, default game/disc/Tombi.bin).
If the executable was already patched, give --names with a pristine copy of it (a file, or a directory with the same
basename); mkdisc.py --relayout uses the executables of its dump of the original disc.
"""
import argparse, json, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REF = os.path.join(ROOT, "game", "disc", "Tombi.bin")
TABLE_OFF = 0x6E6F0          # file offset of entry 0 (header 0x800, load address 0x80010000)
TABLE_VADDR = 0x80010000 + TABLE_OFF - 0x800


def layout_from_image(path):
    """{name: (lba, size)} in directory order, read from the ISO9660 directory of a raw (2352) or cooked (2048) image."""
    f = open(path, "rb")
    f.seek(16 * 2352 + 25)
    raw = f.read(5) == b"CD001"
    ss, uo = (2352, 24) if raw else (2048, 0)

    def sec(n, cnt=1):
        b = []
        for i in range(cnt):
            f.seek((n + i) * ss + uo)
            b.append(f.read(2048))
        return b"".join(b)

    pvd = sec(16)
    if pvd[1:6] != b"CD001":
        f.close()
        sys.exit("%s: not an ISO9660 image" % path)
    out = {}

    def walk(lba, size, pre):
        data = sec(lba, (size + 2047) // 2048)
        p = 0
        while p < size:
            n = data[p]
            if n == 0:
                p = (p // 2048 + 1) * 2048
                continue
            r = data[p:p + n]
            p += n
            elba, esz = struct.unpack("<I", r[2:6])[0], struct.unpack("<I", r[10:14])[0]
            nm = r[33:33 + r[32]]
            if nm in (b"\0", b"\1"):
                continue
            nm = nm.decode()
            if r[25] & 2:
                walk(elba, esz, pre + nm + "/")
            else:
                out[pre + nm.split(";")[0]] = (elba, esz)

    root = pvd[156:190]
    try:
        walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "")
    finally:
        f.close()   # Windows: a handle left open keeps mkpsxiso from rewriting the image in the relayout pass
    return out


def load_layout(path):
    if path.endswith(".json"):
        return {k: tuple(v) for k, v in json.load(open(path)).items()}
    return layout_from_image(path)


def tsize(name, size):
    """Size as stored in the table: bytes, except MOVIE/*.STR = sectors * 2336 (raw sector payload)."""
    return size // 2048 * 2336 if name.startswith("MOVIE/") and name.endswith(".STR") else size


def bcd(x):
    return (x // 10) << 4 | x % 10


def unbcd(x):
    return (x >> 4) * 10 + (x & 15)


def enc_loc(lba):
    a = lba + 150
    return bytes([bcd(a // 4500), bcd(a // 75 % 60), bcd(a % 75), 0])


def dec_loc(b):
    if b[3] or any((x & 15) > 9 or (x >> 4) > 9 for x in b[:3]):
        return None
    return (unbcd(b[0]) * 60 + unbcd(b[1])) * 75 + unbcd(b[2]) - 150


def read_exe(path):
    d = open(path, "rb").read()
    if d[:8] != b"PS-X EXE":
        sys.exit("%s: not a PS-X EXE" % path)
    return bytearray(d)


def table_start(d, known_lbas):
    """TABLE_OFF, or earlier when valid entries precede it (MAIN5 is linked 0x18 bytes earlier: its table starts at 0x6E6D8)."""
    o = TABLE_OFF
    while o - 8 >= 0 and dec_loc(d[o - 8:o - 4]) in known_lbas:
        o -= 8
    return o


def entries(d, known_lbas, limit=None):
    """[(offset, lba, size)] from the table start while the location is a LBA known to some layout."""
    out, o = [], table_start(d, known_lbas)
    while o + 8 <= len(d) and (limit is None or len(out) < limit):
        lba = dec_loc(d[o:o + 4])
        if lba is None or lba not in known_lbas:
            break
        out.append((o, lba, struct.unpack("<I", d[o + 4:o + 8])[0]))
        o += 8
    return out


def resolve(d, ref, extra=()):
    """Names of the entries of executable bytes d: [(offset, lba, size, name or None)] (matching ref, else extra layouts)."""
    byk = {}
    for lay in (ref,) + tuple(extra):
        for n, (l, s) in lay.items():
            byk.setdefault((l, tsize(n, s)), n)
    known = {l for lay in (ref,) + tuple(extra) for (l, s) in lay.values()}
    return [(o, l, s, byk.get((l, s))) for o, l, s in entries(d, known)]


def names_source(args, exe):
    p = args.names
    if p and os.path.isdir(p):
        p = os.path.join(p, os.path.basename(exe))
    return read_exe(p) if p else read_exe(exe)


def cmd_dump(a):
    ref = layout_from_image(a.ref)
    d = read_exe(a.exe)
    rows = resolve(d, ref)
    start = rows[0][0] if rows else TABLE_OFF
    print("# %s: table at file 0x%X (vaddr 0x%08X), %d entries" % (a.exe, start, TABLE_VADDR + start - TABLE_OFF, len(rows)))
    print("# idx  vaddr       lba      size      name")
    for i, (o, l, s, n) in enumerate(rows):
        print("%4d  %08X  %7d  %9d  %s" % (i, TABLE_VADDR + o - TABLE_OFF, l, s, n or "?"))
    return 0


def cmd_check(a):
    ref = layout_from_image(a.ref)
    lay = load_layout(a.image)
    bad_total = 0
    for exe in a.exe:
        d = read_exe(exe)
        names = resolve(names_source(a, exe), ref)
        # the count comes from the pristine table; the entries after it are not table
        cur = entries(d, {l for (l, s) in ref.values()} | {l for (l, s) in lay.values()}, len(names))
        bad = []
        if len(cur) != len(names):
            bad.append("entry count %d, expected %d" % (len(cur), len(names)))
        for i, ((o, l, s), (_, _, _, n)) in enumerate(zip(cur, names)):
            if n is None:
                bad.append("entry %d: unresolved (LBA %d, size %d)" % (i, l, s))
            elif n not in lay:
                bad.append("entry %d: %s not in the layout" % (i, n))
            elif (l, s) != (lay[n][0], tsize(n, lay[n][1])):
                bad.append("entry %d %s: table LBA %d size %d, layout LBA %d size %d"
                           % (i, n, l, s, lay[n][0], tsize(n, lay[n][1])))
        print("%s: %d entries, %s" % (exe, len(cur), "all match" if not bad else "%d MISMATCH" % len(bad)))
        for b in bad[:20]:
            print("   " + b)
        bad_total += len(bad)
    return 1 if bad_total else 0


def update_bytes(exe_bytes, names_bytes, ref, lay):
    """Returns (new bytes, entries changed)."""
    d = bytearray(exe_bytes)
    names = resolve(names_bytes, ref)
    changed = 0
    for o, l, s, n in names:
        if n is None:
            sys.exit("table entry at 0x%X (LBA %d, size %d) does not match any file of the reference image" % (o, l, s))
        if n not in lay:
            sys.exit("%s is referenced by the table but missing from the new layout" % n)
        new = enc_loc(lay[n][0]) + struct.pack("<I", tsize(n, lay[n][1]))
        if d[o:o + 8] != new:
            d[o:o + 8] = new
            changed += 1
    return bytes(d), changed


def cmd_update(a):
    ref = layout_from_image(a.ref)
    lay = load_layout(a.layout)
    d = read_exe(a.exe)
    new, ch = update_bytes(d, names_source(a, a.exe), ref, lay)
    out = a.output or a.exe
    if ch or a.output:
        open(out, "wb").write(new)
    print("%s: %d entries changed%s" % (out, ch, "" if ch or a.output else " (file untouched)"))
    return 0


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("dump"); s.add_argument("exe"); s.add_argument("--ref", default=REF)
    s = sub.add_parser("check"); s.add_argument("image"); s.add_argument("exe", nargs="+")
    s.add_argument("--ref", default=REF); s.add_argument("--names")
    s = sub.add_parser("update"); s.add_argument("exe"); s.add_argument("layout")
    s.add_argument("-o", "--output"); s.add_argument("--ref", default=REF); s.add_argument("--names")
    a = p.parse_args()
    return {"dump": cmd_dump, "check": cmd_check, "update": cmd_update}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
