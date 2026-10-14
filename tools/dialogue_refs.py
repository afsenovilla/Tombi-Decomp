#!/usr/bin/env python3
"""Static census of dialogue references (research tool, read-only).

WFM3 header bytes 16..143 are NOT reserved: they are 64 x u16 group bases. openMessageBox(k, dx, ...) shows dialogue
`base[k] + dx` (src/openMessageBox.c: `*(short*)(wfm + 0x10 + k*2) + dx`). Group 0 (base 0) = system/item/info text.

References found without running the game:
  * `jal` to showMessageBox / ...Timed / ...Direct / ...DirectTimed / openMessageBox in the area overlays (X0nn.BIN) whose
    a0 (k) and a1 (dx) are loaded with an immediate in the same basic block;
  * script op 0x85 (`05 nargs [01 imm32]* 85`) in AREAnn/*.WSS (args: obj, cmd=4, k, dx, ...).
Calls with computed k/dx (NPC objects whose ids come from room data, loops) are NOT seen, so "unreferenced" is only a
candidate list.   usage: dialogue_refs.py [--delta] [AREAnn/MFNTnn0S ...]  (default: all Spanish files)
The static scan is joined by literal (k, dx) pairs of the matched C sources (src_refs; tools/objects.py dialogue_consts).
"""
import glob, os, re, struct, sys, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FS = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "game", "disc", "fs")
TG = {0x8002DC50, 0x8002DC88, 0x8002DCC8, 0x8002DCF0, 0x8002DD1C}
# overlay of area nn -> its Spanish dialogue files (file names shared between areas: 07 uses 010, 12 uses 040, 19 uses 020+190, 13 uses 001+130)
FILES = {0: ["MFNT000S", "MFNT001S"], 1: ["MFNT010S"], 7: ["MFNT010S"], 12: ["MFNT040S"], 13: ["MFNT001S", "MFNT130S"], 19: ["MFNT020S", "MFNT190S"]}


def files_of(a):
    return FILES.get(a, ["MFNT%02d0S" % a])


def call_refs(path):
    d = open(path, "rb").read()
    n = len(d) // 4
    w = struct.unpack("<%dI" % n, d[:n * 4])
    out, unk = set(), 0
    for i, x in enumerate(w):
        if x >> 26 == 3 and (0x80000000 | ((x & 0x3FFFFFF) << 2)) in TG:
            r = {}
            for j in [i + 1] + list(range(i - 1, max(i - 40, 0), -1)):
                y = w[j]
                op, rs, rt = y >> 26, (y >> 21) & 31, (y >> 16) & 31
                if j < i and (op in (2, 3, 4, 5, 6, 7) or (op == 0 and (y & 0x3F) in (8, 9))):
                    break
                if op in (9, 13) and rs == 0 and rt in (4, 5) and rt not in r:
                    r[rt] = y & 0xFFFF
            if 4 in r and 5 in r:
                out.add((r[4], r[5]))
            else:
                unk += 1
    return out, unk


def src_refs(a):
    """Literal (k, dx) pairs in the matched C sources of overlay X0<a>: direct showMessageBox*/openMessageBox calls AND `m = K; n = DX;`
    assignments (NPC handlers set them per state and call once, which the basic-block scan of call_refs cannot see).  AREA00 = src/*.c
    functions headed `// FUNC .. X000`; the others live in src/x0NN/."""
    import objects
    out = set()
    if a == 0:
        files = [f for f in glob.glob(os.path.join(ROOT, "src", "*.c")) if open(f).readline().rstrip().endswith("X000")]
    else:
        files = glob.glob(os.path.join(ROOT, "src", "x%03d" % a, "*.c"))
    for f in files:
        out |= objects.dialogue_consts(open(f).read())
    return out


def wss_refs(path):
    d = open(path, "rb").read()
    out = set()
    for m in re.finditer(rb"\x05([\x01-\x08])", d):
        n, i, vals, ok = m.group(1)[0], m.end(), [], True
        for _ in range(n):
            if i >= len(d) or d[i] not in (0, 1):
                ok = False
                break
            if d[i] == 1:
                vals.append(struct.unpack("<I", d[i + 1:i + 5])[0]); i += 5
            else:
                vals.append(None); i += 2
        if ok and i < len(d) and d[i] == 0x85 and len(vals) >= 4 and vals[1] == 4 and vals[2] is not None and vals[3] is not None:
            if vals[3] < 0x8000:
                out.add((vals[2], vals[3]))
    return out


def main():
    want = set(a.split("/")[-1] for a in sys.argv[1:] if not a.startswith("--"))
    refs, unk = collections.defaultdict(set), collections.defaultdict(int)
    for p in sorted(glob.glob(FS + "/AREA*/X0??.BIN")):
        a = int(os.path.basename(p)[2:4])
        r, u = call_refs(p)
        r0 = set(r)
        r = r | src_refs(a)
        if "--delta" in sys.argv:
            print("X%03d: %d static-call refs + %d from sources = %d" % (a, len(r0), len(r - r0), len(r)))
        for f in files_of(a):
            refs[f] |= r
            unk[f] += u
    # WSS scenes (AREA15): scene prefix -> file (INFERRED from the numbering, not verified)
    for p in sorted(glob.glob(FS + "/AREA15/*.WSS")):
        b = os.path.basename(p)
        pre = 1 if b.startswith("SCR01") else int(b[:2])
        for f in files_of(pre)[:1]:
            refs[f] |= wss_refs(p)
    for f in sorted(refs):
        if want and f not in want:
            continue
        area = [p for p in glob.glob(FS + "/AREA*/" + f + ".WFM")][0]
        w = wfm_py.decode(open(area, "rb").read())
        base = list(struct.unpack("<64H", w.reserved))
        ng = max([i for i, x in enumerate(base) if x] + [0]) + 1
        size = [(base[g + 1] if g + 1 < ng else len(w.dialogues)) - base[g] for g in range(ng)]
        hit = set()
        for k, dx in refs[f]:
            if 0 < k < ng and dx < size[k]:
                hit.add(base[k] + dx)
        print("== %s: %d dialogues, group bases %s, %d computed-arg call sites unseen" % (f, len(w.dialogues), base[:ng], unk[f]))
        for g in range(1, ng):
            allg = range(base[g], base[g] + size[g])
            mine = [i for i in allg if i in hit]
            miss = [i for i in allg if i not in hit]
            tag = "no static reference at all" if not mine else "%d/%d referenced" % (len(mine), size[g])
            print("  group %d (%d dialogues): %s; unreferenced dx %s" % (g, size[g], tag, [i - base[g] for i in miss][:40]))
            for i in miss[:2]:
                d = w.dialogues[i]
                print("      dx %d: %r" % (i - base[g], wfm_py.words_to_text(w, d.words, d.font)[:90]))


if __name__ == "__main__":
    main()
