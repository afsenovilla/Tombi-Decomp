#!/usr/bin/env python3
"""Hash manifest of a PS1 disc image (raw MODE2/2352 or 2048 ISO). Reads only; prints metadata, never file contents.

  python3 -I tools/disc_manifest.py DISC.bin [PREFIX]   one line per file: path, size, ISO date, md5, sha1
                                                        (PREFIX limits it to paths starting with it, e.g. /TOMBA)
Use it to catalogue a disc against docs/BUILDS.md and to compare files between builds by hash.
"""
import hashlib, struct, sys


def manifest(img, prefix=""):
    f = open(img, "rb")
    f.seek(16 * 2352 + 25)
    ss, uo = (2352, 24) if f.read(5) == b"CD001" else (2048, 0)

    def sec(n, c=1):
        b = bytearray()
        for i in range(c):
            f.seek((n + i) * ss + uo)
            b += f.read(2048)
        return bytes(b)

    out = []

    def walk(lba, size, path):
        d, i = sec(lba, (size + 2047) // 2048), 0
        while i < len(d):
            ln = d[i]
            if ln == 0:
                i = (i // 2048 + 1) * 2048
                continue
            r = d[i:i + ln]
            elba, esz = struct.unpack_from("<I", r, 2)[0], struct.unpack_from("<I", r, 10)[0]
            y, mo, dd, hh, mi, se = r[18:24]
            nm = r[33:33 + r[32]].decode("latin1").split(";")[0]
            if nm not in ("\0", "\1"):
                p = path + "/" + nm
                if r[25] & 2:
                    walk(elba, esz, p)
                elif p.upper().startswith(prefix.upper()):
                    if nm.upper().endswith(".STR") or nm.upper().endswith(".XA"):
                        h5 = h1 = "-"  # Form-2 streams: the 2048-byte view is not the real data
                    else:
                        b = sec(elba, (esz + 2047) // 2048)[:esz]
                        h5, h1 = hashlib.md5(b).hexdigest(), hashlib.sha1(b).hexdigest()
                    out.append((p, esz, "%d-%02d-%02d %02d:%02d" % (1900 + y, mo, dd, hh, mi), h5, h1))
            i += ln

    r = sec(16)[156:190]
    walk(struct.unpack_from("<I", r, 2)[0], struct.unpack_from("<I", r, 10)[0], "")
    return out


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    for p, s, d, h5, h1 in manifest(sys.argv[1], sys.argv[2] if len(sys.argv) > 2 else ""):
        print(f"{p}\t{s}\t{d}\t{h5}\t{h1}")
