#!/usr/bin/env python3
"""The game library of the PC launcher (tools/port_launcher.py, docs/PORT_VERSIONS_PLAN.md stage U2): which release a
disc image is, where its data/ folder goes, which port binary runs it, which launcher options it has, and its card art
(the title-screen logo and landscape, rebuilt at run time from the player's own disc: nothing of the game is shipped).

A disc is identified by its boot executable: SYSTEM.CNF `BOOT = cdrom:\\SCUS_942.36;1` and the sha1 of that file (the
PAL discs boot a small loader, SCES_013.31 / SCES_013.30, that runs EXE/MAIN0.EXE: their sha1 is MAIN0's).

  python3 tools/game_library.py IMAGE_OR_DATA_DIR [--art OUT_DIR]     # what it is (and logo.png / title.png)

Qt-free (tools/test_port_launcher.py tests it without a display); the launcher turns the RGBA art into pixmaps.
"""
import hashlib, os, re, struct, sys, zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import extract_data as X

PAL_SHA1 = X.MAIN0_SHA1           # EXE/MAIN0.EXE of SCES-01331 / SCES-01330 Rev 1 (the PAL port's build)
PAL_REV0_SHA1 = X.REV0_SHA1       # the July 1998 Rev 0 of both: other overlays, not supported
USA_SHA1 = "81cbc79f0230aeb4252e058039f47ac95a777f5a"     # SCUS_942.36
JP_SHA1 = "07d577698ad31ec1a69b9d7b80a7c3f90b650888"      # SLPS_011.44

# What the launcher offers for a version (keys of FEATURES): the port options that read PAL addresses or the PAL
# disc's files are PAL-only in the USA build (docs/USA.md "Native port": --lang, --text-patch, --picture ntsc,
# --cut-content, --civ-test, the panel, the title DEBUG entry).
#   lang       the nine languages of the boot menu (--lang)
#   patch      text patches / translations applied to data/ (--text-patch, the improved Spanish translation)
#   hz         the 50 / 60 Hz switch (a PAL game; NTSC versions always run at 60)
#   cut        the Cut content page (--cut-content, --early-title, the rebuilt towns)
#   pokes      every mod of tools/port_mods.py; without it only port_mods.PORTABLE_MODS (the cheats, area select and
#              debug menu name their fields and run in the USA build too; title DEBUG and the 50/60 Hz switch do not)
#   prompts    the button-icon styles (--prompts: redraws the PAL fonts' glyphs)
#   texdump    "Dump all textures" (tools/dump_all_textures.py reads the PAL disc layout)
PAL_FEATURES = frozenset(("lang", "patch", "hz", "cut", "pokes", "prompts", "texdump"))
USA_FEATURES = frozenset()

# Every version the launcher knows. key: the boot executable's sha1.
#   version    port version (port/tools/portver.py: PORT_VERSION; "" = PAL)
#   binary     the port executable next to the launcher (+ .exe on Windows); build_dir: the checkout's CMake tree
#   save_sub   the subfolder of the save folder holding this game's card (tombi_mc0.mcd): "" = the folder itself (PAL,
#              as always); USA saves_usa, the same name as on Android (Library.java saveSub)
#   text_patches  bundled per-version text patches (patches/<file>, tools/data_patch.py); the hook for future
#                 retranslations of the USA / JP texts: none yet (the PAL one is the improved Spanish translation)
#   art        (logo page, its CLUT, title landscape, its CLUT, the title overlay, the landscape's y on screen): files of
#              the disc (see logo_sprites)
VERSIONS = {
    PAL_SHA1: dict(key="pal", version="", title="Tombi!", region="PAL", serials=("SCES-01331", "SCES-01330"),
                   supported=True, binary="tombi-pc", build_dir="port2", hz=50, features=PAL_FEATURES, save_sub="",
                   text_patches=("es_mejorada.tombipatch.json",),
                   art=("SYSTEM/A00202.000", "SYSTEM/A00203.000", "SYSTEM/A00200.GAM", "SYSTEM/A00201.000", "SYSTEM/O0PTSUB0.BIN", 8)),
    USA_SHA1: dict(key="usa", version="usa", title="Tomba!", region="USA", serials=("SCUS-94236",),
                   supported=True, binary="tombi-pc-usa", build_dir="port2-scus94236", hz=60, features=USA_FEATURES,
                   save_sub="saves_usa",
                   text_patches=(),
                   art=("SYSTEM/A00012.000", "SYSTEM/A00013.000", "SYSTEM/A00010.GAM", "SYSTEM/A00011.000", "SYSTEM/OPTSUB00.BIN", 0)),
    JP_SHA1: dict(key="jp", version="jp", title="Ore no Tomba", region="Japan", serials=("SLPS-01144",),
                  supported=False, binary="tombi-pc-jp", build_dir="port2-slps01144", hz=60, features=frozenset(),
                  save_sub="saves_jp",
                  text_patches=(),
                  art=("SYSTEM/A00012.000", "SYSTEM/A00013.000", "SYSTEM/A00010.GAM", "SYSTEM/A00011.000", "SYSTEM/OPTSUB00.BIN", 0)),
    PAL_REV0_SHA1: dict(key="pal-rev0", version=None, title="Tombi! (Rev 0)", region="PAL", serials=("SCES-01331", "SCES-01330"),
                        supported=False, binary="", build_dir="", hz=50, features=frozenset(), text_patches=(),
                        art=("SYSTEM/A00202.000", "SYSTEM/A00203.000", "SYSTEM/A00200.GAM", "SYSTEM/A00201.000", "SYSTEM/O0PTSUB0.BIN", 8)),
}
# the boot menu languages of each PAL disc (SCES-01331: EXE/MAIN0 + MAIN5..8, SCES-01330: MAIN0 + MAIN1..4); the port
# runs all nine from either disc (they share every game file of their revision)
DISC_LANGS = {"SCES-01331": "EN ES PT SV DA", "SCES-01330": "EN FR DE IT NL", "SCUS-94236": "EN", "SLPS-01144": "JA"}
# other Tomba releases a player might add by mistake
OTHER_DISCS = {"SCUS-94454": "Tomba! 2 (USA)", "SCES-02005": "Tombi! 2 (PAL)", "SLPM-80165": "Ore no Tomba (Japan, demo)"}


def version_of(sha1):
    return VERSIONS.get(sha1 or "")


def features(sha1):
    v = version_of(sha1)
    return v["features"] if v else frozenset()


def has(sha1, feature):
    return feature in features(sha1)


def text_patches(sha1):
    """The bundled text patches of a version (the per-version retranslation hook)."""
    v = version_of(sha1)
    return tuple(v["text_patches"]) if v else ()


def binary_name(sha1, windows=None):
    v = version_of(sha1)
    if not v or not v["binary"]:
        return None
    windows = os.name == "nt" if windows is None else windows
    return v["binary"] + (".exe" if windows else "")


# ---------------------------------------------------------------------------------------------- identification
def _boot_path(cnf_text):
    for line in cnf_text.splitlines():
        k, _, v = line.partition("=")
        if k.strip().upper() == "BOOT":
            return v.strip().split(":", 1)[-1].lstrip("\\").split(";")[0].replace("\\", "/")
    return ""


def describe(boot, boot_sha1, main0_sha1=""):
    """{serial, boot, boot_sha1, game_sha1, version (VERSIONS entry or None), name} from what a disc boots."""
    boot = boot or ""
    serial = X.serial_of(os.path.basename(boot)) if boot else ""
    game = main0_sha1 if boot.upper().startswith("SCES_") and main0_sha1 else boot_sha1
    if not boot and main0_sha1:            # (an old data/ manifest: MAIN0 only)
        game = main0_sha1
    v = version_of(game)
    if v and serial not in v["serials"]:
        serial = v["serials"][0] if not serial else serial
    name = v["title"] + " (%s)" % v["region"] if v else OTHER_DISCS.get(serial, "")
    return dict(serial=serial, boot=boot, boot_sha1=boot_sha1 or "", game_sha1=game or "", version=v, name=name)


def identify_image(path):
    """What a disc image (.bin / .cue / .iso / .chd) is: describe() of its SYSTEM.CNF boot file. Raises ValueError for
    a file that is not an ISO9660 image."""
    img = X.Image(path)
    try:
        files = img.files()
        boot, boot_sha = X.boot_exe(img, files)
        main0 = ""
        byname = {p.upper(): (lba, size) for p, lba, size in files}
        if "EXE/MAIN0.EXE" in byname:
            lba, size = byname["EXE/MAIN0.EXE"]
            main0 = hashlib.sha1(img.user(lba, (size + 2047) // 2048)[:size]).hexdigest()
        return describe(boot, boot_sha, main0)
    finally:
        img.close()


def identify_data(data_dir):
    """describe() of an extracted data/ folder (its manifest), or None without a manifest."""
    man = X.read_manifest(data_dir)
    if not man:
        return None
    boot = man.get("boot") or ""
    if boot == "EXE/MAIN0.EXE":       # extract_data records the game exe of the PAL discs; the serial comes from it
        boot = {"SCES-01330": "SCES_013.30"}.get(man.get("serial"), "SCES_013.31")
    return describe(boot, man.get("boot_sha1") or "", man.get("main0_sha1") or "")


def data_complete(data_dir):
    """The game sha1 of a complete extraction in data_dir (every file there with its size), else None."""
    if X.manifest_sha1(data_dir) is None:
        return None
    d = identify_data(data_dir)
    return d["game_sha1"] if d else None


# ---------------------------------------------------------------------------------------------- the library
# A library entry (the launcher saves the list in tombi-port.json "games"):
#   {"serial": "SCES-01331", "sha1": <game sha1>, "image": <disc image path or "">, "data": <folder name next to the
#    launcher>}
# One entry per serial. The first PAL game keeps the data/ folder the launcher always used (no re-extraction for
# players who update); every other game gets data-<serial>/.

def data_name(serial, entries=()):
    pal = serial in VERSIONS[PAL_SHA1]["serials"]
    if pal and not any(e.get("data") == "data" and e.get("serial") != serial for e in entries):
        return "data"
    return "data-" + re.sub(r"[^a-z0-9]", "", (serial or "unknown").lower())


def add_entry(entries, info, image):
    """entries (a list of dicts) with the game described by info (identify_image) added or updated; returns the entry."""
    for e in entries:
        if e.get("serial") == info["serial"]:
            e.update(image=image or e.get("image", ""), sha1=info["game_sha1"])
            return e
    e = dict(serial=info["serial"], sha1=info["game_sha1"], image=image or "", data=data_name(info["serial"], entries))
    entries.append(e)
    return e


def migrate(settings, app_dir):
    """The game list of settings, created from an older launcher's single disc: its data/ folder (extracted from the
    disc it had) and the "disc" path. Returns True when it changed settings."""
    if isinstance(settings.get("games"), list):
        return False
    games = []
    d = identify_data(os.path.join(app_dir, "data"))
    if d and d["serial"]:
        games.append(dict(serial=d["serial"], sha1=d["game_sha1"], image=settings.get("disc") or "", data="data"))
    elif settings.get("disc"):
        # a disc picked but never extracted: identified (and extracted) when the launcher checks it
        games.append(dict(serial="", sha1="", image=settings.get("disc"), data="data"))
    settings["games"] = games
    if games and not settings.get("game"):
        settings["game"] = games[0]["serial"]
    return True


def sort_key(e):
    """Library order: supported versions first (PAL, USA, then the rest), then by serial."""
    order = {"pal": 0, "usa": 1, "jp": 2}
    v = version_of(e.get("sha1"))
    return (order.get(v["key"], 3) if v else 4, e.get("serial") or "~")


# ---------------------------------------------------------------------------------------------- art
def gam_unpack(d):
    """`GAM\\0` + u32 size + 16-bit flag words (bit set: copy `length` bytes from `offset` back); tools/area000.py."""
    if d[:4] != b"GAM\0":
        raise ValueError("not a GAM")
    size = struct.unpack_from("<I", d, 4)[0]
    out = bytearray()
    p, flags, left = 8, 0, 0
    while len(out) < size:
        if left == 0:
            flags = d[p] | d[p + 1] << 8
            p += 2
            left = 16
        bit = flags & 1
        flags >>= 1
        left -= 1
        if bit:
            off, ln = d[p], d[p + 1]
            p += 2
            for _ in range(ln):
                out.append(out[-off])
        else:
            out.append(d[p])
            p += 1
    return bytes(out[:size])


class DirSource:
    """Files of an extracted data/ folder."""
    def __init__(self, d):
        self.d = d

    def read(self, name):
        with open(os.path.join(self.d, *name.split("/")), "rb") as f:
            return f.read()


class ImageSource:
    """Files read straight from the disc image (the art of a game not extracted yet)."""
    def __init__(self, path):
        self.img = X.Image(path)
        self.files = {p.upper(): (lba, size) for p, lba, size in self.img.files()}

    def read(self, name):
        lba, size = self.files[name.upper()]
        return self.img.user(lba, (size + 2047) // 2048)[:size]

    def close(self):
        self.img.close()


def _c8(v):
    return (v << 3) | (v >> 2)


def _clut(b):
    """256 PS1 colours -> RGBA bytes (colour 0 = transparent, as the GPU draws it)."""
    out = []
    for v in struct.unpack_from("<256H", b):
        out.append(bytes((_c8(v & 31), _c8(v >> 5 & 31), _c8(v >> 10 & 31), 0 if v == 0 else 255)))
    return out


def _page(src, page, clut, wpx=256):
    d = src.read(page)
    if page.upper().endswith(".GAM"):
        d = gam_unpack(d)
    c = src.read(clut)
    if len(c) < 512 or len(d) < wpx:
        raise ValueError("%s: not an 8 bpp page" % page)
    return d, _clut(c), wpx, len(d) // wpx


# The logo is drawn by the title overlay (O0PTSUB0 / OPTSUB00) from its own sprite list TITLE_LOGO_SPRITE_RECS
# (src/o0ptsub0/titleLogoDraw.c, titleDrawSpriteRecs): 16-byte records {x, y, u, v, w, h, tpage, clut x, clut y}
# ending at x = -1; the logo's records use the CLUT uploaded at (128, 510). PAL: page 8,0 240x112 at 16,24 and
# 8,112 48x112 at 256,24; USA: a wider "!" piece; JP: "オレっ!" at 16,40 and "トンバ" at 144,24.
SPR = struct.Struct("<hhBBhhhhh")


def logo_sprites(ovl):
    """[(x, y, u, v, w, h)] of the first sprite list in the overlay bytes whose records use the logo CLUT (128, 510)."""
    for o in range(0, len(ovl) - 2 * SPR.size, 2):
        out, q = [], o
        while q + SPR.size <= len(ovl):
            x, y, u, v, w, h, tp, cx, cy = SPR.unpack_from(ovl, q)
            if (cx, cy) != (128, 510) or not (0 < w <= 256 and 0 < h <= 256) or u + w > 256 or v + h > 256:
                break
            out.append((x, y, u, v, w, h))
            q += SPR.size
        if out and SPR.unpack_from(ovl, q)[0] == -1:
            return out
    raise ValueError("no logo sprite list")


def _compose(rows, sprites, w, h):
    """The logo sprites drawn from the page rows (RGBA) -> (x0, y0, W, H, RGBA bytes) cropped to the opaque pixels,
    x0 / y0 = where that crop sits on the game's screen."""
    pts = {}
    for x, y, u, v, sw, sh in sprites:
        for yy in range(v, min(h, v + sh)):
            r = rows[yy]
            for xx in range(u, min(w, u + sw)):
                if r[xx * 4 + 3]:
                    pts[(x + xx - u, y + yy - v)] = r[xx * 4:xx * 4 + 4]
    if not pts:
        raise ValueError("empty logo")
    x0 = min(p[0] for p in pts); y0 = min(p[1] for p in pts)
    W = max(p[0] for p in pts) - x0 + 1; H = max(p[1] for p in pts) - y0 + 1
    out = bytearray(W * H * 4)
    for (x, y), px in pts.items():
        o = ((y - y0) * W + (x - x0)) * 4
        out[o:o + 4] = px
    return x0, y0, W, H, bytes(out)


def _logo(src, sha1):
    v = version_of(sha1)
    if not v:
        raise ValueError("unknown version")
    page, clut, _l, _lc, ovl, _y = v["art"]
    d, pal, w, h = _page(src, page, clut)
    rows = [b"".join(pal[i] for i in d[y * w:(y + 1) * w]) for y in range(h)]
    return _compose(rows, logo_sprites(src.read(ovl)), w, h)


def logo_rgba(src, sha1):
    """The game's title logo (RGBA, transparent around it) from its own files: (w, h, bytes). ValueError / OSError /
    KeyError when the files are not there or not what this version has."""
    return _logo(src, sha1)[2:]


def title_rgba(src, sha1, logo=True):
    """The title screen as a card picture: the 320 x 240 landscape (the PAL frame shows it at y 8) with the logo
    where the game draws it (logo=False: the landscape alone). (w, h, RGBA bytes)."""
    v = version_of(sha1)
    _p, _c, land, landc, _o, land_y = v["art"]
    d, pal, w, h = _page(src, land, landc, 512)
    W, H = 320, 240
    out = bytearray(b"".join(b"".join(pal[i][:3] + b"\xff" for i in d[y * w:y * w + W]) for y in range(H)))
    if not logo:
        return W, H, bytes(out)
    ox, oy, lw, lh, lb = _logo(src, sha1)
    oy -= land_y
    for y in range(lh):
        for x in range(lw):
            o = (y * lw + x) * 4
            if lb[o + 3] and 0 <= ox + x < W and 0 <= oy + y < H:
                t = ((oy + y) * W + ox + x) * 4
                out[t:t + 4] = lb[o:o + 4]
    return W, H, bytes(out)


def png(w, h, rgba):
    raw = b"".join(b"\0" + rgba[y * w * 4:(y + 1) * w * 4] for y in range(h))
    def chunk(t, b):
        return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b) & 0xFFFFFFFF)
    return (b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw, 6)) + chunk(b"IEND", b""))


ART = ("logo", "title", "scene")


def make_art(src, sha1, out_dir):
    """logo.png (the logo alone), title.png (the title screen) and scene.png (its landscape alone) of a game into out_dir
    (the launcher's cache, next to it, never in the repo); returns the paths written ({} when this disc has no art the
    launcher knows)."""
    done = {}
    try:
        logo = logo_rgba(src, sha1)
    except (OSError, ValueError, KeyError, struct.error):
        return done
    os.makedirs(out_dir, exist_ok=True)
    p = os.path.join(out_dir, "logo.png")
    open(p, "wb").write(png(*logo))
    done["logo"] = p
    for name, with_logo in (("title", True), ("scene", False)):
        try:
            t = title_rgba(src, sha1, with_logo)
            p = os.path.join(out_dir, name + ".png")
            open(p, "wb").write(png(*t))
            done[name] = p
        except (OSError, ValueError, KeyError, struct.error):
            pass
    return done


def main(argv):
    if len(argv) < 2:
        print(__doc__)
        return 2
    p = argv[1]
    if os.path.isdir(p):
        info, src = identify_data(p), DirSource(p)
    else:
        import disc_archive as DA, shutil, tempfile
        if DA.kind(p) in ("zip", "7z", "rar"):          # the image inside a .zip / .7z (unpacked to a temporary folder)
            tmp = tempfile.mkdtemp(prefix="tombi-unpack-")
            try:
                p = DA.extract_image(p, tmp)
                info, src = identify_image(p), ImageSource(p)
                return _report(argv, p, info, src)
            except DA.ArchiveError as e:
                print(e)
                return 1
            finally:
                shutil.rmtree(tmp, ignore_errors=True)
        info, src = identify_image(p), ImageSource(p)
    return _report(argv, p, info, src)


def _report(argv, p, info, src):
    if not info:
        print("%s: no data manifest" % p)
        return 1
    v = info["version"]
    print("%s: %s %s, boot %s sha1 %s%s" % (p, info["serial"] or "?", info["name"] or "unknown disc", info["boot"] or "?",
                                         (info["game_sha1"] or "?")[:8],
                                         "" if not v else ", supported" if v["supported"] else ", recognised, not supported yet"))
    if "--art" in argv and v:
        out = argv[argv.index("--art") + 1]
        for k, f in sorted(make_art(src, info["game_sha1"], out).items()):
            print("  %s: %s" % (k, f))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
