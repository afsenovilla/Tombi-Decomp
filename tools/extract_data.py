#!/usr/bin/env python3
"""One-time extraction of the player's own Tombi! disc into a `data/` folder the native port (port2/, tombi2) reads
at runtime instead of the disc image (docs/PORT.md "Running without the disc image: data/").

  python3 tools/extract_data.py <disc.bin|.cue|.iso|.chd> <data_dir> [--quiet]
  python3 tools/extract_data.py --check <data_dir>            # verifies the manifest against the files (exit 1 if not)

The game never reads the ISO9660 directory: it seeks to (LBA, size) pairs from its own file table (tools/cdfiles.py),
so the port maps every sector read onto a file of data/ through `data/manifest.txt`, which lists every ISO9660 file
with its start LBA and how its sectors are stored:

  * Form 1 files (everything but the movies) as plain 2048-byte user data, the exact file size (the ISO9660 tail
    padding of the last sector is zeros: the port zero-fills past the end);
  * files holding Mode 2 Form 2 sectors (MOVIE/*.STR: interleaved MDEC video and XA audio) as raw 2352-byte
    sectors, so the subheaders (the ring of psyq.c skips audio sectors by submode) and the audio survive;
  * the sectors no directory entry covers (the system area and PVD, path tables, directories, the gap after the
    last file) as raw blobs in data/_disc/<lba>.raw, so a read anywhere on the disc has an answer.

Manifest (text, one entry per line, sorted by LBA; `#` comments):
  tombi-data 1
  sectors <disc sector count>
  main0_sha1 <sha1 of EXE/MAIN0.EXE>
  source <image basename>
  file <lba> <sectors> <stored sector bytes: 2048|2352> <file bytes> <path relative to data/>

Importable: extract(image, out_dir, progress=None) -> manifest dict; manifest_sha1(out_dir) -> sha1 or None.
"""
import hashlib, os, struct, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))   # `python3 -I` (CI) leaves the script's folder out
import chd

MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc"     # EXE/MAIN0.EXE of SCES-01331
MANIFEST = "manifest.txt"
BLOB_DIR = "_disc"


def bin_of_cue(path):
    """The data track file named by a .cue sheet (next to it)."""
    if not path.lower().endswith(".cue"):
        return path
    try:
        for line in open(path, encoding="utf-8", errors="replace"):
            line = line.strip()
            if line.upper().startswith("FILE "):
                name = line[5:].rsplit(" ", 1)[0].strip().strip('"')
                return os.path.join(os.path.dirname(path), name)
    except OSError:
        pass
    return path


def open_image(path):
    """Binary file object of a disc image: .bin/.iso as is, a MAME .chd decompressed on the fly (tools/chd.py)."""
    return chd.open_image(path)


class Image:
    """A raw (2352) or cooked (2048) image: sector access and the ISO9660 directory."""
    SYNC = b"\0" + b"\xff" * 10 + b"\0"

    def __init__(self, path):
        self.path = bin_of_cue(path)
        self.f = open_image(self.path)       # a .chd is read through tools/chd.py (track 1 as a raw/cooked stream)
        self.f.seek(0, 2)
        self.size = self.f.tell()
        self.f.seek(0)
        head = self.f.read(12)
        self.f.seek(16 * 2048 + 1)
        pvd_cooked = self.f.read(5) == b"CD001"
        self.raw = head == self.SYNC or (not pvd_cooked and self.size % 2352 == 0)
        self.ss, self.uo = (2352, 24) if self.raw else (2048, 0)
        self.sectors = self.size // self.ss
        if self.user(16)[1:6] != b"CD001":
            raise ValueError("%s: not an ISO9660 image" % os.path.basename(path))

    def close(self):
        self.f.close()

    def user(self, lba, count=1):
        """2048 bytes of user data per sector."""
        out = []
        for i in range(count):
            self.f.seek((lba + i) * self.ss + self.uo)
            out.append(self.f.read(2048))
        return b"".join(out)

    def raw_sector(self, lba):
        """The whole 2352-byte sector; a cooked image gets a synthesised Mode 2 Form 1 frame (zero EDC/ECC)."""
        if self.raw:
            self.f.seek(lba * 2352)
            return self.f.read(2352)
        a = lba + 150
        hdr = bytes([bcd(a // 4500), bcd(a // 75 % 60), bcd(a % 75), 2])
        return self.SYNC + hdr + b"\0\0\x08\0\0\0\x08\0" + self.user(lba) + b"\0" * 280

    def submode(self, lba):
        if not self.raw:
            return 0x08
        self.f.seek(lba * 2352 + 18)
        return self.f.read(1)[0]

    def files(self):
        """[(path, lba, size)] in directory order."""
        out = []

        def walk(lba, size, pre):
            data = self.user(lba, (size + 2047) // 2048)
            p = 0
            while p < size:
                n = data[p]
                if n == 0:
                    p = (p // 2048 + 1) * 2048
                    continue
                r = data[p:p + n]
                p += n
                elba, esz = struct.unpack("<I", r[2:6])[0], struct.unpack("<I", r[10:14])[0]
                nm = r[33:33 + r[32]]
                if nm in (b"\0", b"\1"):
                    continue
                nm = nm.decode("ascii", "replace")
                if r[25] & 2:
                    walk(elba, esz, pre + nm + "/")
                else:
                    out.append((pre + nm.split(";")[0], elba, esz))
        root = self.user(16)[156:190]
        walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "")
        return out


def bcd(x):
    return (x // 10) << 4 | x % 10


def has_form2(img, lba, count):
    """Any Mode 2 Form 2 sector (submode bit 5) in the range: the file must keep its raw sectors."""
    for l in range(lba, lba + count):
        if img.submode(l) & 0x20:
            return True
    return False


def extract(image, out_dir, progress=None, force=False):
    """Writes data/<files>, data/_disc/*.raw and data/manifest.txt; returns the manifest as a dict.
    progress(done_sectors, total_sectors, current_path) is called while copying.  With force=False a file already
    present with the right size is not rewritten (a resumed extraction)."""
    img = Image(image)
    try:
        files = img.files()
        entries = []        # (lba, sectors, stored, bytes, path)
        main0 = None
        for path, lba, size in files:
            n = (size + 2047) // 2048
            raw = path.upper().endswith((".STR", ".XA")) or has_form2(img, lba, n)
            entries.append((lba, n, 2352 if raw else 2048, n * 2352 if raw else size, path))
            if path.upper() == "EXE/MAIN0.EXE":
                main0 = (lba, size)
        entries.sort()
        # the sectors no file covers
        prev = 0
        blobs = []
        for lba, n, _, _, _ in entries:
            if lba > prev:
                blobs.append((prev, lba - prev))
            prev = max(prev, lba + n)
        if prev < img.sectors:
            blobs.append((prev, img.sectors - prev))
        for lba, n in blobs:
            entries.append((lba, n, 2352, n * 2352, "%s/%06d.raw" % (BLOB_DIR, lba)))
        entries.sort()
        total = sum(n for _, n, _, _, _ in entries)
        done = 0
        for lba, n, stored, nbytes, path in entries:
            dst = os.path.join(out_dir, *path.split("/"))
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            if force or not (os.path.isfile(dst) and os.path.getsize(dst) == nbytes):
                with open(dst + ".part", "wb") as o:
                    left = nbytes
                    for i in range(0, n, 64):
                        cnt = min(64, n - i)
                        if stored == 2352:
                            chunk = b"".join(img.raw_sector(lba + i + k) for k in range(cnt))
                        else:
                            chunk = img.user(lba + i, cnt)
                        o.write(chunk[:left])
                        left -= len(chunk)
                        if progress:
                            progress(done + i + cnt, total, path)
                os.replace(dst + ".part", dst)
            done += n
            if progress:
                progress(done, total, path)
        sha = hashlib.sha1(img.user(main0[0], (main0[1] + 2047) // 2048)[:main0[1]]).hexdigest() if main0 else ""
        man = {"sectors": img.sectors, "main0_sha1": sha, "source": os.path.basename(img.path), "files": entries}
        write_manifest(out_dir, man)
        return man
    finally:
        img.close()


def write_manifest(out_dir, man):
    tmp = os.path.join(out_dir, MANIFEST + ".part")
    with open(tmp, "w", encoding="ascii", newline="\n") as o:
        o.write("tombi-data 1\n# one-time extraction of the player's own disc for the native port (tools/extract_data.py)\n")
        o.write("sectors %d\nmain0_sha1 %s\nsource %s\n" % (man["sectors"], man["main0_sha1"], man["source"]))
        o.write("# file <lba> <sectors> <stored sector bytes> <file bytes> <path>\n")
        for lba, n, stored, nbytes, path in man["files"]:
            o.write("file %d %d %d %d %s\n" % (lba, n, stored, nbytes, path))
    os.replace(tmp, os.path.join(out_dir, MANIFEST))


def read_manifest(data_dir):
    """The manifest of data_dir as a dict, or None when there is none / it is not ours."""
    try:
        lines = open(os.path.join(data_dir, MANIFEST), encoding="ascii", errors="replace").read().splitlines()
    except OSError:
        return None
    if not lines or not lines[0].startswith("tombi-data "):
        return None
    man = {"sectors": 0, "main0_sha1": "", "source": "", "files": []}
    for line in lines[1:]:
        w = line.split()
        if not w or w[0].startswith("#"):
            continue
        if w[0] == "sectors":
            man["sectors"] = int(w[1])
        elif w[0] == "main0_sha1":
            man["main0_sha1"] = w[1] if len(w) > 1 else ""
        elif w[0] == "source":
            man["source"] = line[7:].strip()
        elif w[0] == "file" and len(w) >= 6:
            man["files"].append((int(w[1]), int(w[2]), int(w[3]), int(w[4]), " ".join(w[5:])))
    return man


def manifest_sha1(data_dir):
    """sha1 of MAIN0.EXE recorded by a complete extraction in data_dir (every file present with its size), else None."""
    man = read_manifest(data_dir)
    if not man:
        return None
    for _, _, _, nbytes, path in man["files"]:
        p = os.path.join(data_dir, *path.split("/"))
        if not os.path.isfile(p) or os.path.getsize(p) != nbytes:
            return None
    return man["main0_sha1"]


def check(data_dir):
    man = read_manifest(data_dir)
    if not man:
        print("%s: no manifest" % data_dir)
        return 1
    bad = 0
    for lba, n, stored, nbytes, path in man["files"]:
        p = os.path.join(data_dir, *path.split("/"))
        if not os.path.isfile(p):
            print("missing: %s" % path); bad += 1
        elif os.path.getsize(p) != nbytes:
            print("size: %s is %d bytes, manifest says %d" % (path, os.path.getsize(p), nbytes)); bad += 1
    covered = sum(n for _, n, _, _, _ in man["files"])
    print("%s: %d files, %d of %d sectors covered, MAIN0 sha1 %s%s" % (
        data_dir, len(man["files"]), covered, man["sectors"], man["main0_sha1"][:8],
        " (SCES-01331)" if man["main0_sha1"] == MAIN0_SHA1 else " (NOT the PAL release)"))
    return 1 if bad or covered != man["sectors"] else 0


def main(argv):
    quiet = "--quiet" in argv
    argv = [a for a in argv if a != "--quiet"]
    if len(argv) == 3 and argv[1] == "--check":
        return check(argv[2])
    if len(argv) != 3:
        print(__doc__)
        return 2
    last = [-1]

    def progress(done, total, path):
        pct = done * 100 // max(1, total)
        if pct != last[0] and not quiet:
            last[0] = pct
            sys.stdout.write("\r%3d%%  %s%s" % (pct, path, " " * 30))
            sys.stdout.flush()
    man = extract(argv[1], argv[2], progress)
    if not quiet:
        sys.stdout.write("\n")
    print("%s: %d files, %d sectors, MAIN0 sha1 %s%s" % (argv[2], len(man["files"]), man["sectors"], man["main0_sha1"][:8],
                                                        "" if man["main0_sha1"] == MAIN0_SHA1 else " (NOT SCES-01331)"))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
