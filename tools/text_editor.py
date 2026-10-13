#!/usr/bin/env python3
"""Tombi! editor de textos (Windows exe: tombi-text.exe). Abre el disco PAL, edita los diálogos con vista previa en vivo,
guarda el proyecto y genera una ISO nueva. La lógica está en text_core.py / wfm_py.py / text_preview.py; aquí solo la ventana.

  python tools/text_editor.py [disco.bin | carpeta_fs]
"""
import base64, io, json, os, re, sys, threading, traceback

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import text_core as C
import wfm_py as W

try:
    import tkinter as tk
    from tkinter import filedialog, messagebox, ttk
except ImportError:      # pragma: no cover
    sys.exit("Falta tkinter (en Linux: apt install python3-tk).")

AYUDA = """Etiquetas del texto (los saltos de línea reales se ignoran, solo cuentan las etiquetas):

{NL}  salto de línea            {CLEAR}  nueva página (limpia la caja)
{WAIT}  espera un botón         {PAUSE n}  pausa de n fotogramas
{BOX w h}  tamaño de la caja en píxeles (ancho = línea más larga + 16, alto = 16 + 13 por línea)
{COLOR n}  color   {POSE n}  pose de Tombi   {MOVE dx dy}  mueve la caja   {VOICE tono cola}
{HALT} {CHOICE} {F4}  códigos de control   {C052}...  palabra desconocida (se conserva tal cual)
{G:n}  glifo n del archivo sin carácter asociado (se conserva tal cual)

Solo se pueden escribir los caracteres que existen en las fuentes del juego (letras, cifras, á é í ó ú ñ ¿ ¡ y poco más).
Si falta uno, se toma prestado de otro archivo del disco; si no existe en ninguno, el editor lo avisa y no genera el disco.
Rojo en la vista previa: una línea no cabe en la caja, sobran líneas o hay más de 128 caracteres en una página."""


class App:
    def __init__(self, root, path=None):
        self.root = root
        root.title("Tombi! – editor de textos")
        root.geometry("1280x800")
        self.proj = None
        self.cur = None            # (path, idx)
        self.dirty = False
        self.busy = False
        self._after = None
        self._img = None
        self._loading = False
        self.scale = tk.IntVar(value=2)
        self.all_lang = tk.BooleanVar(value=False)
        self.only_edited = tk.BooleanVar(value=False)
        self.query = tk.StringVar()
        self._build()
        root.protocol("WM_DELETE_WINDOW", self.on_close)
        if path:
            self.open_path(path)

    # ---- ventana
    def _build(self):
        bar = ttk.Frame(self.root, padding=4)
        bar.pack(fill="x")
        self.buttons = []
        for item in (("Abrir disco…", self.ask_open_image), ("Abrir carpeta…", self.ask_open_folder), None,
                          ("Guardar proyecto", self.save_project), ("Cargar proyecto", self.load_project), None,
                          ("Exportar todo…", self.export_all), ("Importar…", self.import_any), None,
                          ("Generar ISO", self.generate), ("Ayuda", lambda: messagebox.showinfo("Ayuda", AYUDA))):
            if item is None:
                ttk.Separator(bar, orient="vertical").pack(side="left", fill="y", padx=6)
                continue
            text, cmd = item
            b = ttk.Button(bar, text=text, command=cmd)
            b.pack(side="left", padx=2)
            self.buttons.append(b)
        self.status = tk.StringVar(value="Abre la imagen del disco (Tombi.bin) o la carpeta con los archivos extraídos.")
        ttk.Label(self.root, textvariable=self.status, anchor="w", relief="sunken", padding=3).pack(side="bottom", fill="x")

        pw = ttk.PanedWindow(self.root, orient="horizontal")
        pw.pack(fill="both", expand=True)
        left = ttk.Frame(pw)
        pw.add(left, weight=1)
        sf = ttk.Frame(left, padding=(4, 4, 4, 0))
        sf.pack(fill="x")
        ttk.Label(sf, text="Buscar:").pack(side="left")
        e = ttk.Entry(sf, textvariable=self.query)
        e.pack(side="left", fill="x", expand=True, padx=4)
        self.query.trace_add("write", lambda *_: self.schedule_tree())
        of = ttk.Frame(left, padding=(4, 0))
        of.pack(fill="x")
        ttk.Checkbutton(of, text="Solo modificados", variable=self.only_edited, command=self.fill_tree).pack(side="left")
        ttk.Checkbutton(of, text="Otros idiomas", variable=self.all_lang, command=self.fill_tree).pack(side="left", padx=8)
        tf = ttk.Frame(left)
        tf.pack(fill="both", expand=True, padx=4, pady=4)
        self.tree = ttk.Treeview(tf, show="tree", selectmode="browse")
        sb = ttk.Scrollbar(tf, command=self.tree.yview)
        self.tree.configure(yscrollcommand=sb.set)
        self.tree.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")
        self.tree.tag_configure("edited", foreground="#b05a00")
        self.tree.bind("<<TreeviewOpen>>", self.on_open_node)
        self.tree.bind("<<TreeviewSelect>>", self.on_select)

        right = ttk.PanedWindow(pw, orient="vertical")
        pw.add(right, weight=4)
        top = ttk.Frame(right)
        right.add(top, weight=2)
        hf = ttk.Frame(top, padding=(4, 4))
        hf.pack(fill="x")
        self.title = tk.StringVar(value="")
        ttk.Label(hf, textvariable=self.title, font=("TkDefaultFont", 10, "bold")).pack(side="left")
        ttk.Button(hf, text="Restaurar original", command=self.restore).pack(side="right", padx=2)
        ttk.Button(hf, text="Ajustar caja", command=self.fit_box).pack(side="right", padx=2)
        xf = ttk.Frame(top)
        xf.pack(fill="both", expand=True, padx=4)
        self.editor = tk.Text(xf, wrap="word", undo=True, height=10, font=("Consolas", 12), state="disabled")
        sy = ttk.Scrollbar(xf, command=self.editor.yview)
        self.editor.configure(yscrollcommand=sy.set)
        self.editor.pack(side="left", fill="both", expand=True)
        sy.pack(side="right", fill="y")
        self.editor.tag_configure("tag", foreground="#1a5fb4")
        self.editor.tag_configure("bad", foreground="#c01c28", underline=True)
        self.editor.bind("<<Modified>>", self.on_modified)

        bottom = ttk.Frame(right)
        right.add(bottom, weight=3)
        pf = ttk.Frame(bottom, padding=(4, 2))
        pf.pack(fill="x")
        ttk.Label(pf, text="Vista previa (tamaño nativo ×):").pack(side="left")
        for n in (1, 2, 3):
            ttk.Radiobutton(pf, text="×%d" % n, value=n, variable=self.scale, command=self.refresh_preview).pack(side="left", padx=3)
        self.info = tk.StringVar()
        ttk.Label(pf, textvariable=self.info).pack(side="right")
        cf = ttk.Frame(bottom)
        cf.pack(fill="both", expand=True, padx=4)
        self.canvas = tk.Canvas(cf, bg="#222228", highlightthickness=0)
        vs = ttk.Scrollbar(cf, command=self.canvas.yview)
        hs = ttk.Scrollbar(bottom, orient="horizontal", command=self.canvas.xview)
        self.canvas.configure(yscrollcommand=vs.set, xscrollcommand=hs.set)
        vs.pack(side="right", fill="y")
        self.canvas.pack(side="left", fill="both", expand=True)
        hs.pack(fill="x", padx=4)
        self.warn = tk.Text(bottom, height=5, wrap="word", state="disabled", font=("TkDefaultFont", 10))
        self.warn.pack(fill="x", padx=4, pady=4)
        self.warn.tag_configure("err", foreground="#c01c28")
        self.warn.tag_configure("ok", foreground="#2a7b2a")
        self.warn.tag_configure("info", foreground="#555555")

    def lock(self, busy):
        self.busy = busy
        for b in self.buttons:
            b.state(["disabled"] if busy else ["!disabled"])

    def say(self, msg):
        self.status.set(msg)

    # ---- abrir
    def ask_open_image(self):
        p = filedialog.askopenfilename(title="Imagen del disco", filetypes=[("Imagen de disco", "*.bin *.iso *.img"), ("Todos", "*.*")])
        if p:
            self.open_path(p)

    def ask_open_folder(self):
        p = filedialog.askdirectory(title="Carpeta con los archivos del disco (la que contiene AREA00…)")
        if p:
            self.open_path(p)

    def open_path(self, path):
        if not self.confirm_discard():
            return
        try:
            self.say("Leyendo %s…" % path)
            self.root.update_idletasks()
            src = C.Source(path)
            self.proj = C.Project(src)
        except Exception as e:
            messagebox.showerror("No se puede abrir", str(e))
            self.say("Error al abrir.")
            return
        self.cur = None
        self.dirty = False
        self.set_editor("", False)
        self.fill_tree()
        self.say("%s: %d archivos WFM. Elige un diálogo en la lista." % (os.path.basename(path), len(src.files)))

    def confirm_discard(self):
        if self.proj and self.dirty and self.proj.n_edits():
            r = messagebox.askyesnocancel("Cambios sin guardar", "Hay cambios sin guardar en el proyecto. ¿Guardarlos ahora?")
            if r is None:
                return False
            if r:
                return self.save_project()
        return True

    def on_close(self):
        if self.busy:
            if not messagebox.askyesno("Trabajando", "Se está generando el disco. ¿Salir de todas formas?"):
                return
        elif not self.confirm_discard():
            return
        self.root.destroy()

    # ---- árbol
    def schedule_tree(self):
        if self._after:
            self.root.after_cancel(self._after)
        self._after = self.root.after(300, self.fill_tree)

    def fill_tree(self):
        self._after = None
        self.tree.delete(*self.tree.get_children())
        if not self.proj:
            return
        q, only = self.query.get().strip(), self.only_edited.get()
        if q or only:
            res = self.proj.search(q, self.all_lang.get(), only)
            for n, i in res[:1500]:
                self.tree.insert("", "end", iid="d|%s|%d" % (n, i), text=self.label(n, i), tags=self.tags(n, i))
            self.say("%d resultado(s)%s" % (len(res), " (se muestran 1500)" if len(res) > 1500 else ""))
            return
        areas = {}
        for n in self.proj.src.names():
            if self.all_lang.get() or C.is_main_lang(n):
                areas.setdefault(n.split("/")[0], []).append(n)
        for a in sorted(areas):
            node = self.tree.insert("", "end", iid="a|" + a, text=a, open=False)
            for n in areas[a]:
                f = self.tree.insert(node, "end", iid="f|" + n, text=os.path.basename(n)[:-4])
                self.tree.insert(f, "end", iid="x|" + n, text="…")

    def label(self, n, i):
        t = C.plain(self.proj.text(n, i), 50) or "(vacío)"
        base = "#%d  %s" % (i, t)
        return base if "|" in self.tree.focus() or not (self.query.get().strip() or self.only_edited.get()) \
            else "%s #%d  %s" % (os.path.basename(n)[:-4], i, t)

    def tags(self, n, i):
        return ("edited",) if self.proj.edited(n, i) else ()

    def on_open_node(self, _):
        iid = self.tree.focus()
        if not iid.startswith("f|"):
            return
        kids = self.tree.get_children(iid)
        if len(kids) == 1 and kids[0].startswith("x|"):
            self.tree.delete(kids[0])
            n = iid[2:]
            try:
                cnt = self.proj.count(n)
            except Exception as e:
                messagebox.showerror("Error", str(e))
                return
            for i in range(cnt):
                self.tree.insert(iid, "end", iid="d|%s|%d" % (n, i), text="#%d  %s" % (i, C.plain(self.proj.text(n, i), 50) or "(vacío)"),
                                 tags=self.tags(n, i))

    def on_select(self, _):
        iid = self.tree.focus()
        if not iid.startswith("d|"):
            return
        _, n, i = iid.split("|")
        self.cur = (n, int(i))
        t = self.proj.text(n, int(i))
        self.title.set("%s  #%s" % (n, i))
        self.set_editor(t, True)
        self.refresh_preview()

    def set_editor(self, text, enabled):
        self._loading = True
        self.editor.configure(state="normal")
        self.editor.delete("1.0", "end")
        self.editor.insert("1.0", text)
        self.editor.edit_reset()
        self.editor.edit_modified(False)
        self.editor.configure(state="normal" if enabled else "disabled")
        self._loading = False
        self.colorize()
        if not enabled:
            self.canvas.delete("all")
            self.show_warnings([])
            self.title.set("")
            self.info.set("")

    # ---- edición
    def on_modified(self, _):
        if self._loading or not self.editor.edit_modified():
            return
        self.editor.edit_modified(False)
        if not self.cur:
            return
        n, i = self.cur
        self.proj.set_text(n, i, self.editor.get("1.0", "end-1c"))
        self.dirty = True
        self.update_node(n, i)
        self.colorize()
        if getattr(self, "_pv", None):
            self.root.after_cancel(self._pv)
        self._pv = self.root.after(100, self.refresh_preview)

    def update_node(self, n, i):
        iid = "d|%s|%d" % (n, i)
        if self.tree.exists(iid):
            self.tree.item(iid, tags=self.tags(n, i))

    def colorize(self):
        for t in ("tag", "bad"):
            self.editor.tag_remove(t, "1.0", "end")
        txt = self.editor.get("1.0", "end-1c")
        for m in re.finditer(r"\{[^}]*\}", txt):
            self.editor.tag_add("tag", "1.0+%dc" % m.start(), "1.0+%dc" % m.end())

    def restore(self):
        if self.cur:
            n, i = self.cur
            self.set_editor(self.proj.original(n, i), True)
            self.proj.set_text(n, i, self.proj.original(n, i))
            self.update_node(n, i)
            self.refresh_preview()

    def fit_box(self):
        if not self.cur:
            return
        import text_preview
        n, i = self.cur
        text = self.editor.get("1.0", "end-1c")
        try:
            words, wc = self.proj.words(n, i, text)
        except W.WfmError as e:
            messagebox.showerror("Texto no válido", str(e))
            return
        w, h = text_preview.fit_box(wc, words)
        tag = "{BOX %d %d}" % (w, h)
        if re.search(r"\{BOX [^}]*\}", text):
            text = re.sub(r"\{BOX [^}]*\}", tag, text, count=1)
        else:
            text = tag + "\n" + text
        self.set_editor(text, True)
        self.proj.set_text(n, i, text)
        self.dirty = True
        self.update_node(n, i)
        self.refresh_preview()

    # ---- vista previa
    def refresh_preview(self):
        if not self.cur or not self.proj:
            return
        import text_preview
        n, i = self.cur
        text = self.editor.get("1.0", "end-1c")
        try:
            words, wc = self.proj.words(n, i, text)
        except W.WfmError as e:
            self.show_warnings([("err", str(e))])
            self.info.set("")
            return
        except Exception as e:
            self.show_warnings([("err", "Error interno: %s" % e)])
            return
        im, warns = text_preview.render(wc, words, scale=self.scale.get())
        buf = io.BytesIO()
        im.save(buf, "PNG")
        self._img = tk.PhotoImage(data=base64.b64encode(buf.getvalue()))
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, image=self._img, anchor="nw")
        self.canvas.configure(scrollregion=(0, 0, im.width, im.height))
        items = [("info" if w.startswith("i:") else "err", w[2:] if w.startswith("i:") else w) for w in warns]
        base = self.proj.wfm(n)
        if len(wc.glyphs) > len(base.glyphs):
            items.append(("info", "Se añadirán %d glifo(s) tomados de otros archivos." % (len(wc.glyphs) - len(base.glyphs))))
        if not any(k == "err" for k, _ in items):
            items.insert(0, ("ok", "Sin avisos."))
        self.show_warnings(items)
        self.info.set("%d palabras (%d bytes), fuente %dpx" % (len(words), 2 * len(words) + 2, base.dialogues[i].font[0]))

    def show_warnings(self, items):
        self.warn.configure(state="normal")
        self.warn.delete("1.0", "end")
        for k, m in items:
            self.warn.insert("end", m + "\n", k)
        self.warn.configure(state="disabled")

    # ---- proyecto
    def need_proj(self):
        if not self.proj:
            messagebox.showinfo("Sin disco", "Abre primero el disco o la carpeta.")
            return False
        return True

    def save_project(self):
        if not self.need_proj():
            return False
        p = filedialog.asksaveasfilename(title="Guardar proyecto", defaultextension=".json", initialfile="tombi-textos.json",
                                         filetypes=[("Proyecto JSON", "*.json")])
        if not p:
            return False
        json.dump(self.proj.to_json(), open(p, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        self.dirty = False
        self.say("Proyecto guardado: %s (%d cambios)" % (p, self.proj.n_edits()))
        return True

    def load_project(self):
        if not self.need_proj():
            return
        p = filedialog.askopenfilename(title="Cargar proyecto", filetypes=[("Proyecto JSON", "*.json"), ("Todos", "*.*")])
        if p:
            self.apply_data(lambda: self.proj.load_edits(json.load(open(p, encoding="utf-8")), replace=True), "Proyecto cargado")

    def export_all(self):
        if not self.need_proj():
            return
        p = filedialog.asksaveasfilename(title="Exportar todo", defaultextension=".txt", initialfile="tombi-textos.txt",
                                         filetypes=[("Texto", "*.txt"), ("JSON", "*.json")])
        if not p:
            return
        al = self.all_lang.get()
        self.say("Exportando…")
        self.root.update_idletasks()
        if p.lower().endswith(".json"):
            json.dump(self.proj.export_json(al), open(p, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        else:
            open(p, "w", encoding="utf-8").write(self.proj.export_txt(al))
        self.say("Exportado a %s" % p)

    def import_any(self):
        if not self.need_proj():
            return
        p = filedialog.askopenfilename(title="Importar", filetypes=[("Texto o JSON", "*.txt *.json"), ("Todos", "*.*")])
        if not p:
            return
        if p.lower().endswith(".json"):
            self.apply_data(lambda: self.proj.load_edits(json.load(open(p, encoding="utf-8")), replace=False), "Importado")
        else:
            self.apply_data(lambda: self.proj.import_txt(open(p, encoding="utf-8").read()), "Importado")

    def apply_data(self, fn, what):
        try:
            applied, problems = fn()
        except Exception as e:
            messagebox.showerror("Error", "%s" % e)
            return
        self.dirty = True
        self.fill_tree()
        if self.cur:
            self.set_editor(self.proj.text(*self.cur), True)
            self.refresh_preview()
        self.say("%s: %d diálogo(s) cambiados." % (what, applied))
        if problems:
            messagebox.showwarning("Avisos", "\n".join(problems[:25]) + ("\n…" if len(problems) > 25 else ""))

    # ---- ISO
    def generate(self):
        if not self.need_proj():
            return
        if not self.proj.n_edits():
            messagebox.showinfo("Sin cambios", "No hay diálogos modificados.")
            return
        files, errors = self.proj.encode_edited()
        if errors:
            messagebox.showerror("No se puede generar", "Corrige estos textos:\n\n" + "\n".join(errors[:15]))
            return
        out = filedialog.asksaveasfilename(title="Guardar la ISO nueva como", defaultextension=".bin", initialfile="Tombi-ES.bin",
                                           filetypes=[("Imagen de disco", "*.bin")])
        if not out:
            return
        work = os.path.join(os.path.dirname(os.path.abspath(out)), "tombi-text-trabajo")
        self.lock(True)
        self.say("Generando la ISO (puede tardar varios minutos; necesita ~1,5 GB libres)…")
        res = {}

        def job():
            try:
                if not C.have_mkpsxiso() or not self.proj.src.image:
                    d = os.path.splitext(out)[0] + "_archivos"
                    res["files"] = C.write_patched(self.proj, d)
                    res["dir"] = d
                else:
                    res["files"] = C.build_disc(self.proj, out, work)
            except Exception as e:
                res["err"] = str(e) if isinstance(e, (C.CoreError, W.WfmError)) else traceback.format_exc()
            self.root.after(0, done)

        def done():
            self.lock(False)
            if "err" in res:
                self.say("Error al generar la ISO.")
                messagebox.showerror("Error", res["err"])
            elif "dir" in res:
                self.say("Archivos modificados escritos en %s" % res["dir"])
                messagebox.showinfo("Archivos modificados", (
                    "No se pudo montar la ISO (falta mkpsxiso o solo se abrió una carpeta).\n\nSe escribieron %d archivo(s) en:\n%s\n\n"
                    "Cópialos sobre la carpeta extraída del disco y reconstruye con mkpsxiso: "
                    "python tools/mkdisc.py build --relayout (ver README del repositorio).") % (len(res["files"]), res["dir"]))
            else:
                self.say("ISO generada: %s" % out)
                messagebox.showinfo("Listo", "ISO generada:\n%s\n\nArchivos cambiados: %s\n\nSe puede borrar la carpeta de trabajo:\n%s"
                                    % (out, ", ".join(res["files"]), work))
        threading.Thread(target=job, daemon=True).start()


def main():
    root = tk.Tk()
    App(root, sys.argv[1] if len(sys.argv) > 1 else None)
    root.mainloop()


if __name__ == "__main__":
    main()
