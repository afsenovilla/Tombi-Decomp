#!/usr/bin/env python3
"""Runs the same pad script in tools/psxemu on the PAL MAIN0.EXE and on the USA SCUS_942.36 (Tomba!, SCUS-94236) and
compares Tombi's per-frame position (docs/PORT.md "60 Hz mode": the PAL logic is the NTSC logic frame for frame).

The script is port/tools/jumpcheck.py's: new game, the opening dialogue (capture.TALK), a standing jump, a running jump
and a jump with Cross held. The USA executable runs on the same HLE: every PAL function name of notes/*_main0.csv is
moved to its USA address through its masked twin (tools/ntsc_diff.py), names without a twin through `--us-symbols`
(psx_tomba's symbols/scus_942.36/symbol_addrs.txt) or, failing that, the nearest data/code relocation pair; the
player object (PAL 0x800A6038) is translated the same way.

  python3 tools/isoext.py game_usa/tomba_us.bin US_FS                       (once)
  python3 -I tools/ntsc_check.py US_FS game_usa/tomba_us.bin OUT_DIR [--us-symbols FILE] [--port TRACE]
      writes OUT_DIR/emu_pal.txt, OUT_DIR/emu_us.txt ("t x y pad" per frame after the title) and prints the jump arcs;
      --port TRACE: a port `-v -v -v` log of the same inputs (build/port2/tombi2 --headless --skip-movies --hz 60 ...
      --pad-script "$(python3 tools/ntsc_check.py --port-script)"), compared the same way.
Takes about ten minutes per executable."""
import argparse, bisect, collections, os, re, struct, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)
import ntsc_diff as N  # noqa: E402
import xmatch as X  # noqa: E402

PLAYER = 0x800A6038
CROSS, RIGHT, START = 0x4000, 0x0020, 0x0008
PORT_SCRIPT = ("1000:start:4,1504:cross:3x4/33,1736:select:3,1859:cross:3x18/43,2693:cross:3x4/33,2915:cross:3,"
               "3008:right:40,3048:right+cross:3,3051:right:90,3171:right:40,3211:right+cross:30,3241:right:60")


def us_map(us_exe):
    """(name -> USA address for PAL main0 functions with a masked twin, PAL address -> USA address for functions and for
    the data addresses built by lui/lo pairs at equal positions in twin functions)."""
    funcs = N.pal_funcs("main0")
    ub = N.load(us_exe)
    uw = X.words(ub)
    um = [X.mask(w) for w in uw]
    idx = collections.defaultdict(list)
    for i in range(len(um) - 4):
        idx[tuple(um[i:i + 4])].append(i)
    cov, fmap = bytearray(len(um)), {}
    for f in sorted(funcs, key=lambda f: -f[3]):
        mw = tuple(X.mask(w) for w in X.words(f[4]))
        for i in idx.get(mw[:4], ()):
            if tuple(um[i:i + len(mw)]) == mw and not any(cov[i:i + len(mw)]):
                cov[i:i + len(mw)] = b"\1" * len(mw)
                fmap[f[2]] = 0x80010000 + i * 4
                break
    votes = collections.defaultdict(collections.Counter)
    for f in funcs:
        if f[2] not in fmap:
            continue
        pw = X.words(f[4])
        o = (fmap[f[2]] - 0x80010000) // 4
        qw = uw[o:o + len(pw)]
        hp, hq = {}, {}
        for p, q in zip(pw, qw):
            op = p >> 26
            if op == 15:
                hp[(p >> 16) & 31], hq[(q >> 16) & 31] = (p & 0xFFFF) << 16, (q & 0xFFFF) << 16
            elif op in (9, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E, 0x32, 0x3A):
                rs = (p >> 21) & 31
                if rs in hp and rs in hq:
                    a = (hp[rs] + ((p & 0xFFFF) ^ 0x8000) - 0x8000) & 0xFFFFFFFF
                    b = (hq[rs] + ((q & 0xFFFF) ^ 0x8000) - 0x8000) & 0xFFFFFFFF
                    if 0x80000000 <= a < 0x80200000:
                        votes[a][b] += 1
                if op == 9:
                    hp.pop((p >> 16) & 31, None)
                    hq.pop((p >> 16) & 31, None)
    amap = {a: c.most_common(1)[0][0] for a, c in votes.items()}
    amap.update(fmap)
    return amap


def translate(amap, keys, a):
    if a in amap:
        return amap[a]
    k = keys[max(0, bisect.bisect_right(keys, a) - 1)]
    return amap[k] + (a - k)


def run(out_path, exe=None, disc=None, us=None, us_symbols=None):
    from psxemu import capture, machine
    player, names = PLAYER, None
    if us:
        amap = us_map(exe)
        keys = sorted(amap)
        sym = {}
        if us_symbols:
            for line in open(us_symbols):
                m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", line)
                if m:
                    sym[m.group(1)] = int(m.group(2), 16)
        names = {}
        for n, al in machine.load_names().items():
            out = []
            for a in al:
                if a in amap:
                    out.append(amap[a])
                elif n in sym:
                    out.append(sym[n])
                elif 0x80010000 <= a < 0x80078000:
                    out.append(translate(amap, keys, a))
                else:
                    out.append(a)          # overlays, scratchpad: same addresses
            names[n] = out
        machine.MOVIE_TASK = names["moviePlayerTask"][0]
        player = translate(amap, keys, PLAYER)
    m = machine.Machine(exe=exe, disc=disc, names=names)
    if us:
        b = open(exe, "rb").read()
        o = names["joypadInit"][0] - 0x80010000 + 0x800
        hi, lo = struct.unpack_from("<2I", b, o + 8)
        if hi >> 16 == 0x3C04 and lo >> 16 == 0x2484:
            m.pad_buf = (((hi & 0xFFFF) << 16) + ((lo & 0xFFFF) ^ 0x8000) - 0x8000) & 0xFFFFFFFF
    out = open(out_path, "w")
    m.render = False
    capture.title(m, log=lambda *a: None)
    m.render = False
    t0 = m.frame_count

    def log():
        c = m.cpu
        hp = c.r32(player + 0x40)
        px = c.r16(hp + 2) if 0x80000000 <= hp < 0x80200000 else -1
        py = c.r16(player + 0x16)
        print(m.frame_count - t0, px - 0x10000 if px >= 0x8000 else px, py - 0x10000 if py >= 0x8000 else py,
              "%04x" % (m.pad or 0), file=out, flush=True)

    def play(script):
        for buttons, n in script:
            m.set_pad(buttons)
            for _ in range(n):
                m.run_frames(1, max_instr=3_000_000)
                log()
        m.set_pad(0)
    play([(START, 4), (0, 500)] + capture.TALK)
    play([(0, 30), (CROSS, 3), (0, 90)])
    play([(RIGHT, 40), (RIGHT | CROSS, 3), (RIGHT, 90), (0, 30)])
    play([(RIGHT, 40), (RIGHT | CROSS, 30), (RIGHT, 60), (0, 30)])
    out.close()


def load_trace(path):
    tr = []
    for line in open(path):
        m = re.match(r"\[port\] frame (\d+): area \d+ section \d+, player at (-?\d+),(-?\d+)", line)
        if m:
            tr.append((int(m.group(1)) - 1000, int(m.group(2)), int(m.group(3))))
            continue
        w = line.split()
        if len(w) >= 3 and w[0].isdigit():
            tr.append((int(w[0]), int(w[1]), int(w[2])))
    for i in range(1, len(tr) - 1):  # tools/psxemu slices vblanks by instruction count: drop its one-frame glitches
        a, b, c = tr[i - 1], tr[i], tr[i + 1]
        if (b[1], b[2]) != (a[1], a[2]) and (a[1], a[2]) == (c[1], c[2]):
            tr[i] = (b[0], a[1], a[2])
    return [p for p in tr if p[1] > 0]


def jumps(tr):
    """[(start x, end x, ground y, peak px, frames in the air, dy list)] of the jumps after the opening drop."""
    out, i = [], 0
    while i < len(tr) - 1:
        g = tr[i][2]
        if g > -200 and i + 2 < len(tr) and tr[i + 1][2] - g <= -3 and tr[i + 2][2] < tr[i + 1][2]:
            j = i + 2       # in the air until back near the take-off height or standing still for 6 frames (a ledge)
            while j < len(tr) and tr[j][2] < g - 2 and not (j + 6 < len(tr) and len({p[1:] for p in tr[j:j + 7]}) == 1):
                j += 1
            if j < len(tr):
                ys = [p[2] for p in tr[i:j + 1]]
                out.append((tr[i][1], tr[j][1], g, g - min(ys), j - i, [ys[k] - ys[k - 1] for k in range(1, len(ys))]))
            i = j
        else:
            i += 1
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("usfs", nargs="?")
    ap.add_argument("usdisc", nargs="?")
    ap.add_argument("out", nargs="?")
    ap.add_argument("--us-symbols")
    ap.add_argument("--port")
    ap.add_argument("--port-script", action="store_true")
    ap.add_argument("--skip-run", action="store_true", help="only compare OUT_DIR/emu_*.txt of an earlier run")
    a = ap.parse_args()
    if a.port_script:
        print(PORT_SCRIPT)
        return
    os.makedirs(a.out, exist_ok=True)
    pal, us = os.path.join(a.out, "emu_pal.txt"), os.path.join(a.out, "emu_us.txt")
    if not a.skip_run:
        run(pal)
        run(us, exe=os.path.join(a.usfs, "SCUS_942.36"), disc=a.usdisc, us=True, us_symbols=a.us_symbols)
    traces = [("PAL emu", pal), ("USA emu", us)] + ([("port", a.port)] if a.port else [])
    res = {n: jumps(load_trace(p)) for n, p in traces}
    for n, js in res.items():
        print(n)
        for j in js:
            print("   x %d -> %d, ground %d, peak %d px, %d frames in the air" % j[:5])
    ref = res["PAL emu"]
    for n, js in res.items():
        verdict = "same as" if len(js) == len(ref) else "DIFFERENT from (jump count)"
        for k, (x, y) in enumerate(zip(js, ref)):
            px = [0] + list(__import__("itertools").accumulate(x[5]))
            py = [0] + list(__import__("itertools").accumulate(y[5]))
            exact = x[3:5] == y[3:5] and px == py
            # tools/psxemu samples some frames before the game's update of that vblank: allow one frame of lag per sample
            near = x[3:5] == y[3:5] and all(px[t] in py[max(0, t - 1):t + 2] or py[t] in px[max(0, t - 1):t + 2] for t in range(len(px)))
            if not exact:
                print("   jump %d: %s\n      dy %s\n  PAL dy %s" % (k + 1, "same within one-frame emulator sampling" if near else
                      "DIFFERENT", " ".join(map(str, x[5])), " ".join(map(str, y[5]))))
            if not near:
                verdict = "DIFFERENT from"
        print("%s: %d jumps, %s the PAL arcs (peak, frames in the air, per-frame height)" % (n, len(js), verdict))


if __name__ == "__main__":
    main()
