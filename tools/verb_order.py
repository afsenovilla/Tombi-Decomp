#!/usr/bin/env python3
"""Item message order: "Rana Ganada" -> "Ganada Rana" (so a language can say "Obtienes: Rana").

The popup shown when Tombi gets an item is ONE box built by printInfoMessage (MAIN0 0x8004D620, notes/formats.md): the box width is
a table value, the glyphs of the item name (MFNTnnnS dialogue k, the box codes of the dialogue are ignored) are laid out one after the
other from x = 8, and for b = 0 / 3 (obtained) the verb glyphs (dialogue 2, " Ganada") are appended after the name, while for b = 1 / 4
(used / equipped, dialogue 0 / 1) the verb is drawn BEFORE the name (`callit`). The patch makes b = 0 / 3 draw dialogue 2 first too:

  before:  b == 1 || b == 4 -> drawInfoMessageText(o, 0 or 1)          (then the name loop)
           name loop ... end of text: b == 0 || b == 3 -> drawInfoMessageText(o, 2)
  after:   b == 1 || b == 4 -> same
           b == 0 || b == 3 -> drawInfoMessageText(o, 2)               (new, before the name)
           name loop ... end of text: return

Same size: the end-of-text test (9 words) is replaced by `return` plus the new test, a jump into the existing `jal drawInfoMessageText`
of callit. The code is found by its instruction pattern in each EXE/MAINn.EXE (MAIN5, the Spanish one, is linked 6 words earlier than
the others), so no address is hard-coded.

  python3 tools/verb_order.py FS_DIR_OR_EXE...      (reports / checks; --apply writes the patched copy next to it: NAME.EXE.patched)
"""
import os
import struct
import sys

HEADER = 0x800
FUNC = 0x3DE20            # file offset of printInfoMessage in every MAINn.EXE (0x8004D620 in MAIN0)
FUNC_LEN = 0x560


def _w(x):
    return struct.pack("<I", x)


def _u(b, o):
    return struct.unpack_from("<I", b, o)[0]


def _find(exe):
    """File offsets of the three places of printInfoMessage, or None: (bne, tail, jal) where bne = `bne s2,v0,continue` after
    `beq s2,1 / li v0,4`, tail = the end-of-text test (beqz s2 ... j return), jal = the jal of callit."""
    exe = bytes(exe)
    if len(exe) < FUNC + FUNC_LEN or exe[FUNC:FUNC + 8] != bytes.fromhex("d0ffbd271c00b3af"):
        return None
    f = exe[FUNC:FUNC + FUNC_LEN]
    pat_a = bytes.fromhex("03004212" "04000224" "0f004216" "00000000")      # beq s2,v0,+3 / li v0,4 / bne s2,v0,+0xf / nop
    a = f.find(pat_a)
    if a < 0 or f.find(pat_a, a + 1) >= 0:
        return None
    # callit's jal: `move a0,s0 ; li a1,1 ; jal ; nop`
    pat_j = bytes.fromhex("21200002" "01000524")
    j = f.find(pat_j, a)
    if j < 0 or f[j + 8 + 3] != 0x0C or _u(f, j + 12) != 0:
        return None
    jal = j + 8
    # tail: beqz s2,+3 / li v0,3 / bne s2,v0,+0x44 / move v0,s0 / move a0,s0 / jal / li a1,2 / j / move v0,s0
    pre = bytes.fromhex("03004012" "03000224" "44004216" "21100002" "21200002")
    t = f.find(pre, jal)
    if t < 0:
        return None
    if f[t + 20 + 3] != 0x0C or _u(f, t + 24) != 0x24050002 or f[t + 28 + 3] != 0x08 or _u(f, t + 32) != 0x02001021:
        return None
    return FUNC + a + 8, FUNC + t, FUNC + jal


def is_patched(exe):
    """True when the verb-first patch is already in (the new test `li v0,3 ; beq s2,v0 ; move a0,s0 ; bne s2,zero` after a `j`)."""
    f = bytes(exe)[FUNC:FUNC + FUNC_LEN]
    i = f.find(bytes.fromhex("21100002" "03000224"))
    return (i >= 4 and f[i - 1] == 0x08 and f[i + 11] == 0x12 and f[i + 12:i + 16] == bytes.fromhex("21200002")
            and f[i + 19] == 0x16)


def patch(exe):
    """Patched copy of an EXE/MAINn.EXE (bytes), the same bytes when already patched, None when the code was not found."""
    exe = bytes(exe)
    loc = _find(exe)
    if loc is None:
        return exe if is_patched(exe) else None
    bne, tail, jal = loc
    b = bytearray(exe)
    pc = lambda o: 0x80010000 + o - HEADER          # address of a file offset
    cont = pc(bne) + 4 + 4 * struct.unpack_from("<h", b, bne)[0]     # where `bne` went: after callit
    ret = (_u(b, tail + 0x1C) & 0x03FFFFFF) << 2 | 0x80000000          # the `j` to the epilogue at the end of the tail
    w2 = tail + 8
    # new test at tail+8 (w2): reached from the old bne instead of `cont`
    br = lambda op, rs, rt, here, to: _w(op << 26 | rs << 21 | rt << 16 | ((to - (here + 4)) >> 2 & 0xFFFF))
    code = [
        _w(0x08000000 | ret >> 2 & 0x03FFFFFF),                    # w0: j ret              (end of text: nothing more to draw)
        _w(0x02001021),                                            # w1:  move v0,s0        (returns the object)
        _w(0x24020003),                                            # w2: li v0,3
        br(4, 18, 2, pc(w2 + 4), pc(w2 + 20)),                     # w3: beq s2,v0,w7       (b == 3)
        _w(0x02002021),                                            # w4:  move a0,s0
        br(5, 18, 0, pc(w2 + 12), cont),                           # w5: bne s2,zero,cont   (b != 0: nothing)
        _w(0),                                                     # w6:  nop
        _w(0x08000000 | pc(jal) >> 2 & 0x03FFFFFF),                # w7: j jal              (callit's drawInfoMessageText(a0, a1))
        _w(0x24050002),                                            # w8:  li a1,2
    ]
    for i, c in enumerate(code):
        b[tail + 4 * i:tail + 4 * i + 4] = c
    b[bne:bne + 4] = br(5, 18, 2, pc(bne), pc(w2))                 # bne s2,v0,w2 (was: cont)
    return bytes(b)


def _slots(exe):
    """({case b: file offset of the `addiu v0,v0,W` that adds the verb width to the item width} for b = 0, 1, 4 (b = 1 and 4 may share
    one instruction: MAIN5), file offset of the item width table (u16 per item) used by case 0) or None.
    The cases are found through the jump table of `switch (b)` (lui at / addu / lw v0,X(at) / jr v0)."""
    exe = bytes(exe)
    f = exe[FUNC:FUNC + FUNC_LEN]
    pat = bytes.fromhex("80101200" "0180013c" "21082200")            # sll v0,s2,2 / lui at,0x8001 / addu at,at,v0
    i = f.find(pat)
    if i < 0 or f[i + 12 + 3] != 0x8C:                                # lw v0,X(at)
        return None
    jt = 0x80010000 + struct.unpack_from("<h", f, i + 12)[0] - 0x80010000 + HEADER
    slots, table = {}, None
    for case in (0, 1, 4):
        t = _u(exe, jt + 4 * case) - 0x80010000 + HEADER
        for k in range(t, t + 40, 4):
            w = _u(exe, k)
            if w >> 16 == 0x9422 and table is None and case == 0:       # lhu v0,disp(at) after lui at,0x8008
                table = 0x80080000 + struct.unpack_from("<h", exe, k)[0] - 0x80010000 + HEADER
            if w >> 16 == 0x2442:                                      # addiu v0,v0,imm
                slots[case] = k
                break
        else:
            return None
    return slots, table


def widths(exe):
    """{b: verb width constant} (0x60 / 0x58 / 0x78 in the English executable) or None."""
    r = _slots(exe)
    return None if r is None else {c: struct.unpack_from("<h", exe, k)[0] for c, k in r[0].items()}


MAX_VERB = 0xC0       # largest verb constant (box = item name + verb; the screen is 320 px and the widest names are ~190 px)


def adjust(exe, d0, d1):
    """Copy of exe with the verb width constants moved by d0 px (case 0, "obtained", dialogue 2) and d1 px (cases 1 and 4, "used /
    equipped", dialogues 0 and 1), clamped to 16..MAX_VERB (a constant already above that is left as it is). None when the code was not found."""
    r = _slots(exe)
    if r is None:
        return None
    slots = r[0]
    b = bytearray(exe)
    for k in sorted(set(slots.values())):
        cases = [c for c, v in slots.items() if v == k]
        d = d0 if cases == [0] else d1
        old = struct.unpack_from("<h", b, k)[0]
        struct.pack_into("<h", b, k, max(16, min(old + d, max(MAX_VERB, old))))
    return bytes(b)


def check(root):
    ok = True
    for n in range(9):
        p = os.path.join(root, "MAIN%d.EXE" % n)
        if not os.path.isfile(p):
            continue
        d = open(p, "rb").read()
        loc = _find(d)
        new = patch(d)
        print("MAIN%d: %s%s" % (n, "not found" if new is None else "patchable" if new != d else "already patched",
                                " at 0x%X/0x%X/0x%X" % loc if loc else ""))
        ok = ok and new is not None
    return 0 if ok else 1


if __name__ == "__main__":
    a = [x for x in sys.argv[1:] if not x.startswith("--")]
    if "--apply" in sys.argv:
        for p in a:
            d = open(p, "rb").read()
            n = patch(d)
            if n is None:
                sys.exit("%s: code not found" % p)
            open(p + ".patched", "wb").write(n)
            print(p + ".patched")
    else:
        sys.exit(check(a[0] if a else "game/disc/fs/EXE"))
