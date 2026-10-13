#!/usr/bin/env python3
"""Layout of the event titles (CFNT999*.WFM, 24 px font): where each letter is drawn, found in the code overlays.

The WFM only holds the letters of a title (e.g. "Pulseradelabuelo"). The gaps between words and the line breaks are NOT in it:
they are in the overlays (AREAxx/X?NN.BIN, AREAxx/I?NFO*.BIN, SYSTEM/...; `?` = language variant 0..8, MAINn), one list of
4-byte records per title, one record per letter, in the order of the letters:

    u8 0 (unused), s8 col, s8 row, u8 flags

The engine (`spawnEventTitle`, FUN_8005aa74) puts letter i at x = col * 10, y = row * 14 (relative to the title origin) and
uses flags & 0xF as an index into two small tables (animation timing / colour group); bit 7 marks the first 11 letters of a
title that appear one after another (0x80 | i). A list ends with a record whose first byte is 0xFF. The lists of the
dialogues 2.. of the CFNT file are stored one after another (dialogue k = list k - 2), in every language the same way; the
event tables of the overlay point INTO them (at record 10 of each title: the first 10 letters are drawn from the records before
the pointer), so a list may be rewritten in place but not moved or made longer. Dialogues 0 and 1 (the "locked" titles) have none.

Geometry measured on all nine languages: letters are 2 columns (20 px) apart, a space adds 1 column (10 px), a line break goes
2 rows (28 px) down... lines are centred on column 0.
"""
import os, struct, sys

MIN_CHAIN = 8        # consecutive lists that must agree with the dialogue lengths to accept a block
FIRST = 2            # first dialogue that has a list


def parse_list(data, o):
    """Records of the list at byte offset o, or None if it does not look like one (no 0xFF terminator within 64 records)."""
    recs = []
    n = len(data)
    while o + 4 <= n:
        if data[o] == 0xFF:
            return recs
        if data[o] != 0 or len(recs) > 64:
            return None
        recs.append(struct.unpack_from("<bbbB", data, o)[1:])
        o += 4
    return None


def find_blocks(data, counts):
    """Offsets where the lists of the dialogues FIRST.. (lengths `counts`, from FIRST on) are stored one after another.
    Returns [(offset, number of lists that agree)] (usually one per overlay)."""
    found, o, n = [], 0, len(data)
    c0 = counts[0]
    while o + 4 * (c0 + 1) <= n:
        if data[o] == 0 and data[o + 4 * c0] == 0xFF:
            p, k = o, 0
            while k < len(counts):
                r = parse_list(data, p)
                if r is None or len(r) != counts[k]:
                    break
                p += 4 * (len(r) + 1)
                k += 1
            if k >= MIN_CHAIN:
                found.append((o, k))
                o = p
                continue
        o += 4
    return found


def read_lists(data, off, counts):
    """[(offset, [(col, row, flags)])] for the dialogues FIRST.. that agree with `counts`."""
    out, p = [], off
    for c in counts:
        r = parse_list(data, p)
        if r is None or len(r) != c:
            break
        out.append((p, r))
        p += 4 * (c + 1)
    return out


def counts_of(wfm):
    return [len(d.words) for d in wfm.dialogues[FIRST:]]


def all_lists(data, off):
    """Every list stored from `off` on, one after another, while they parse (the block is followed by other data)."""
    out, p = [], off
    while True:
        r = parse_list(data, p)
        if r is None or not r:
            break
        out.append((p, r))
        p += 4 * (len(r) + 1)
    return out


def align(counts, lists):
    """Order-preserving match of lists to dialogues by length (counts[i] for dialogue FIRST + i): {dialogue index: list index}.
    Longest common subsequence, so a title without a list is skipped instead of shifting all that follow."""
    n, m = len(counts), len(lists)
    best = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(n - 1, -1, -1):
        for j in range(m - 1, -1, -1):
            best[i][j] = max(best[i + 1][j], best[i][j + 1],
                             best[i + 1][j + 1] + 1 if counts[i] == len(lists[j][1]) else 0)
    out, i, j = {}, 0, 0
    while i < n and j < m:
        if counts[i] == len(lists[j][1]) and best[i][j] == best[i + 1][j + 1] + 1:
            out[FIRST + i] = j
            i += 1
            j += 1
        elif best[i + 1][j] >= best[i][j + 1]:
            i += 1
        else:
            j += 1
    return out


# ---- geometry: text <-> records --------------------------------------------------------------------------------------

STEP, SPACE, ROW = 2, 1, 2     # columns per letter, extra columns per space, rows per line (rows are 14 px, columns 10 px)


def records_to_lines(recs):
    """[[letter counts and gaps]] of a list: per line a list of 0 (letter) / 1.. (spaces before the letter), plus None when the
    spacing is not a multiple of the standard step (the caller then keeps the original records)."""
    lines, cur, prev = [], [], None
    for col, row, _ in recs:
        if prev is not None and row != prev[1]:
            lines.append(cur)
            cur, prev = [], None
        if prev is None:
            cur.append(0)
        else:
            d = col - prev[0] - STEP
            if d < 0 or d % SPACE:
                return None
            cur.append(d // SPACE)
        prev = (col, row)
    lines.append(cur)
    return lines


def lines_to_records(lines, flags=None):
    """Records for lines given as in records_to_lines (gap before each letter; the first one is ignored). Flags come from `flags`
    (one per letter, e.g. the originals) or are 0."""
    recs, k, n = [], 0, len(lines)
    for li, gaps in enumerate(lines):
        row = -ROW * (n - 1 - li)
        width = STEP * len(gaps) + SPACE * sum(gaps[1:])
        col = -(width // 2)
        for j, g in enumerate(gaps):
            if j:
                col += SPACE * g
            recs.append((col, row, flags[k] if flags else 0))
            col += STEP
            k += 1
    return recs


# ---- text <-> lines ---------------------------------------------------------------------------------------------------

import re

LANG_N = "BFGIHSPWD"       # language letter of the CFNT/MFNT files -> digit n of MAINn / X?NN / I?NFO (EXE/MAINn.EXE, tools/cdfiles.py)
_ATOM = re.compile(r"\{[A-Za-z0-9]+(?::\d+| +-?\d+)*\}| |[^\r\n]")


def is_title_file(path):
    return os.path.basename(path).upper().startswith("CFNT999")


def text_to_lines(text):
    """(lines, letters) of a title text: a letter is any character or {G:n}/{XXXX} tag; ' ' is a gap of one column and {NL} a line
    break (real line feeds are cosmetic). lines = [[gap before each letter]] as records_to_lines gives them; leading, trailing and
    empty lines are dropped."""
    lines, cur, gap, n = [], [], 0, 0
    for m in _ATOM.finditer(text):
        a = m.group(0)
        if a == " ":
            gap += 1
        elif a.upper() == "{NL}":
            if cur:
                lines.append(cur)
            cur, gap = [], 0
        else:
            cur.append(gap if cur else 0)
            gap = 0
            n += 1
    if cur:
        lines.append(cur)
    return lines, n


def lines_to_text(tokens, lines):
    """Text of a title: the letter tokens (one per glyph) laid out as `lines` says."""
    out, k = [], 0
    for li, gaps in enumerate(lines):
        if li:
            out.append("{NL}\n")
        for j, g in enumerate(gaps):
            out.append(" " * g if j else "")
            out.append(tokens[k])
            k += 1
    return "".join(out)


# ---- the layout of one language in a disc -----------------------------------------------------------------------------

class Layout:
    """Lists of one language: .recs[dialogue] = [(col, row, flags)], .where = (file, offset) they were read from."""

    def __init__(self, counts, data, off, name):
        self.counts, self.name, self.off = counts, name, off
        self.lists = all_lists(data, off)
        self.at = align(counts, self.lists)            # dialogue -> index in self.lists
        self.recs = {k: self.lists[j][1] for k, j in self.at.items()}

    def lines(self, k):
        r = self.recs.get(k)
        return records_to_lines(r) if r else None


def overlay_names(names, n):
    """Overlay files of language n among the file names of a disc (AREAxx/X?NN.BIN, I?NFO*.BIN, SYSTEM/...)."""
    rx = re.compile(r"(AREA\d\d|SYSTEM)/[XI]%d[A-Z0-9]*\.BIN$" % n, re.I)
    return sorted(x for x in names if rx.search(x))


def load_layout(read, names, path, wfm):
    """Layout for the CFNT file `path` (AREA19/CFNT999S.WFM) or None. read(name) -> bytes; names = every file name of the disc."""
    lang = os.path.splitext(os.path.basename(path))[0][-1:].upper()
    if lang not in LANG_N or len(wfm.dialogues) <= FIRST:
        return None
    n = LANG_N.index(lang)
    counts = counts_of(wfm)
    area = path.split("/")[0]
    cands = overlay_names(names, n)
    pref = [c for c in cands if c.upper().startswith(area.upper() + "/I")] + \
           [c for c in cands if c.upper().startswith("SYSTEM/I")] + cands
    for name in pref:
        try:
            data = read(name)
        except (OSError, KeyError):
            continue
        for off, k in find_blocks(data, counts):
            lay = Layout(counts, data, off, name)
            if len(lay.recs) >= MIN_CHAIN:
                return lay
    return None


def patch_files(read, names, wfm, lang, new):
    """Writes the new records ({dialogue: [(col, row, flags)]}, same lengths as the original lists) into every overlay of the language
    that holds the lists. Returns {file name: bytes} of the changed files."""
    n = LANG_N.index(lang)
    counts = counts_of(wfm)
    out = {}
    for name in overlay_names(names, n):
        try:
            data = read(name)
        except (OSError, KeyError):
            continue
        blocks = find_blocks(data, counts)
        if not blocks:
            continue
        buf = bytearray(data)
        lay = Layout(counts, data, blocks[0][0], name)
        for k, recs in new.items():
            j = lay.at.get(k)
            if j is None or len(lay.lists[j][1]) != len(recs):
                continue
            o = lay.lists[j][0]
            for c, r, f in recs:
                buf[o:o + 4] = struct.pack("<Bbbb", 0, c, r, 0)[:3] + bytes([f & 0xFF])
                o += 4
        if bytes(buf) != data:
            out[name] = bytes(buf)
    return out


def check(root):
    """Every CFNT file of a disc folder: layout found, text round trip with the layout (python3 tools/title_layout.py [FS_DIR])."""
    import text_core as C, wfm_py as W
    p = C.Project(C.Source(root))
    p.langs = set(C.LANG_NAMES)
    files = [n for n in p.src.names() if is_title_file(n)]
    ok = bad = missing = 0
    for n in files:
        lay = p.layout(n)
        if lay is None:
            missing += 1
            continue
        w = p.wfm(n)
        for i, d in enumerate(w.dialogues):
            t = p.original(n, i)
            good = p.words(n, i, t)[0] == d.words and (i not in lay.recs or p.title_problem(n, i, t) is None)
            ok += good
            bad += not good
    print("%d CFNT files, %d without layout, %d dialogues ok, %d bad" % (len(files), missing, ok, bad))
    return 0 if not bad and not missing else 1


if __name__ == "__main__":
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    sys.exit(check(sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "game", "disc", "fs")))
