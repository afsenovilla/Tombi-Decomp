#!/usr/bin/env python3
"""Tombi! (PAL) STR movie extraction: demux + pure Python MDEC (BS v2) decoder, no dependencies.

  python3 tools/str.py info   Tombi.bin                     list the MOVIE/*.STR files (frames, size, audio)
  python3 tools/str.py frames Tombi.bin NAME OUT_DIR [N] [STEP]   PNG frames of MOVIE/NAME.STR (every STEP-th, at most N; slow: ~0.3 s/frame)
  python3 tools/str.py mp4    Tombi.bin NAME OUT.mp4        H.264 + AAC through ffmpeg (only if ffmpeg is in PATH)
  python3 tools/str.py all    Tombi.bin OUT_DIR             posters (OUT_DIR/video/NAME.png) and, with ffmpeg, OUT_DIR/video/NAME.mp4

The .STR files are CD-XA streams: 2352 byte raw sectors, video sectors (Form 1, 2016 bytes of payload after a 32 byte header: 0x0160,
0x8001, chunk, nchunks, frame, demux size, w, h, ...) interleaved 7:1 with XA audio sectors (audio.py).  A frame is a Bitstream v2 of
16x16 macroblocks stored column by column, 6 blocks each (Cr, Cb, Y0..Y3); format notes in notes/formats.md."""
import os, sys, struct, zlib, math, shutil, subprocess, tempfile

# (code length without sign bit, code, run, level) of the MPEG-1 intra AC table used by the MDEC (from the standard); escape = 000001
VLC = (
    (2,3,0,1),(4,4,0,2),(5,5,0,3),(7,6,0,4),(8,38,0,5),(8,33,0,6),(10,10,0,7),(12,29,0,8),(12,24,0,9),(12,19,0,10),
    (12,16,0,11),(13,26,0,12),(13,25,0,13),(13,24,0,14),(13,23,0,15),(14,31,0,16),(14,30,0,17),(14,29,0,18),(14,28,0,19),
    (14,27,0,20),(14,26,0,21),(14,25,0,22),(14,24,0,23),(14,23,0,24),(14,22,0,25),(14,21,0,26),(14,20,0,27),(14,19,0,28),
    (14,18,0,29),(14,17,0,30),(14,16,0,31),(15,24,0,32),(15,23,0,33),(15,22,0,34),(15,21,0,35),(15,20,0,36),(15,19,0,37),
    (15,18,0,38),(15,17,0,39),(15,16,0,40),(3,3,1,1),(6,6,1,2),(8,37,1,3),(10,12,1,4),(12,27,1,5),(13,22,1,6),(13,21,1,7),
    (15,31,1,8),(15,30,1,9),(15,29,1,10),(15,28,1,11),(15,27,1,12),(15,26,1,13),(15,25,1,14),(16,19,1,15),(16,18,1,16),
    (16,17,1,17),(16,16,1,18),(4,5,2,1),(7,4,2,2),(10,11,2,3),(12,20,2,4),(13,20,2,5),(5,7,3,1),(8,36,3,2),(12,28,3,3),
    (13,19,3,4),(5,6,4,1),(10,15,4,2),(12,18,4,3),(6,7,5,1),(10,9,5,2),(13,18,5,3),(6,5,6,1),(12,30,6,2),(16,20,6,3),
    (6,4,7,1),(12,21,7,2),(7,7,8,1),(12,17,8,2),(7,5,9,1),(13,17,9,2),(8,39,10,1),(13,16,10,2),(8,35,11,1),(16,26,11,2),
    (8,34,12,1),(16,25,12,2),(8,32,13,1),(16,24,13,2),(10,14,14,1),(16,23,14,2),(10,13,15,1),(16,22,15,2),(10,8,16,1),
    (16,21,16,2),(12,31,17,1),(12,26,18,1),(12,25,19,1),(12,23,20,1),(12,22,21,1),(13,31,22,1),(13,30,23,1),(13,29,24,1),
    (13,28,25,1),(13,27,26,1),(16,31,27,1),(16,30,28,1),(16,29,29,1),(16,28,30,1),(16,27,31,1),
)
ZIGZAG = (0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,35,42,49,56,57,50,43,36,29,22,15,23,30,
          37,44,51,58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63)
QUANT = (8,16,19,22,26,27,29,34, 16,16,22,24,27,29,34,37, 19,22,26,27,29,34,34,38, 22,22,26,27,29,34,37,40,
         22,26,27,29,32,35,40,48, 26,27,29,32,35,40,48,58, 26,27,29,34,38,46,56,69, 27,29,35,38,46,56,69,83)

# 16 bit lookup: (bits used incl. sign, run, level); level 0 = escape, 127 = end of block
LUT = [None] * 65536
for _l, _c, _r, _v in VLC:
    for _i in range(1 << (16 - _l)):
        LUT[(_c << (16 - _l)) | _i] = (_l + 1, _r, _v)
for _i in range(1 << 10): LUT[(1 << 10) | _i] = (6, 0, 0)
for _i in range(1 << 14): LUT[(2 << 14) | _i] = (2, 0, 127)

_B = [[(0.5 * math.sqrt(0.5) if u == 0 else 0.5) * math.cos((2 * x + 1) * u * math.pi / 16) for x in range(8)] for u in range(8)]


def idct(coef):
    """coef: dict raster index (v*8+u) -> value.  Returns 64 floats (row major) = orthonormal 2D IDCT."""
    cols = {}
    for k, val in coef.items():
        cols.setdefault(k & 7, []).append((k >> 3, val))
    col = {}
    for u, lst in cols.items():
        col[u] = [sum(val * _B[v][y] for v, val in lst) for y in range(8)]
    out = [0.0] * 64
    for u, cv in col.items():
        bu = _B[u]
        for y in range(8):
            c = cv[y]
            if c:
                o = y * 8
                for x in range(8): out[o + x] += c * bu[x]
    return out


class Bits:
    def __init__(self, data):
        if len(data) & 1: data += b"\0"
        self.w = struct.unpack("<%dH" % (len(data) // 2), data) + (0, 0, 0)
        self.p = 0
    def peek16(self):
        p = self.p; i = p >> 4; v = (self.w[i] << 16) | self.w[i + 1]
        return (v >> (16 - (p & 15))) & 0xFFFF
    def get(self, n):
        p = self.p; i = p >> 4; v = (self.w[i] << 32) | (self.w[i + 1] << 16) | self.w[i + 2]
        self.p = p + n
        return (v >> (48 - (p & 15) - n)) & ((1 << n) - 1)


# BS v3 DC differentials: MPEG-1 intra DC size VLCs (luma, chroma): size = index
DC_LUM = ((3, 4), (2, 0), (2, 1), (3, 5), (3, 6), (4, 14), (5, 30), (6, 62), (7, 126), (8, 254), (9, 510), (9, 511))
DC_CHR = ((2, 0), (2, 1), (2, 2), (3, 6), (4, 14), (5, 30), (6, 62), (7, 126), (8, 254), (9, 510), (10, 1022), (10, 1023))


def dc_size(br, tab):
    pk = br.peek16()
    for size, (l, c) in enumerate(tab):
        if pk >> (16 - l) == c:
            br.p += l; return size
    raise ValueError("bad DC VLC")


def decode_block(br, qscale, ver=2, comp=0, last_dc=None):
    """-> dict of dequantised coefficients (raster index -> value), pixel offset 128 removed.
    BS v2: 10 bit signed DC (x2).  BS v3: DC difference (VLC size + bits) against the previous block of the same component
    (Y shared by the 4 luma blocks, Cr, Cb), start 128.  Then run/level pairs until EOB."""
    if ver == 2:
        dc = br.get(10)
        if dc & 512: dc -= 1024
        coef = {0: 2 * dc}
    else:
        size = dc_size(br, DC_CHR if comp else DC_LUM)
        if size:
            c = br.get(size)
            diff = c if c >= 1 << (size - 1) else c - (1 << size) + 1
        else: diff = 0
        last_dc[comp] += diff
        coef = {0: last_dc[comp] * 8 - 1024}
    i = 0
    while True:
        e = LUT[br.peek16()]
        if e is None: raise ValueError("bad VLC")
        n, run, level = e
        if level == 127:
            br.p += 2; break
        if level:
            sg = br.get(n)  # code + sign bit
            neg = sg & 1
            i += run + 1
            if i > 63: raise ValueError("coefficient overflow")
            j = ZIGZAG[i]; v = (level * qscale * QUANT[j]) >> 3
            coef[j] = -v if neg else v
        else:
            br.p += 6
            run = br.get(6) + 1; level = br.get(10)
            if level & 512: level -= 1024
            i += run
            if i > 63: raise ValueError("coefficient overflow")
            j = ZIGZAG[i]
            a = (abs(level) * qscale * QUANT[j]) >> 3
            a = (a - 1) | 1
            coef[j] = -a if level < 0 else a
    return coef


def decode_frame(bs, w, h):
    """bs = frame data from the 8 byte header (u16 words, qscale, version) on.  Returns RGB bytes (w*h*3)."""
    nwords, magic, qscale, ver = struct.unpack("<4H", bs[:8])
    if ver not in (2, 3): raise ValueError("BS version %d not supported" % ver)
    last_dc = [128, 128, 128]
    br = Bits(bs[8:])
    mbw, mbh = (w + 15) // 16, (h + 15) // 16
    W, H = mbw * 16, mbh * 16
    rgb = bytearray(W * H * 3)
    for mx in range(mbw):
        for my in range(mbh):
            blk = [idct(decode_block(br, qscale, ver, (2 if k == 0 else 1 if k == 1 else 0), last_dc)) for k in range(6)]
            cr, cb = blk[0], blk[1]
            for by in range(2):
                for bx in range(2):
                    yb = blk[2 + by * 2 + bx]
                    for yy in range(8):
                        py = my * 16 + by * 8 + yy
                        o = (py * W + mx * 16 + bx * 8) * 3
                        for xx in range(8):
                            ci = ((by * 8 + yy) >> 1) * 8 + ((bx * 8 + xx) >> 1)
                            Y = yb[yy * 8 + xx] + 128; r = cr[ci]; b = cb[ci]
                            R = Y + 1.402 * r; G = Y - 0.3437 * b - 0.7143 * r; B = Y + 1.772 * b
                            rgb[o] = 0 if R < 0 else 255 if R > 255 else int(R + .5)
                            rgb[o + 1] = 0 if G < 0 else 255 if G > 255 else int(G + .5)
                            rgb[o + 2] = 0 if B < 0 else 255 if B > 255 else int(B + .5)
                            o += 3
    if W != w:
        rgb = b"".join(bytes(rgb[y * W * 3:y * W * 3 + w * 3]) for y in range(h))
    return bytes(rgb[:w * h * 3]) if W == w else rgb


def write_png(path, w, h, rgb):
    raw = b"".join(b"\0" + rgb[y * w * 3:(y + 1) * w * 3] for y in range(h))
    def ch(t, d): return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + ch(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)) + ch(b"IDAT", zlib.compress(raw, 6)) + ch(b"IEND", b""))


def frames(img, lba, size):
    """Yield (frame number, width, height, bitstream bytes) for every complete video frame of a STR file."""
    from audio import sectors
    cur = None
    for fn, ch, sm, cod, pay in sectors(img, lba, size):
        if sm & 4 or pay[0:4] != b"\x60\x01\x01\x80": continue
        idx, n, num, dsz, w, h = struct.unpack("<HHIIHH", pay[4:20])
        if cur is None or cur[0] != num: cur = [num, n, {}, dsz, w, h]
        cur[2][idx] = pay[32:32 + 2016]
        if len(cur[2]) == n:
            yield num, w, h, b"".join(cur[2][i] for i in range(n))[:dsz]
            cur = None


def str_files(img):
    from audio import iso_files
    return {p: v for p, v in iso_files(img).items() if p.startswith("MOVIE/") and p.endswith(".STR")}


def poster(img, lba, size, out, want=24):
    """Decode frame number `want` (or the last one before it) of a movie to a PNG. Returns (frame, w, h)."""
    last = None
    for num, w, h, bs in frames(img, lba, size):
        last = (num, w, h, bs)
        if num >= want: break
    num, w, h, bs = last
    write_png(out, w, h, decode_frame(bs, w, h)); return num, w, h


def to_mp4(img, lba, size, out):
    """ffmpeg demuxes the raw sectors itself (PSX STR demuxer + MDEC + XA ADPCM) and encodes H.264/AAC."""
    ff = shutil.which("ffmpeg")
    if not ff: return False
    with tempfile.NamedTemporaryFile(suffix=".str", delete=False) as t:
        with open(img, "rb") as f:
            f.seek(lba * 2352)
            for _ in range((size + 2047) // 2048): t.write(f.read(2352))
    try:
        r = subprocess.run([ff, "-v", "error", "-y", "-i", t.name, "-c:v", "libx264", "-pix_fmt", "yuv420p", "-crf", "20", "-c:a", "aac", "-b:a", "128k", out],
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if r.returncode:
            r = subprocess.run([ff, "-v", "error", "-y", "-i", t.name, "-c:v", "mpeg4", "-q:v", "3", "-c:a", "aac", out], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return r.returncode == 0 and os.path.getsize(out) > 0
    finally:
        os.unlink(t.name)


def export_all(img, out, log=print):
    vd = os.path.join(out, "video"); os.makedirs(vd, exist_ok=True)
    npng = nmp4 = 0
    for p, (lba, size) in sorted(str_files(img).items()):
        name = os.path.basename(p)[:-4]
        poster(img, lba, size, os.path.join(vd, name + ".png")); npng += 1
        if to_mp4(img, lba, size, os.path.join(vd, name + ".mp4")): nmp4 += 1
    log("video: %d poster frames, %d mp4 (needs ffmpeg in PATH%s) under %s" % (npng, nmp4, "" if nmp4 else ": not found / failed", vd))
    return npng, nmp4


def main():
    a = sys.argv[1:]
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    if len(a) >= 2 and a[0] == "info":
        for p, (lba, size) in sorted(str_files(a[1]).items()):
            fr = list(frames(a[1], lba, size)); print("%-18s lba %7d  %9d bytes  %5d frames  %dx%d" % (p, lba, size, len(fr), fr[0][1], fr[0][2]))
    elif len(a) >= 4 and a[0] == "frames":
        lba, size = str_files(a[1])["MOVIE/%s.STR" % a[2]]; os.makedirs(a[3], exist_ok=True)
        mx = int(a[4]) if len(a) > 4 else 1 << 30; step = int(a[5]) if len(a) > 5 else 1; n = 0
        for num, w, h, bs in frames(a[1], lba, size):
            if (num - 1) % step: continue
            write_png(os.path.join(a[3], "frame_%05d.png" % num), w, h, decode_frame(bs, w, h)); n += 1
            if n >= mx: break
        print(n, "frames")
    elif len(a) == 4 and a[0] == "mp4":
        lba, size = str_files(a[1])["MOVIE/%s.STR" % a[2]]; print("ok" if to_mp4(a[1], lba, size, a[3]) else "ffmpeg missing or failed")
    elif len(a) == 3 and a[0] == "all": export_all(a[1], a[2])
    else: sys.exit(__doc__)


if __name__ == "__main__":
    main()
