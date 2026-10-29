#!/usr/bin/env python3
"""Read-only MAME CHD (v5) CD images in pure Python: the first data track as a file-like stream of sectors.

  python3 tools/chd.py info DISC.chd
  python3 tools/chd.py tobin DISC.chd OUT.bin        # the first track, 2352-byte raw sectors (as a .bin dump)

Codecs: cdzl (deflate), cdlz (LZMA + deflate subcode), cdfl (FLAC + deflate subcode, a small pure-Python FLAC frame
decoder: slow but CD-data CHDs use it for few hunks), and the generic zlib / lzma / none. Hunks that copy another
hunk (self references) work; parent CHDs (delta images) do not. Every hunk is checked against its CRC16.

Importable: ChdFile(path) is a read-only file object (seek/read/tell/close) over track 1 (2352 bytes per sector for
raw tracks, 2048 for cooked MODE1/MODE2_FORM1 tracks); is_chd(path).
"""
import binascii, lzma, struct, sys, zlib

FRAME = 2448            # CD frame in a CHD: 2352 sector bytes + 96 subcode bytes
SECTOR = 2352
SUB = 96
SYNC = b"\0" + b"\xff" * 10 + b"\0"


class ChdError(ValueError):
    pass


def is_chd(path):
    try:
        with open(path, "rb") as f:
            return f.read(8) == b"MComprHD"
    except OSError:
        return False


# ---- bit reader and Huffman (MAME util/bitstream.h, util/huffman.cpp) ----------------------------------------------

class Bits:
    def __init__(self, data):
        self.d = data
        self.p = 0              # bit position

    def read(self, n):
        if n == 0:
            return 0
        v = 0
        for _ in range(n):
            byte = self.p >> 3
            b = (self.d[byte] >> (7 - (self.p & 7))) & 1 if byte < len(self.d) else 0
            v = v << 1 | b
            self.p += 1
        return v


def huffman_rle_tree(bits, numcodes=16, maxbits=8):
    """import_tree_rle + assign_canonical_codes: {(length, code): symbol}."""
    nb = 5 if maxbits >= 16 else 4 if maxbits >= 8 else 3
    lens = []
    while len(lens) < numcodes:
        n = bits.read(nb)
        if n != 1:
            lens.append(n)
        else:
            n = bits.read(nb)
            if n == 1:
                lens.append(1)
            else:
                lens += [n] * (bits.read(nb) + 3)
    if len(lens) != numcodes:
        raise ChdError("bad Huffman tree in the map")
    histo = [0] * 33
    for n in lens:
        if n > maxbits:
            raise ChdError("bad Huffman tree in the map")
        histo[n] += 1
    cur = 0
    for ln in range(32, 0, -1):
        nxt = (cur + histo[ln]) >> 1
        if ln != 1 and nxt * 2 != cur + histo[ln]:
            raise ChdError("bad Huffman tree in the map")
        histo[ln] = cur
        cur = nxt
    codes = {}
    for sym, n in enumerate(lens):
        if n:
            codes[(n, histo[n])] = sym
            histo[n] += 1
    return codes


def huffman_decode(bits, codes):
    v, n = 0, 0
    while n < 32:
        v = v << 1 | bits.read(1)
        n += 1
        s = codes.get((n, v))
        if s is not None:
            return s
    raise ChdError("bad Huffman code in the map")


# ---- CD sector ECC (MAME cdrom.cpp ecc_generate) -------------------------------------------------------------------

_F = [0] * 256
_B = [0] * 256
for _i in range(256):
    _j = (_i << 1) ^ (0x11D if _i & 0x80 else 0)
    _F[_i] = _j
    _B[_i ^ _j] = _i


def _ecc_block(s, major_count, minor_count, major_mult, minor_inc, dest):
    size = major_count * minor_count
    for major in range(major_count):
        index = (major >> 1) * major_mult + (major & 1)
        a = b = 0
        for _ in range(minor_count):
            t = s[12 + index]
            index += minor_inc
            if index >= size:
                index -= size
            a ^= t
            b ^= t
            a = _F[a]
        a = _B[_F[a] ^ b]
        s[dest + major] = a
        s[dest + major + major_count] = a ^ b


def ecc_generate(sector):
    """P and Q parity of a raw sector (bytearray, in place). Mode 2 Form 1 computes them over a zeroed address header."""
    hdr = bytes(sector[12:16])
    if hdr[3] == 2:
        sector[12:16] = b"\0\0\0\0"
    _ecc_block(sector, 86, 24, 2, 86, 0x81C)
    _ecc_block(sector, 52, 43, 86, 88, 0x8C8)
    sector[12:16] = hdr


# ---- FLAC frames (enough for chdman's cdfl: 2 channels, 16 bits) ---------------------------------------------------

def _rice_signed(u):
    return (u >> 1) ^ -(u & 1)


class FlacBits(Bits):
    def unary(self):
        n = 0
        while self.read(1) == 0:
            n += 1
        return n

    def signed(self, n):
        v = self.read(n)
        return v - (1 << n) if n and v >> (n - 1) else v

    def align(self):
        self.p = (self.p + 7) & ~7


def _residual(bits, blocksize, order, out):
    method = bits.read(2)
    if method > 1:
        raise ChdError("FLAC: reserved residual coding")
    pbits, esc = (4, 15) if method == 0 else (5, 31)
    porder = bits.read(4)
    parts = 1 << porder
    for p in range(parts):
        n = (blocksize >> porder) - (order if p == 0 else 0)
        k = bits.read(pbits)
        if k == esc:
            w = bits.read(5)
            out.extend(bits.signed(w) for _ in range(n))
        else:
            for _ in range(n):
                q = bits.unary()
                out.append(_rice_signed(q << k | bits.read(k)))


_FIXED = [[], [1], [2, -1], [3, -3, 1], [4, -6, 4, -1]]


def _subframe(bits, blocksize, bps):
    if bits.read(1):
        raise ChdError("FLAC: bad subframe")
    typ = bits.read(6)
    wasted = 0
    if bits.read(1):
        wasted = bits.unary() + 1
        bps -= wasted
    if typ == 0:
        s = [bits.signed(bps)] * blocksize
    elif typ == 1:
        s = [bits.signed(bps) for _ in range(blocksize)]
    elif 8 <= typ <= 12 or typ >= 32:
        if typ < 32:
            order = typ - 8
            coefs, shift = _FIXED[order], 0
            s = [bits.signed(bps) for _ in range(order)]
        else:
            order = typ - 31
            s = [bits.signed(bps) for _ in range(order)]
            prec = bits.read(4) + 1
            shift = bits.signed(5)
            coefs = [bits.signed(prec) for _ in range(order)]
        res = []
        _residual(bits, blocksize, order, res)
        for r in res:
            acc = 0
            for i, c in enumerate(coefs):
                acc += c * s[-1 - i]
            s.append(r + (acc >> shift))
    else:
        raise ChdError("FLAC: reserved subframe type %d" % typ)
    if wasted:
        s = [x << wasted for x in s]
    return s


def flac_decode(data, nsamples, blocksize_default):
    """Decodes FLAC frames from data until nsamples stereo samples; returns (big-endian interleaved bytes, bytes used)."""
    bits = FlacBits(data)
    out = bytearray()
    done = 0
    while done < nsamples:
        bits.align()
        if bits.read(15) != 0x7FFC:
            raise ChdError("FLAC: lost frame sync")
        bits.read(1)
        bs_code, sr_code = bits.read(4), bits.read(4)
        chan, ss_code = bits.read(4), bits.read(3)
        bits.read(1)
        # UTF-8 coded frame number
        b = bits.read(8)
        extra = 0
        while b & (0x80 >> extra) and extra < 7:
            extra += 1
        for _ in range(max(0, extra - 1)):
            bits.read(8)
        if bs_code == 1:
            bs = 192
        elif 2 <= bs_code <= 5:
            bs = 576 << (bs_code - 2)
        elif bs_code == 6:
            bs = bits.read(8) + 1
        elif bs_code == 7:
            bs = bits.read(16) + 1
        elif bs_code >= 8:
            bs = 256 << (bs_code - 8)
        else:
            bs = blocksize_default
        if sr_code == 12:
            bits.read(8)
        elif sr_code in (13, 14):
            bits.read(16)
        bits.read(8)                                  # CRC-8
        bps = {0: 16, 1: 8, 2: 12, 4: 16, 5: 20, 6: 24}.get(ss_code, 16)
        if chan not in (1, 8, 9, 10):
            raise ChdError("FLAC: channel assignment %d, expected stereo" % chan)
        if chan == 1:
            l, r = _subframe(bits, bs, bps), _subframe(bits, bs, bps)
        elif chan == 8:       # left / side
            l = _subframe(bits, bs, bps); sd = _subframe(bits, bs, bps + 1)
            r = [a - c for a, c in zip(l, sd)]
        elif chan == 9:       # side / right
            sd = _subframe(bits, bs, bps + 1); r = _subframe(bits, bs, bps)
            l = [a + c for a, c in zip(r, sd)]
        elif chan == 10:      # mid / side
            m = _subframe(bits, bs, bps); sd = _subframe(bits, bs, bps + 1)
            l, r = [], []
            for a, c in zip(m, sd):
                a = a << 1 | (c & 1)
                l.append((a + c) >> 1); r.append((a - c) >> 1)
        else:
            raise ChdError("FLAC: channel assignment %d" % chan)
        bits.align()
        bits.read(16)                                 # CRC-16
        n = min(bs, nsamples - done)
        out += struct.pack(">%dh" % (2 * n), *[v for pair in zip(l[:n], r[:n]) for v in pair])
        done += n
    bits.align()
    return bytes(out), bits.p >> 3


# ---- codecs --------------------------------------------------------------------------------------------------------

def inflate(data, n):
    d = zlib.decompressobj(-15)
    out = d.decompress(data, n)
    if len(out) != n:
        raise ChdError("deflate: %d of %d bytes" % (len(out), n))
    return out


def lzma_dict_size(hunkbytes):
    """LzmaEncProps_Normalize with level 9 and reduceSize = hunkbytes (MAME chd lzma_compressor::configure_properties)."""
    for i in range(11, 31):
        if hunkbytes <= 2 << i:
            return 2 << i
        if hunkbytes <= 3 << i:
            return 3 << i
    return 64 << 20


def unlzma(data, n, hunkbytes):
    f = [{"id": lzma.FILTER_LZMA1, "dict_size": max(lzma_dict_size(hunkbytes), 4096), "lc": 3, "lp": 0, "pb": 2}]
    out = lzma.LZMADecompressor(lzma.FORMAT_RAW, filters=f).decompress(data, n)
    if len(out) != n:
        raise ChdError("lzma: %d of %d bytes" % (len(out), n))
    return out


def cd_decompress(codec, src, hunkbytes):
    frames = hunkbytes // FRAME
    if codec == b"cdfl":
        bsize = frames * SECTOR
        while bsize > 2048:
            bsize //= 2
        base, used = flac_decode(src, frames * SECTOR // 4, bsize // 4 if bsize >= 64 else bsize)
        sub = inflate(src[used:], frames * SUB)
        ecc = b"\0" * ((frames + 7) // 8)
    else:
        cl = 2 if hunkbytes < 65536 else 3
        eb = (frames + 7) // 8
        ecc = src[:eb]
        base_len = int.from_bytes(src[eb:eb + cl], "big")
        hb = eb + cl
        if codec == b"cdzl":
            base = inflate(src[hb:hb + base_len], frames * SECTOR)
        else:
            base = unlzma(src[hb:hb + base_len], frames * SECTOR, hunkbytes)
        sub = inflate(src[hb + base_len:], frames * SUB)
    out = bytearray(hunkbytes)
    for i in range(frames):
        sec = bytearray(base[i * SECTOR:(i + 1) * SECTOR])
        if ecc[i // 8] & (1 << (i % 8)):
            sec[:12] = SYNC
            ecc_generate(sec)
        out[i * FRAME:i * FRAME + SECTOR] = sec
        out[i * FRAME + SECTOR:(i + 1) * FRAME] = sub[i * SUB:(i + 1) * SUB]
    return bytes(out)


# ---- the CHD file --------------------------------------------------------------------------------------------------

COOKED = {"MODE1": 2048, "MODE2_FORM1": 2048}
RAW = ("MODE1_RAW", "MODE2_RAW", "AUDIO", "MODE2_FORM_MIX")


class Chd:
    def __init__(self, path):
        self.path = path
        self.f = open(path, "rb")
        h = self.f.read(124)
        if h[:8] != b"MComprHD":
            self.f.close()
            raise ChdError("%s: not a CHD file" % path)
        ver = struct.unpack(">I", h[12:16])[0]
        if ver != 5:
            self.f.close()
            raise ChdError("%s: CHD version %d (only v5, chdman 0.146 and later; convert with: chdman copy -i old.chd -o new.chd)" % (path, ver))
        self.codecs = [h[16 + 4 * i:20 + 4 * i] for i in range(4)]
        self.logical, self.mapoff, self.metaoff = struct.unpack(">QQQ", h[32:56])
        self.hunkbytes, self.unitbytes = struct.unpack(">II", h[56:64])
        self.parent = h[104:124] != b"\0" * 20
        self.hunks = (self.logical + self.hunkbytes - 1) // self.hunkbytes
        self.cache = {}
        self._map()
        self.tracks = self._tracks()

    def close(self):
        self.f.close()

    def _map(self):
        if self.codecs[0] == b"\0\0\0\0":       # uncompressed: 4-byte hunk offsets in units of hunkbytes
            self.f.seek(self.mapoff)
            raw = self.f.read(4 * self.hunks)
            self.map = [(4, o * self.hunkbytes, self.hunkbytes, None) if o else (-1, 0, 0, None)
                        for o in struct.unpack(">%dI" % self.hunks, raw)]
            return
        self.f.seek(self.mapoff)
        mh = self.f.read(16)
        mapbytes = struct.unpack(">I", mh[:4])[0]
        cur = int.from_bytes(mh[4:10], "big")
        mapcrc = struct.unpack(">H", mh[10:12])[0]
        lbits, sbits, pbits = mh[12], mh[13], mh[14]
        bits = Bits(self.f.read(mapbytes))
        codes = huffman_rle_tree(bits)
        types, rep, last = [], 0, 0
        for _ in range(self.hunks):
            if rep:
                types.append(last); rep -= 1
                continue
            v = huffman_decode(bits, codes)
            if v == 7:
                types.append(last); rep = 2 + huffman_decode(bits, codes)
            elif v == 8:
                types.append(last); rep = 2 + 16 + (huffman_decode(bits, codes) << 4); rep += huffman_decode(bits, codes)
            else:
                types.append(v); last = v
        out, raw = [], bytearray()
        last_self, last_parent = 0, 0
        for hn, t in enumerate(types):
            off, ln, crc = cur, 0, 0
            if t <= 3:
                ln = bits.read(lbits); crc = bits.read(16); cur += ln
            elif t == 4:
                ln = self.hunkbytes; crc = bits.read(16); cur += ln
            elif t == 5:
                last_self = off = bits.read(sbits)
            elif t == 6:
                off = last_parent = bits.read(pbits)
            elif t in (9, 10):
                if t == 10:
                    last_self += 1
                t, off = 5, last_self
            elif t == 11:
                t = 6; last_parent = off = hn * self.hunkbytes // self.unitbytes
            elif t in (12, 13):
                if t == 13:
                    last_parent += self.hunkbytes // self.unitbytes
                t, off = 6, last_parent
            else:
                raise ChdError("bad map entry type %d" % t)
            out.append((t, off, ln, crc))
            raw += bytes([t]) + ln.to_bytes(3, "big") + off.to_bytes(6, "big") + crc.to_bytes(2, "big")
        if binascii.crc_hqx(bytes(raw), 0xFFFF) != mapcrc:
            raise ChdError("%s: map CRC mismatch (damaged file?)" % self.path)
        self.map = out

    def _meta(self):
        off = self.metaoff
        while off:
            self.f.seek(off)
            h = self.f.read(16)
            tag, fl, nxt = h[:4], int.from_bytes(h[5:8], "big"), struct.unpack(">Q", h[8:16])[0]
            yield tag, self.f.read(fl)
            off = nxt

    def _tracks(self):
        tr = []
        for tag, data in self._meta():
            if tag in (b"CHT2", b"CHTR"):
                d = dict(w.split(":", 1) for w in data.rstrip(b"\0").decode("ascii", "replace").split() if ":" in w)
                tr.append({"track": int(d.get("TRACK", 0)), "type": d.get("TYPE", ""), "frames": int(d.get("FRAMES", 0)),
                           "pregap": int(d.get("PREGAP", 0) or 0), "pgtype": d.get("PGTYPE", "")})
            elif tag == b"CHGD":
                raise ChdError("%s: a GD-ROM CHD, not a PlayStation disc" % self.path)
        tr.sort(key=lambda t: t["track"])
        return tr

    def hunk(self, n):
        h = self.cache.get(n)
        if h is not None:
            return h
        t, off, ln, crc = self.map[n]
        if t == -1:
            data = bytes(self.hunkbytes)
        elif t == 5:
            data = self.hunk(off)
        elif t == 6 or self.parent and t == 6:
            raise ChdError("%s needs its parent CHD (a delta image): not supported, convert with chdman" % self.path)
        else:
            self.f.seek(off)
            src = self.f.read(ln)
            if t == 4:
                data = src
            else:
                codec = self.codecs[t]
                if codec in (b"cdzl", b"cdlz", b"cdfl"):
                    data = cd_decompress(codec, src, self.hunkbytes)
                elif codec == b"zlib":
                    data = inflate(src, self.hunkbytes)
                elif codec == b"lzma":
                    data = unlzma(src, self.hunkbytes, self.hunkbytes)
                else:
                    raise ChdError("%s: codec %r not supported (convert: chdman extractcd -i X.chd -o X.cue -ob X.bin)"
                                   % (self.path, codec.decode("ascii", "replace")))
            if crc is not None and t != 5 and binascii.crc_hqx(data, 0xFFFF) != crc:
                raise ChdError("%s: hunk %d CRC mismatch (damaged file?)" % (self.path, n))
        if len(self.cache) > 64:
            self.cache.clear()
        self.cache[n] = data
        return data

    def frame(self, n):
        per = self.hunkbytes // FRAME
        h = self.hunk(n // per)
        o = (n % per) * FRAME
        return h[o:o + FRAME]


class ChdFile:
    """Read-only file object over track 1 of a CD CHD: datasize bytes per sector (2352 raw, 2048 cooked)."""

    def __init__(self, path):
        self.chd = Chd(path)
        if self.chd.unitbytes != FRAME or self.chd.hunkbytes % FRAME:
            self.chd.close()
            raise ChdError("%s: not a CD CHD (unit %d bytes)" % (path, self.chd.unitbytes))
        t = self.chd.tracks[0] if self.chd.tracks else {"type": "MODE2_RAW", "frames": self.chd.logical // FRAME, "pregap": 0, "pgtype": ""}
        self.track = t
        self.dsize = COOKED.get(t["type"]) or (SECTOR if t["type"] in RAW else None)
        if not self.dsize:
            self.chd.close()
            raise ChdError("%s: track 1 is %s, expected a MODE1/MODE2 data track" % (path, t["type"]))
        # a pregap stored in the file (PGTYPE V...) comes first; the track data starts after it
        self.first = t["pregap"] if t["pgtype"].startswith("V") else 0
        self.sectors = t["frames"] - self.first
        self.size = self.sectors * self.dsize
        self.pos = 0

    def seek(self, off, whence=0):
        self.pos = off if whence == 0 else self.pos + off if whence == 1 else self.size + off
        return self.pos

    def tell(self):
        return self.pos

    def read(self, n=-1):
        if n < 0:
            n = self.size - self.pos
        n = max(0, min(n, self.size - self.pos))
        out = bytearray()
        while n > 0:
            s, o = divmod(self.pos, self.dsize)
            take = min(n, self.dsize - o)
            out += self.chd.frame(self.first + s)[o:o + take]
            self.pos += take
            n -= take
        return bytes(out)

    def close(self):
        self.chd.close()

    def __enter__(self):
        return self

    def __exit__(self, *a):
        self.close()


def open_image(path):
    """A binary file object for a disc image: ChdFile for a .chd, else the plain file."""
    if is_chd(path):
        return ChdFile(path)
    return open(path, "rb")


def main(argv):
    if len(argv) >= 2 and argv[0] == "info":
        c = ChdFile(argv[1])
        ch = c.chd
        print("%s: CHD v5, codecs %s, %d hunks of %d bytes, %d frames" % (argv[1], ",".join(x.decode("ascii", "replace").strip("\0") or "-" for x in ch.codecs),
                                                                        ch.hunks, ch.hunkbytes, ch.logical // FRAME))
        for t in ch.tracks:
            print("  track %d %s %d frames" % (t["track"], t["type"], t["frames"]))
        return 0
    if len(argv) >= 3 and argv[0] == "tobin":
        c = ChdFile(argv[1])
        with open(argv[2], "wb") as o:
            for s in range(c.sectors):
                o.write(c.read(c.dsize))
        print("%s: %d sectors of %d bytes" % (argv[2], c.sectors, c.dsize))
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except ChdError as e:
        sys.exit("error: %s" % e)
