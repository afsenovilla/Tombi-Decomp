#!/usr/bin/env python3
"""Tombi! (PAL) `*.000` resource files: info, unpack, pack, check.  Format notes: notes/formats.md (section "*.000").

The `.000` files are the UNCOMPRESSED twins of the `.GAM` resources: the same load records (SYS/LD?AR??.BIN, see
include/tomba24/game/cdload.h) that read a `.GAM` with LOAD_TYPE_LZ_* read a `.000` with LOAD_TYPE_STAGING/RAW/RAW_4.
There is no common header; what a file is depends on the *kind* of the load record that names it:

  kind 0x10 GRPX   raw VRAM rectangle (16-bit words, w x h, sent with LoadImage): textures, 4/8/16 bpp pages, CLUTs (512 B)
  kind 0x50/0x60.. u32 table container (WPP packed sprite frames 0x2E008 B, 0x62 tables, WMD 3D models 0x50, ...)
  kind 0x8e APD    2D byte grid: u16 width, u16 height, width*height cells, zero padding
  anything else    kept as one opaque `data.bin`

  tools/area000.py info FILE...            structure of each file (kind, header, sections)
  tools/area000.py unpack FILE OUT_DIR [--png]   manifest.json + section files (+ PNG views of images and grids)
  tools/area000.py pack DIR [-o OUT]       rebuild the file from manifest.json + sections (byte-identical if unchanged)
  tools/area000.py --check [PATH...]       unpack+pack every *.000 under PATH (default game/disc/fs), print identical count

The classification uses the load records when game/disc/Tombi.bin and fs/SYS/LD0*.BIN exist (authoritative) and falls back to
content heuristics otherwise (a table container is recognised by its header, a grid by its size, anything else is raw).
PNG writing is pure Python (zlib). unpack/pack themselves do not need the disc image.
"""
import argparse, hashlib, json, os, struct, sys, zlib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FS = os.path.join(ROOT, "game", "disc", "fs")

KIND_NAMES = {0x10: "GRPX", 0x20: "NONE", 0x30: "SPR", 0x40: "FOUR", 0x50: "WMD", 0x60: "WPP", 0x70: "WHM", 0x80: "APD",
              0x90: "WVD", 0xA0: "UNKA", 0xB0: "SEQ", 0xC0: "WSS", 0xD0: "WFM3", 0xF0: "INS", 0xF8: "LDSYS"}
TYPE_NAMES = {0: "STAGING", 1: "LZ_TO_VRAM", 2: "RAW", 3: "LZ_TO_RAM", 4: "RAW_4"}


# ---------------------------------------------------------------- PNG (pure python)
def _chunk(tag, data):
    c = struct.pack(">I", len(data)) + tag + data
    return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)


def png_bytes(w, h, rows, mode):
    """rows: list of bytes. mode 'rgb' (3 B/px), 'gray' (1 B/px) or 'idx' (1 B/px, built-in 256 colour palette)."""
    ct = {"rgb": 2, "gray": 0, "idx": 3}[mode]
    raw = b"".join(b"\0" + r for r in rows)
    out = b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, ct, 0, 0, 0))
    if mode == "idx":
        pal = bytearray(768)
        for i in range(1, 256):  # 0 = black; other values spread over hues so grid values are distinguishable
            pal[i * 3:i * 3 + 3] = bytes(((i * 67) & 0xFF | 0x40, (i * 131) & 0xFF | 0x40, (i * 29) & 0xFF | 0x40))
        out += _chunk(b"PLTE", bytes(pal))
    return out + _chunk(b"IDAT", zlib.compress(raw, 9)) + _chunk(b"IEND", b"")


def vram_views(data, w, h):
    """{filename: png bytes}: 16 bpp (RGB555), 8 bpp gray and 4 bpp gray reading of a w x h (16-bit words) VRAM rectangle."""
    out = {}
    rows = []
    for y in range(h):
        line = bytearray()
        for (v,) in struct.iter_unpack("<H", data[y * w * 2:(y + 1) * w * 2]):
            line += bytes(((v & 31) * 255 // 31, (v >> 5 & 31) * 255 // 31, (v >> 10 & 31) * 255 // 31))
        rows.append(bytes(line))
    out["view16.png"] = png_bytes(w, h, rows, "rgb")
    out["view8.png"] = png_bytes(w * 2, h, [data[y * w * 2:(y + 1) * w * 2] for y in range(h)], "gray")
    rows = []
    for y in range(h):
        line = bytearray()
        for b in data[y * w * 2:(y + 1) * w * 2]:
            line += bytes(((b & 15) * 17, (b >> 4) * 17))
        rows.append(bytes(line))
    out["view4.png"] = png_bytes(w * 4, h, rows, "gray")
    return out


# ---------------------------------------------------------------- load records (optional, authoritative)
_REC = None


def load_records(fs=FS):
    """{'AREA05/D003.000': (record file, kind byte, type, arg, w, h)} read from SYS/LD0*.BIN; {} if the disc image is missing."""
    global _REC
    if _REC is not None:
        return _REC
    _REC = {}
    try:
        sys.path.insert(0, os.path.join(ROOT, "tools"))
        import cdfiles
        ref = cdfiles.layout_from_image(os.path.join(os.path.dirname(fs), "Tombi.bin"))
        rows = cdfiles.resolve(cdfiles.read_exe(os.path.join(fs, "EXE", "MAIN0.EXE")), ref)
        names = {i: r[3] for i, r in enumerate(rows) if r[3]}
        sysdir = os.path.join(fs, "SYS")
        for fn in sorted(os.listdir(sysdir)):
            if not fn.startswith("LD0"):
                continue
            d = open(os.path.join(sysdir, fn), "rb").read()

            def rec(o):
                if o < 0 or o + 20 > len(d):
                    return None
                return struct.unpack_from("<hBBIIhhI", d, o)

            def plausible(r):
                return r is not None and (r[0] == -1 or r[0] in names) and ((r[2] & 0xF0) in KIND_NAMES or r[2] == 0)

            for o in range(0, len(d) - 20, 4):
                r = rec(o)
                n = names.get(r[0], "") if r else ""
                if not n.endswith(".000") or r[7] not in (0, 1, 2, 3, 4, 0x10, 0x11, 0x12, 0x13, 0xFFFFFFFF):
                    continue
                if not (plausible(rec(o - 20)) or plausible(rec(o + 20))):
                    continue
                kind = r[2] & 0xF0
                if kind not in KIND_NAMES or not (r[3] == 0 or r[3] >> 24 == 0x80):
                    continue
                path = os.path.join(fs, n)
                size = os.path.getsize(path) if os.path.exists(path) else None
                if kind == 0x10 and (r[5] * r[6] * 2 != size or r[5] == 0):
                    continue
                if kind != 0x10 and (r[5] or r[6]):
                    continue
                _REC.setdefault(n, (fn, r[2], r[7], r[4], r[5], r[6]))
    except Exception as e:  # no disc image: heuristics only
        sys.stderr.write("area000: load records unavailable (%s), using heuristics\n" % e)
    return _REC


def rel_name(path):
    p = os.path.abspath(path).replace(os.sep, "/").split("/")
    return "/".join(p[-2:])


# ---------------------------------------------------------------- classification
def parse_table(d):
    """u32 table container: [N][offsets...]; first offset = header size = 4*(N+1) (no end offset) or 4*(N+2) (end offset).
    Returns (N, hdr, bounds, has_end) with item i = d[bounds[i]:bounds[i+1]], or None."""
    if len(d) < 12:
        return None
    n, hdr = struct.unpack_from("<II", d, 0)
    if not (1 <= n <= 4096) or hdr % 4 or hdr not in (4 * (n + 1), 4 * (n + 2)) or hdr > len(d):
        return None
    cnt = hdr // 4 - 1
    offs = list(struct.unpack_from("<%dI" % cnt, d, 4))
    has_end = hdr == 4 * (n + 2)
    end = offs.pop() if has_end else len(d)
    if end > len(d) or offs[0] != hdr or any(a > b for a, b in zip(offs, offs[1:] + [end])):
        return None
    return n, hdr, offs + [end], has_end


def parse_grid(d):
    """u16 w, u16 h, then w*h cells (the data may spill into one more row), the rest zero."""
    if len(d) < 16:
        return None
    w, h = struct.unpack_from("<HH", d, 0)
    if not (8 <= w <= 1024 and 8 <= h <= 1024) or 4 + w * h > len(d) or any(d[4 + w * (h + 1):]):
        return None
    return w, h


def classify(d, rec=None):
    """-> (kind, meta); kind is vram, table, grid or raw."""
    if rec and (rec[1] & 0xF0) == 0x10:
        return "vram", {"arg": rec[3], "w": rec[4], "h": rec[5]}
    if not rec or (rec[1] & 0xF0) in (0x50, 0x60, 0x70):
        t = parse_table(d)
        if t:
            return "table", {"n": t[0]}
    if not rec or (rec[1] & 0xF0) == 0x80:
        g = parse_grid(d)
        if g:
            return "grid", {"w": g[0], "h": g[1]}
    return "raw", {}


# ---------------------------------------------------------------- unpack / pack (pure, in memory)
def unpack_part(d, rec, png, pre):
    """One container -> (kind, meta, sections, files); section/file names get the prefix `pre`."""
    kind, meta = classify(d, rec)
    files, secs = {}, []

    def sec(fn, off, size, note=""):
        secs.append({"file": pre + fn, "off": off, "size": size, "note": note})
        files[pre + fn] = d[off:off + size]

    if kind == "table":
        n, hdr, bounds, has_end = parse_table(d)
        meta.update({"header_bytes": hdr, "end_offset_word": has_end})
        sec("header.bin", 0, hdr, "u32 count N, then item offsets%s" % (" + end offset" if has_end else ""))
        for i in range(len(bounds) - 1):
            sec("item_%04d.bin" % i, bounds[i], bounds[i + 1] - bounds[i])
        if bounds[-1] < len(d):
            sec("tail.bin", bounds[-1], len(d) - bounds[-1], "bytes after the last item")
    elif kind == "grid":
        w, h = meta["w"], meta["h"]
        sec("header.bin", 0, 4, "u16 width, u16 height")
        sec("grid.bin", 4, w * h, "width*height byte cells, row major")
        if len(d) > 4 + w * h:
            sec("pad.bin", 4 + w * h, len(d) - 4 - w * h, "padding to the buffer size (may hold one extra row)")
        if png:
            files[pre + "view.png"] = png_bytes(w, h, [d[4 + y * w:4 + (y + 1) * w] for y in range(h)], "idx")
    elif kind == "vram":
        sec("image.bin", 0, len(d), "raw VRAM words, %d x %d" % (meta["w"], meta["h"]))
        if png:
            files.update({pre + k: v for k, v in vram_views(d, meta["w"], meta["h"]).items()})
    else:
        sec("data.bin", 0, len(d), "opaque")
    return kind, meta, secs, files


def split_parts(d, rec):
    """Files read with LOAD_TYPE_RAW_4 (or followed by more records) hold several containers back to back; the load record
    `arg` is the size of the first one (0x2E008 for WPP, 0x4008 for an APD grid)."""
    if rec and (rec[1] & 0xF0) != 0x10 and 0 < rec[3] < len(d) and rec[3] % 8 == 0 and (rec[3] >> 24) == 0:
        return [(0, rec[3], rec), (rec[3], len(d) - rec[3], None)]
    return [(0, len(d), rec)]


def unpack_bytes(d, name="", rec=None, png=False):
    man = {"format": "area000", "name": name, "size": len(d), "sha1": hashlib.sha1(d).hexdigest(), "parts": []}
    if rec:
        man["load_record"] = {"file": rec[0], "kind": "0x%02X" % rec[1], "type": rec[2], "arg": "0x%X" % rec[3]}
    files = {}
    for i, (off, size, r) in enumerate(split_parts(d, rec)):
        pre = "" if i == 0 else "p%d_" % i
        kind, meta, secs, f = unpack_part(d[off:off + size], r, png, pre)
        for s in secs:
            s["off"] += off
        man["parts"].append({"kind": kind, "meta": meta, "off": off, "size": size, "sections": secs})
        files.update(f)
    man["kind"] = "+".join(p["kind"] for p in man["parts"])
    return man, files


def pack_part(part, files):
    secs = part["sections"]
    parts = [files[s["file"]] for s in secs]
    if part["kind"] == "table":
        m = part["meta"]
        tail = parts.pop() if secs[-1]["file"].endswith("tail.bin") else b""
        items = parts[1:]
        offs, o = [], m["header_bytes"]
        for it in items:
            offs.append(o)
            o += len(it)
        words = [m["n"]] + offs + ([o] if m["end_offset_word"] else [])
        header = struct.pack("<%dI" % len(words), *words)
        assert len(header) == m["header_bytes"], "header size changed"
        parts = [header] + items + ([tail] if tail else [])
    return b"".join(parts)


def pack_bytes(man, files):
    return b"".join(pack_part(p, files) for p in man["parts"])


def read_dir(dirpath):
    man = json.load(open(os.path.join(dirpath, "manifest.json")))
    return man, {s["file"]: open(os.path.join(dirpath, s["file"]), "rb").read() for p in man["parts"] for s in p["sections"]}


# ---------------------------------------------------------------- commands
def get_rec(nm):
    """Load record of a file; SYSTEM/A2..A9xxxx (variants for MAIN1..8, not in the MAIN0 table) use the A0/A1 twin's record."""
    r = load_records()
    if nm in r:
        return r[nm]
    if nm.startswith("SYSTEM/A") and len(nm) > 10 and nm[8] in "23456789":
        for v in "01":
            if nm[:8] + v + nm[9:] in r:
                return r[nm[:8] + v + nm[9:]]
    return None


def describe(p, d, off, part):
    m, k = part["meta"], part["kind"]
    secs = part["sections"]
    pre = "  " if off == 0 else "  [part at 0x%X, %d bytes] " % (off, part["size"])
    if k == "vram":
        print(pre + "VRAM rectangle x=%d y=%d, %d x %d words (= %d px at 16bpp, %d at 8bpp, %d at 4bpp)" % (
            m["arg"] & 0xFFFF, m["arg"] >> 16 & 0xFFFF, m["w"], m["h"], m["w"], m["w"] * 2, m["w"] * 4))
    elif k == "table":
        items = [s for s in secs if "item_" in s["file"]]
        sizes = [s["size"] for s in items]
        tail = secs[-1] if secs[-1]["file"].endswith("tail.bin") else None
        print(pre + "table: N=%d, header %d bytes, end-offset word %s, %d items of %d..%d bytes%s" % (
            m["n"], m["header_bytes"], "yes" if m["end_offset_word"] else "no", len(items), min(sizes), max(sizes),
            ", tail %d bytes" % tail["size"] if tail else ""))
        print(pre + "item 0 at 0x%X: %s" % (items[0]["off"], d[items[0]["off"]:items[0]["off"] + 32].hex(" ")))
    elif k == "grid":
        print(pre + "grid %d x %d cells (%d bytes), padding %d bytes" % (m["w"], m["h"], m["w"] * m["h"], part["size"] - 4 - m["w"] * m["h"]))
    else:
        print(pre + "opaque; first 32 bytes: %s" % d[off:off + 32].hex(" "))


def cmd_info(a):
    for p in a.files:
        d = open(p, "rb").read()
        nm = rel_name(p)
        rec = get_rec(nm)
        man, _ = unpack_bytes(d, nm, rec)
        print("%s  %d bytes (0x%X)  kind=%s" % (nm, len(d), len(d), man["kind"]))
        if rec:
            k = rec[1]
            print("  load record: %s kind=0x%02X (%s[%d]) type=%s arg=0x%X w=%d h=%d" % (
                rec[0], k, KIND_NAMES.get(k & 0xF0, "?"), k & 15, TYPE_NAMES.get(rec[2] & 15, rec[2]), rec[3], rec[4], rec[5]))
        else:
            print("  load record: none found (heuristic classification)")
        for part in man["parts"]:
            describe(p, d, part["off"], part)
    return 0


def cmd_unpack(a):
    d = open(a.file, "rb").read()
    nm = rel_name(a.file)
    man, files = unpack_bytes(d, nm, get_rec(nm), png=a.png)
    os.makedirs(a.out, exist_ok=True)
    for fn, b in files.items():
        open(os.path.join(a.out, fn), "wb").write(b)
    json.dump(man, open(os.path.join(a.out, "manifest.json"), "w"), indent=1)
    print("%s -> %s: kind=%s, %d files" % (a.file, a.out, man["kind"], len(files)))
    return 0


def cmd_pack(a):
    man, files = read_dir(a.dir)
    out = pack_bytes(man, files)
    same = hashlib.sha1(out).hexdigest() == man["sha1"]
    if a.o:
        open(a.o, "wb").write(out)
    print("packed %d bytes%s" % (len(out), " (identical to the original)" if same else " (differs from the original)"))
    return 0


def cmd_check(paths):
    todo = []
    for p in paths or [FS]:
        if os.path.isdir(p):
            for r, _, fs in os.walk(p):
                todo += [os.path.join(r, f) for f in fs if f.endswith(".000")]
        else:
            todo.append(p)
    todo.sort()
    ok, kinds, bad = 0, {}, []
    for p in todo:
        d = open(p, "rb").read()
        nm = rel_name(p)
        man, files = unpack_bytes(d, nm, get_rec(nm))
        kinds[man["kind"]] = kinds.get(man["kind"], 0) + 1
        if pack_bytes(json.loads(json.dumps(man)), files) == d:
            ok += 1
        else:
            bad.append(nm)
    print("identical %d/%d   kinds: %s" % (ok, len(todo), ", ".join("%s=%d" % kv for kv in sorted(kinds.items()))))
    for b in bad[:20]:
        print("  DIFF", b)
    return 0 if ok == len(todo) else 1


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--check":
        return cmd_check(sys.argv[2:])
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sp = ap.add_subparsers(dest="cmd", required=True)
    p = sp.add_parser("info"); p.add_argument("files", nargs="+"); p.set_defaults(f=cmd_info)
    p = sp.add_parser("unpack"); p.add_argument("file"); p.add_argument("out"); p.add_argument("--png", action="store_true"); p.set_defaults(f=cmd_unpack)
    p = sp.add_parser("pack"); p.add_argument("dir"); p.add_argument("-o"); p.set_defaults(f=cmd_pack)
    a = ap.parse_args()
    return a.f(a)


if __name__ == "__main__":
    sys.exit(main())
