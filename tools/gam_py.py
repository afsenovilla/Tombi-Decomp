#!/usr/bin/env python3
"""GAM (the game's LZ container) in pure Python: the decoder is area000.gam_unpack, this is the matching encoder, so that the Windows
editors (tombi-gfx.exe) can repack a changed page without the Go TombaTools.  Format: `GAM\\0`, u32 size, 16 bit LE flag words (bit 0
first, 1 = reference = offset byte + length byte, 0 = literal byte).  Encoder rules (found in notes/assets_tombatools.md, body part):
greedy, minimum length 5, window 255, maximum length 255, a copy never overlaps itself (length <= offset), candidates ranked by
(min(length, offset), length), a full tie goes to the farthest offset; a full last flag word is followed by one more 0000 word.
The special end-of-data rules of the original tool (zero pad model) are NOT reproduced: the stream stops exactly at the size, so a changed
file gets a valid stream that may differ in its last ~260 bytes from the original tool's.  `pack(data, orig)` keeps the original bytes
when the unpacked data did not change.

  tools/gam_py.py check [FS_DIR]   re-encode every GAM of the tree, decode again and count byte-identical streams
"""
import os, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

MINLEN, WINDOW, MAXLEN = 5, 255, 255


def unpack(d):
    import area000
    return area000.gam_unpack(d)


def encode(data):
    n = len(data)
    out = bytearray(b"GAM\0" + struct.pack("<I", n))
    idx = {}                       # 5 byte key -> positions (ascending)
    toks = []
    i = 0

    def add(p):
        if p + MINLEN <= n:
            idx.setdefault(data[p:p + MINLEN], []).append(p)

    while i < n:
        best = None
        if i + MINLEN <= n:
            lst = idx.get(data[i:i + MINLEN])
            if lst:
                lim = min(MAXLEN, n - i)
                bk = None
                for p in reversed(lst):          # nearest first; a tie goes to the farthest, so use >=
                    off = i - p
                    if off > WINDOW:
                        break
                    mx = min(lim, off)
                    ln = MINLEN
                    while ln < mx and data[p + ln] == data[i + ln]:
                        ln += 1
                    k = (min(ln, off), ln)
                    if bk is None or k >= bk:
                        bk, best = k, (off, ln)
        if best and best[1] >= MINLEN:
            toks.append((1, bytes(best)))
            for q in range(i, i + best[1]):
                add(q)
            i += best[1]
        else:
            toks.append((0, data[i:i + 1]))
            add(i)
            i += 1
    for k in range(0, len(toks), 16):
        g = toks[k:k + 16]
        out += struct.pack("<H", sum(t << b for b, (t, _) in enumerate(g)))
        for _, x in g:
            out += x
    if toks and len(toks) % 16 == 0:
        out += b"\0\0"
    return bytes(out)


def pack(data, orig=None):
    """GAM bytes for `data`; `orig` = the original packed bytes, kept as they are when they unpack to exactly `data`."""
    if orig is not None:
        try:
            if unpack(orig) == bytes(data):
                return orig
        except (ValueError, IndexError):
            pass
    return encode(bytes(data))


def main():
    import area000
    root = sys.argv[2] if len(sys.argv) > 2 else area000.FS
    tot = same = 0
    for d, _, fs in os.walk(root):
        for f in sorted(fs):
            if f.upper().endswith(".GAM"):
                o = open(os.path.join(d, f), "rb").read()
                u = unpack(o)
                e = encode(u)
                assert unpack(e) == u, f
                tot += 1
                same += e == o
    print("%d GAM files: all decode back; %d byte-identical to the original streams" % (tot, same))


if __name__ == "__main__":
    main()
