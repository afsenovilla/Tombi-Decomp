#!/usr/bin/env python3
"""Distributable patches of the text editor's edits (no game data inside).

Two formats:

  *.tombipatch.json  "text patch" (primary): the edited dialogues only (file/dialogue index -> new text) plus the sha1 and
                     size of the original disc it was made for.  Tiny, human-readable, diffable and mergeable (two patches
                     touching different dialogues can be combined with --merge).  Applying it rebuilds the disc with the
                     same pipeline as "Generar ISO" (WFM re-encoding + tools/mkdisc.py --relayout), so it needs mkpsxiso.
  *.ppf              PPF 3.0 binary patch of the .bin (secondary, for standard patchers: PPF-O-Matic, ApplyPPF, ...).
                     Made by rebuilding the disc and storing only the bytes that differ from the original image.  PPF
                     cannot express insertions: this works because mkdisc --relayout keeps the raw-sector layout (files
                     that grow are moved to the end of the image and the LBA tables are patched) so the rebuilt image is
                     a byte-for-byte overlay of the original that may only be longer (PPF writes past the end extend it).
                     A PPF only applies to exactly the same original image (sha1 printed in its description).

Command line (the editor tools/text_editor.py has "Generar parche" / "Aplicar parche" buttons doing the same):
  text_patch.py make  DISC.bin PROJECT.json|IMPORT.txt -o FIX.tombipatch.json [--author A] [--title T] [--ppf FIX.ppf]
  text_patch.py apply DISC.bin FIX.tombipatch.json [MORE.json ...] -o PATCHED.bin [--force] [--files-out DIR]
  text_patch.py apply DISC.bin FIX.ppf -o PATCHED.bin [--force]
  text_patch.py info  FIX.tombipatch.json|FIX.ppf
  text_patch.py merge A.json B.json ... -o MERGED.json
--files-out DIR only writes the patched game files (e.g. the extracted data/ folder of the native port) instead of a disc.
"""
import argparse, hashlib, json, os, shutil, struct, sys, tempfile, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
from i18n import tr
import text_core as C

FORMAT = "tombi-text-patch"
VERSION = 1

_S = {
    "es": {"tp.badfmt": "No es un parche de textos de Tombi: {0}",
           "tp.newer": "El parche es de una versión más nueva del formato ({0}).",
           "tp.wrongdisc": "El disco no es el original para el que se hizo el parche.\nEsperado: {0}\nEste disco: {1}",
           "tp.ppfsize": "La imagen reconstruida es más corta que la original: PPF no puede expresarlo.",
           "tp.ppfbad": "Archivo PPF no válido o no soportado: {0}",
           "tp.noedits": "El parche no contiene cambios.",
           "tp.conflict": "Conflicto: {0} #{1} distinto en dos parches (gana el último)."},
    "en": {"tp.badfmt": "Not a Tombi text patch: {0}",
           "tp.newer": "The patch uses a newer format version ({0}).",
           "tp.wrongdisc": "This disc is not the original the patch was made for.\nExpected: {0}\nThis disc: {1}",
           "tp.ppfsize": "The rebuilt image is shorter than the original: PPF cannot express that.",
           "tp.ppfbad": "Invalid or unsupported PPF file: {0}",
           "tp.noedits": "The patch contains no changes.",
           "tp.conflict": "Conflict: {0} #{1} differs between two patches (the last one wins)."},
    "fr": {"tp.badfmt": "Ce n'est pas un patch de textes Tombi : {0}",
           "tp.newer": "Le patch utilise une version de format plus récente ({0}).",
           "tp.wrongdisc": "Ce disque n'est pas l'original du patch.\nAttendu : {0}\nCe disque : {1}",
           "tp.ppfsize": "L'image reconstruite est plus courte que l'originale : PPF ne peut pas l'exprimer.",
           "tp.ppfbad": "Fichier PPF invalide ou non pris en charge : {0}",
           "tp.noedits": "Le patch ne contient aucune modification.",
           "tp.conflict": "Conflit : {0} #{1} diffère entre deux patchs (le dernier l'emporte)."},
    "de": {"tp.badfmt": "Kein Tombi-Textpatch: {0}",
           "tp.newer": "Der Patch nutzt eine neuere Formatversion ({0}).",
           "tp.wrongdisc": "Diese Disc ist nicht das Original des Patches.\nErwartet: {0}\nDiese Disc: {1}",
           "tp.ppfsize": "Das neue Image ist kürzer als das Original: PPF kann das nicht abbilden.",
           "tp.ppfbad": "Ungültige oder nicht unterstützte PPF-Datei: {0}",
           "tp.noedits": "Der Patch enthält keine Änderungen.",
           "tp.conflict": "Konflikt: {0} #{1} ist in zwei Patches verschieden (der letzte gewinnt)."},
    "it": {"tp.badfmt": "Non è una patch di testi di Tombi: {0}",
           "tp.newer": "La patch usa una versione del formato più recente ({0}).",
           "tp.wrongdisc": "Il disco non è l'originale della patch.\nAtteso: {0}\nQuesto disco: {1}",
           "tp.ppfsize": "L'immagine ricostruita è più corta dell'originale: PPF non può esprimerlo.",
           "tp.ppfbad": "File PPF non valido o non supportato: {0}",
           "tp.noedits": "La patch non contiene modifiche.",
           "tp.conflict": "Conflitto: {0} #{1} diverso tra due patch (vince l'ultima)."},
    "pt": {"tp.badfmt": "Não é um patch de textos do Tombi: {0}",
           "tp.newer": "O patch usa uma versão de formato mais recente ({0}).",
           "tp.wrongdisc": "Este disco não é o original do patch.\nEsperado: {0}\nEste disco: {1}",
           "tp.ppfsize": "A imagem reconstruída é menor que a original: PPF não pode expressar isso.",
           "tp.ppfbad": "Arquivo PPF inválido ou não suportado: {0}",
           "tp.noedits": "O patch não contém alterações.",
           "tp.conflict": "Conflito: {0} #{1} difere entre dois patches (vence o último)."},
}
i18n.add(_S)


def sha1_file(path, chunk=1 << 22):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        for b in iter(lambda: f.read(chunk), b""):
            h.update(b)
    return h.hexdigest()


def disc_id(path):
    return {"name": os.path.basename(path), "size": os.path.getsize(path), "sha1": sha1_file(path)}


# ---- text patch ----------------------------------------------------------------------------------------------------

def make(proj, author="", title="", notes=""):
    """Text patch dict of the project's edits (only dialogues whose text differs from the original disc)."""
    edits = {}
    for path, e in sorted(proj.edits.items()):
        for i, t in sorted(e.items()):
            if C.norm(t) != C.norm(proj.original(path, i)):
                edits.setdefault(path, {})[str(i)] = t
    p = {"format": FORMAT, "version": VERSION, "title": title, "author": author, "notes": notes,
         "created": time.strftime("%Y-%m-%d"), "disc": disc_id(proj.src.image) if proj.src.image else None,
         "verbFirst": bool(proj.verb_first), "edits": edits}
    return p


def save(patch, path):
    with open(path, "w", encoding="utf-8") as f:
        json.dump(patch, f, ensure_ascii=False, indent=1)
        f.write("\n")


def load(path):
    try:
        p = json.load(open(path, encoding="utf-8"))
    except (OSError, ValueError) as e:
        raise C.CoreError(tr("tp.badfmt", "%s (%s)" % (path, e)))
    if not isinstance(p, dict) or p.get("format") != FORMAT or not isinstance(p.get("edits"), dict):
        raise C.CoreError(tr("tp.badfmt", path))
    if int(p.get("version", 0)) > VERSION:
        raise C.CoreError(tr("tp.newer", p.get("version")))
    return p


def merge(patches):
    """Combines several text patches (later ones win on conflicts). Returns (patch, warnings)."""
    out, warns = None, []
    for p in patches:
        if out is None:
            out = json.loads(json.dumps(p))
            continue
        out["verbFirst"] = out.get("verbFirst") or p.get("verbFirst")
        for path, d in p["edits"].items():
            o = out["edits"].setdefault(path, {})
            for k, t in d.items():
                if k in o and o[k] != t:
                    warns.append(tr("tp.conflict", path, k))
                o[k] = t
        for k in ("author", "title"):
            if p.get(k) and p[k] not in (out.get(k) or ""):
                out[k] = ((out.get(k) or "") + " + " + p[k]).strip(" +")
    return out, warns


def check_disc(patch, disc, force=False):
    """Raises CoreError if disc is not the patch's original image (unless force). Returns the sha1 of disc."""
    want = (patch.get("disc") or {}).get("sha1")
    have = sha1_file(disc)
    if want and want != have and not force:
        raise C.CoreError(tr("tp.wrongdisc", want, have))
    return have


def apply_to_project(proj, patch):
    """Loads the patch's edits into proj (enabling the game languages it touches). Returns (applied, problems)."""
    langs = {C.lang_of(p) for p in patch["edits"]} & set(proj.languages())
    proj.langs |= langs
    proj.verb_first = proj.verb_first or bool(patch.get("verbFirst"))
    return proj.load_edits({"edits": patch["edits"]}, replace=False)


def apply(disc, patches, out_bin, work=None, force=False, files_out=None, log=print):
    """Applies text patches to the original disc and builds out_bin (+ .cue), or writes the patched files to files_out."""
    if isinstance(patches, dict):
        patches = [patches]
    for p in patches:
        check_disc(p, disc, force)
    patch, warns = merge(patches)
    proj = C.Project(C.Source(disc))
    applied, problems = apply_to_project(proj, patch)
    problems = warns + problems
    if not proj.n_edits() and not proj.verb_first:
        raise C.CoreError(tr("tp.noedits"))
    if files_out:
        return {"files": C.write_patched(proj, files_out), "out": files_out, "applied": applied, "problems": problems}
    work = work or tempfile.mkdtemp(prefix="tombi-patch-")
    try:
        res = C.build_report(proj, out_bin, work, disc, log)
    finally:
        shutil.rmtree(work, ignore_errors=True)
    res.update(applied=applied, problems=problems)
    return res


# ---- PPF 3.0 -------------------------------------------------------------------------------------------------------

def ppf_write(orig, new, out, description="", chunk=1 << 20):
    """PPF3 (BIN image, no block check, no undo data) with the bytes of new that differ from orig. Returns record count."""
    if os.path.getsize(new) < os.path.getsize(orig):
        raise C.CoreError(tr("tp.ppfsize"))
    desc = description.encode("ascii", "replace")[:50].ljust(50, b" ")
    n = 0
    with open(orig, "rb") as fa, open(new, "rb") as fb, open(out, "wb") as fo:
        fo.write(b"PPF30" + bytes([2]) + desc + bytes([0, 0, 0, 0]))
        pos = 0
        while True:
            b = fb.read(chunk)
            if not b:
                break
            a = fa.read(len(b))
            if a != b:
                i, L = 0, len(b)
                while i < L:
                    if i < len(a) and a[i] == b[i]:
                        i += 1
                        continue
                    j = i
                    while j < L and j - i < 255 and not (j < len(a) and a[j] == b[j]):
                        j += 1
                    fo.write(struct.pack("<QB", pos + i, j - i) + b[i:j])
                    n += 1
                    i = j
            pos += len(b)
    return n


def ppf_read(path):
    """(description, [(offset, bytes)]) of a PPF3 file (block check / undo data / file_id.diz handled)."""
    d = open(path, "rb").read()
    if d[:5] != b"PPF30" or len(d) < 60:
        raise C.CoreError(tr("tp.ppfbad", path))
    desc = d[6:56].decode("ascii", "replace").rstrip()
    blockcheck, undo = d[57], d[58]
    p = 60 + (1024 if blockcheck else 0)
    end = len(d)
    if d[-4:-2] == b".D" or d[end - 6:end - 2] == b".DIZ":     # trailing file_id.diz: "@BEGIN_FILE_ID.DIZ" ... "@END_FILE_ID.DIZ" + u16/u32 len
        k = d.rfind(b"@BEGIN_FILE_ID.DIZ")
        if k > 0:
            end = k
    recs = []
    while p + 9 <= end:
        off, ln = struct.unpack_from("<QB", d, p)
        p += 9
        recs.append((off, d[p:p + ln]))
        p += ln * (2 if undo else 1)
    return desc, recs


def ppf_check(patch, disc):
    """Raises CoreError if the PPF's description names an original sha1 that disc does not have."""
    desc, _ = ppf_read(patch)
    if "sha1 " in desc:
        want, have = desc.split("sha1 ")[1].strip(), sha1_file(disc)
        if not have.startswith(want):
            raise C.CoreError(tr("tp.wrongdisc", want, have))


def ppf_apply(src, patch, out):
    """Copies src to out and writes the PPF records onto it."""
    _, recs = ppf_read(patch)
    if os.path.abspath(src) != os.path.abspath(out):
        shutil.copyfile(src, out)
    with open(out, "r+b") as f:
        for off, data in recs:
            f.seek(off)
            f.write(data)
    cue = os.path.splitext(out)[0] + ".cue"
    open(cue, "w", newline="\r\n").write('FILE "%s" BINARY\n  TRACK 01 MODE2/2352\n    INDEX 01 00:00:00\n' % os.path.basename(out))
    return len(recs)


def make_ppf(proj, out_ppf, work=None, log=print):
    """Rebuilds the disc from proj into a temporary image and writes the PPF3 diff against the opened original."""
    orig = proj.src.image
    if not orig:
        raise C.CoreError(tr("tc.needimage"))
    tmp = tempfile.mkdtemp(prefix="tombi-ppf-")
    try:
        new = os.path.join(tmp, "patched.bin")
        C.build_report(proj, new, work or os.path.join(tmp, "work"), orig, log)
        return ppf_write(orig, new, out_ppf, "Tombi! text patch sha1 " + sha1_file(orig)[:24])
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def info(path):
    if path.lower().endswith(".ppf"):
        desc, recs = ppf_read(path)
        return "PPF3: %s\n%d records, %d bytes" % (desc, len(recs), sum(len(r[1]) for r in recs))
    p = load(path)
    n = sum(len(d) for d in p["edits"].values())
    return "%s by %s (%s)\ndisc: %s\n%d dialogues in %d files%s" % (
        p.get("title") or "-", p.get("author") or "-", p.get("created", "?"), p.get("disc"), n, len(p["edits"]),
        ", verb first" if p.get("verbFirst") else "")


# ---- command line --------------------------------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    m = sub.add_parser("make")
    m.add_argument("disc"); m.add_argument("project"); m.add_argument("-o", "--out", required=True)
    m.add_argument("--author", default=""); m.add_argument("--title", default=""); m.add_argument("--ppf")
    a = sub.add_parser("apply")
    a.add_argument("disc"); a.add_argument("patches", nargs="+"); a.add_argument("-o", "--out")
    a.add_argument("--force", action="store_true", help="apply even if the disc sha1 differs")
    a.add_argument("--files-out", help="write the patched game files to this folder instead of building a disc")
    a.add_argument("--work")
    i = sub.add_parser("info"); i.add_argument("patch")
    g = sub.add_parser("merge"); g.add_argument("patches", nargs="+"); g.add_argument("-o", "--out", required=True)
    a_ = ap.parse_args()
    if a_.cmd == "info":
        print(info(a_.patch))
    elif a_.cmd == "merge":
        p, w = merge([load(x) for x in a_.patches])
        save(p, a_.out)
        for x in w:
            print(x)
    elif a_.cmd == "make":
        proj = C.Project(C.Source(a_.disc))
        if a_.project.lower().endswith(".txt"):
            ap_, pr = proj.import_txt(open(a_.project, encoding="utf-8").read())
        else:
            data = json.load(open(a_.project, encoding="utf-8"))
            if data.get("format") == FORMAT:
                ap_, pr = apply_to_project(proj, data)
            else:
                ap_, pr = proj.load_edits(data)
        print("loaded %d edits; problems: %s" % (ap_, pr))
        p = make(proj, a_.author, a_.title)
        save(p, a_.out)
        print("text patch:", a_.out, "(%d dialogues)" % sum(len(d) for d in p["edits"].values()))
        if a_.ppf:
            print("ppf: %d records -> %s" % (make_ppf(proj, a_.ppf), a_.ppf))
    else:
        if not a_.out and not a_.files_out:
            ap.error("apply needs -o OUT.bin or --files-out DIR")
        if len(a_.patches) == 1 and a_.patches[0].lower().endswith(".ppf"):
            if not a_.force:
                ppf_check(a_.patches[0], a_.disc)
            print("ppf: %d records -> %s" % (ppf_apply(a_.disc, a_.patches[0], a_.out), a_.out))
            return 0
        res = apply(a_.disc, [load(x) for x in a_.patches], a_.out, a_.work, a_.force, a_.files_out)
        for x in res["problems"]:
            print("warning:", x)
        print("applied %d edits -> %s (%s)" % (res["applied"], res["out"], ", ".join(res["files"])))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except C.CoreError as e:
        sys.exit("error: %s" % e)
