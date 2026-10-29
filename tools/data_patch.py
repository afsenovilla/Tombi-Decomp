#!/usr/bin/env python3
"""Applies a text patch (*.tombipatch.json, tools/text_patch.py) to an extracted data/ folder (tools/extract_data.py) for
the native port, reversibly: the original of every touched file (and manifest.txt) is kept in data/.orig/ and put back
before another patch is applied or when the patch is switched off. A patch with "portGfx": "es" also gets the port's
Spanish graphic redraws (tools/gfx_es), generated from data/ into data/port_gfx_es.tbtx (loaded with --patch-tex, --lang 5).

  python3 tools/data_patch.py apply DATA_DIR PATCH.tombipatch.json [--disc IMAGE] [--force]
  python3 tools/data_patch.py restore DATA_DIR
  python3 tools/data_patch.py status DATA_DIR

The patched WFM files are re-encoded from the pristine files of data/ (text_core.Project over the folder). Sizes:
  * manifest.txt gets the new byte size of every patched file (the port reads a file up to the manifest size);
  * a file that still fits its sectors on the disc keeps its LBA; the size entries of the file tables in
    EXE/MAIN1..8.EXE are updated too (the game reads whole sectors, so the old size reads the same sectors);
  * the area load lists SYS/LDnARxx.BIN hold their own size of each file (the next file is loaded right after it):
    Project.encode_edited rewrites those entries too (tools/load_lists.py), or a grown WFM got its tail overwritten in memory
    (garbage "UUUUUU" dialogues and a crash, AREA00);
  * a file that needs more sectors than the disc gave it is moved past the end of the disc (new LBA, manifest entry
    and `sectors` count) and the tables of EXE/MAIN1..8.EXE point there. The port takes MAINn's table from data/ for
    --lang N (runtime/lang.c), but MAIN0's table is compiled into the port: a grown file read through it (--lang 0)
    still gets its original sectors only, reported as a warning (the language files, *S.WFM etc., are not in it).
The disc check: the patch names the sha1 of its original image; with --disc it is compared, and data/ must come from
the PAL release (MAIN0.EXE sha1 of manifest.txt).
"""
import argparse, hashlib, json, os, shutil, sys

import extract_data as X

ORIG = ".orig"
STATE = "state.json"
GFX_TBTX = "port_gfx_es.tbtx"     # tools/gfx_es: graphic redraws for the port (--patch-tex), built from data/ when the patch asks


class PatchError(Exception):
    pass


def disc_sha1(path, chunk=1 << 22):
    """sha1 of a disc image (.bin / .iso, or the track a .chd holds, as raw sectors: the same as the .bin it came from)."""
    h = hashlib.sha1()
    f = X.open_image(X.bin_of_cue(path))
    try:
        for b in iter(lambda: f.read(chunk), b""):
            h.update(b)
    finally:
        f.close()
    return h.hexdigest()


def file_sha1(path):
    return hashlib.sha1(open(path, "rb").read()).hexdigest()


def status(data_dir):
    """The state of the applied patch ({"patch", "sha1", "title", "files", "moved"}) or None."""
    try:
        return json.load(open(os.path.join(data_dir, ORIG, STATE), encoding="utf-8"))
    except (OSError, ValueError):
        return None


def restore(data_dir):
    """Puts the originals kept in data/.orig back (every file there, manifest.txt among them); returns their count."""
    gfx = os.path.join(data_dir, GFX_TBTX)
    if os.path.isfile(gfx):
        os.remove(gfx)
    od = os.path.join(data_dir, ORIG)
    if not os.path.isdir(od):
        return 0
    n = 0
    for d, _, fs in os.walk(od):
        for f in fs:
            src = os.path.join(d, f)
            rel = os.path.relpath(src, od)
            if rel == STATE:
                continue
            dst = os.path.join(data_dir, rel)
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            shutil.copy2(src, dst + ".part")
            os.replace(dst + ".part", dst)
            n += 1
    shutil.rmtree(od)
    return n


def _write(path, data):
    with open(path + ".part", "wb") as o:
        o.write(data)
    os.replace(path + ".part", path)


def apply(data_dir, patch_path, disc=None, disc_hash=None, force=False, log=print):
    """Restores data/ and applies the patch. disc_hash (or disc, hashed here) is checked against the patch's original image.
    Returns {"files", "applied", "problems", "moved", "title"}."""
    import text_core as C
    import text_patch as TP
    import cdfiles
    restore(data_dir)
    man = X.read_manifest(data_dir)
    if not man or X.manifest_sha1(data_dir) is None:
        raise PatchError("data/ is not a complete extraction (manifest.txt)")
    if man["main0_sha1"] != X.MAIN0_SHA1 and not force:
        raise PatchError("data/ is not from the PAL disc SCES-01331 (MAIN0.EXE sha1 %s)" % man["main0_sha1"][:8])
    try:
        patch = TP.load(patch_path)
    except C.CoreError as e:
        raise PatchError(str(e))
    want = (patch.get("disc") or {}).get("sha1")
    if disc_hash is None and disc and os.path.isfile(X.bin_of_cue(disc)):
        disc_hash = disc_sha1(disc)
    if want and disc_hash and want != disc_hash and not force:
        raise PatchError("the patch is for the image with sha1 %s; this disc is %s" % (want[:12], disc_hash[:12]))
    try:
        proj = C.Project(C.Source(data_dir))
        applied, problems = TP.apply_to_project(proj, patch)
        files, errors = proj.encode_edited()
    except C.CoreError as e:
        raise PatchError(str(e))
    if errors:
        raise PatchError("\n".join(errors[:20]))
    if not files:
        raise PatchError("the patch changes nothing")
    by_path = {e[4]: i for i, e in enumerate(man["files"])}
    ref = {p: (lba, nbytes if stored == 2048 else n * 2048) for lba, n, stored, nbytes, p in man["files"]
           if not p.startswith(X.BLOB_DIR + "/")}
    lay = dict(ref)
    exes = sorted(p for p in ref if p.upper().startswith("EXE/MAIN") and p.upper() != "EXE/MAIN0.EXE")
    touched = sorted(set(files) | set(exes) | {X.MANIFEST})
    # back up first (and record what), so a failure half-way can still be undone by restore()
    od = os.path.join(data_dir, ORIG)
    for p in touched:
        src = os.path.join(data_dir, *p.split("/"))
        if not os.path.isfile(src):
            raise PatchError("%s is not in data/" % p)
        dst = os.path.join(od, *p.split("/"))
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        shutil.copy2(src, dst)
    main0 = open(os.path.join(data_dir, "EXE", "MAIN0.EXE"), "rb").read()
    in_main0 = {n for _, _, _, n in cdfiles.resolve(main0, ref)}
    moved = []
    for p, data in sorted(files.items()):
        if p.upper().startswith("EXE/"):
            continue
        i = by_path.get(p)
        if i is None:
            raise PatchError("%s is not in manifest.txt" % p)
        lba, n, stored, nbytes, path = man["files"][i]
        if stored != 2048:
            raise PatchError("%s: raw sectors, not patchable" % p)
        need = (len(data) + 2047) // 2048
        if need <= n:
            man["files"][i] = (lba, n, stored, len(data), path)
            lay[p] = (lba, len(data))
        else:
            new = man["sectors"]
            man["sectors"] += need
            man["files"][i] = (lba, n, stored, len(data), path)          # the old place reads its first sectors
            man["files"].append((new, need, 2048, len(data), path))
            lay[p] = (new, len(data))
            moved.append((p, lba, n, new, need))
            if p in in_main0:
                log("warning: %s grew from %d to %d sectors: moved to LBA %d for MAIN1..8, but MAIN0's table (compiled into "
                    "the port, --lang 0) still reads %d sectors" % (p, n, need, new, n))
    man["files"].sort()
    # the file tables of the language executables: new sizes (and LBAs of moved files)
    for e in exes:
        cur = files.get(e) or open(os.path.join(data_dir, *e.split("/")), "rb").read()
        pristine = open(os.path.join(od, *e.split("/")), "rb").read()
        try:
            new_bytes, ch = cdfiles.update_bytes(cur, pristine, ref, lay)
        except SystemExit as ex:
            raise PatchError("%s: %s" % (e, ex))
        if ch or e in files:
            files[e] = new_bytes
    gfx = 0
    if patch.get("portGfx") == "es":    # built from the pristine data (the files the patch touches are not read)
        import gfx_es
        gfx = gfx_es.build(data_dir, os.path.join(data_dir, GFX_TBTX), log=log)
    state = {"patch": os.path.abspath(patch_path), "sha1": file_sha1(patch_path), "title": patch.get("title") or "",
             "files": sorted(files), "moved": moved, "gfx": gfx}
    json.dump(state, open(os.path.join(od, STATE), "w", encoding="utf-8"), indent=1)
    for p, data in files.items():
        _write(os.path.join(data_dir, *p.split("/")), data)
    X.write_manifest(data_dir, man)
    return {"files": sorted(files), "applied": applied, "problems": problems, "moved": moved, "title": state["title"],
            "gfx": gfx}


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    a = sub.add_parser("apply"); a.add_argument("data"); a.add_argument("patch"); a.add_argument("--disc"); a.add_argument("--force", action="store_true")
    sub.add_parser("restore").add_argument("data")
    sub.add_parser("status").add_argument("data")
    a_ = ap.parse_args(argv)
    if a_.cmd == "restore":
        print("%d files restored" % restore(a_.data))
    elif a_.cmd == "status":
        st = status(a_.data)
        print(json.dumps(st, indent=1) if st else "no patch applied")
    else:
        r = apply(a_.data, a_.patch, a_.disc, force=a_.force)
        for x in r["problems"]:
            print("warning:", x)
        print("applied %d edits: %s" % (r["applied"], ", ".join(r["files"])))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except PatchError as e:
        sys.exit("error: %s" % e)
