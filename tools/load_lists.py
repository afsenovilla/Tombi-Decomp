#!/usr/bin/env python3
"""Keeps the area load lists (SYS/LDnARxx.BIN) in step with patched WFM files that changed size.

The game loads an area's files with queueLoadList (src/queueLoadList.c): every entry of a list is 20 bytes
  s16 file index (the position in MAINn's file table), u8 slot, u8 type, u32 ptr, u32 size, u32 pad, s32 flags
and the next file goes to base + size (rounded up to 4). The size is the list's own copy, not the file table's: a WFM that
grew (a borrowed glyph, longer texts) while its list entry kept the old size got its tail overwritten by the next file of
the list (AREA00: MFNT001S over the end of MFNT000S), so the last dialogues of the file (MFNT000S #172, the jump tutorial
sign) decoded as garbage ("UUUUUU" boxes) and the engine then ran into wild words and crashed.

WFM entries have type 0xD0+n (WFM3_DATA[n]). A list LDnARxx only loads files of AREAxx, so an entry is matched by the
area, the type and the old size; every match of one file must carry the same file index (else nothing is guessed).

  fix(read, names, files) -> {list path: new bytes}
  problems(read, names, sizes) -> [text]   entries of WFM files whose size differs from `sizes` ({path: size})
"""
import re
import struct

ENTRY = struct.Struct("<hBBIIIi")
LIST = re.compile(r"SYS/LD(\d)AR(\d\d)\.BIN", re.I)
AREA = re.compile(r"AREA(\d\d)/[^/]+\.WFM", re.I)


def lists(names):
    """{area number string: [list paths]}"""
    out = {}
    for n in sorted(names):
        m = LIST.fullmatch(n)
        if m:
            out.setdefault(m.group(2), []).append(n)
    return out


def _wfm_entries(data):
    for o in range(0, len(data) - ENTRY.size + 1, 4):
        i, slot, typ, ptr, size, pad, flags = ENTRY.unpack_from(data, o)
        if (typ & 0xf0) == 0xd0 and ptr == 0 and pad == 0 and i >= 0 and size > 0:
            yield o, i, slot, typ, ptr, size, pad, flags


def _matches(read, names, old_sizes):
    """{path: [(list, offset, entry tuple)]} for the WFM files of old_sizes ({path: size in the lists})."""
    by_list = lists(names)
    found = {}
    for p, old in old_sizes.items():
        m = AREA.fullmatch(p)
        if not m:
            continue
        for ln in by_list.get(m.group(1), []):
            for e in _wfm_entries(read(ln)):
                if e[5] == old:
                    found.setdefault(p, []).append((ln, e))
    # a size shared by two files of the same area would be ambiguous: keep only files whose matches agree on one index,
    # and drop indices claimed by two files
    owner = {}
    for p, ms in list(found.items()):
        ids = {e[1] for _, e in ms}
        if len(ids) != 1:
            del found[p]
            continue
        owner.setdefault(ids.pop(), []).append(p)
    for i, ps in owner.items():
        if len(ps) > 1:
            for p in ps:
                found.pop(p, None)
    return found


def fix(read, names, files):
    """read(path) -> pristine bytes; names: every path of the disc; files: {path: new bytes}.
    Returns {SYS/LDnARxx.BIN: new bytes} with the size of every changed WFM of `files` updated."""
    old = {}
    for p, data in files.items():
        if AREA.fullmatch(p):
            o = len(read(p))
            if o != len(data):
                old[p] = o
    out = {}
    for p, ms in _matches(read, names, old).items():
        new = len(files[p])
        for ln, (o, i, slot, typ, ptr, size, pad, flags) in ms:
            buf = bytearray(out.get(ln) or files.get(ln) or read(ln))
            ENTRY.pack_into(buf, o, i, slot, typ, ptr, new, pad, flags)
            out[ln] = bytes(buf)
    return out


def problems(read, names, sizes, pristine_sizes):
    """Entries still listing a WFM with its pristine size although the file now has another size."""
    changed = {p: s for p, s in pristine_sizes.items() if sizes.get(p, s) != s}
    return ["%s +0x%x: %s listed with %d bytes, the file has %d" % (ln, e[0], p, e[5], sizes[p])
            for p, ms in sorted(_matches(read, names, changed).items()) for ln, e in ms]
