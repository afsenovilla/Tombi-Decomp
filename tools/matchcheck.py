#!/usr/bin/env python3
"""Compila src/*.c con el GCC 2.7.2.SN.1 original (Psy-Q, vía DOSBox) y compara
el código con el del juego byte a byte (enmascarando relocaciones).

Cada .c lleva en sus primeras líneas:
    // FUNC <addr_hex> <size> [MAIN0|X000]     (una función por fichero)
    // FLAGS -O2 -G0                            (opcional)
Uso:  tools/matchcheck.py [--mark] [--asm] [src/fichero.c ...]
  --mark  añade `// MATCHING <addr> <size>` a los que coinciden (lo lee progress.py)
  --asm   deja el ensamblador generado en build/<nombre>.s
Entorno: PSYQ_DIR (carpeta con CC1PSX.EXE, ASPSX.EXE, CPPPSX.EXE; por defecto /opt/psyq/new)
         WORK (carpeta de trabajo para DOSBox; por defecto /opt/psyq/w)
El SDK es de Sony: no se sube al repo.
"""
import os, re, shutil, struct, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PSYQ = os.environ.get("PSYQ_DIR", "/opt/psyq/new")
WORK = os.environ.get("WORK", "/opt/psyq/w")
GAME = os.path.join(ROOT, "game")
IMAGES = {"MAIN0": ("MAIN0.EXE", "exe"), "X000": ("AREA00/X000.BIN", "raw")}
RAW_BASE = {"X000": 0x800E8028}


def game_bytes(prog, addr, size):
    fn, kind = IMAGES[prog]
    d = open(os.path.join(GAME, fn), "rb").read()
    if kind == "exe":
        load = struct.unpack("<I", d[0x18:0x1C])[0]
        off = 0x800 + addr - load
    else:
        off = addr - RAW_BASE[prog]
    return d[off:off + size]


def parse_expr(d, i):
    b = d[i]; i += 1
    if b == 0x00: return i + 4
    if b in (0x02, 0x04, 0x06, 0x08, 0x0C): return i + 2
    if b >= 0x20:  # operador binario prefijo
        i = parse_expr(d, i)
        return parse_expr(d, i)
    raise ValueError("expr op %02x" % b)


def obj_text(path):
    """Devuelve (bytes .text, lista de (offset, tipo)) de un PSY-Q OBJ."""
    d = open(path, "rb").read()
    assert d[:3] == b"LNK"
    i = 4; sect = None; names = {}; code = bytearray(); relocs = []; base = 0
    while i < len(d):
        t = d[i]; i += 1
        if t == 0x00: break
        elif t == 0x10:
            sid = struct.unpack("<H", d[i:i + 2])[0]; n = d[i + 5]
            names[sid] = d[i + 6:i + 6 + n].decode(); i += 6 + n
        elif t == 0x1C:
            n = d[i + 2]; i += 3 + n
        elif t == 0x06:
            sect = names.get(struct.unpack("<H", d[i:i + 2])[0]); i += 2
        elif t == 0x02:
            n = struct.unpack("<H", d[i:i + 2])[0]; i += 2
            if sect == ".text":
                base = len(code); code += d[i:i + n]
            i += n
        elif t == 0x0A:
            ty = d[i]; off = struct.unpack("<H", d[i + 1:i + 3])[0]
            i = parse_expr(d, i + 3)
            if sect == ".text": relocs.append((base + off, ty))
        elif t == 0x2E: i += 1  # procesador (07 = R3000)
        elif t == 0x08: i += 4
        elif t == 0x0E: i += 2  # section switch alias
        else:
            # XDEF/XREF/etc: no hay más código tras esto
            break
    return bytes(code), relocs


def mask(code, relocs):
    c = bytearray(code)
    for off, ty in relocs:
        w = struct.unpack("<I", c[off:off + 4])[0]
        w &= 0xFC000000 if ty == 0x4A else 0xFFFF0000
        c[off:off + 4] = struct.pack("<I", w)
    return bytes(c)


def header(src):
    txt = open(src).read()
    m = re.search(r"//\s*FUNC\s+([0-9a-fA-F]+)\s+(\d+)\s*(MAIN0|X000)?", txt)
    f = re.search(r"//\s*FLAGS\s+(.*)", txt)
    if not m: return None
    return int(m.group(1), 16), int(m.group(2)), m.group(3) or "MAIN0", (f.group(1).strip() if f else "-O2 -G0")


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    mark = "--mark" in sys.argv; keep_asm = "--asm" in sys.argv
    files = args or sorted(os.path.join(ROOT, "src", f) for f in os.listdir(os.path.join(ROOT, "src")) if f.endswith(".c")) \
        if os.path.isdir(os.path.join(ROOT, "src")) else []
    jobs = []
    shutil.rmtree(WORK, ignore_errors=True); os.makedirs(WORK + "/inc")
    inc = os.path.join(ROOT, "include")
    if os.path.isdir(inc):
        for f in os.listdir(inc): shutil.copy(os.path.join(inc, f), WORK + "/inc/" + f.upper())
    bat = ["mount c %s" % os.path.dirname(WORK), "mount d %s" % PSYQ, "c:", "cd %s" % os.path.basename(WORK)]
    for n, f in enumerate(files):
        h = header(f)
        if not h: print("SKIP (sin // FUNC):", f); continue
        shutil.copy(f, "%s/F%d.C" % (WORK, n)); jobs.append((n, f, h))
        flags = h[3]
        bat += [r"d:\CPPPSX.EXE -undef -D__GNUC__=2 -DMIPSEL -IC:\%s\INC F%d.C F%d.I" % (os.path.basename(WORK).upper(), n, n),
                r"d:\CC1PSX.EXE -quiet %s F%d.I -o F%d.S" % (flags, n, n),
                r"d:\ASPSX.EXE -q F%d.S -o F%d.OBJ" % (n, n)]
    open(WORK + "/run.conf", "w").write("[sdl]\nfullscreen=false\n[cpu]\ncycles=max\n[autoexec]\n" + "\n".join(bat) + "\nexit\n")
    env = dict(os.environ, SDL_VIDEODRIVER="dummy", SDL_AUDIODRIVER="dummy", XDG_RUNTIME_DIR="/tmp")
    subprocess.run(["dosbox", "-conf", WORK + "/run.conf", "-noconsole"], env=env, timeout=600,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    ok = bad = 0
    for n, f, (addr, size, prog, _) in jobs:
        name = os.path.basename(f)
        obj = "%s/F%d.OBJ" % (WORK, n)
        if not os.path.exists(obj): print("ERROR compilación:", name); bad += 1; continue
        if keep_asm:
            os.makedirs(ROOT + "/build", exist_ok=True)
            shutil.copy("%s/F%d.S" % (WORK, n), ROOT + "/build/" + name.replace(".c", ".s"))
        code, rel = obj_text(obj)
        ref = game_bytes(prog, addr, size)
        if mask(code, rel) == mask(ref, rel) and len(code) == size:
            print("MATCH   %s  %08x %d" % (name, addr, size)); ok += 1
            if mark and "// MATCHING" not in open(f).read():
                t = open(f).read().splitlines()
                t.insert(1, "// MATCHING %08x %d" % (addr, size)); open(f, "w").write("\n".join(t) + "\n")
        else:
            bad += 1
            print("DIFF    %s  %08x  (ours %d B, game %d B)" % (name, addr, len(code), size))
            m1, m2 = mask(code, rel), mask(ref, rel)
            for k in range(0, max(len(m1), len(m2)), 4):
                a, b = m1[k:k + 4].hex(), m2[k:k + 4].hex()
                if a != b: print("   +%04x ours=%s game=%s" % (k, a, b))
    print("%d match, %d fallan" % (ok, bad))
    sys.exit(1 if bad else 0)


main()
