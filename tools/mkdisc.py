#!/usr/bin/env python3
"""Rebuilds the PAL disc image (.bin/.cue) from game/disc/fs with mkpsxiso. Nothing from the disc is stored in the repo.

The layout (file order, LBAs, dates, license area, the 150-sector ZZZ gap) is not guessed: it is read from the
original image with dumpsxiso (`-s` writes an mkpsxiso XML that reproduces the disc), then the XML is rewritten
so that every ordinary file (type="data") comes from game/disc/fs (so edited files are packed) and the 30 MOVIE
`*.STR` files (type="mixed": XA/STR sectors with their own subheaders, form 1 and form 2 mixed) come from the raw
dump, because game/disc/fs only keeps the user data of those sectors (see tools/isoext.py) and cannot be turned
back into sectors. If a file in fs changes size, mkpsxiso moves the following LBAs by itself (file order is
kept). The game reads files by the LBA/size table inside EXE/MAIN0..8.EXE (notes/cdfiles.md), so with --relayout
the tables are rewritten from the new layout (tools/cdfiles.py) and the image is built again.

  tools/mkdisc.py [build] [-o OUT.bin] [--fs DIR] [--disc ORIGINAL.bin] [--work DIR]
                  writes OUT.bin and OUT.cue (default build/mkdisc/Tombi.bin)
  tools/mkdisc.py build --relayout
                  after building, reads the LBAs/sizes of the new image, patches the file tables of EXE/MAIN*.EXE in --fs (IN PLACE, only
                  when something changed; names come from the executables of the original disc) and builds again; also writes OUT.layout.json
  tools/mkdisc.py --check
                  builds into /tmp (dump cache in /tmp/mkdisc_work), compares size and SHA-1 with the original image, lists the sectors that
                  differ (if any) and exits 1 if the image is not identical

Tools: mkpsxiso and dumpsxiso 2.30 (https://github.com/Lameguy64/mkpsxiso, build it with cmake; the git submodule
of psx_tomba is not needed). Looked up in $MKPSXISO_DIR, then /opt/psyq/mkpsxiso/build, then PATH.
"""
import argparse, hashlib, os, shutil, subprocess, sys, tempfile
import xml.etree.ElementTree as ET

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cdfiles

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FS = os.path.join(ROOT, "game", "disc", "fs")
DISC = os.path.join(ROOT, "game", "disc", "Tombi.bin")
WORK = os.path.join(ROOT, "build", "mkdisc")
SECTOR = 2352


def tool(name):
    for d in (os.environ.get("MKPSXISO_DIR"), "/opt/psyq/mkpsxiso/build"):
        if d and os.access(os.path.join(d, name), os.X_OK):
            return os.path.join(d, name)
    p = shutil.which(name)
    if p:
        return p
    sys.exit("%s not found: build https://github.com/Lameguy64/mkpsxiso (cmake) and set MKPSXISO_DIR=<build dir>" % name)


def run(cmd, quiet=True):
    r = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode:
        sys.stderr.write(r.stdout)
        sys.exit("failed: %s" % " ".join(cmd))
    return r.stdout


def dump_layout(disc, work):
    """dumpsxiso the original image into work/dump (raw files + license) and work/layout.xml."""
    dump = os.path.join(work, "dump")
    xml = os.path.join(work, "layout.xml")
    stamp = os.path.join(work, "dump.stamp")
    key = "%s %d" % (os.path.abspath(disc), os.path.getmtime(disc))
    if not (os.path.exists(xml) and os.path.exists(stamp) and open(stamp).read() == key):
        shutil.rmtree(dump, ignore_errors=True)
        os.makedirs(work, exist_ok=True)
        print("dumpsxiso %s -> %s" % (disc, dump), file=sys.stderr)
        run([tool("dumpsxiso"), "-x", dump, "-s", xml, disc])
        open(stamp, "w").write(key)
    return dump, xml


def make_project(xml, dump, fs, out_bin, out_cue, project):
    tree = ET.parse(xml)
    root = tree.getroot()
    root.set("image_name", out_bin)
    root.set("cue_sheet", out_cue)
    track = root.find("track")
    lic = track.find("license")
    lic.set("file", os.path.join(dump, "license_data.dat"))
    stats = {"fs": 0, "dump": 0, "missing": []}

    def walk(el, rel):
        for ch in el:
            if ch.tag == "dir":
                walk(ch, rel + [ch.get("name")])
            elif ch.tag == "file":
                path = os.path.join(*(rel + [ch.get("name")]))
                from_fs = os.path.join(fs, path)
                from_dump = os.path.join(dump, path)
                if ch.get("type") == "data" and os.path.isfile(from_fs):
                    src = from_fs
                    stats["fs"] += 1
                else:
                    src = from_dump
                    stats["dump"] += 1
                    if ch.get("type") == "data":
                        stats["missing"].append(path)
                ch.set("source", os.path.abspath(src))
    walk(track.find("directory_tree"), [])
    tree.write(project)
    return stats


def build(a):
    work = os.path.abspath(a.work)
    os.makedirs(work, exist_ok=True)
    dump, xml = dump_layout(a.disc, work)
    out_bin = os.path.abspath(a.out)
    out_cue = os.path.splitext(out_bin)[0] + ".cue"
    os.makedirs(os.path.dirname(out_bin), exist_ok=True)
    project = os.path.join(work, "project.xml")
    st = make_project(xml, dump, os.path.abspath(a.fs), out_bin, out_cue, project)
    run([tool("mkpsxiso"), "-y", "-q", "-o", out_bin, "-c", out_cue, project])
    if a.relayout:
        if relayout(a, dump, out_bin):
            run([tool("mkpsxiso"), "-y", "-q", "-o", out_bin, "-c", out_cue, project])
            relayout(a, dump, out_bin)   # second pass: layout must not move (EXE sizes are fixed); 0 changes expected
    # mkpsxiso writes the .bin name it was given in the cue
    print("built %s (%d bytes, %d sectors); %d files from %s, %d from the raw dump"
          % (out_bin, os.path.getsize(out_bin), os.path.getsize(out_bin) // SECTOR, st["fs"], a.fs, st["dump"]))
    for m in st["missing"]:
        print("warning: %s not in %s, taken from the dump" % (m, a.fs), file=sys.stderr)
    return out_bin


def relayout(a, dump, out_bin):
    """Rewrites the file tables of fs/EXE/MAIN*.EXE from the layout of out_bin; True if an executable changed."""
    import glob, json
    ref = cdfiles.layout_from_image(a.disc)
    lay = cdfiles.layout_from_image(out_bin)
    changed = 0
    for exe in sorted(glob.glob(os.path.join(a.fs, "EXE", "MAIN*.EXE"))):
        pristine = os.path.join(dump, "EXE", os.path.basename(exe))
        cur = open(exe, "rb").read()
        new, n = cdfiles.update_bytes(cur, open(pristine, "rb").read(), ref, lay)
        if n:
            open(exe, "wb").write(new)
            changed += 1
        print("relayout %s: %d table entries changed" % (os.path.relpath(exe, a.fs), n))
    json.dump({k: list(v) for k, v in lay.items()}, open(os.path.splitext(out_bin)[0] + ".layout.json", "w"), indent=0)
    return changed


def sha1(path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for blk in iter(lambda: f.read(1 << 22), b""):
            h.update(blk)
    return h.hexdigest()


def diff_sectors(a, b, limit=20):
    """Prints the sectors that differ between two raw images."""
    bad = []
    with open(a, "rb") as fa, open(b, "rb") as fb:
        n = 0
        while True:
            x, y = fa.read(SECTOR), fb.read(SECTOR)
            if not x and not y:
                break
            if x != y:
                bad.append(n)
            n += 1
    print("sectors that differ: %d (of %d)" % (len(bad), n))
    for s in bad[:limit]:
        print("  sector %d (LBA %d, offset 0x%X)" % (s, s, s * SECTOR))
    return bad


def check(a):
    tmp = tempfile.mkdtemp(prefix="mkdisc_check_")
    try:
        a.out = os.path.join(tmp, "Tombi.bin")
        out = build(a)
        s_new, s_old = sha1(out), sha1(a.disc)
        print("original SHA-1 %s\nrebuilt  SHA-1 %s" % (s_old, s_new))
        if s_new == s_old:
            print("identical: yes")
            return 0
        print("identical: NO (size %d vs %d)" % (os.path.getsize(out), os.path.getsize(a.disc)))
        diff_sectors(out, a.disc)
        return 1
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def main():
    argv = sys.argv[1:]
    if "--check" in argv:
        argv.remove("--check")
        argv.insert(0, "check")
    elif not argv or argv[0].startswith("-"):
        argv.insert(0, "build")
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    for name in ("build", "check"):
        s = sub.add_parser(name)
        s.add_argument("--fs", default=FS, help="extracted file system (default game/disc/fs)")
        s.add_argument("--disc", default=DISC, help="original image, source of the layout (default game/disc/Tombi.bin)")
        s.add_argument("--work", default=None, help="cache for the dump of the original image (default build/mkdisc; /tmp/mkdisc_work for --check)")
        if name == "build":
            s.add_argument("-o", "--out", default=os.path.join(WORK, "Tombi.bin"))
            s.add_argument("--relayout", action="store_true", help="patch the file tables of fs/EXE/MAIN*.EXE with the new LBAs/sizes and rebuild")
    a = p.parse_args(argv)
    if a.work is None:
        a.work = "/tmp/mkdisc_work" if a.cmd == "check" else WORK
    for path in (a.disc, a.fs):
        if not os.path.exists(path):
            sys.exit("%s not found" % path)
    a.relayout = getattr(a, "relayout", False)
    return check(a) if a.cmd == "check" else (build(a) and 0)


if __name__ == "__main__":
    sys.exit(main())
