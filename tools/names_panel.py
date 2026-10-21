"""Panel "Sin nombre / Unnamed" of the graphics editor (tools/gfx_editor.py): the functions of notes/unnamed.json (tools/unnamed.py) that
still carry a generic name.  For the selected one it shows the `// probably:` note, the callers, the object table slot, the area /
handler whose animation scripts reach it and, when the SPR table resolved, a strip of its sprite frames (rendered like the
"Animaciones" node; "Animar" opens the player window with the linked scripts).  The user types a name and a comment per entry; the
choices are autosaved to names_user.json in the TombiTools config folder and "Exportar nombres" writes that list to a file the
decomp side applies with tools/apply_names.py.  No emulator runs: everything comes from the committed JSON and the disc files."""
import os, re, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tkinter as tk
from tkinter import filedialog, messagebox, ttk
from PIL import Image, ImageTk
import i18n
from i18n import tr
import unnamed

i18n.add({
    "es": {
        "un.title": "Funciones sin nombre",
        "un.hdr": "{0}  ·  {1}  ·  {2}  ·  {3} bytes", "un.note": "Nota (probablemente):", "un.nonote": "(sin nota)",
        "un.callers": "Llamada desde:", "un.nocallers": "nadie (sin referencias en el código)", "un.twins": "Copias en las áreas: {0}",
        "un.object": "Objeto: tabla {0}, tipo 0x{1:02X} (entrada en {2})", "un.refs": "Punteros a la función: {0}",
        "un.area": "Área {0}: {1} ({2}, {3} colocaciones){4}", "un.areaself": "Área 0: la función elige sus propios guiones de animación ({0})",
        "un.reached": " · alcanzada desde el manejador", "un.spr": "Tabla SPR: lista {0}, tipo {1}, {2} fotogramas · animaciones {3}",
        "un.nospr": "Sin tabla SPR resuelta (no se puede dibujar)", "un.frames": "Fotogramas del manejador (primeros {0} de {1}):",
        "un.name": "Nombre:", "un.comment": "Comentario:", "un.export": "Exportar nombres", "un.import": "Importar", "un.anim": "Animar",
        "un.bad": "«{0}» no es un identificador C válido (letras, cifras y _ sin empezar por cifra).",
        "un.generic": "«{0}» sigue siendo un nombre genérico.", "un.exported": "{0} nombre(s) exportados a {1}",
        "un.imported": "{0} nombre(s) importados de {1}", "un.saved": "{0} con nombre · {1} con comentario",
        "un.help": "Escribe un nombre (identificador C, p. ej. updateFooBar) y, si quieres, por qué. Exportar nombres escribe names_user.json: "
                   "envíalo al proyecto; se aplica con tools/apply_names.py.",
        "un.nojson": "No se encontró notes/unnamed.json (genera la lista con python tools/unnamed.py).",
    },
    "en": {
        "un.title": "Unnamed functions",
        "un.hdr": "{0}  ·  {1}  ·  {2}  ·  {3} bytes", "un.note": "Note (probably):", "un.nonote": "(no note)",
        "un.callers": "Called from:", "un.nocallers": "nobody (unreferenced in the code)", "un.twins": "Copies in the areas: {0}",
        "un.object": "Object: table {0}, type 0x{1:02X} (entry at {2})", "un.refs": "Pointers to the function: {0}",
        "un.area": "Area {0}: {1} ({2}, {3} placements){4}", "un.areaself": "Area 0: the function selects its own animation scripts ({0})",
        "un.reached": " · reached from the handler", "un.spr": "SPR table: list {0}, kind {1}, {2} frames · animations {3}",
        "un.nospr": "No SPR table resolved (nothing to draw)", "un.frames": "Frames of the handler (first {0} of {1}):",
        "un.name": "Name:", "un.comment": "Comment:", "un.export": "Export names", "un.import": "Import", "un.anim": "Animate",
        "un.bad": "“{0}” is not a valid C identifier (letters, digits and _, not starting with a digit).",
        "un.generic": "“{0}” is still a generic name.", "un.exported": "{0} name(s) exported to {1}",
        "un.imported": "{0} name(s) imported from {1}", "un.saved": "{0} named · {1} with a comment",
        "un.help": "Type a name (a C identifier, e.g. updateFooBar) and, if you like, why. Export names writes names_user.json: send it to the "
                   "project; it is applied with tools/apply_names.py.",
        "un.nojson": "notes/unnamed.json not found (make the list with python tools/unnamed.py).",
    },
})

IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]{0,63}$")
GENERIC = re.compile(r"^(?:FUN|func|D|DAT)_[0-9a-fA-F]{6,8}$")
MAX_FRAMES = 24


class Store:
    """The user's names and comments, keyed "PROG:address"; autosaved to unnamed.user_path()."""

    def __init__(self):
        self.names = unnamed.read_user(unnamed.user_path())

    def get(self, key):
        return self.names.get(key, {"new": "", "comment": ""})

    def set(self, key, new, comment, entries):
        new, comment = new.strip(), comment.strip()
        if new or comment:
            self.names[key] = dict(new=new, comment=comment)
        else:
            self.names.pop(key, None)
        try:
            unnamed.write_user(unnamed.user_path(), self.names, entries)
        except OSError:
            pass

    def counts(self):
        return sum(1 for v in self.names.values() if v.get("new")), sum(1 for v in self.names.values() if v.get("comment"))


def node_text(e, store):
    """Tree text of an entry: name, size, a frames marker and the user's name."""
    mark = "  ▣" if e.get("anim") and e["anim"].get("spr") else ""
    nm = store.get(unnamed.key_of(e)).get("new")
    note = e.get("note") or ""
    if len(note) > 70:
        note = note[:69].rstrip() + "…"
    return "%s  %d B%s%s%s" % (e["name"], e["size"], mark, ("  → " + nm) if nm else "", ("  — " + note) if note else "")


class NamesPanel(ttk.Frame):
    """show(key) fills the panel for one entry.  provider(owner) -> (owner dict, frame source) as gfx_core.Core.anim_owner;
    play(owner, anim id) opens the animation player; renamed(key) tells the editor to refresh the tree text."""

    def __init__(self, master, entries, store, provider, play, renamed, say):
        super().__init__(master)
        self.entries = {unnamed.key_of(e): e for e in entries}
        self.store, self.provider, self.play, self.renamed, self.say = store, provider, play, renamed, say
        self.key = None
        self._photo = None
        self._owner = None
        top = ttk.Frame(self, padding=(6, 4))
        top.pack(fill="x")
        self.hdr = tk.StringVar()
        ttk.Label(top, textvariable=self.hdr, font=("TkDefaultFont", 10, "bold")).pack(anchor="w")
        self.note = tk.StringVar()
        ttk.Label(top, textvariable=self.note, wraplength=720, justify="left", font=("TkDefaultFont", 10), foreground="#0a3a7a").pack(anchor="w", pady=(2, 0))
        self.info = tk.Text(self, height=7, wrap="word", state="disabled", relief="flat", background=self.winfo_toplevel().cget("background"))
        self.info.pack(fill="x", padx=6)
        self.info.tag_configure("b", font=("TkDefaultFont", 9, "bold"))
        self.info.tag_configure("g", foreground="#555")
        self.frames_lbl = tk.StringVar()
        ttk.Label(self, textvariable=self.frames_lbl).pack(anchor="w", padx=6, pady=(4, 0))
        cf = ttk.Frame(self)
        cf.pack(fill="x", padx=6)
        self.canvas = tk.Canvas(cf, bg="#282838", height=120, highlightthickness=0)
        hs = ttk.Scrollbar(cf, orient="horizontal", command=self.canvas.xview)
        self.canvas.configure(xscrollcommand=hs.set)
        self.canvas.pack(fill="x")
        hs.pack(fill="x")
        row = ttk.Frame(self, padding=(6, 6))
        row.pack(fill="x")
        ttk.Label(row, text=tr("un.name")).grid(row=0, column=0, sticky="w")
        self.name_var = tk.StringVar()
        self.name_ent = ttk.Entry(row, textvariable=self.name_var, width=44)
        self.name_ent.grid(row=0, column=1, sticky="w", padx=4)
        self.anim_btn = ttk.Button(row, text=tr("un.anim"), command=self.animate)
        self.anim_btn.grid(row=0, column=2, padx=8)
        ttk.Label(row, text=tr("un.comment")).grid(row=1, column=0, sticky="nw", pady=(4, 0))
        self.comment = tk.Text(row, width=60, height=4, wrap="word")
        self.comment.grid(row=1, column=1, columnspan=2, sticky="we", padx=4, pady=(4, 0))
        row.columnconfigure(1, weight=1)
        self.name_var.trace_add("write", lambda *_: self.save())
        self.comment.bind("<KeyRelease>", lambda e: self.save())
        self.name_ent.bind("<FocusOut>", lambda e: self.check_name())
        bar = ttk.Frame(self, padding=(6, 0, 6, 6))
        bar.pack(fill="x")
        ttk.Button(bar, text=tr("un.export"), command=self.export).pack(side="left")
        ttk.Button(bar, text=tr("un.import"), command=self.import_).pack(side="left", padx=4)
        self.counts = tk.StringVar()
        ttk.Label(bar, textvariable=self.counts, foreground="#555").pack(side="left", padx=12)
        ttk.Label(self, text=tr("un.help"), wraplength=640, justify="left", foreground="#555").pack(anchor="w", padx=6, pady=(0, 6))
        self.refresh_counts()

    # ---- display
    def _put(self, text, tag=None):
        self.info.insert("end", text, tag) if tag else self.info.insert("end", text)

    def show(self, key):
        e = self.entries.get(key)
        if not e:
            return
        self.key = None            # no autosave while the fields are filled
        self.hdr.set(tr("un.hdr", e["name"], e["prog"], e["file"], e["size"]))
        self.info.configure(state="normal")
        self.info.delete("1.0", "end")
        self.note.set(tr("un.note") + " " + (e.get("note") or tr("un.nonote")))
        self._put(tr("un.callers") + " ", "b")
        self._put((", ".join(e.get("callers") or []) or tr("un.nocallers")) + "\n")
        if e.get("twins"):
            self._put(tr("un.twins", ", ".join(e["twins"])) + "\n", "g")
        ob = e.get("object")
        if ob:
            self._put(tr("un.object", ob["table"], ob["type"], ob["slot"]) + "\n")
        elif e.get("refs"):
            self._put(tr("un.refs", ", ".join("%s:%s" % (r["prog"], r["at"]) for r in e["refs"][:8])) + "\n", "g")
        an = e.get("anim")
        if an:
            if an.get("owner"):
                self._put(tr("un.area", an["area"], an["handler"], ",".join(an.get("types") or []), an.get("placements", 0),
                             "" if an.get("direct") else tr("un.reached")) + "\n")
            else:
                self._put(tr("un.areaself", len(an.get("scripts") or [])) + "\n")
            sp = an.get("spr")
            if sp:
                self._put(tr("un.spr", sp["list"], sp["kind"], sp["frames"], ", ".join("%02d" % i for i in an.get("anims") or [])) + "\n")
            else:
                self._put(tr("un.nospr") + "\n", "g")
        self.info.configure(state="disabled")
        cur = self.store.get(key)
        self.name_var.set(cur.get("new", ""))
        self.comment.delete("1.0", "end")
        self.comment.insert("1.0", cur.get("comment", ""))
        self.key = key
        self.draw_frames(e)

    def owner_of(self, e):
        """(owner dict, frame source) of the entry's animation owner, or None."""
        an = e.get("anim")
        if not an or not an.get("spr"):
            return None
        oid = an.get("owner") or ("unnamed:" + e["address"])
        try:
            return self.provider(oid)
        except Exception as ex:       # the disc of the editor may lack the area files
            self.say(str(ex))
            return None

    def draw_frames(self, e):
        import anim
        self.canvas.delete("all")
        self._photo = None
        self._owner = None
        self.anim_btn.state(["disabled"])
        self.frames_lbl.set("")
        ow = self.owner_of(e)
        if not ow:
            return
        owd, spr = ow
        if spr is None:
            return
        self._owner = owd
        ids = e["anim"].get("anims") or []
        frames, seen = [], set()
        for a in owd["animations"]:
            if ids and a["id"] not in ids:
                continue
            for f in a["frames"]:
                if f not in seen and spr.frame(f) is not None:
                    seen.add(f)
                    frames.append(f)
        total = len(frames)
        frames = frames[:MAX_FRAMES]
        if not frames:
            return
        self.anim_btn.state(["!disabled"])
        self.frames_lbl.set(tr("un.frames", len(frames), total))
        box = anim.bounds(spr, frames)
        z = 2
        tiles = [anim.compose(spr, f, box, z, (40, 40, 56, 255)) for f in frames]
        w, h = tiles[0].size
        strip = Image.new("RGBA", ((w + 4) * len(tiles), h + 4), (40, 40, 56, 255))
        for i, t in enumerate(tiles):
            strip.alpha_composite(t, (i * (w + 4) + 2, 2))
        self._photo = ImageTk.PhotoImage(strip)
        self.canvas.configure(height=min(max(h + 4, 48), 260), scrollregion=(0, 0, strip.width, strip.height))
        self.canvas.create_image(0, 0, anchor="nw", image=self._photo)
        for i, f in enumerate(frames):
            self.canvas.create_text(i * (w + 4) + 4, 2, anchor="nw", text=str(f), fill="#ddd", font=("TkFixedFont", 8))

    def animate(self):
        e = self.entries.get(self.key)
        if not e or not self._owner:
            return
        ids = e["anim"].get("anims") or [a["id"] for a in self._owner["animations"]]
        if ids:
            self.play(self._owner["owner"], ids[0])

    # ---- names
    def check_name(self):
        nm = self.name_var.get().strip()
        if nm and not IDENT.match(nm):
            self.say(tr("un.bad", nm))
        elif nm and GENERIC.match(nm):
            self.say(tr("un.generic", nm))

    def save(self):
        if not self.key:
            return
        self.store.set(self.key, self.name_var.get(), self.comment.get("1.0", "end"), list(self.entries.values()))
        self.renamed(self.key)
        self.refresh_counts()

    def refresh_counts(self):
        self.counts.set(tr("un.saved", *self.store.counts()))

    def export(self):
        p = filedialog.asksaveasfilename(title=tr("un.export"), defaultextension=".json", initialfile="names_user.json", filetypes=[("JSON", "*.json")])
        if not p:
            return
        bad = [k for k, v in self.store.names.items() if v.get("new") and not IDENT.match(v["new"])]
        if bad:
            messagebox.showwarning(tr("un.export"), "\n".join(tr("un.bad", self.store.names[k]["new"]) for k in bad[:10]))
        n = unnamed.write_user(p, self.store.names, list(self.entries.values()))
        self.say(tr("un.exported", n, p))

    def import_(self):
        p = filedialog.askopenfilename(title=tr("un.import"), filetypes=[("JSON", "*.json")])
        if not p:
            return
        got = unnamed.read_user(p)
        for k, v in got.items():
            if v.get("new") or v.get("comment"):
                self.store.names[k] = v
        try:
            unnamed.write_user(unnamed.user_path(), self.store.names, list(self.entries.values()))
        except OSError:
            pass
        self.say(tr("un.imported", len(got), p))
        self.refresh_counts()
        for k in got:
            self.renamed(k)
        if self.key:
            self.show(self.key)
