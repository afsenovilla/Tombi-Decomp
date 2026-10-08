#!/usr/bin/env python3
"""Shows the header of a PS-X EXE (e.g. SCES_013.31).

Usage: python tools/psxexe_info.py game/SCES_013.31
"""
import struct
import sys


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    with open(sys.argv[1], "rb") as f:
        hdr = f.read(0x800)
    if hdr[:8] != b"PS-X EXE":
        sys.exit("Not a PS-X EXE (missing the 'PS-X EXE' signature)")
    pc, gp, load, size = struct.unpack_from("<4I", hdr, 0x10)
    sp_base, sp_off = struct.unpack_from("<2I", hdr, 0x30)
    region = hdr[0x4C:hdr.find(b"\0", 0x4C)].decode("ascii", "replace")
    print("Initial PC : 0x%08X" % pc)
    print("GP         : 0x%08X" % gp)
    print("Load addr  : 0x%08X" % load)
    print("Size       : 0x%X (%d bytes, without header)" % (size, size))
    print("End        : 0x%08X" % (load + size))
    print("SP         : 0x%08X + 0x%X" % (sp_base, sp_off))
    print("Region     : %s" % region)


if __name__ == "__main__":
    main()
