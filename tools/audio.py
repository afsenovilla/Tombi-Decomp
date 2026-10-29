#!/usr/bin/env python3
"""Tombi! (PAL) sound extraction, pure Python 3 (no dependencies).

  python3 tools/audio.py all  Tombi.bin OUT_DIR   everything below into OUT_DIR/sound
  python3 tools/audio.py wvd  FILE.WVD OUT_DIR    VAB header + every VAG sample as WAV (+ vab.json)
  python3 tools/audio.py seq  FILE.SEQ OUT.mid    Sony pQES sequence -> standard MIDI file (type 0)
  python3 tools/audio.py xa   Tombi.bin OUT_DIR   the XA ADPCM audio track of every MOVIE/*.STR as WAV
  python3 tools/audio.py render FILE.SEQ FILE.WVD OUT.wav   software sequencer: SEQ played with the samples of a bank -> stereo 44.1 kHz audio
  python3 tools/audio.py music Tombi.bin OUT_DIR  every SEQ rendered with its bank into OUT_DIR/sound/music_audio (needs numpy)
  option --audio-format wav|mp3 (mp3 needs the lameenc package, falls back to WAV; --bitrate 192).  Loop points (smpl chunk) only in WAV.

Layouts: notes/formats.md ("SOUND" and "MOVIE").  The XA audio is interleaved in the .STR files as Mode 2 Form 2 sectors, which
the ISO extractor (isoext.py) mangles, so this module reads the raw 2352 byte sectors of the image itself."""
import os, struct, sys, json, wave, math

AUDIO_FORMATS = ("wav", "mp3")      # FLAC / OGG would need a pure Python encoder or an external library: not offered
MP3_BITRATE = 192

# ---------------------------------------------------------------- SPU / XA ADPCM
SPU_F = ((0, 0), (60, 0), (115, -52), (98, -55), (122, -60))   # filter coefficients / 64
VAG_RATE = 44100   # the SPU plays a sample at its root note at 44100 Hz for pitch 0x1000; the real source rate is not stored


def decode_vag(data):
    """Raw SPU ADPCM (16 byte blocks: shift|filter<<4, flags, 14 data bytes) -> (list of s16, loop_start, loop_end) in samples (None if no loop)."""
    out = []; s1 = s2 = 0; ls = le = None
    for o in range(0, len(data) - 15, 16):
        b = data[o]; shift = b & 15; flt = min(b >> 4, 4); fl = data[o + 1]
        if shift > 12: shift = 9
        c1, c2 = SPU_F[flt]
        if fl == 7 and o > 0 and not any(data[o + 2:o + 16]): break      # end marker block
        if fl & 4 and ls is None: ls = len(out)
        for v in data[o + 2:o + 16]:
            for nib in (v & 15, v >> 4):
                if nib & 8: nib -= 16
                s = ((nib << 12) >> shift) + ((s1 * c1 + s2 * c2 + 32) >> 6)
                s = -32768 if s < -32768 else 32767 if s > 32767 else s
                out.append(s); s2 = s1; s1 = s
        if fl & 1:
            if fl & 2: le = len(out)
            break
    return out, ls, le


def write_wav(path, chans, rate, loop=None):
    """chans = list of channel sample lists (s16); loop = (start, end) sample frames -> 'smpl' chunk."""
    n = len(chans[0]); nc = len(chans)
    if hasattr(chans[0], "tobytes"):                 # numpy int16 arrays
        import numpy as np
        pcm = np.stack(chans, 1).astype("<i2").tobytes()
    elif nc == 1: pcm = struct.pack("<%dh" % n, *chans[0])
    else:
        il = [0] * (n * nc)
        for c in range(nc): il[c::nc] = chans[c]
        pcm = struct.pack("<%dh" % (n * nc), *il)
    fmt = struct.pack("<HHIIHH", 1, nc, rate, rate * nc * 2, nc * 2, 16)
    extra = b""
    if loop:
        extra = b"smpl" + struct.pack("<I", 60) + struct.pack("<9I", 0, 0, int(1e9 / rate), 60, 0, 0, 0, 1, 0) + struct.pack("<6I", 0, 0, loop[0], max(loop[1] - 1, loop[0]), 0, 0)
    body = b"WAVEfmt " + struct.pack("<I", 16) + fmt + extra + b"data" + struct.pack("<I", len(pcm)) + pcm
    with open(path, "wb") as f: f.write(b"RIFF" + struct.pack("<I", len(body)) + body)


def write_mp3(path, chans, rate, bitrate=MP3_BITRATE):
    """lameenc (pip install lameenc); LAME resamples rates MP3 has no mode for (the 37.8 / 18.9 kHz XA tracks) to 44.1 kHz."""
    import lameenc
    nc = len(chans)
    if hasattr(chans[0], "tobytes"):
        import numpy as np
        pcm = np.stack(chans, 1).astype("<i2").tobytes()
    else:
        n = len(chans[0]); il = [0] * (n * nc)
        for c in range(nc): il[c::nc] = chans[c]
        pcm = struct.pack("<%dh" % (n * nc), *il)
    e = lameenc.Encoder()
    e.set_bit_rate(bitrate); e.set_in_sample_rate(rate); e.set_channels(nc); e.set_quality(2)
    if rate not in (8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000): e.set_out_sample_rate(44100)
    with open(path, "wb") as f: f.write(bytes(e.encode(pcm)) + bytes(e.flush()))


_warned = set()
def write_audio(base, chans, rate, fmt="wav", loop=None, bitrate=MP3_BITRATE, log=print):
    """Writes base + '.wav' / '.mp3' (chans: lists of s16 or numpy int16 arrays) and returns the path.  mp3 falls back to wav when lameenc is
    missing; the smpl loop chunk exists only in WAV."""
    if fmt == "mp3":
        try:
            import lameenc  # noqa: F401
        except ImportError:
            if "mp3" not in _warned: log("audio: lameenc is not installed (pip install lameenc), writing WAV instead of MP3"); _warned.add("mp3")
            fmt = "wav"
    if fmt == "mp3":
        write_mp3(base + ".mp3", chans, rate, bitrate); return base + ".mp3"
    write_wav(base + ".wav", chans, rate, loop); return base + ".wav"


# ---------------------------------------------------------------- WVD / VAB
def split_wvd(d):
    """-> list of (vab_header_bytes, vb_body_bytes), one per bank."""
    bt, ht, _ = struct.unpack("<3I", d[:12])
    def tbl(t):
        n = struct.unpack("<I", d[t:t + 4])[0] // 4
        return struct.unpack("<%dI" % n, d[t:t + 4 * n])
    bo, ho = tbl(bt), tbl(ht)
    if d[bt + bo[0]:bt + bo[0] + 4] == b"pBAV":     # the Japanese demo (SLPM-80165) stores the header table first
        bt, ht, bo, ho = ht, bt, ho, bo
    n = len(bo) - 1
    out = []
    for i in range(n):
        body = d[bt + bo[i]:bt + bo[i + 1]]
        hs = ht + ho[i]; he = ht + ho[i + 1] if i + 1 < len(ho) else len(d)
        out.append((d[hs:he], body))
    return out


def parse_vab(h):
    if h[:4] != b"pBAV": raise ValueError("no pBAV")
    ver, vid, fsz, _, ps, ts, vs, mvol, pan, a1, a2 = struct.unpack("<IIIHHHHBBBB", h[4:28])
    progs = []; o = 32
    for p in range(128):
        progs.append(struct.unpack("<8B H", h[o:o + 10])); o += 16     # tones, mvol, prior, mode, mpan, rsvd, attr, rsvd, parent
    tones = []
    o = 32 + 128 * 16
    for p in range(ps):
        for t in range(16):
            r = struct.unpack("<16B 6H", h[o:o + 28]); o += 32
            tones.append(dict(prog=p, tone=t, prior=r[0], mode=r[1], vol=r[2], pan=r[3], center=r[4], shift=r[5], min=r[6], max=r[7], pbmin=r[12], pbmax=r[13],
                              adsr1=r[16], adsr2=r[17], vag=r[19]))
    sizes = struct.unpack("<256H", h[o:o + 512])
    return dict(progs=progs, ps=ps, ts=ts, vs=vs, mvol=mvol, pan=pan, tones=tones, sizes=[s * 8 for s in sizes[:vs + 1]])


def extract_wvd(path, outdir, rate=VAG_RATE, fmt="wav", log=print):
    d = open(path, "rb").read()
    os.makedirs(outdir, exist_ok=True)
    info = []; n = 0
    for bi, (h, body) in enumerate(split_wvd(d)):
        v = parse_vab(h)
        # sizes[k] = byte size of VAG k (1 based, as the tones store it; sizes[0] = 0); bodies are laid out back to back
        off = 0; sizes = v["sizes"]; vags = []
        for k in range(1, v["vs"] + 1):
            sz = sizes[k]
            if sz == 0: continue
            chunk = body[off:off + sz]; off += sz
            pcm, ls, le = decode_vag(chunk)
            if not pcm: continue
            name = "bank%d_vag%03d" % (bi, k)
            name = os.path.basename(write_audio(os.path.join(outdir, name), [pcm], rate, fmt, (ls, le or len(pcm)) if ls is not None else None, log=log))
            vags.append(dict(file=name, vag=k, bytes=sz, samples=len(pcm), loop=ls is not None)); n += 1
        info.append(dict(bank=bi, programs=v["ps"], tones=[t for t in v["tones"] if t["vag"]], vags=vags, body=len(body), used=off))
    with open(os.path.join(outdir, "vab.json"), "w") as f: json.dump(info, f, indent=1)
    return n


# ---------------------------------------------------------------- SEQ -> MIDI
def vlq(n):
    b = [n & 0x7F]; n >>= 7
    while n: b.append((n & 0x7F) | 0x80); n >>= 7
    return bytes(reversed(b))


def seq_to_midi(d):
    """Sony 'pQES' (BE: version u32, resolution u16, tempo u24 us/quarter, time signature num + log2 den, then standard MIDI events with
    delta times and running status, ending FF 2F 00) -> SMF type 0."""
    if d[:4] != b"pQES": raise ValueError("not a SEQ")
    ver, res = struct.unpack(">IH", d[4:10]); tempo = int.from_bytes(d[10:13], "big"); num, den = d[13], d[14]
    ev = d[15:]
    trk = b"\x00\xff\x51\x03" + tempo.to_bytes(3, "big") + b"\x00\xff\x58\x04" + bytes([num, den, 24, 8]) + ev
    if not trk.endswith(b"\xff\x2f\x00"): trk += b"\x00\xff\x2f\x00"
    return b"MThd" + struct.pack(">IHHH", 6, 0, 1, res) + b"MTrk" + struct.pack(">I", len(trk)) + trk


# ---------------------------------------------------------------- software sequencer (SEQ + VAB -> stereo WAV/MP3)
# Which bank plays which sequence.  Derived from the disc load lists (src/cdLoadTask.c: every S_*.SEQ entry follows the SND*.WVD entry
# it is loaded with; the names differ for the "_" / "X" variants).  Bank 0 of the WVD is the music/area bank (its program numbers are the
# ones the SEQ selects with program changes, checked for all 36), bank 1 (if present) is a shared sound effect bank that is not used here.
# S_AR140 is loaded with SND0140..SND0146 (same music bank, only the effects bank differs); SND0140 is used.
SEQ_BANK = {
    "S_TITLE": "SND_TTL", "S_SYS00": "SND_SYS", "S_END": "SND_END",
    "S_AR000": "SND0000", "S_AR003": "SND0003", "S_AR010": "SND001_", "S_AR010X": "SND001_X", "S_AR020": "SND0020", "S_AR023": "SND0023",
    "S_AR030": "SND0030", "S_AR030X": "SND0030X", "S_AR032": "SND0032", "S_AR032X": "SND0032X", "S_AR040": "SND0040", "S_AR040X": "SND004_X",
    "S_AR044": "SND0044", "S_AR050": "SND0050", "S_AR060": "SND0060", "S_AR062": "SND0062", "S_AR080": "SND0080", "S_AR090": "SND0090",
    "S_AR091": "SND0091", "S_AR096": "SND0096", "S_AR100": "SND010_", "S_AR100X": "SND010_X", "S_AR103": "SND0103", "S_AR103X": "SND0103X",
    "S_AR110": "SND0110", "S_AR130": "SND0130", "S_AR140": "SND0140", "S_AR147": "SND0147", "S_AR160": "SND0160", "S_AR170": "SND0170",
    "S_AR181": "SND0181", "S_AR1802": "SND0182", "S_AR192": "SND0192",
}
FADE_SECONDS = 4.0       # fade-out at the end of the second pass of the loop
TAIL_SECONDS = 1.5       # release tails after the end (also faded)
MAX_RELEASE = 4.0        # seconds; longer SPU releases are cut with a short fade
MAX_SONG = 600.0


def parse_seq(d):
    """-> (resolution, initial tempo us/quarter, events).  events = [(tick, order, kind, a, b, c)] in file order with kinds
    'on' (ch, key, vel), 'off' (ch, key), 'prog' (ch, n), 'cc' (ch, n, v), 'bend' (ch, 14 bit value), 'tempo' (us), and the loop points
    (CC99 = 20 loop start, 30 loop end, the Sony libsnd convention used by this game) in loop = (start tick, end tick) or None."""
    if d[:4] != b"pQES": raise ValueError("not a SEQ")
    res = struct.unpack(">H", d[8:10])[0]; tempo = int.from_bytes(d[10:13], "big")
    ev = d[15:]; i = 0; t = 0; rs = None; out = []; ls = le = None
    while i < len(ev):
        dt = 0
        while True:
            b = ev[i]; i += 1; dt = (dt << 7) | (b & 127)
            if not b & 128: break
        t += dt; s = ev[i]
        if s == 0xFF:
            ty = ev[i + 1]; ln = ev[i + 2]; body = ev[i + 3:i + 3 + ln]; i += 3 + ln
            if ty == 0x51 and ln == 3: out.append((t, 'tempo', int.from_bytes(body, "big"), 0, 0))
            if ty == 0x2F: break
            continue
        if s & 128: rs = s; i += 1
        hi = rs >> 4; ch = rs & 15
        if hi in (0xC, 0xD): a = ev[i]; b = 0; i += 1
        else: a = ev[i]; b = ev[i + 1]; i += 2
        if hi == 9: out.append((t, 'on' if b else 'off', ch, a, b))
        elif hi == 8: out.append((t, 'off', ch, a, 0))
        elif hi == 0xC: out.append((t, 'prog', ch, a, 0))
        elif hi == 0xB:
            if a == 99 and b == 20 and ls is None: ls = t
            elif a == 99 and b == 30 and ls is not None and le is None: le = t
            out.append((t, 'cc', ch, a, b))
        elif hi == 0xE: out.append((t, 'bend', ch, a | (b << 7), 0))
    return res, tempo, out, ((ls, le) if ls is not None and le is not None and le > ls else None), t


class Bank:
    """One VAB (bank 0 of a WVD): decoded samples as float arrays + program/tone lookup."""
    def __init__(self, hdr, body):
        import numpy as np
        self.np = np; v = parse_vab(hdr); self.v = v; self.mvol = v["mvol"]
        self.vag = {}; off = 0
        for k in range(1, v["vs"] + 1):
            sz = v["sizes"][k]
            if not sz: continue
            pcm, ls, le = decode_vag(body[off:off + sz]); off += sz
            if not pcm: continue
            x = np.array(pcm, dtype=np.float32) / 32768.0
            if ls is not None and le and le > ls:
                x = np.concatenate([x[:le], x[ls:ls + 1]]); self.vag[k] = (x, ls, le)      # x[le] = x[ls]: interpolation across the loop point
            else:
                self.vag[k] = (np.concatenate([x, np.zeros(1, np.float32)]), None, None)
        self.prog = {}      # program number -> (program attrs, [tones])
        used = [p for p in range(128) if v["progs"][p][0]]
        for gi, p in enumerate(used):
            self.prog[p] = (v["progs"][p], [t for t in v["tones"] if t["prog"] == gi][:v["progs"][p][0]])


def _seg(np, level0, up, exp, shift, step, n):
    """SPU envelope segment of n samples (levels 0..32767, float).  Step per `wait` samples: increase +7-step, decrease -(8-step), scaled
    by 2^(11-shift) for shift < 11 and waiting 2^(shift-11) samples for shift > 11; exponential decrease multiplies the step by the level;
    exponential increase waits four times longer above 0x6000."""
    t = np.arange(n); sc = max(0, 11 - shift); w = 1 << max(0, shift - 11)
    if up:
        s = (7 - step) << sc
        if not exp: lv = level0 + (t // w) * float(s)
        else:
            k1 = max(0, math.ceil((0x6000 - level0) / s)) if level0 < 0x6000 else 0; t1 = k1 * w
            lv = np.where(t < t1, level0 + (t // w) * float(s), level0 + k1 * float(s) + ((t - t1) // (4 * w)) * float(s))
        return np.minimum(lv, 32767.0)
    s = (8 - step) << sc
    if not exp: return np.maximum(level0 - (t // w) * float(s), 0.0)
    return level0 * (1.0 - s / 32768.0) ** (t // w)


def make_envelope(np, a1, a2, n_on, cache={}):
    """Float32 gain 0..1 for a note held n_on samples then released: attack, decay to the sustain level, sustain (mode bits of ADSR2), release."""
    key = (a1, a2, n_on)
    if key in cache: return cache[key]
    ar_exp = a1 >> 15; ash = (a1 >> 10) & 31; ast = (a1 >> 8) & 3; dsh = (a1 >> 4) & 15; sl = a1 & 15
    sexp = a2 >> 15; sdec = (a2 >> 14) & 1; ssh = (a2 >> 8) & 31; sst = (a2 >> 6) & 3; rexp = (a2 >> 5) & 1; rsh = a2 & 31
    parts = []; have = 0; lvl = 0.0
    # attack
    sA = (7 - ast) << max(0, 11 - ash); wA = 1 << max(0, ash - 11)
    if ar_exp and sA:
        k1 = math.ceil(0x6000 / sA); na = k1 * wA + math.ceil((32767 - k1 * sA) / sA) * 4 * wA if 32767 > k1 * sA else k1 * wA
    else: na = math.ceil(32767 / sA) * wA
    na = min(na + 1, n_on)
    seg = _seg(np, 0.0, True, bool(ar_exp), ash, ast, na); parts.append(seg); have = na; lvl = float(seg[-1]) if na else 0.0
    # decay
    SL = (sl + 1) * 0x800
    if have < n_on and lvl >= 32767 and SL < 32768:
        sD = (8 << max(0, 11 - dsh)); wD = 1 << max(0, dsh - 11); f = 1.0 - sD / 32768.0
        nd = math.ceil(math.log(SL / 32767.0) / math.log(f)) * wD if SL < 32767 else 0
        nd = min(nd, n_on - have)
        if nd > 0:
            seg = 32767.0 * (1.0 - sD / 32768.0) ** (np.arange(nd) // wD)
            parts.append(seg); have += nd; lvl = float(seg[-1])
    # sustain
    if have < n_on:
        rest = n_on - have
        seg = _seg(np, lvl, not sdec, bool(sexp), ssh, sst, rest)
        dead = np.nonzero(seg < 2.0)[0] if sdec else []
        if len(dead): seg = seg[:dead[0]]; n_on = have + len(seg)
        parts.append(seg); have += len(seg); lvl = float(seg[-1]) if len(seg) else lvl
    on = np.concatenate(parts) if parts else np.zeros(0)
    # release
    if lvl > 2.0:
        sR = (8 << max(0, 11 - rsh)); wR = 1 << max(0, rsh - 11)
        if rexp:
            f = 1.0 - sR / 32768.0
            nr = math.ceil(math.log(2.0 / lvl) / math.log(f)) * wR if f > 0 else wR
        else: nr = math.ceil(lvl / sR) * wR
        cap = int(MAX_RELEASE * 44100); nr = max(1, min(nr, cap))
        t = np.arange(nr)
        rel = lvl * (1.0 - sR / 32768.0) ** (t // wR) if rexp else np.maximum(lvl - (t // wR) * float(sR), 0.0)
        if nr == cap: rel *= np.minimum(1.0, (cap - t) / 4410.0)
        on = np.concatenate([on, rel])
    env = (on / 32767.0).astype(np.float32)
    if len(cache) > 400: cache.clear()
    cache[key] = env
    return env


def render_song(seq, wvd, log=None, loop_passes=2):
    """SEQ bytes + WVD bytes -> (left, right) numpy int16 arrays at 44100 Hz.

    Voices: every note-on plays all tones of its program whose key range contains the key (layers).  Pitch = 2^((key - center + fine/128 +
    bend)/12) at the 44.1 kHz base rate (linear interpolation, sample loop from the VAG loop flags); gain = velocity * CC7 * CC11 * program
    volume * tone volume * VAB master volume (all /127); pan = CC10 + (tone pan - 64) + (program pan - 64), constant power.  CC7/CC10/pitch
    bend are sampled at note-on.  No voice limit (the SPU has 24) and no reverb.  Tempo events are honoured.
    Looping: the song is played to the loop end (CC99=30), the loop body (CC99=20..30) is played once more (loop_passes=2 passes of the body
    in total), the last FADE_SECONDS fade out, TAIL_SECONDS of release tails.  Songs without loop markers play once with their tails.
    The result is peak normalised to 0.89 full scale."""
    import numpy as np
    res, tempo, ev, loop, endt = parse_seq(seq)
    hdr, body = split_wvd(wvd)[0]
    bank = Bank(hdr, body)
    # expand the loop: intro + body, then (loop_passes - 1) more copies of the body
    ev = [(t, 1, k, a, b, c) for (t, k, a, b, c) in ev]
    if loop:
        a0, b0 = loop; full = [e for e in ev if e[0] < b0]; endt = b0
        for p in range(loop_passes - 1):
            off = (b0 - a0) * (p + 1)
            full.append((b0 + (b0 - a0) * p, 0, 'alloff', 0, 0, 0))
            full += [(e[0] + off, 1, e[2], e[3], e[4], e[5]) for e in ev if a0 <= e[0] < b0]
            endt = b0 + off
        ev = full
    ev.append((endt, 2, 'alloff', 0, 0, 0))
    ev.sort(key=lambda e: (e[0], e[1]))
    # tick -> sample
    sr = 44100; cur_t = 0; cur_s = 0.0; spt = tempo * 1e-6 / res * sr
    chans = [dict(prog=0, vol=127, pan=64, expr=127, bend=8192) for _ in range(16)]
    live = {}; notes = []        # live[(ch,key)] -> list of note records [start, end, ch, key, vel, prog, vol, pan, expr, bend]
    for (t, _o, k, a, b, c) in ev:
        cur_s += (t - cur_t) * spt; cur_t = t; s = int(cur_s)
        if k == 'tempo': spt = a * 1e-6 / res * sr
        elif k == 'prog': chans[a]['prog'] = b
        elif k == 'cc':
            st = chans[a]
            if b == 7: st['vol'] = c
            elif b == 10: st['pan'] = c
            elif b == 11: st['expr'] = c
        elif k == 'bend': chans[a]['bend'] = b
        elif k == 'on':
            st = chans[a]; n = [s, None, a, b, c, st['prog'], st['vol'], st['pan'], st['expr'], st['bend']]
            live.setdefault((a, b), []).append(n); notes.append(n)
        elif k == 'off':
            q = live.get((a, b))
            if q: q.pop(0)[1] = s
        elif k == 'alloff':
            for q in live.values():
                for n in q: n[1] = s
            live = {}
    end_s = int(cur_s)
    if end_s > MAX_SONG * sr: end_s = int(MAX_SONG * sr)
    total = end_s + int((TAIL_SECONDS + MAX_RELEASE) * sr) + 8
    buf = np.zeros((2, total), np.float32)
    for st_, en, ch, key, vel, prog, vol, pan, expr, bend in notes:
        if st_ >= end_s: continue
        pr = bank.prog.get(prog)
        if not pr: continue
        pa, tones = pr
        n_on = max(1, min(en if en is not None else end_s, end_s) - st_)
        for tn in tones:
            if not tn["vag"] or not tn["min"] <= key <= tn["max"] or tn["vag"] not in bank.vag: continue
            x, ls, le = bank.vag[tn["vag"]]
            bsem = (bend - 8192) / 8192.0 * ((tn["pbmax"] if bend >= 8192 else tn["pbmin"]) or 2)
            r = 2.0 ** ((key - tn["center"] + tn["shift"] / 128.0 + bsem) / 12.0)
            env = make_envelope(np, tn["adsr1"], tn["adsr2"], n_on)
            n = len(env)
            pos = np.arange(n, dtype=np.float64) * r
            if ls is None:
                m = int(np.searchsorted(pos, len(x) - 1))        # one shot: the voice ends with the sample
                if m < n: n = m; env = env[:n]; pos = pos[:n]
            else:
                span = le - ls
                pos = np.where(pos >= le, ls + (pos - ls) % span, pos)
            if n <= 0: continue
            i0 = pos.astype(np.int64); fr = (pos - i0).astype(np.float32)
            y = (x[i0] * (1.0 - fr) + x[i0 + 1] * fr) * env
            g = (vel / 127.0) * (vol / 127.0) * (expr / 127.0) * (pa[1] / 127.0) * (tn["vol"] / 127.0) * (bank.mvol / 127.0)
            pn = min(127, max(0, pan + (tn["pan"] - 64) + (pa[4] - 64))) / 127.0 * (math.pi / 2)
            e = min(total, st_ + n)
            y = y[:e - st_] * g
            buf[0, st_:e] += y * (math.cos(pn) * math.sqrt(2)); buf[1, st_:e] += y * (math.sin(pn) * math.sqrt(2))
    out_len = end_s + int(TAIL_SECONDS * sr)
    buf = buf[:, :out_len]
    if loop:       # fade out from FADE_SECONDS before the end of the last pass to the end of the tails
        f0 = max(0, end_s - int(FADE_SECONDS * sr)); ramp = np.ones(out_len, np.float32)
        ramp[f0:] = np.linspace(1.0, 0.0, out_len - f0, dtype=np.float32) ** 1.5
        buf *= ramp
    else:          # trim near-silent tail
        nz = np.nonzero(np.abs(buf).max(0) > 1e-4)[0]
        if len(nz): buf = buf[:, :min(out_len, int(nz[-1]) + sr // 10)]
    pk = float(np.abs(buf).max())
    if pk > 0: buf *= 0.89 / pk
    pcm = (buf * 32767.0).round().astype(np.int16)
    return pcm[0], pcm[1]


def render_music(seq_files, wvd_files, outdir, fmt="wav", bitrate=MP3_BITRATE, log=print):
    """seq_files / wvd_files: {basename without extension: bytes}.  Writes outdir/<SEQ>.wav|mp3 for every sequence with a known bank."""
    os.makedirs(outdir, exist_ok=True); n = 0
    for name in sorted(seq_files):
        bank = SEQ_BANK.get(name)
        if bank is None or bank not in wvd_files:
            log("music_audio: no bank for %s, skipped" % name); continue
        l, r = render_song(seq_files[name], wvd_files[bank])
        write_audio(os.path.join(outdir, name), [l, r], 44100, fmt, None, bitrate, log); n += 1
    return n


# ---------------------------------------------------------------- raw disc access / XA
def iso_files(img):
    """{path: (lba, size)} from the ISO9660 directory tree of a raw 2352 byte/sector image."""
    f = open(img, "rb")
    def rd(lba, size):
        b = []
        for i in range((size + 2047) // 2048):
            f.seek((lba + i) * 2352 + 24); b.append(f.read(2048))
        return b"".join(b)
    root = rd(16, 2048)[156:190]; res = {}
    def walk(lba, size, path):
        d = rd(lba, size); i = 0
        while i < size:
            l = d[i]
            if l == 0: i = (i // 2048 + 1) * 2048; continue
            el, es = struct.unpack("<I", d[i + 2:i + 6])[0], struct.unpack("<I", d[i + 10:i + 14])[0]
            fl, nl = d[i + 25], d[i + 32]; nm = d[i + 33:i + 33 + nl]
            if nm not in (b"\x00", b"\x01"):
                n = nm.decode("ascii", "replace").split(";")[0]
                if fl & 2: walk(el, es, path + n + "/")
                else: res[path + n] = (el, es)
            i += l
    walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "")
    f.close()
    return res


def sectors(img, lba, size):
    """Yield (file, channel, submode, coding, 2328 bytes after the subheader) for each sector of a file (size rounded up to 2048)."""
    with open(img, "rb") as f:
        f.seek(lba * 2352)
        for _ in range((size + 2047) // 2048):
            s = f.read(2352)
            if len(s) < 2352: break
            yield s[16], s[17], s[18], s[19], s[24:2352]


XA_F = ((0, 0), (60, 0), (115, -52), (98, -55))


class XADecoder:
    """CD-XA ADPCM (4 bit) of one channel: feed 2304 byte audio payloads, get per channel sample lists."""
    def __init__(self, stereo):
        self.stereo = stereo; self.nc = 2 if stereo else 1
        self.out = [[] for _ in range(self.nc)]; self.st = [[0, 0] for _ in range(self.nc)]

    def feed(self, pay):
        for g in range(18):
            grp = pay[g * 128:(g + 1) * 128]
            for u in range(8):
                h = grp[4 + u]; shift = h & 15; flt = (h >> 4) & 3
                if shift > 12: shift = 9
                c1, c2 = XA_F[flt]
                ch = (u & 1) if self.stereo else 0
                s1, s2 = self.st[ch]; o = self.out[ch]; sh = 4 * (u & 1); base = 16 + (u >> 1)
                # NOTE: unit u uses nibble (u & 1) of byte (u >> 1) of each 4 byte word
                for k in range(28):
                    nib = (grp[base + 4 * k] >> sh) & 15
                    if nib & 8: nib -= 16
                    s = ((nib << 12) >> shift) + ((s1 * c1 + s2 * c2 + 32) >> 6)
                    s = -32768 if s < -32768 else 32767 if s > 32767 else s
                    o.append(s); s2 = s1; s1 = s
                self.st[ch] = [s1, s2]


def extract_xa(img, name, lba, size, outprefix, log=print, fmt="wav", bitrate=MP3_BITRATE):
    """Decode the audio sectors (submode bit 2) of one STR/XA file -> one WAV per (file, channel) pair. Returns the list of paths."""
    decs = {}
    for fn, ch, sm, cod, pay in sectors(img, lba, size):
        if not (sm & 4) or cod & 0x30: continue          # audio, 4 bit only
        key = (fn, ch, cod & 5)
        if key not in decs: decs[key] = XADecoder(bool(cod & 1))
        decs[key].feed(pay[:2304])
    paths = []
    for (fn, ch, cod), dec in sorted(decs.items()):
        rate = 18900 if cod & 4 else 37800
        p = write_audio("%s%s" % (outprefix, "" if len(decs) == 1 else "_ch%d" % ch), dec.out, rate, fmt, None, bitrate, log); paths.append(p)
    return paths


def wav_stats(path):
    w = wave.open(path); n = w.getnframes(); nc = w.getnchannels(); raw = w.readframes(n); w.close()
    v = struct.unpack("<%dh" % (len(raw) // 2), raw)
    rms = (sum(x * x for x in v) / max(len(v), 1)) ** .5
    return dict(seconds=n / w.getframerate(), channels=nc, rate=w.getframerate(), rms=rms, peak=max(map(abs, v)) if v else 0)


def export_all(img, out, log=print, xa=True, banks=True, music=True, music_audio=False, fmt="wav", bitrate=MP3_BITRATE):
    """fmt: 'wav' or 'mp3' for every audio output (banks, music_audio, movie audio); MIDI is always .mid."""
    if fmt not in AUDIO_FORMATS: raise ValueError("audio format must be one of %s" % ", ".join(AUDIO_FORMATS))
    snd = os.path.join(out, "sound")
    files = iso_files(img)
    cnt = dict(wvd=0, vag=0, seq=0, xa=0, rendered=0)
    seqs = {}; wvds = {}
    with open(img, "rb") as f:
        for path, (lba, size) in sorted(files.items()):
            if not path.startswith("SOUND/") or not (banks or music or music_audio): continue
            base = os.path.basename(path)
            if not ((path.endswith(".WVD") and (banks or music_audio)) or (path.endswith(".SEQ") and (music or music_audio))): continue
            data = []
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * 2352 + 24); data.append(f.read(2048))
            data = b"".join(data)[:size]
            if path.endswith(".WVD"):
                if music_audio: wvds[base[:-4]] = data
                if banks: cnt["vag"] += extract_wvd_bytes(data, os.path.join(snd, "bank", base[:-4]), fmt=fmt, log=log); cnt["wvd"] += 1
            else:
                if music_audio: seqs[base[:-4]] = data
                if music:
                    os.makedirs(os.path.join(snd, "music"), exist_ok=True)
                    with open(os.path.join(snd, "music", base[:-4] + ".mid"), "wb") as o: o.write(seq_to_midi(data))
                    cnt["seq"] += 1
    if banks or music: log("sound: %d banks -> %d VAG samples (%s), %d SEQ -> MIDI under %s" % (cnt["wvd"], cnt["vag"], fmt, cnt["seq"], snd))
    if music_audio:
        log("music_audio: rendering %d songs with their banks (software sequencer, a few seconds each) ..." % len(seqs))
        cnt["rendered"] = render_music(seqs, wvds, os.path.join(snd, "music_audio"), fmt, bitrate, log)
        log("sound: %d songs rendered (%s, 44.1 kHz stereo) under %s" % (cnt["rendered"], fmt, os.path.join(snd, "music_audio")))
    if xa:
        os.makedirs(os.path.join(snd, "movie_audio"), exist_ok=True)
        for path, (lba, size) in sorted(files.items()):
            if path.startswith("MOVIE/") and path.endswith(".STR"):
                ps = extract_xa(img, path, lba, size, os.path.join(snd, "movie_audio", os.path.basename(path)[:-4]), fmt=fmt, bitrate=bitrate)
                cnt["xa"] += len(ps)
        log("sound: %d movie audio tracks (XA ADPCM, %s) under %s" % (cnt["xa"], fmt, os.path.join(snd, "movie_audio")))
    return cnt


def extract_wvd_bytes(d, outdir, rate=VAG_RATE, fmt="wav", log=print):
    import tempfile
    tmp = tempfile.NamedTemporaryFile(delete=False, suffix=".WVD"); tmp.write(d); tmp.close()
    try: return extract_wvd(tmp.name, outdir, rate, fmt, log)
    finally: os.unlink(tmp.name)


def main():
    a = sys.argv[1:]; fmt = "wav"; br = MP3_BITRATE; rest = []
    it = iter(a)
    for x in it:
        if x == "--audio-format": fmt = next(it, "")
        elif x.startswith("--audio-format="): fmt = x.split("=", 1)[1]
        elif x == "--bitrate": br = int(next(it, "192"))
        else: rest.append(x)
    a = rest
    if fmt not in AUDIO_FORMATS or not a: sys.exit(__doc__)
    cmd = a[0]
    if cmd == "render" and len(a) == 4:
        l, r = render_song(open(a[1], "rb").read(), open(a[2], "rb").read())
        print(write_audio(os.path.splitext(a[3])[0], [l, r], 44100, fmt, None, br))
    elif len(a) != 3: sys.exit(__doc__)
    elif cmd == "wvd": print(extract_wvd(a[1], a[2], fmt=fmt), "samples")
    elif cmd == "seq": open(a[2], "wb").write(seq_to_midi(open(a[1], "rb").read()))
    elif cmd == "xa": [print(p) for path, (l, s) in sorted(iso_files(a[1]).items()) if path.endswith(".STR") for p in extract_xa(a[1], path, l, s, os.path.join(a[2], os.path.basename(path)[:-4]), fmt=fmt, bitrate=br)]
    elif cmd == "music": export_all(a[1], a[2], xa=False, banks=False, music=False, music_audio=True, fmt=fmt, bitrate=br)
    elif cmd == "all": export_all(a[1], a[2], music_audio=True, fmt=fmt, bitrate=br)
    else: sys.exit(__doc__)


if __name__ == "__main__":
    main()
