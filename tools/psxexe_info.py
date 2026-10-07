#!/usr/bin/env python3
"""Muestra la cabecera de un PS-X EXE (p. ej. SCES_013.31).

Uso: python tools/psxexe_info.py game/SCES_013.31
"""
import struct
import sys


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    with open(sys.argv[1], "rb") as f:
        hdr = f.read(0x800)
    if hdr[:8] != b"PS-X EXE":
        sys.exit("No es un PS-X EXE (falta la firma 'PS-X EXE')")
    pc, gp, load, size = struct.unpack_from("<4I", hdr, 0x10)
    sp_base, sp_off = struct.unpack_from("<2I", hdr, 0x30)
    region = hdr[0x4C:hdr.find(b"\0", 0x4C)].decode("ascii", "replace")
    print("PC inicial : 0x%08X" % pc)
    print("GP         : 0x%08X" % gp)
    print("Carga en   : 0x%08X" % load)
    print("Tamano     : 0x%X (%d bytes, sin cabecera)" % (size, size))
    print("Fin        : 0x%08X" % (load + size))
    print("SP         : 0x%08X + 0x%X" % (sp_base, sp_off))
    print("Region     : %s" % region)


if __name__ == "__main__":
    main()
