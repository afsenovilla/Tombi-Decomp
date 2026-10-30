#!/usr/bin/env python3
"""PS1 Classics (PSN) EBOOT.PBP reader: Tombi!/Tomba! NPJJ00566 / NPUJ94236.

Usage:
  psn_eboot.py info EBOOT.PBP KEYS.BIN [--json out.json]
  psn_eboot.py extract EBOOT.PBP KEYS.BIN out.bin      (EXPERIMENTAL, see below)

Format (PSISOIMG0000, one disc):
  PBP header (8 offsets) -> DATA.PSAR.  PSAR+0x400 is a PGD container
  (AMCTRL / sceDrmBBCipher type 1) that holds the ISO header.  We decrypt it
  with the per-title version key (KEYS.BIN, 16 bytes) -- this part works.
  The header has: disc id (+0x00), TOC (+0x400, BCD), compressed ISO end
  offset + title (+0xE20), block index (+0x3C00; 32-byte entries
  offset:u32 size:u16 flag:u16 hash[16] pad[8]).  Every block is 16 raw
  sectors (0x9300 bytes) and is stored from PSAR+0x100000.

  Blocks under 0x9300 bytes are compressed.  'extract' runs a port of Hykem's
  Sony LZ range decoder (RPCS3 Crypto/lz.cpp, as used for EDAT).  On these two
  releases it does NOT decode yet (it fails at the first match), so either
  the PS1 Classics coder is different or the blocks have another layer.  The
  'info' mode is reliable.

Needs only the stdlib and 'cryptography'.  No game data is embedded.
"""
import argparse
import os
import struct
import sys

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

# public KIRK keyvault entries (libkirk keyvault[0x39], [0x63])
KIRK_39 = bytes.fromhex('c43bb6d653ee67493ea95fbc0ced6f8a')
KIRK_63 = bytes.fromhex('9c9b1372f8c640cf1c62f5d592ddb582')
LOC_1CE4 = bytes([0x13, 0x5F, 0xA4, 0x7C, 0xAB, 0x39, 0x5B, 0xA4, 0x76, 0xB8, 0xCC, 0xA9, 0x8F, 0x3A, 0x04, 0x45])
LOC_1CF4 = bytes([0x67, 0x8D, 0x7F, 0xA3, 0x2A, 0x9C, 0xA0, 0xD1, 0x50, 0x8A, 0xD8, 0x38, 0x5E, 0x4B, 0x01, 0x7E])


def _xor(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


def _aes_ecb_dec(key, data):
    d = Cipher(algorithms.AES(key), modes.ECB()).decryptor()
    return d.update(data) + d.finalize()


def bb_cipher(header_key, vkey, data, seed=0):
    """sceDrmBBCipher type 1, mode 2 (decrypt). seed = byte_offset>>4."""
    key = _xor(header_key, vkey) if vkey else header_key
    t = _xor(_aes_ecb_dec(KIRK_39, _xor(key, LOC_1CF4)), LOC_1CE4)[:12]
    n = (len(data) + 15) // 16
    s0 = seed + 1
    ctr = b''.join(t + struct.pack('<I', (s0 + i) & 0xffffffff) for i in range(n))
    dec = _aes_ecb_dec(KIRK_63, ctr)
    prev = (t + struct.pack('<I', (s0 - 1) & 0xffffffff)) if s0 != 1 else bytes(16)
    chain = prev + ctr[:-16]
    ks = _xor(dec, chain)
    return _xor(data, ks)


def pgd_decrypt(buf, vkey):
    """Decrypt a PGD blob (no MAC check). Returns plaintext data."""
    if buf[:4] != b'\0PGD':
        raise ValueError('not a PGD')
    if struct.unpack_from('<I', buf, 8)[0] != 1:
        raise ValueError('only drm_type 1 supported')
    desc = bb_cipher(buf[0x10:0x20], vkey, buf[0x30:0x60])
    dkey = desc[:16]
    size, bsize, doff = struct.unpack_from('<III', desc, 0x14)
    if bsize not in (0x400, 0x800, 0x1000, 0x2000, 0x10000) or doff > len(buf):
        raise ValueError('bad PGD descriptor (wrong key?)')
    alen = (size + 15) & ~15
    return bb_cipher(dkey, vkey, buf[doff:doff + alen])[:size]


# ---------------------------------------------------------------- LZ
# Port of Hykem's Sony LZ range decoder (psxtract / RPCS3 Crypto/lz.cpp,
# GPL-2.0+).  This is what PS1 Classics use for 0x9300-byte ISO blocks.

def lz_decompress(src, out_len):
    src = bytes(src) + bytes(64)
    head = src[0]
    code = int.from_bytes(src[1:5], 'big')
    if head > 0x80:
        return src[5:5 + code]
    rng = 0xffffffff
    pos = 5
    tmp = bytearray([0x80]) * 0xCA8 + bytearray(0x20)
    out = bytearray()

    def bit(c):
        nonlocal rng, code, pos
        if not (rng >> 24):
            rng = (rng << 8) & 0xffffffff
            code = ((code << 8) + src[pos]) & 0xffffffff
            pos += 1
        p = tmp[c]
        val = (rng >> 8) * p
        p -= p >> 3
        if code < val:
            rng = val
            tmp[c] = (p + 31) & 0xff
            return 1
        code -= val
        rng -= val
        tmp[c] = p
        return 0

    def direct(i, n):
        nonlocal rng, code, pos
        if not (rng >> 24):
            rng = (rng << 8) & 0xffffffff
            code = ((code << 8) + src[pos]) & 0xffffffff
            pos += 1
        while n >= 5:
            i <<= 1
            rng >>= 1
            if code < rng:
                i += 1
            else:
                code -= rng
            n -= 1
        return i

    def number(ptr, index, step, hi):
        i = 1
        if index >= 3:
            i = (i << 1) + bit(ptr + hi)
            if index >= 4:
                i = (i << 1) + bit(ptr + hi)
                if index >= 5:
                    i = direct(i, index)
        f = bit(ptr)
        i = (i << 1) + f
        if index >= 1:
            i = (i << 1) + bit(ptr + step)
            if index >= 2:
                i = (i << 1) + bit(ptr + 2 * step)
        return i, f

    offset = 0
    prev = 0
    while True:
        s1 = offset + 0xB68
        if not bit(s1):
            if offset > 0:
                offset -= 1
            if len(out) >= out_len:
                return bytes(out)
            sect = (((((len(out) & 7) << 8) + prev) >> head) & 7) * 0xFF - 1
            index = 1
            while (index >> 8) == 0:
                index = (index << 1) + bit(sect + index)
            out.append(index & 0xff)
        else:
            index = -1
            while True:
                s1 += 8
                f = bit(s1)
                index += f
                if not (f and index < 6):
                    break
            b_size = 0x160
            s2 = index + 0x7F1
            if index >= 0 or f:
                sect = (index << 5) | (((len(out) << index) & 3) << 3) | (offset & 7)
                dlen, f = number(0xBA8 + sect, index, 8, 0x18)
                if dlen == 0xFF:
                    return bytes(out)
            else:
                dlen = 1
            if dlen <= 2:
                s2 += 0xF8
                b_size = 0x40
            shift = 1
            while True:
                diff = (shift << 4) - b_size
                f = bit(s2 + (shift << 3))
                shift = (shift << 1) + f
                if diff >= 0:
                    break
            if diff > 0 or f:
                if not f:
                    diff -= 8
                doff, f = number(0x928 + diff, diff // 8, 1, 4)
            else:
                doff = 1
            start = len(out) - doff
            end = len(out) + dlen + 1
            if start < 0 or end > out_len:
                raise ValueError('lz: bad match at %d' % len(out))
            offset = ((end + 1) & 1) + 6
            while len(out) < end:
                out.append(out[start])
                start += 1
        prev = out[-1]


# ---------------------------------------------------------------- PBP
def bcd(x):
    return (x >> 4) * 10 + (x & 15)


def read_header(pbp_path, keys_path):
    vk = open(keys_path, 'rb').read(16)
    with open(pbp_path, 'rb') as f:
        head = f.read(0x28)
        if head[:4] != b'\0PBP':
            raise ValueError('not a PBP')
        offs = struct.unpack_from('<8I', head, 8)
        psar = offs[7]
        f.seek(psar)
        magic = f.read(12)
        if magic != b'PSISOIMG0000':
            raise ValueError('unsupported PSAR %r (multi-disc PSTITLEIMG not handled)' % magic)
        f.seek(psar + 0x400)
        hdr = pgd_decrypt(f.read(0xB6600), vk)
    disc_id = hdr[:0x20].split(b'\0')[0].decode('ascii', 'replace')
    toc = {}
    for i in range(0x400, 0x400 + 10 * 102, 10):
        e = hdr[i:i + 10]
        if not any(e):
            break
        toc[e[2]] = (e[0], bcd(e[7]), bcd(e[8]), bcd(e[9]))
    blocks = []
    for i in range(0x3C00, len(hdr) - 32, 32):
        o, s, fl = struct.unpack_from('<IHH', hdr, i)
        if s == 0:
            break
        blocks.append((o, s, fl, hdr[i + 8:i + 24]))
    end, unk = struct.unpack_from('<II', hdr, 0xE20)
    title = hdr[0xE2C:0xE2C + 0x80].split(b'\0')[0].decode('utf-8', 'replace')
    return {'psar': psar, 'disc_id': disc_id, 'title': title, 'iso_end': end,
            'toc': toc, 'blocks': blocks, 'header': hdr}


def cmd_info(a):
    h = read_header(a.pbp, a.keys)
    lo = h['toc'].get(0xA2)
    sectors = len(h['blocks']) * 16
    info = {'disc_id': h['disc_id'], 'title': h['title'], 'blocks': len(h['blocks']),
            'sectors': sectors, 'image_bytes': sectors * 2352,
            'leadout_msf': lo[1:] if lo else None,
            'compressed_bytes': sum(b[1] for b in h['blocks']),
            'stored_blocks': sum(1 for b in h['blocks'] if b[1] == 0x9300),
            'flags': sorted({b[2] for b in h['blocks']})}
    for k, v in info.items():
        print('%-17s %s' % (k, v))
    if a.json:
        import json
        json.dump(info, open(a.json, 'w'), indent=1)


def cmd_extract(a):
    h = read_header(a.pbp, a.keys)
    base = h['psar'] + 0x100000
    with open(a.pbp, 'rb') as f, open(a.out, 'wb') as o:
        for n, (off, size, fl, _) in enumerate(h['blocks']):
            f.seek(base + off)
            data = f.read(size)
            o.write(data if size == 0x9300 else lz_decompress(data, 0x9300))
    print('wrote', a.out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    sp = ap.add_subparsers(dest='cmd', required=True)
    p = sp.add_parser('info')
    p.add_argument('pbp'); p.add_argument('keys'); p.add_argument('--json')
    p.set_defaults(fn=cmd_info)
    p = sp.add_parser('extract')
    p.add_argument('pbp'); p.add_argument('keys'); p.add_argument('out')
    p.set_defaults(fn=cmd_extract)
    a = ap.parse_args()
    try:
        a.fn(a)
    except ValueError as e:
        sys.exit('error: %s' % e)


if __name__ == '__main__':
    main()
