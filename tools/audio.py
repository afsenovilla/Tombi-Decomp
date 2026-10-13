#!/usr/bin/env python3
"""Tombi! (PAL) sound extraction, pure Python 3 (no dependencies).

  python3 tools/audio.py all  Tombi.bin OUT_DIR   everything below into OUT_DIR/sound
  python3 tools/audio.py wvd  FILE.WVD OUT_DIR    VAB header + every VAG sample as WAV (+ vab.json)
  python3 tools/audio.py seq  FILE.SEQ OUT.mid    Sony pQES sequence -> standard MIDI file (type 0)
  python3 tools/audio.py xa   Tombi.bin OUT_DIR   the XA ADPCM audio track of every MOVIE/*.STR as WAV

Layouts: notes/formats.md ("SOUND" and "MOVIE").  The XA audio is interleaved in the .STR files as Mode 2 Form 2 sectors, which
the ISO extractor (isoext.py) mangles, so this module reads the raw 2352 byte sectors of the image itself."""
import os, struct, sys, json, wave

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
    if nc == 1: pcm = struct.pack("<%dh" % n, *chans[0])
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


# ---------------------------------------------------------------- WVD / VAB
def split_wvd(d):
    """-> list of (vab_header_bytes, vb_body_bytes), one per bank."""
    bt, ht, _ = struct.unpack("<3I", d[:12])
    def tbl(t):
        n = struct.unpack("<I", d[t:t + 4])[0] // 4
        return struct.unpack("<%dI" % n, d[t:t + 4 * n])
    bo, ho = tbl(bt), tbl(ht)
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
        progs.append(struct.unpack("<8B H", h[o:o + 10])); o += 16
    tones = []
    o = 32 + 128 * 16
    for p in range(ps):
        for t in range(16):
            r = struct.unpack("<16B 6H", h[o:o + 28]); o += 32
            tones.append(dict(prog=p, tone=t, prior=r[0], mode=r[1], vol=r[2], pan=r[3], center=r[4], shift=r[5], min=r[6], max=r[7],
                              adsr1=r[16], adsr2=r[17], vag=r[19]))
    sizes = struct.unpack("<256H", h[o:o + 512])
    return dict(ps=ps, ts=ts, vs=vs, mvol=mvol, pan=pan, tones=tones, sizes=[s * 8 for s in sizes[:vs + 1]])


def extract_wvd(path, outdir, rate=VAG_RATE):
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
            name = "bank%d_vag%03d.wav" % (bi, k)
            write_wav(os.path.join(outdir, name), [pcm], rate, (ls, le or len(pcm)) if ls is not None else None)
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


def extract_xa(img, name, lba, size, outprefix, log=print):
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
        p = "%s%s.wav" % (outprefix, "" if len(decs) == 1 else "_ch%d" % ch)
        write_wav(p, dec.out, rate); paths.append(p)
    return paths


def wav_stats(path):
    w = wave.open(path); n = w.getnframes(); nc = w.getnchannels(); raw = w.readframes(n); w.close()
    v = struct.unpack("<%dh" % (len(raw) // 2), raw)
    rms = (sum(x * x for x in v) / max(len(v), 1)) ** .5
    return dict(seconds=n / w.getframerate(), channels=nc, rate=w.getframerate(), rms=rms, peak=max(map(abs, v)) if v else 0)


def export_all(img, out, log=print, xa=True):
    snd = os.path.join(out, "sound")
    files = iso_files(img)
    cnt = dict(wvd=0, vag=0, seq=0, xa=0)
    with open(img, "rb") as f:
        for path, (lba, size) in sorted(files.items()):
            if not path.startswith("SOUND/"): continue
            f.seek(lba * 2352); data = []
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * 2352 + 24); data.append(f.read(2048))
            data = b"".join(data)[:size]
            base = os.path.basename(path)
            if path.endswith(".WVD"):
                cnt["vag"] += extract_wvd_bytes(data, os.path.join(snd, "bank", base[:-4])); cnt["wvd"] += 1
            elif path.endswith(".SEQ"):
                os.makedirs(os.path.join(snd, "music"), exist_ok=True)
                with open(os.path.join(snd, "music", base[:-4] + ".mid"), "wb") as o: o.write(seq_to_midi(data))
                cnt["seq"] += 1
    log("sound: %d banks -> %d VAG WAVs, %d SEQ -> MIDI under %s" % (cnt["wvd"], cnt["vag"], cnt["seq"], snd))
    if xa:
        os.makedirs(os.path.join(snd, "movie_audio"), exist_ok=True)
        for path, (lba, size) in sorted(files.items()):
            if path.startswith("MOVIE/") and path.endswith(".STR"):
                ps = extract_xa(img, path, lba, size, os.path.join(snd, "movie_audio", os.path.basename(path)[:-4]))
                cnt["xa"] += len(ps)
        log("sound: %d movie audio tracks (XA ADPCM) under %s" % (cnt["xa"], os.path.join(snd, "movie_audio")))
    return cnt


def extract_wvd_bytes(d, outdir, rate=VAG_RATE):
    import tempfile
    tmp = tempfile.NamedTemporaryFile(delete=False, suffix=".WVD"); tmp.write(d); tmp.close()
    try: return extract_wvd(tmp.name, outdir, rate)
    finally: os.unlink(tmp.name)


def main():
    a = sys.argv[1:]
    if len(a) != 3: sys.exit(__doc__)
    cmd = a[0]
    if cmd == "wvd": print(extract_wvd(a[1], a[2]), "samples")
    elif cmd == "seq": open(a[2], "wb").write(seq_to_midi(open(a[1], "rb").read()))
    elif cmd == "xa": export_all(a[1], a[2]) if False else [print(p) for path, (l, s) in sorted(iso_files(a[1]).items()) if path.endswith(".STR") for p in extract_xa(a[1], path, l, s, os.path.join(a[2], os.path.basename(path)[:-4]))]
    elif cmd == "all": export_all(a[1], a[2])
    else: sys.exit(__doc__)


if __name__ == "__main__":
    main()
