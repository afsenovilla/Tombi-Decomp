#!/usr/bin/env python3
"""Logic of the Tomba! text editor (no GUI): open a disc image or extracted fs, edit dialogues, build the disc.

  Source      a raw/cooked .bin image or a folder with the disc files; lists and reads the *.WFM files
  Project     decoded files + the edits (path -> {dialogue index: text}); JSON project, TXT/JSON export and import,
              validation, search, encoding of the edited WFM files and the disc build (tools/mkdisc.py --relayout)

Command line (used by the tests; the GUI is tools/text_editor.py):
  text_core.py render SRC PATH IDX [-o OUT.png] [--scale N]     draw one dialogue (PATH like AREA05/MFNT050S.WFM)
  text_core.py export SRC OUT.json|OUT.txt [--all-languages]    every dialogue as text
  text_core.py build  SRC PROJECT.json|IMPORT.json|IMPORT.txt -o OUT.bin [--work DIR] [--disc IMAGE]
"""
import argparse, contextlib, io, json, os, re, shutil, struct, sys, unicodedata

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import wfm_py as W

LANG_MAIN = "S"          # the PAL Spanish disc uses the *S.WFM files; the other languages are still on the disc
PROJECT_VERSION = 1


class CoreError(Exception):
    pass


def is_main_lang(path):
    return os.path.splitext(os.path.basename(path))[0].endswith(LANG_MAIN)


def norm(text):
    """Cosmetic line feeds do not count."""
    return text.replace("\r", "").replace("\n", "")


# ---- sources -----------------------------------------------------------------------------------------------------

class Source:
    def __init__(self, path):
        path = os.path.abspath(path)
        self.path = path
        self.image = None
        self.root = None
        if os.path.isdir(path):
            self.kind = "folder"
            self.root = path if os.path.isdir(os.path.join(path, "AREA00")) or not os.path.isdir(os.path.join(path, "fs")) \
                else os.path.join(path, "fs")
            self.files = {}
            for d, _, fs in os.walk(self.root):
                for f in fs:
                    if f.upper().endswith(".WFM"):
                        full = os.path.join(d, f)
                        self.files[os.path.relpath(full, self.root).replace(os.sep, "/")] = full
            sib = os.path.join(os.path.dirname(self.root), "Tombi.bin")
            if os.path.isfile(sib):
                self.image = sib
        elif os.path.isfile(path):
            import cdfiles
            self.kind = "image"
            self.image = path
            with open(path, "rb") as f:
                f.seek(16 * 2352 + 25)
                raw = f.read(5) == b"CD001"
                f.seek(16 * 2048 + 1)
                cooked = f.read(5) == b"CD001"
            if not (raw or cooked):
                raise CoreError("%s no es una imagen de disco ISO9660 (.bin de 2352 bytes por sector o .iso)" % path)
            self.sector, self.off = (2352, 24) if raw else (2048, 0)
            lay = cdfiles.layout_from_image(path)
            self.layout = lay
            self.files = {k: v for k, v in lay.items() if k.upper().endswith(".WFM")}
        else:
            raise CoreError("no existe %s" % path)
        if not self.files:
            raise CoreError("no se encontró ningún archivo .WFM en %s" % path)

    def names(self):
        return sorted(self.files)

    def read(self, name):
        if self.kind == "folder":
            return open(self.files[name], "rb").read()
        lba, size = self.files[name]
        out = bytearray()
        with open(self.image, "rb") as f:
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * self.sector + self.off)
                out += f.read(2048)
        return bytes(out[:size])

    def read_any(self, name):
        """Any file of the disc (used to take the executables)."""
        if self.kind == "folder":
            return open(os.path.join(self.root, name), "rb").read()
        lba, size = self.layout[name]
        out = bytearray()
        with open(self.image, "rb") as f:
            for i in range((size + 2047) // 2048):
                f.seek((lba + i) * self.sector + self.off)
                out += f.read(2048)
        return bytes(out[:size])


# ---- project -----------------------------------------------------------------------------------------------------

def plain(text, n=60):
    """One-line summary of a dialogue text for lists."""
    t = re.sub(r"\{[^}]*\}", " ", text.replace("\n", ""))
    t = re.sub(r"\s+", " ", t).strip()
    return t if len(t) <= n else t[:n - 1] + "…"


def fold(s):
    return "".join(c for c in unicodedata.normalize("NFD", s.casefold()) if unicodedata.category(c) != "Mn")


class Project:
    def __init__(self, source):
        self.src = source
        self.edits = {}            # path -> {idx: text}
        self._wfm = {}
        self._orig = {}
        self._pool = None

    # decoded files
    def wfm(self, path):
        if path not in self._wfm:
            try:
                self._wfm[path] = W.decode(self.src.read(path))
            except Exception as e:
                raise CoreError("%s: %s" % (path, e))
        return self._wfm[path]

    def count(self, path):
        return len(self.wfm(path).dialogues)

    def original(self, path, idx):
        key = (path, idx)
        if key not in self._orig:
            w = self.wfm(path)
            d = w.dialogues[idx]
            self._orig[key] = W.words_to_text(w, d.words, d.font)
        return self._orig[key]

    def text(self, path, idx):
        return self.edits.get(path, {}).get(idx, self.original(path, idx))

    def edited(self, path, idx):
        return idx in self.edits.get(path, {})

    def set_text(self, path, idx, text):
        if norm(text) == norm(self.original(path, idx)):
            e = self.edits.get(path)
            if e and idx in e:
                del e[idx]
                if not e:
                    del self.edits[path]
        else:
            self.edits.setdefault(path, {})[idx] = text

    def n_edits(self):
        return sum(len(v) for v in self.edits.values())

    # glyph pool (characters a file lacks are borrowed from other files)
    def pool(self):
        if self._pool is None:
            self._pool = W.GlyphPool()
            for n in self.src.names():
                if is_main_lang(n):
                    self._pool.add(self.wfm(n))
        return self._pool

    def words(self, path, idx, text):
        """Encodes text for dialogue idx -> (words, clone of the wfm that has the glyphs it needs). Raises WfmError;
        characters the file lacks are borrowed from other files (UnknownGlyph only if no file has them)."""
        base = self.wfm(path)
        font = base.dialogues[idx].font
        wc = clone(base)
        try:
            return W.text_to_words(wc, text, font), wc
        except W.UnknownGlyph:
            wc = clone(base)
            return W.text_to_words(wc, text, font, self.pool()), wc

    def validate(self, path, idx, text):
        try:
            self.words(path, idx, text)
            return None
        except W.WfmError as e:
            return str(e)

    # search
    def search(self, query, all_languages=False, edited_only=False):
        q = fold(query)
        res = []
        for n in self.src.names():
            if not all_languages and not is_main_lang(n):
                continue
            for i in range(self.count(n)):
                if edited_only and not self.edited(n, i):
                    continue
                if not q or q in fold(self.text(n, i)):
                    res.append((n, i))
        return res

    # project file
    def to_json(self):
        return {"version": PROJECT_VERSION, "disc": os.path.basename(self.src.path),
                "edits": {p: {str(i): t for i, t in sorted(e.items())} for p, e in sorted(self.edits.items())}}

    def load_edits(self, data, replace=True):
        """data: {"edits": {path: {idx: text}}} (project) or {path: {idx: text}} (export). Returns (applied, problems)."""
        ed = data.get("edits") if isinstance(data.get("edits"), dict) else data
        if replace:
            self.edits = {}
        applied, problems = 0, []
        for path, d in ed.items():
            if path not in self.src.files:
                problems.append("%s: archivo no encontrado en este disco" % path)
                continue
            if not isinstance(d, dict):
                continue
            for k, t in d.items():
                try:
                    i = int(k)
                    if not 0 <= i < self.count(path):
                        raise ValueError
                except ValueError:
                    problems.append("%s #%s: diálogo inexistente" % (path, k))
                    continue
                before = self.text(path, i)
                self.set_text(path, i, t)
                applied += norm(before) != norm(t)
        return applied, problems

    def export_json(self, all_languages=False):
        out = {}
        for n in self.src.names():
            if all_languages or is_main_lang(n):
                out[n] = {str(i): self.text(n, i) for i in range(self.count(n))}
        return out

    def export_txt(self, all_languages=False):
        lines = ["# Tombi! textos. Cada diálogo empieza con '=== ARCHIVO #n ==='; los saltos de línea reales se ignoran,",
                 "# usa {NL} {CLEAR} {WAIT} ... (ver ayuda del editor).", ""]
        for n in self.src.names():
            if all_languages or is_main_lang(n):
                for i in range(self.count(n)):
                    lines.append("=== %s #%d ===" % (n, i))
                    lines.append(self.text(n, i).rstrip("\n"))
                    lines.append("")
        return "\n".join(lines)

    def import_txt(self, txt):
        data, cur = {}, None
        for ln in txt.splitlines():
            m = re.fullmatch(r"=== (\S+) #(\d+) ===", ln)
            if m:
                cur = (m.group(1), m.group(2))
                data.setdefault(cur[0], {})[cur[1]] = []
            elif cur is not None:
                data[cur[0]][cur[1]].append(ln)
        out = {p: {i: "\n".join(l).strip("\n") for i, l in d.items()} for p, d in data.items()}
        return self.load_edits(out, replace=False)

    # encoding
    def encode_edited(self):
        """{path: bytes} for every edited file, and a list of readable errors (nothing is returned if there are any)."""
        files, errors = {}, []
        for path, e in sorted(self.edits.items()):
            w = clone(self.wfm(path))
            for idx, text in sorted(e.items()):
                try:
                    d = w.dialogues[idx]
                    try:
                        words = W.text_to_words(w, text, d.font)
                    except W.UnknownGlyph:
                        words = W.text_to_words(w, text, d.font, self.pool())
                    d.words = words
                except W.WfmError as ex:
                    errors.append("%s #%d: %s" % (path, idx, ex))
            if not errors:
                try:
                    files[path] = W.encode(w)
                except W.WfmError as ex:
                    errors.append("%s: %s" % (path, ex))
        return ({} if errors else files), errors


def clone(w):
    c = W.Wfm()
    c.__dict__.update(w.__dict__)
    c.glyphs = list(w.glyphs)
    c.charidx = {k: list(v) for k, v in w.charidx.items()}
    c.dialogues = [W.Dialogue(d.words, d.term, d.font) for d in w.dialogues]
    return c


# ---- disc build ----------------------------------------------------------------------------------------------------

def have_mkpsxiso():
    import mkdisc
    try:
        mkdisc.tool("mkpsxiso")
        mkdisc.tool("dumpsxiso")
        return True
    except SystemExit:
        return False


def build_disc(proj, out_bin, work, disc=None, log=print):
    """Writes the edited WFM files and builds out_bin (+ .cue) with tools/mkdisc.py --relayout. Returns the list of files."""
    import mkdisc
    files, errors = proj.encode_edited()
    if errors:
        raise CoreError("No se puede generar el disco:\n" + "\n".join(errors[:20]))
    if not files:
        raise CoreError("No hay cambios que aplicar.")
    disc = disc or proj.src.image
    if not disc or not os.path.isfile(disc):
        raise CoreError("Hace falta la imagen original (.bin) para generar la ISO: ábrela en lugar de la carpeta.")
    work = os.path.abspath(work)
    fs = os.path.join(work, "fs")
    shutil.rmtree(fs, ignore_errors=True)
    for name, data in files.items():
        p = os.path.join(fs, *name.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "wb").write(data)
    src = Source(disc)
    for name in sorted(src.layout):
        if re.fullmatch(r"EXE/MAIN\d\.EXE", name):
            p = os.path.join(fs, *name.split("/"))
            os.makedirs(os.path.dirname(p), exist_ok=True)
            open(p, "wb").write(src.read_any(name))
    ns = argparse.Namespace(fs=fs, disc=disc, work=os.path.join(work, "mkdisc"), out=os.path.abspath(out_bin), relayout=True)
    buf = io.StringIO()
    try:
        with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(buf):
            mkdisc.build(ns)
    except SystemExit as e:
        raise CoreError("mkpsxiso falló: %s\n%s" % (e, buf.getvalue()[-1500:]))
    log(next((l for l in buf.getvalue().splitlines() if l.startswith("built")), ""))
    return sorted(files)


def write_patched(proj, out_dir):
    files, errors = proj.encode_edited()
    if errors:
        raise CoreError("\n".join(errors[:20]))
    for name, data in files.items():
        p = os.path.join(out_dir, *name.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        open(p, "wb").write(data)
    return sorted(files)


# ---- command line --------------------------------------------------------------------------------------------------

def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("render")
    r.add_argument("src"); r.add_argument("path"); r.add_argument("idx", type=int)
    r.add_argument("-o", "--out", default="preview.png"); r.add_argument("--scale", type=int, default=2)
    r.add_argument("--text", help="render this text instead of the dialogue's own")
    e = sub.add_parser("export")
    e.add_argument("src"); e.add_argument("out"); e.add_argument("--all-languages", action="store_true")
    b = sub.add_parser("build")
    b.add_argument("src"); b.add_argument("project"); b.add_argument("-o", "--out", required=True)
    b.add_argument("--work"); b.add_argument("--disc")
    a = p.parse_args()
    proj = Project(Source(a.src))
    if a.cmd == "render":
        import text_preview
        text = a.text if a.text is not None else proj.text(a.path, a.idx)
        words, wc = proj.words(a.path, a.idx, text)
        im, warns = text_preview.render(wc, words, scale=a.scale)
        im.save(a.out)
        for w in warns:
            print(w)
        print(proj.original(a.path, a.idx))
    elif a.cmd == "export":
        if a.out.lower().endswith(".json"):
            json.dump(proj.export_json(a.all_languages), open(a.out, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        else:
            open(a.out, "w", encoding="utf-8").write(proj.export_txt(a.all_languages))
    else:
        if a.project.lower().endswith(".txt"):
            ap, pr = proj.import_txt(open(a.project, encoding="utf-8").read())
        else:
            ap, pr = proj.load_edits(json.load(open(a.project, encoding="utf-8")))
        print("applied %d edits; problems: %s" % (ap, pr))
        work = a.work or os.path.join(os.path.dirname(os.path.abspath(a.out)), "tombi-text-work")
        print("changed:", build_disc(proj, a.out, work, a.disc))
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (CoreError, W.WfmError) as e:
        sys.exit("error: %s" % e)
