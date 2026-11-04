#!/usr/bin/env python3
"""Download a public MEGA file link (https://mega.nz/file/ID#KEY, or the old #!ID!KEY form) without megatools: asks the
MEGA API for the file's download URL, streams the encrypted file and decrypts it (AES-128-CTR with the key of the link).
Works through an HTTPS proxy (plain urllib), which megadl 1.11 does not. Nothing of the link is stored anywhere.

  python3 tools/mega_dl.py 'https://mega.nz/file/ID#KEY' OUT

Needs the cryptography package (pip install cryptography) or, failing that, pycryptodome."""
import base64, json, os, re, struct, sys, urllib.request


def b64d(s):
    s = s.replace("-", "+").replace("_", "/")
    return base64.b64decode(s + "=" * (-len(s) % 4))


def ctr(key, iv):
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        return Cipher(algorithms.AES(key), modes.CTR(iv)).decryptor()
    except ImportError:
        from Crypto.Cipher import AES                      # pycryptodome
        c = AES.new(key, AES.MODE_CTR, initial_value=iv, nonce=b"")
        class D:                                            # the same .update() shape
            def update(self, b): return c.decrypt(b)
        return D()


def main(argv):
    if len(argv) != 2:
        print(__doc__); return 2
    link, out = argv
    m = re.search(r"mega(?:\.co)?\.nz/(?:file/([A-Za-z0-9_-]+)#|#!([A-Za-z0-9_-]+)!)([A-Za-z0-9_-]+)", link.strip())
    if not m:
        print("not a public MEGA file link", file=sys.stderr); return 2
    fid, key_b64 = m.group(1) or m.group(2), m.group(3)
    k = struct.unpack(">8I", b64d(key_b64)[:32])
    key = struct.pack(">4I", k[0] ^ k[4], k[1] ^ k[5], k[2] ^ k[6], k[3] ^ k[7])
    iv = struct.pack(">2I", k[4], k[5]) + b"\0" * 8
    req = urllib.request.Request("https://g.api.mega.co.nz/cs?id=0", data=json.dumps([{"a": "g", "g": 1, "p": fid}]).encode(),
                                 headers={"Content-Type": "application/json"})
    r = json.loads(urllib.request.urlopen(req, timeout=60).read())[0]
    if not isinstance(r, dict) or "g" not in r:
        print("MEGA API answered %r (a negative number is an error code; -9 = not found, -17 = quota)" % (r,), file=sys.stderr); return 1
    size, url = r["s"], re.sub(r"^http://", "https://", r["g"])   # MEGA hands out http URLs; https works and goes through proxies
    dec = ctr(key, iv)
    done = 0
    with urllib.request.urlopen(urllib.request.Request(url), timeout=120) as src, open(out, "wb") as f:
        while True:
            chunk = src.read(1 << 20)
            if not chunk:
                break
            f.write(dec.update(chunk)); done += len(chunk)
            if done % (50 << 20) < (1 << 20):
                print("  %d / %d MB" % (done >> 20, size >> 20), file=sys.stderr)
    if done != size:
        print("short download: %d of %d bytes" % (done, size), file=sys.stderr); os.unlink(out); return 1
    print("%s: %d bytes" % (out, size))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
