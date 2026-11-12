#!/usr/bin/env python3
"""WSS event-script disassembler and cross-build comparer.

Format (docs/FORMATS.md "WSS"; MAIN0 execCoreOpcode / scriptOpBranch / scriptLoadArgs / execGameOpcode):
  +0  "W\\0S\\0", +4 u32 size, +8 64 x u16 entry table (code offset + 1, 0 = none), code at +0x88.
  The area loader (initAreaScripts) only ever starts entry 0; the other entries are reached by CALL (0x12 e).
  Branch targets are absolute code offsets (u16).

Usage (prefix --ops N for a pre-release build's op count: 40 JP demo / Dengeki, 39 PrePre 9, 32 Hyper 12):
  wss.py dis FILE                 full listing with reachability marks ('!' = never reached from entry 0)
  wss.py summary FILE...          per file: entries, reached entries, orphan entries/blocks, events / items / messages
  wss.py diff A.WSS B.WSS         op-level diff of two files (normalised: jump targets replaced by labels)
  wss.py sem FILE                 semantic census: events, items, dialogue (ObjCmd 4/8 -> k,dx), flags, spawns...
  wss.py semdiff A.WSS B.WSS      what only A / only B has in that census (multiset), with reachability
"""
import struct
import sys
import difflib

CORE = {
    1: ("END", ""), 2: ("MOV", "vv"), 3: ("MOVI", "vi"), 4: ("SWAP", "vv"), 6: ("RAND", "v"),
    8: ("JMP", "a"), 9: ("JEQ", "a"), 10: ("JNE", "a"), 11: ("JEQ2", "a"), 12: ("JNE2", "a"),
    13: ("JLT", "a"), 14: ("JGT", "a"), 15: ("JLE", "a"), 16: ("JGE", "a"),
    17: ("LOOP", "va"), 18: ("CALL", "e"), 19: ("RET", ""), 20: ("PUSHV", ""), 21: ("POPV", ""),
    22: ("CMP", "vv"), 23: ("CMPI", "vi"), 24: ("INC", "v"), 25: ("DEC", "v"), 26: ("NOT", "v"), 27: ("NEG", "v"),
    44: ("WAIT", "b"),
}
_ARITH = ["ADD", "SUB", "MUL", "DIV", "MOD", "AND", "OR", "XOR"]
for _i, _n in enumerate(_ARITH):
    CORE[0x1C + 2 * _i] = (_n, "vv")
    CORE[0x1D + 2 * _i] = (_n + "I", "vi")

GAME = {
    0x80: "SpawnObject", 0x81: "SetObjAnim", 0x82: "KillObject", 0x83: "SetPlayerPos", 0x84: "GetPlayerPos",
    0x85: "ObjCmd", 0x86: "GetObj6A", 0x87: "SetObjPos", 0x88: "GetObjPos", 0x89: "SetObjFrame",
    0x8A: "GetObjFrame", 0x8B: "ObjIsState2", 0x8C: "SetObjEnabled", 0x8D: "GetObjVisible", 0x8E: "ReadFlag",
    0x8F: "WriteFlag", 0x90: "GetObj68", 0x91: "ReadGlobal", 0x92: "WriteGlobal", 0x93: "LoadBackground",
    0x94: "SetObj6B", 0x95: "SetObjFacing", 0x96: "SetPlayer6B", 0x97: "SetPlayerStateEx", 0x98: "GetPlayerTouch",
    0x99: "SetPlayerFrame", 0x9A: "AwardEvent", 0x9B: "GetPlayerHit", 0x9C: "SetDialogId", 0x9D: "InfoMessage",
    0x9E: "PlaySound", 0x9F: "ReleaseType19", 0xA0: "Fade", 0xA1: "InfoMessage2", 0xA2: "GiveItem",
    0xA3: "GetObj69", 0xA4: "SetObj69", 0xA5: "SetObj9C", 0xA6: "SetPlayerAnim", 0xA7: "GetAreaId",
    0xA8: "GetPlayerE2", 0xA9: "HealPlayer", 0xAA: "GetAP", 0xAB: "Yield",
}


# Number of game ops of the build (the last one is always Yield; earlier builds lack the later ops, the rest keep
# their numbers). From the exe dispatchers (op - 0x80, sltiu N): retail PAL/USA/JP and beta 980203 44 (0x80..0xAB),
# JP demo SLPM-80165 and Dengeki D5 40 (Yield 0xA7), PrePre 9 39 (Yield 0xA6; its 0xA2 GiveItem has no -1 =
# max-health branch), Hyper Re-mix 12 32 (Yield 0x9F: no GiveItem / Fade ops at all).
GAME_N = 44


class Ins:
    __slots__ = ("off", "op", "name", "ops", "size", "args")

    def __init__(self, off, op, name, ops, size, args=None):
        self.off, self.op, self.name, self.ops, self.size, self.args = off, op, name, ops, size, args


def load(path):
    d = open(path, "rb").read()
    if d[:4] != b"W\x00S\x00":
        raise ValueError("%s: not a WSS file" % path)
    ent = struct.unpack_from("<64H", d, 8)
    return d, ent, d[0x88:]


def decode(code, off):
    """Decode one instruction at `off`; returns Ins (name '??' for an undefined byte, which the VM treats as END)."""
    op = code[off]
    if op == 5:
        n = code[off + 1]
        p = off + 2
        args = []
        for _ in range(n):
            kind = code[p]
            if kind == 0:
                args.append(("v", code[p + 1]))
                p += 2
            else:
                args.append(("i", struct.unpack_from("<i", code, p + 1)[0]))
                p += 5
        return Ins(off, op, "ARGS", [], p - off, args)
    if op in CORE:
        name, fmt = CORE[op]
        p = off + 1
        ops = []
        for f in fmt:
            if f in "veb":
                ops.append(code[p]); p += 1
            elif f == "i":
                ops.append(struct.unpack_from("<i", code, p)[0]); p += 4
            elif f == "a":
                ops.append(struct.unpack_from("<H", code, p)[0]); p += 2
        return Ins(off, op, name, ops, p - off)
    if op in GAME and op < 0x80 + GAME_N:
        return Ins(off, op, "Yield" if op == 0x80 + GAME_N - 1 else GAME[op], [], 1)
    return Ins(off, op, "??%02X" % op, [], 1)


def walk(code, ent, start_entries=(0,)):
    """Control-flow walk. Returns (insns by offset, reached entry set, unresolved notes)."""
    seen = {}
    entries = set()
    work = []
    for e in start_entries:
        if ent[e]:
            entries.add(e)
            work.append(ent[e] - 1)
    while work:
        off = work.pop()
        while 0 <= off < len(code) and off not in seen:
            try:
                ins = decode(code, off)
            except (IndexError, struct.error):
                break
            seen[off] = ins
            if ins.name in ("END", "RET") or ins.name.startswith("??"):
                break
            if ins.name == "CALL":
                e = ins.ops[0]
                if e < 64 and ent[e] and e not in entries:
                    entries.add(e)
                    work.append(ent[e] - 1)
            if ins.op in range(8, 17) or ins.name == "LOOP":
                work.append(ins.ops[-1])
                if ins.name == "JMP":
                    break
            off += ins.size
    return seen, entries


def linear(code):
    """Linear sweep of the whole code area (for listing and for finding never-reached bytes)."""
    out = {}
    off = 0
    while off < len(code):
        try:
            ins = decode(code, off)
        except (IndexError, struct.error):
            break
        out[off] = ins
        off += ins.size
    return out


def fmt_ins(ins):
    if ins.name == "ARGS":
        return "ARGS " + ", ".join(("r%02X" % v) if k == "v" else str(v) for k, v in ins.args)
    if not ins.ops:
        return ins.name
    return ins.name + " " + ", ".join(str(x) if ins.name in ("WAIT",) else
                                        ("@%04X" % x if CORE.get(ins.op, ("", ""))[1][i] == "a" else
                                         "e%d" % x if CORE.get(ins.op, ("", ""))[1][i] == "e" else
                                         "r%02X" % x if CORE.get(ins.op, ("", ""))[1][i] == "v" else str(x))
                                        for i, x in enumerate(ins.ops))


def calls(seq):
    """Game calls with their ARGS immediates: list of (offset, opname, args tuple of int|'var')."""
    out = []
    last = None
    for ins in seq:
        if ins.name == "ARGS":
            last = tuple(v if k == "i" else "r%02X" % v for k, v in ins.args)
        elif ins.op >= 0x80:
            out.append((ins.off, ins.name, last or ()))
            last = None
        elif ins.name not in ("MOV", "MOVI"):
            last = None
    return out


def info(path, extra_entries=()):
    d, ent, code = load(path)
    seen, entries = walk(code, ent, (0,) + tuple(extra_entries))
    lin = linear(code)
    return d, ent, code, seen, entries, lin


def entry_of(ent, off):
    best = None
    for e, v in enumerate(ent):
        if v and v - 1 <= off and (best is None or v - 1 > ent[best] - 1):
            best = e
    return best


def cmd_dis(path):
    d, ent, code, seen, entries, lin = info(path)
    starts = {v - 1: e for e, v in enumerate(ent) if v}
    for off in sorted(lin):
        if off in starts:
            e = starts[off]
            print("\n; ---- entry %d%s" % (e, "" if e in entries else "   (ORPHAN: never called)"))
        ins = lin[off]
        print("%s %04X  %s" % (" " if off in seen else "!", off, fmt_ins(ins)))


def cmd_summary(paths):
    for p in paths:
        d, ent, code, seen, entries, lin = info(p)
        defined = [e for e, v in enumerate(ent) if v]
        orphan = [e for e in defined if e not in entries]
        dead = sorted(o for o in lin if o not in seen)
        print("== %s  size %d, %d entries defined, %d reached; orphan entries %s; never-reached insns %d"
              % (p, len(d), len(defined), len(entries), orphan, len(dead)))
        # calls in orphan entries / dead code
        if orphan:
            _, oents = walk(code, ent, tuple(orphan))
            oseen, _ = walk(code, ent, tuple(orphan))
            only = [oseen[o] for o in sorted(oseen) if o not in seen]
            for c in calls(only):
                if c[1] in ("AwardEvent", "GiveItem", "ObjCmd", "WriteFlag", "ReadFlag", "SpawnObject", "InfoMessage"):
                    print("   orphan  %04X e%s %s %s" % (c[0], entry_of(ent, c[0]), c[1], c[2]))
        live = [seen[o] for o in sorted(seen)]
        for c in calls(live):
            if c[1] in ("AwardEvent", "GiveItem"):
                print("   live    %04X e%s %s %s" % (c[0], entry_of(ent, c[0]), c[1], c[2]))


def norm(path):
    d, ent, code = load(path)
    lin = linear(code)
    starts = {v - 1: e for e, v in enumerate(ent) if v}
    lines = []
    for off in sorted(lin):
        if off in starts:
            lines.append("entry %d:" % starts[off])
        ins = lin[off]
        s = fmt_ins(ins)
        if ins.op in range(8, 18):
            tgt = ins.ops[-1]
            e = entry_of(ent, tgt)
            base = ent[e] - 1 if e is not None else 0
            idx = sum(1 for o in lin if base <= o < tgt)
            s = s.split("@")[0] + "e%s#%d" % (e, idx)
        lines.append(s)
    return lines


KEEP = {"AwardEvent": 2, "GiveItem": 3, "ReadFlag": 1, "WriteFlag": 2, "SpawnObject": 6, "InfoMessage": 1,
        "InfoMessage2": 1, "SetDialogId": 1, "PlaySound": 1, "LoadBackground": 0, "ReadGlobal": 1, "WriteGlobal": 2,
        "SetPlayerStateEx": 2, "SetPlayerPos": 3, "HealPlayer": 1, "GetAP": 0, "Fade": 2}


def sem(path):
    """Census of the meaningful game calls: list of (key string, reached?)."""
    d, ent, code, seen, entries, lin = info(path)
    out = []
    for off, name, args in calls([lin[o] for o in sorted(lin)]):
        live = off in seen
        if name == "ObjCmd" and len(args) >= 4 and args[1] in (4, 8):
            key = "Message k=%s dx=%s" % (args[2], args[3])
        elif name in KEEP:
            key = "%s%s" % (name, tuple(args[:KEEP[name]]) if KEEP[name] else "")
        else:
            continue
        out.append((key, live, off))
    return out


def cmd_sem(path):
    for key, live, off in sem(path):
        print("%s %04X  %s" % (" " if live else "!", off, key))


def cmd_semdiff(a, b):
    import collections
    ca = collections.Counter(k for k, live, _ in sem(a) if live)
    cb = collections.Counter(k for k, live, _ in sem(b) if live)
    for k in sorted(set(ca) | set(cb)):
        if ca[k] != cb[k]:
            print("%-40s %s=%d %s=%d" % (k, a.split("/")[-3] if a.count("/") >= 2 else "A", ca[k],
                                         b.split("/")[-3] if b.count("/") >= 2 else "B", cb[k]))


def cmd_diff(a, b):
    for l in difflib.unified_diff(norm(a), norm(b), a, b, lineterm="", n=2):
        print(l)


if __name__ == "__main__":
    if len(sys.argv) > 2 and sys.argv[1] == "--ops":
        GAME_N = int(sys.argv[2], 0)
        del sys.argv[1:3]
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    c = sys.argv[1]
    if c == "dis":
        cmd_dis(sys.argv[2])
    elif c == "summary":
        cmd_summary(sys.argv[2:])
    elif c == "diff":
        cmd_diff(sys.argv[2], sys.argv[3])
    elif c == "sem":
        cmd_sem(sys.argv[2])
    elif c == "semdiff":
        cmd_semdiff(sys.argv[2], sys.argv[3])
    else:
        sys.exit(__doc__)
