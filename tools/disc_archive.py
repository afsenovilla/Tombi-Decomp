#!/usr/bin/env python3
"""Disc images inside archives, for the launchers' "Add game" (tools/port_launcher.py, tools/game_library.py,
tools/extract_data.py): a .zip or .7z holding the image (in any folder, any case: .bin / .img / .iso / .chd; with a
.cue, the data track it names, else the largest), as the CI's game zips (tools/disc_unpack.sh).  The image is
written to a temporary file next to where its data goes (the ISO reader needs random access), used, then deleted.

A zip / 7z of the disc's loose files (SYSTEM.CNF, no image) cannot be played: the game reads the disc by sector
number, which loose files have lost.  It is identified (SYSTEM.CNF BOOT and that file's sha1, EXE/MAIN0.EXE on the
PAL discs) so the message can name the game.  .rar is refused with a message.

.zip: Python's zipfile.  .7z: a small reader of the 7z container here (the format of 7-Zip's 7zFormat.txt) with the
standard library's decoders: LZMA, LZMA2, the BCJ / delta filters (lzma), Deflate (zlib), BZip2 (bz2), Copy; an
encrypted archive, PPMd or BCJ2 gets a clear error (extract it with 7-Zip first).  No third-party package.

  python3 tools/disc_archive.py ARCHIVE [OUT_DIR]     # what is inside; with OUT_DIR, the image written there
"""
import bz2, hashlib, lzma, os, re, struct, sys, zlib, zipfile

IMAGE_EXT = (".bin", ".img", ".iso", ".chd")
MIN_IMAGE = 4 << 20            # smaller "images" are not a PlayStation disc
SIG_7Z = b"7z\xbc\xaf\x27\x1c"


class ArchiveError(Exception):
    """code: notimage, rar, noimage, nested, files (game = name/serial or ''), cue (track = name), bad (reason),
    space (need, free in bytes), cancelled. str() is an English message (the launchers translate by code)."""
    def __init__(self, code, msg, **kw):
        super().__init__(msg)
        self.code = code
        self.__dict__.update(kw)


def kind(path):
    """'zip', '7z', 'rar', 'cue', 'image' or 'notimage' from the first bytes (and the name)."""
    low = path.lower()
    try:
        with open(path, "rb") as f:
            h = f.read(16 * 2352 + 48)
    except OSError:
        return "image"                 # (the reader says why)
    if h[:2] == b"PK" and h[2:3] in (b"\3", b"\5", b"\7"):
        return "zip"
    if h[:6] == SIG_7Z:
        return "7z"
    if h[:4] == b"Rar!":
        return "rar"
    if low.endswith((".zip", ".7z", ".rar")):
        return {".zip": "zip", ".7z": "7z", ".rar": "rar"}[os.path.splitext(low)[1]]
    if low.endswith(".cue") or h[:8] == b"MComprHD":
        return "cue" if low.endswith(".cue") else "image"
    if h[:12] == b"\0" + b"\xff" * 10 + b"\0":
        return "image"
    for o in (16 * 2048 + 1, 16 * 2352 + 25, 16 * 2352 + 17):
        if h[o:o + 5] == b"CD001":
            return "image"
    return "image" if len(h) < 16 * 2048 + 6 else "notimage"


def is_archive(path):
    return kind(path) in ("zip", "7z", "rar")


# ---------------------------------------------------------------------------------------------------------- 7z
def _num(b, i):
    first = b[i]; i += 1
    mask, val = 0x80, 0
    for n in range(8):
        if not first & mask:
            return val | ((first & (mask - 1)) << (8 * n)), i
        val |= b[i] << (8 * n); i += 1
        mask >>= 1
    return val, i


def _bits(b, i, n):
    out = []
    for k in range(n):
        out.append(bool(b[i + k // 8] & (0x80 >> (k % 8))))
    return out, i + (n + 7) // 8


def _defined(b, i, n):
    all_ = b[i]; i += 1
    if all_:
        return [True] * n, i
    return _bits(b, i, n)


class SevenZip:
    """The file list of a .7z and a sequential decoder of its folders (solid blocks)."""

    def __init__(self, path):
        self.f = open(path, "rb")
        try:
            sh = self.f.read(32)
            if sh[:6] != SIG_7Z:
                raise ArchiveError("bad", "not a 7z archive")
            off, size = struct.unpack_from("<QQ", sh, 12)
            self.f.seek(32 + off)
            hdr = self.f.read(size)
            if len(hdr) != size:
                raise ArchiveError("bad", "truncated 7z archive")
            while hdr[:1] == b"\x17":           # an encoded (compressed) header
                st, _ = self._streams(hdr, 1)
                hdr = b"".join(self._folder_bytes(st, k) for k in range(len(st["folders"])))
            if hdr[:1] != b"\x01":
                raise ArchiveError("bad", "7z header not understood")
            self._header(hdr)
        except (IndexError, struct.error) as e:
            self.f.close()
            raise ArchiveError("bad", "damaged 7z header (%s)" % e)
        except Exception:
            self.f.close()
            raise

    def close(self):
        self.f.close()

    # -- parsing
    def _streams(self, b, i):
        st = dict(pack_pos=0, pack_sizes=[], folders=[], unpack_sizes=[], sub_counts=None, sub_sizes=None)
        while True:
            t = b[i]; i += 1
            if t == 0x00:
                break
            if t == 0x06:                                    # PackInfo
                st["pack_pos"], i = _num(b, i)
                n, i = _num(b, i)
                while True:
                    t2 = b[i]; i += 1
                    if t2 == 0x00:
                        break
                    if t2 == 0x09:
                        for _ in range(n):
                            v, i = _num(b, i); st["pack_sizes"].append(v)
                    elif t2 == 0x0A:
                        d, i = _defined(b, i, n); i += 4 * sum(d)
                    else:
                        raise ArchiveError("bad", "7z PackInfo %d" % t2)
            elif t == 0x07:                                  # CodersInfo
                if b[i] != 0x0B:
                    raise ArchiveError("bad", "7z CodersInfo")
                i += 1
                nf, i = _num(b, i)
                if b[i]:
                    raise ArchiveError("bad", "7z external folders")
                i += 1
                for _ in range(nf):
                    fo, i = self._folder(b, i)
                    st["folders"].append(fo)
                if b[i] != 0x0C:
                    raise ArchiveError("bad", "7z CodersUnpackSize")
                i += 1
                for fo in st["folders"]:
                    fo["sizes"] = []
                    for _ in range(fo["nout"]):
                        v, i = _num(b, i); fo["sizes"].append(v)
                while True:
                    t2 = b[i]; i += 1
                    if t2 == 0x00:
                        break
                    if t2 == 0x0A:
                        d, i = _defined(b, i, nf); i += 4 * sum(d)
                    else:
                        raise ArchiveError("bad", "7z CodersInfo %d" % t2)
            elif t == 0x08:                                  # SubStreamsInfo
                counts = [1] * len(st["folders"])
                sizes = None
                while True:
                    t2 = b[i]; i += 1
                    if t2 == 0x00:
                        break
                    if t2 == 0x0D:
                        for k in range(len(counts)):
                            counts[k], i = _num(b, i)
                    elif t2 == 0x09:
                        sizes = []
                        for k, fo in enumerate(st["folders"]):
                            if counts[k] == 0:
                                continue
                            tot = 0
                            for _ in range(counts[k] - 1):
                                v, i = _num(b, i); sizes.append(v); tot += v
                            sizes.append(self.unpack_size(fo) - tot)
                    elif t2 == 0x0A:
                        nd = sum(c for k, c in enumerate(counts) if not (c == 1 and st["folders"][k].get("crc")))
                        d, i = _defined(b, i, nd); i += 4 * sum(d)
                    else:
                        raise ArchiveError("bad", "7z SubStreamsInfo %d" % t2)
                if sizes is None:
                    sizes = [self.unpack_size(fo) for k, fo in enumerate(st["folders"]) if counts[k] == 1]
                st["sub_counts"], st["sub_sizes"] = counts, sizes
            else:
                raise ArchiveError("bad", "7z StreamsInfo %d" % t)
        if st["sub_counts"] is None:
            st["sub_counts"] = [1] * len(st["folders"])
            st["sub_sizes"] = [self.unpack_size(fo) for fo in st["folders"]]
        return st, i

    def _folder(self, b, i):
        nc, i = _num(b, i)
        coders, nin, nout = [], 0, 0
        for _ in range(nc):
            flag = b[i]; i += 1
            cid = bytes(b[i:i + (flag & 0x0F)]); i += flag & 0x0F
            ci = co = 1
            if flag & 0x10:
                ci, i = _num(b, i); co, i = _num(b, i)
            props = b""
            if flag & 0x20:
                n, i = _num(b, i); props = bytes(b[i:i + n]); i += n
            if flag & 0x80:
                raise ArchiveError("bad", "7z alternative methods")
            coders.append(dict(id=cid, nin=ci, nout=co, props=props, in0=nin, out0=nout))
            nin += ci; nout += co
        bonds = []
        for _ in range(nout - 1):
            a, i = _num(b, i); c, i = _num(b, i); bonds.append((a, c))
        npk = nin - len(bonds)
        packed = []
        if npk == 1:
            packed = [next(k for k in range(nin) if all(a != k for a, _ in bonds))]
        else:
            for _ in range(npk):
                v, i = _num(b, i); packed.append(v)
        return dict(coders=coders, bonds=bonds, packed=packed, nin=nin, nout=nout), i

    @staticmethod
    def unpack_size(fo):
        bound = {c for _, c in fo["bonds"]}
        for k in range(fo["nout"]):
            if k not in bound:
                return fo["sizes"][k]
        return 0

    def _header(self, b):
        i = 1
        st = dict(folders=[], pack_sizes=[], pack_pos=0, sub_counts=[], sub_sizes=[])
        while True:
            t = b[i]; i += 1
            if t == 0x00:
                break
            if t == 0x02:                                    # ArchiveProperties
                while b[i]:
                    i += 1; n, i = _num(b, i); i += n
                i += 1
            elif t == 0x03:                                  # AdditionalStreamsInfo
                _, i = self._streams(b, i)
            elif t == 0x04:                                  # MainStreamsInfo
                st, i = self._streams(b, i)
            elif t == 0x05:                                  # FilesInfo
                i = self._files(b, i)
            else:
                raise ArchiveError("bad", "7z header %d" % t)
        self.st = st
        if not hasattr(self, "names"):
            self.names, self.empty, self.dirs = [], [], []
        # files -> (folder, offset in the folder's output, size): the non-empty files take the streams in order
        streams, si = [], 0
        for folder, cnt in enumerate(st["sub_counts"]):
            off = 0
            for _ in range(cnt):
                size = st["sub_sizes"][si]; si += 1
                streams.append((folder, off, size)); off += size
        it = iter(streams)
        self.entries = []
        for n, nm in enumerate(self.names):
            if self.empty[n]:
                self.entries.append(dict(name=nm, size=0, folder=None, off=0, dir=self.dirs[n]))
            else:
                folder, off, size = next(it)
                self.entries.append(dict(name=nm, size=size, folder=folder, off=off, dir=False))

    def _files(self, b, i):
        n, i = _num(b, i)
        self.names, self.empty, self.dirs = [""] * n, [False] * n, [False] * n
        empty_file = []
        attrs = [None] * n
        while True:
            t = b[i]; i += 1
            if t == 0x00:
                break
            size, i = _num(b, i)
            j = i
            if t == 0x0E:
                self.empty, _ = _bits(b, j, n)
            elif t == 0x0F:
                empty_file, _ = _bits(b, j, sum(self.empty))
            elif t == 0x11:
                if b[j]:
                    raise ArchiveError("bad", "7z external names")
                raw = bytes(b[j + 1:i + size]).decode("utf-16-le", "replace").split("\0")
                self.names = [x.replace("\\", "/") for x in raw[:n]]
            elif t == 0x15:
                d, j = _defined(b, j, n)
                if b[j]:
                    raise ArchiveError("bad", "7z external attributes")
                j += 1
                for k in range(n):
                    if d[k]:
                        attrs[k] = struct.unpack_from("<I", b, j)[0]; j += 4
            i += size
        ek = 0
        for k in range(n):
            if self.empty[k]:
                is_file = empty_file[ek] if ek < len(empty_file) else False
                ek += 1
                self.dirs[k] = not is_file or bool(attrs[k] is not None and attrs[k] & 0x10)
        return i

    # -- decoding
    def _decoder(self, fo):
        """a function bytes -> bytes (incremental) for the folder's chain of coders, and its pack stream index"""
        if len(fo["packed"]) != 1 or any(c["nin"] != 1 or c["nout"] != 1 for c in fo["coders"]):
            ids = ",".join(c["id"].hex() for c in fo["coders"])
            raise ArchiveError("bad", "7z method not supported (%s: BCJ2 or another multi-stream coder); extract it with 7-Zip" % ids)
        # the chain from the main output back to the packed stream
        out_of = {}                         # out stream index -> coder
        for c in fo["coders"]:
            out_of[c["out0"]] = c
        bound_out = {o: n for n, o in fo["bonds"]}          # (in, out): in stream n reads out stream o
        in_from = {n: o for n, o in fo["bonds"]}
        main = next(k for k in range(fo["nout"]) if k not in bound_out)
        chain, c = [], out_of[main]
        while True:
            chain.append(c)
            if c["in0"] in in_from:
                c = out_of[in_from[c["in0"]]]
            else:
                break
        filters, simple = [], []
        for c in chain:                      # main output first
            cid, p = c["id"], c["props"]
            if cid == b"\x21":
                d = p[0] if p else 24
                filters.append(dict(id=lzma.FILTER_LZMA2, dict_size=0xFFFFFFFF if d >= 40 else (2 | (d & 1)) << (d // 2 + 11)))
            elif cid == b"\x03\x01\x01":
                filters.append(dict(id=lzma.FILTER_LZMA1, dict_size=struct.unpack_from("<I", p, 1)[0],
                                    lc=p[0] % 9, lp=p[0] // 9 % 5, pb=p[0] // 45))
            elif cid == b"\x03\x03\x01\x03":
                filters.append(dict(id=lzma.FILTER_X86))
            elif cid == b"\x03\x03\x05\x01":
                filters.append(dict(id=lzma.FILTER_ARM))
            elif cid == b"\x03\x03\x04\x01":
                filters.append(dict(id=lzma.FILTER_POWERPC))
            elif cid == b"\x03\x03\x08\x05":
                filters.append(dict(id=lzma.FILTER_SPARC))
            elif cid == b"\x03":
                filters.append(dict(id=lzma.FILTER_DELTA, dist=(p[0] + 1) if p else 1))
            elif cid in (b"\x00", b"\x04\x01\x08", b"\x04\x02\x02") and len(chain) == 1:
                simple.append(cid)
            elif cid == b"\x06\xf1\x07\x01":
                raise ArchiveError("bad", "the 7z archive is encrypted")
            else:
                raise ArchiveError("bad", "7z method %s not supported; extract it with 7-Zip" % cid.hex())
        if simple:
            cid = simple[0]
            if cid == b"\x00":
                return lambda data: data
            d = zlib.decompressobj(-15) if cid == b"\x04\x01\x08" else bz2.BZ2Decompressor()
            return d.decompress
        if filters[-1]["id"] not in (lzma.FILTER_LZMA1, lzma.FILTER_LZMA2):
            raise ArchiveError("bad", "7z filter chain without LZMA")
        # (the chain as the compressor applied it: the filters first, LZMA last; 7z lists the coders the same way)
        dec = lzma.LZMADecompressor(format=lzma.FORMAT_RAW, filters=filters)
        return dec.decompress

    def _pack_offset(self, st, folder):
        k = sum(len(fo["packed"]) for fo in st["folders"][:folder])
        return 32 + st["pack_pos"] + sum(st["pack_sizes"][:k]), st["pack_sizes"][k]

    def _folder_chunks(self, st, folder, until=None, cancel=None):
        """the folder's output, in chunks, up to `until` bytes (all of it by default)"""
        fo = st["folders"][folder]
        total = self.unpack_size(fo) if until is None else min(until, self.unpack_size(fo))
        dec = self._decoder(fo)
        pos, size = self._pack_offset(st, folder)
        self.f.seek(pos)
        done = 0
        try:
            while size > 0 and done < total:
                if cancel and cancel():
                    raise ArchiveError("cancelled", "cancelled")
                data = self.f.read(min(size, 1 << 20))
                if not data:
                    raise ArchiveError("bad", "truncated 7z archive")
                size -= len(data)
                out = dec(data)
                if out:
                    out = out[:total - done]
                    done += len(out)
                    yield out
        except (lzma.LZMAError, zlib.error, EOFError) as e:
            raise ArchiveError("bad", "damaged 7z data (%s)" % e)
        if done < total:
            raise ArchiveError("bad", "7z data ends early (%d of %d bytes)" % (done, total))

    def _folder_bytes(self, st, folder):
        return b"".join(self._folder_chunks(st, folder))

    def members(self):
        return [(e["name"], e["size"]) for e in self.entries if not e["dir"]]

    def read_members(self, wanted, sink, cancel=None):
        """streams the folders holding `wanted` (names) and calls sink(name, chunk) for their bytes, in order"""
        want = [e for e in self.entries if e["name"] in wanted and e["folder"] is not None]
        for folder in sorted({e["folder"] for e in want}):
            fe = sorted((e for e in want if e["folder"] == folder), key=lambda e: e["off"])
            end = max(e["off"] + e["size"] for e in fe)
            pos = 0
            for chunk in self._folder_chunks(self.st, folder, end, cancel):
                cs, ce = pos, pos + len(chunk)
                for e in fe:
                    a, z = max(cs, e["off"]), min(ce, e["off"] + e["size"])
                    if a < z:
                        sink(e["name"], chunk[a - cs:z - cs])
                pos = ce
        for e in want:
            if e["size"] == 0:
                sink(e["name"], b"")


# ---------------------------------------------------------------------------------------------------------- both
class Archive:
    """members() -> [(name, size)], read_members(names, sink(name, chunk), cancel)"""

    def __init__(self, path):
        self.path = path
        self.k = kind(path)
        if self.k == "rar":
            raise ArchiveError("rar", "%s: a .rar archive cannot be opened; extract it first (or use a .zip / .7z)" % os.path.basename(path))
        if self.k == "zip":
            try:
                self.z = zipfile.ZipFile(path)
            except (zipfile.BadZipFile, OSError) as e:
                raise ArchiveError("bad", "%s: %s" % (os.path.basename(path), e))
        elif self.k == "7z":
            self.z = SevenZip(path)
        else:
            raise ArchiveError("notimage", "%s is not an archive" % os.path.basename(path))

    def close(self):
        self.z.close()

    def members(self):
        if self.k == "zip":
            return [(i.filename.replace("\\", "/"), i.file_size) for i in self.z.infolist() if not i.is_dir()]
        return self.z.members()

    def read_members(self, wanted, sink, cancel=None):
        if self.k == "7z":
            return self.z.read_members(wanted, sink, cancel)
        for name in wanted:
            try:
                with self.z.open(name.replace("/", "\\") if name not in self.z.NameToInfo else name) as f:
                    while True:
                        if cancel and cancel():
                            raise ArchiveError("cancelled", "cancelled")
                        b = f.read(1 << 20)
                        if not b:
                            break
                        sink(name, b)
            except (zipfile.BadZipFile, zlib.error, NotImplementedError, RuntimeError) as e:
                raise ArchiveError("bad", "%s: %s" % (os.path.basename(self.path), e))


def cue_data(text):
    """the file name of a .cue's first FILE (the data track)"""
    for line in text.splitlines():
        t = line.strip()
        if t.upper().startswith("FILE "):
            t = t[5:].strip()
            m = re.match(r'"([^"]*)"', t)
            nm = m.group(1) if m else t.rsplit(" ", 1)[0]
            return nm.replace("\\", "/").rsplit("/", 1)[-1]
    return None


def boot_of(cnf):
    for line in cnf.splitlines():
        k, _, v = line.partition("=")
        if k.strip().upper() == "BOOT":
            v = v.strip().split(":", 1)[-1].replace("\\", "/").lstrip("/").split(";")[0].split(" ")[0]
            return v or None
    return None


def _base(n):
    return n.rsplit("/", 1)[-1]


def inspect(path, cancel=None):
    """What an archive holds: dict(image=member or None, size, cue_missing, loose (dict boot, sha1, serial) or None,
    nested) without extracting the image."""
    a = Archive(path)
    try:
        mem = [(n, s) for n, s in a.members() if not n.startswith("__MACOSX/") and "/__MACOSX/" not in n]
        imgs = [(n, s) for n, s in mem if n.lower().endswith(IMAGE_EXT) and s >= MIN_IMAGE]
        cues = [n for n, s in mem if n.lower().endswith(".cue") and s < (1 << 16)]
        small = {}
        want = cues[:1]
        cnfs = sorted((n for n, s in mem if _base(n).upper() == "SYSTEM.CNF"), key=lambda n: n.count("/"))
        exes = [n for n, s in mem if s < (8 << 20) and re.fullmatch(r"(main0\.exe|[a-z]{4}[_-]\d{3}\.\d{2}(;1)?)", _base(n).lower())]
        if not imgs:
            want += cnfs[:1] + exes
        if want:
            bufs = {n: [] for n in want}
            a.read_members(want, lambda n, b: bufs[n].append(b), cancel)
            small = {n: b"".join(v) for n, v in bufs.items()}
        track = cue_data(small[cues[0]].decode("latin-1")) if cues else None
        r = dict(image=None, size=0, cue_missing=None, loose=None, nested=False, entries=len(mem))
        if imgs:
            pick = None
            if track:
                pick = next((n for n, s in imgs if _base(n).lower() == track.lower()), None)
            if pick is None:
                pick = max(imgs, key=lambda x: x[1])[0]
            r["image"], r["size"] = pick, dict(imgs)[pick]
            return r
        if track:
            r["cue_missing"] = track
        if cnfs:
            boot = boot_of(small[cnfs[0]].decode("latin-1"))
            sha = {_base(n).upper().replace(";1", ""): hashlib.sha1(small[n]).hexdigest() for n in exes}
            lo = dict(boot=boot, sha1=None, serial=None)
            if boot:
                b = _base(boot).upper()
                u = b.replace(".", "")
                lo["serial"] = u[:4] + "-" + u[5:] if len(u) == 10 and u[4] == "_" else None
                lo["sha1"] = sha.get("MAIN0.EXE") if b.startswith("SCES_") and "MAIN0.EXE" in sha else sha.get(b)
            r["loose"] = lo
        r["nested"] = any(n.lower().endswith((".zip", ".7z", ".rar", ".ecm", ".pbp")) for n, _ in mem)
        return r
    finally:
        a.close()


def free_space(d):
    try:
        import shutil
        return shutil.disk_usage(d).free
    except OSError:
        return None


def extract_image(path, out_dir, progress=None, cancel=None, reserve=64 << 20):
    """The disc image of an archive written into out_dir (a new file named after the member): its path. Room checked
    first: twice the image (it, then its extracted files) plus reserve. progress(done, total) in bytes.
    Raises ArchiveError (code noimage / files / cue / nested / bad / space / rar / cancelled)."""
    info = inspect(path, cancel)
    nm = os.path.basename(path)
    if not info["image"]:
        if info["loose"]:
            raise ArchiveError("files", "%s holds the disc's loose files, not its image: the game needs the whole image "
                               "(.bin with its .cue, .iso or .chd)" % nm, loose=info["loose"])
        if info["cue_missing"]:
            raise ArchiveError("cue", "%s holds the .cue but not the data track it names (%s)" % (nm, info["cue_missing"]),
                               track=info["cue_missing"])
        if info["nested"]:
            raise ArchiveError("nested", "%s holds another archive, not a disc image" % nm)
        raise ArchiveError("noimage", "%s holds no disc image (.bin, .iso, .img or .chd)" % nm)
    os.makedirs(out_dir, exist_ok=True)
    size = info["size"]
    free = free_space(out_dir)
    if free is not None and free < 2 * size + reserve:
        raise ArchiveError("space", "not enough free space to unpack %s: %d MB needed, %d MB free" % (nm, (2 * size + reserve) >> 20, free >> 20),
                           need=2 * size + reserve, free=free)
    safe = re.sub(r"[^A-Za-z0-9._()\[\] !,+-]", "_", _base(info["image"])) or "image.bin"
    dst = os.path.join(out_dir, safe)
    done = [0]
    a = Archive(path)
    try:
        with open(dst, "wb") as o:
            def sink(_n, b):
                o.write(b)
                done[0] += len(b)
                if progress:
                    progress(done[0], size)
            a.read_members([info["image"]], sink, cancel)
    except BaseException:
        try:
            os.remove(dst)
        except OSError:
            pass
        raise
    finally:
        a.close()
    if os.path.getsize(dst) != size:
        os.remove(dst)
        raise ArchiveError("bad", "%s: %s unpacked to %d of %d bytes" % (nm, info["image"], os.path.getsize(dst), size))
    return dst


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    p = argv[1]
    print("%s: %s" % (p, kind(p)))
    try:
        if len(argv) > 2:
            print(extract_image(p, argv[2], lambda d, t: None))
        else:
            print(inspect(p))
    except ArchiveError as e:
        print("error (%s): %s" % (e.code, e))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
