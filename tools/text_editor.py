#!/usr/bin/env python3
"""Tombi! editor de textos (Windows exe: tombi-text.exe). Abre el disco PAL, edita los diálogos con vista previa en vivo,
guarda el proyecto y genera una ISO nueva. La lógica está en text_core.py / wfm_py.py / text_preview.py; aquí solo la ventana.

  python tools/text_editor.py [disco.bin | carpeta_fs]
"""
import base64, io, json, os, re, sys, threading, traceback

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
from i18n import tr
import text_strings
import text_core as C
import wfm_py as W

try:
    import tkinter as tk
    from tkinter import filedialog, messagebox, ttk
except ImportError:      # pragma: no cover
    sys.exit("Falta tkinter (en Linux: apt install python3-tk).")

def set_icon(root, name):
    """Window/taskbar icon: the bundled .ico on Windows, a PNG (iconphoto) elsewhere or as fallback."""
    base = [os.path.join(getattr(sys, "_MEIPASS", ""), "icons"), os.path.join(os.path.dirname(os.path.abspath(__file__)), "icons")]
    for d in base:
        ico, png = os.path.join(d, name + ".ico"), os.path.join(d, name + ".png")
        try:
            if os.name == "nt" and os.path.isfile(ico):
                root.iconbitmap(ico)
                return
        except Exception:
            pass
        try:
            if os.path.isfile(png):
                root._icon_img = tk.PhotoImage(file=png)
                root.iconphoto(True, root._icon_img)
                return
        except Exception:
            pass


class App:
    def __init__(self, root, path=None):
        self.root = root
        root.title(tr("te.title"))
        root.geometry("1280x800")
        self.proj = None
        self.cur = None            # (path, idx)
        self.dirty = False
        self.busy = False
        self._after = None
        self._img = None
        self._loading = False
        self.scale = tk.IntVar(value=2)
        self.lang_vars = {}
        self.only_edited = tk.BooleanVar(value=False)
        self.query = tk.StringVar()
        self.link_var = tk.BooleanVar(value=True)
        self.status_text = tr("te.nodisc.status")
        set_icon(root, "text")
        self._build()
        root.protocol("WM_DELETE_WINDOW", self.on_close)
        if path:
            self.open_path(path)

    # ---- ventana
    def _build(self):
        bar = ttk.Frame(self.root, padding=4)
        bar.pack(fill="x")
        self.buttons = []
        for item in ((tr("te.opendisc"), self.ask_open_image), (tr("te.openfolder"), self.ask_open_folder), None,
                          (tr("te.saveproj"), self.save_project), (tr("te.loadproj"), self.load_project), None,
                          (tr("te.exportall"), self.export_all), (tr("te.import"), self.import_any), None,
                          (tr("te.replace"), self.replace_dialog), None,
                          (tr("te.iso"), self.generate), (tr("te.help"), lambda: messagebox.showinfo(tr("te.help"), tr("te.helptext")))):
            if item is None:
                ttk.Separator(bar, orient="vertical").pack(side="left", fill="y", padx=6)
                continue
            text, cmd = item
            b = ttk.Button(bar, text=text, command=cmd)
            b.pack(side="left", padx=2)
            self.buttons.append(b)
        i18n.selector(bar, self.rebuild).pack(side="right")
        self.status = tk.StringVar(value=self.status_text)
        ttk.Label(self.root, textvariable=self.status, anchor="w", relief="sunken", padding=3).pack(side="bottom", fill="x")

        pw = ttk.PanedWindow(self.root, orient="horizontal")
        pw.pack(fill="both", expand=True)
        left = ttk.Frame(pw)
        pw.add(left, weight=1)
        sf = ttk.Frame(left, padding=(4, 4, 4, 0))
        sf.pack(fill="x")
        ttk.Label(sf, text=tr("te.search")).pack(side="left")
        e = ttk.Entry(sf, textvariable=self.query)
        e.pack(side="left", fill="x", expand=True, padx=4)
        self.query.trace_add("write", lambda *_: self.schedule_tree())
        of = ttk.Frame(left, padding=(4, 0))
        of.pack(fill="x")
        ttk.Checkbutton(of, text=tr("te.onlyedited"), variable=self.only_edited, command=self.fill_tree).pack(side="left")
        ttk.Checkbutton(of, text=tr("te.link"), variable=self.link_var, command=self.on_link).pack(side="left", padx=8)
        self.langf = ttk.LabelFrame(left, text=tr("te.langs"), padding=(4, 0))
        self.langf.pack(fill="x", padx=4)
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

        right = ttk.Frame(pw)
        pw.add(right, weight=4)
        # grid with fixed row ratios (a vertical PanedWindow gave the editor no height on Windows)
        right.columnconfigure(0, weight=1)
        right.rowconfigure(0, weight=2, minsize=220, uniform="pane")
        right.rowconfigure(1, weight=3, minsize=260, uniform="pane")
        top = ttk.Frame(right)
        top.grid(row=0, column=0, sticky="nsew")
        hf = ttk.Frame(top, padding=(4, 4))
        hf.pack(fill="x")
        self.title = tk.StringVar(value="")
        ttk.Label(hf, textvariable=self.title, font=("TkDefaultFont", 10, "bold")).pack(side="left")
        ttk.Button(hf, text=tr("te.restore"), command=self.restore).pack(side="right", padx=2)
        ttk.Button(hf, text=tr("te.fitbox"), command=self.fit_box).pack(side="right", padx=2)
        xf = ttk.Frame(top)
        xf.pack(fill="both", expand=True, padx=4)
        self.editor = tk.Text(xf, wrap="word", undo=True, height=6, width=40, font=("Consolas", 12), state="disabled")
        sy = ttk.Scrollbar(xf, command=self.editor.yview)
        self.editor.configure(yscrollcommand=sy.set)
        self.editor.pack(side="left", fill="both", expand=True)
        sy.pack(side="right", fill="y")
        self.editor.tag_configure("tag", foreground="#1a5fb4")
        self.editor.tag_configure("bad", foreground="#c01c28", underline=True)
        self.editor.bind("<<Modified>>", self.on_modified)

        bottom = ttk.Frame(right)
        bottom.grid(row=1, column=0, sticky="nsew")
        pf = ttk.Frame(bottom, padding=(4, 2))
        pf.pack(fill="x")
        ttk.Label(pf, text=tr("te.preview")).pack(side="left")
        for n in (1, 2, 3):
            ttk.Radiobutton(pf, text="×%d" % n, value=n, variable=self.scale, command=self.refresh_preview).pack(side="left", padx=3)
        self.info = tk.StringVar()
        ttk.Label(pf, textvariable=self.info).pack(side="right")
        cf = ttk.Frame(bottom)
        cf.pack(fill="both", expand=True, padx=4)
        self.canvas = tk.Canvas(cf, bg="#222228", highlightthickness=0, height=120)
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
        self.status_text = msg
        self.status.set(msg)

    def rebuild(self):
        """Interface language changed: rebuild every widget in the new language and restore the state."""
        text = self.editor.get("1.0", "end-1c") if self.cur else ""
        for w in self.root.winfo_children():
            w.destroy()
        self._build()
        self.root.title(tr("te.title"))
        if self.proj:
            self.build_langs()
            self.fill_tree()
            if self.cur:
                self.title.set("%s  #%s" % self.cur)
                self.set_editor(text, True)
                self.refresh_preview()

    # ---- abrir
    def ask_open_image(self):
        p = filedialog.askopenfilename(title=tr("te.discimg"), filetypes=[(tr("te.discimg"), "*.bin *.iso *.img"), (tr("te.all"), "*.*")])
        if p:
            self.open_path(p)

    def ask_open_folder(self):
        p = filedialog.askdirectory(title=tr("te.folderdlg"))
        if p:
            self.open_path(p)

    def open_path(self, path):
        if not self.confirm_discard():
            return
        try:
            self.say(tr("te.reading", path))
            self.root.update_idletasks()
            src = C.Source(path)
            self.proj = C.Project(src)
        except Exception as e:
            messagebox.showerror(tr("te.cantopen"), str(e))
            self.say(tr("te.openerr"))
            return
        self.cur = None
        self.dirty = False
        self.set_editor("", False)
        self.build_langs()
        self.fill_tree()
        self.say(tr("te.opened", os.path.basename(path), len(src.files)))

    def build_langs(self):
        """One checkbox per language found on the disc (+ Todos / Ninguno); the choice lives in proj.langs."""
        for w in self.langf.winfo_children():
            w.destroy()
        self.lang_vars = {}
        langs = self.proj.languages()
        if not self.proj.langs & set(langs):
            self.proj.langs = {langs[0]}
        for k, l in enumerate(langs):
            v = tk.BooleanVar(value=l in self.proj.langs)
            self.lang_vars[l] = v
            ttk.Checkbutton(self.langf, text=tr("lang." + l) if ("lang." + l) in i18n.CAT["es"] else l, variable=v, command=self.on_langs).grid(
                row=k // 3, column=k % 3, sticky="w", padx=2)
        r = (len(langs) + 2) // 3
        ttk.Button(self.langf, text=tr("te.allbtn"), width=8, command=lambda: self.set_langs(True)).grid(row=r, column=0, pady=2)
        ttk.Button(self.langf, text=tr("te.nonebtn"), width=8, command=lambda: self.set_langs(False)).grid(row=r, column=1, pady=2)

    def set_langs(self, on):
        for v in self.lang_vars.values():
            v.set(on)
        self.on_langs()

    def on_langs(self):
        if not self.proj:
            return
        self.proj.langs = {l for l, v in self.lang_vars.items() if v.get()}
        self.proj._pool = None
        self.dirty = True
        if self.cur and not self.proj.selected(self.cur[0]):
            self.cur = None
            self.set_editor("", False)
        self.fill_tree()

    def confirm_discard(self):
        if self.proj and self.dirty and self.proj.n_edits():
            r = messagebox.askyesnocancel(tr("te.unsaved"), tr("te.unsavedq"))
            if r is None:
                return False
            if r:
                return self.save_project()
        return True

    def on_close(self):
        if self.busy:
            if not messagebox.askyesno(tr("te.working"), tr("te.quitbusy")):
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
            res = self.proj.search(q, only)
            seen = set()
            shown = 0
            for n, i in res:
                if (n, i) in seen:
                    continue
                grp = [c for c in self.proj.copies(n, i) if (c, i) in set(res)] if self.proj.link else [n]
                seen.update((c, i) for c in grp)
                shown += 1
                if shown > 1500:
                    break
                lab = self.label(n, i) + (tr("te.copies", len(grp)) if len(grp) > 1 else "")
                top = self.tree.insert("", "end", iid="d|%s|%d" % (n, i), text=lab, tags=self.tags(n, i))
                if len(grp) > 1:
                    for c in grp:
                        if c != n:
                            self.tree.insert(top, "end", iid="d|%s|%d" % (c, i), text=c.split("/")[0] + "  " + os.path.basename(c)[:-4], tags=self.tags(c, i))
            self.say(tr("te.results", len(res), tr("te.shown1500") if len(res) > 1500 else ""))
            return
        areas = {}
        for n in self.proj.src.names():
            if self.proj.selected(n):
                areas.setdefault(n.split("/")[0], []).append(n)
        for a in sorted(areas):
            node = self.tree.insert("", "end", iid="a|" + a, text=a, open=False)
            for n in areas[a]:
                f = self.tree.insert(node, "end", iid="f|" + n, text=os.path.basename(n)[:-4])
                self.tree.insert(f, "end", iid="x|" + n, text="…")

    def label(self, n, i):
        t = C.plain(self.proj.text(n, i), 50) or tr("te.empty")
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
                messagebox.showerror(tr("error"), str(e))
                return
            for i in range(cnt):
                self.tree.insert(iid, "end", iid="d|%s|%d" % (n, i), text="#%d  %s" % (i, C.plain(self.proj.text(n, i), 50) or tr("te.empty")),
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
        for c in self.proj.set_text_linked(n, i, self.editor.get("1.0", "end-1c")):
            self.update_node(c, i)
        self.dirty = True
        self.colorize()
        if getattr(self, "_pv", None):
            self.root.after_cancel(self._pv)
        self._pv = self.root.after(100, self.refresh_preview)

    def on_link(self):
        if self.proj:
            self.proj.link = self.link_var.get()
            self.dirty = True
            self.fill_tree()

    def replace_dialog(self):
        if not self.need_proj():
            return
        d = tk.Toplevel(self.root)
        d.title(tr("te.replace"))
        d.transient(self.root)
        f = ttk.Frame(d, padding=8)
        f.pack(fill="both", expand=True)
        find, repl = tk.StringVar(), tk.StringVar()
        case, word, tags, sel = tk.BooleanVar(), tk.BooleanVar(), tk.BooleanVar(), tk.BooleanVar(value=True)
        for r, (lab, var) in enumerate(((tr("te.rfind"), find), (tr("te.rwith"), repl))):
            ttk.Label(f, text=lab).grid(row=r, column=0, sticky="w", pady=2)
            ttk.Entry(f, textvariable=var, width=36).grid(row=r, column=1, sticky="ew", pady=2)
        f.columnconfigure(1, weight=1)
        for r, (lab, var) in enumerate(((tr("te.rcase"), case), (tr("te.rword"), word), (tr("te.rtags"), tags), (tr("te.rsel"), sel)), 2):
            ttk.Checkbutton(f, text=lab, variable=var).grid(row=r, column=0, columnspan=2, sticky="w")
        msg = tk.StringVar()
        ttk.Label(f, textvariable=msg, wraplength=380).grid(row=7, column=0, columnspan=2, sticky="w", pady=4)

        def run(only):
            if not find.get():
                return
            opts = dict(case=case.get(), word=word.get(), tags=tags.get(), all_langs=not sel.get())
            if only is not None and self.proj.link:
                only = [(c, only[0][1]) for c in self.proj.copies(*only[0])]
            n, probs = self.proj.replace_all(find.get(), repl.get(), only=only, **opts)
            if n:
                self.dirty = True
                self.build_langs()
                self.fill_tree()
                if self.cur:
                    self.set_editor(self.proj.text(*self.cur), True)
                    self.refresh_preview()
            msg.set(tr("te.rdone", n, len(probs)))
            if probs:
                messagebox.showwarning(tr("te.warnings"), "\n".join(probs[:25]) + ("\n…" if len(probs) > 25 else ""), parent=d)

        def undo():
            if self.proj.undo_replace():
                self.dirty = True
                self.fill_tree()
                if self.cur:
                    self.set_editor(self.proj.text(*self.cur), True)
                    self.refresh_preview()
                msg.set(tr("te.rundone"))
            else:
                msg.set(tr("te.rnoundo"))
        bf = ttk.Frame(f)
        bf.grid(row=6, column=0, columnspan=2, pady=6)
        ttk.Button(bf, text=tr("te.rone"), command=lambda: run([self.cur] if self.cur else [])).pack(side="left", padx=3)
        ttk.Button(bf, text=tr("te.rall"), command=lambda: run(None)).pack(side="left", padx=3)
        ttk.Button(bf, text=tr("te.rundo"), command=undo).pack(side="left", padx=3)

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
            for c in self.proj.set_text_linked(n, i, self.proj.original(n, i)):
                self.update_node(c, i)
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
            messagebox.showerror(tr("te.badtext"), str(e))
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
            self.show_warnings([("err", tr("te.internal", e))])
            return
        im, warns = text_preview.render(wc, words, scale=self.scale.get(), title=self.proj.title_lines(n, i, text))
        prob = self.proj.title_problem(n, i, text)
        if prob:
            warns.append(prob)
        tmax = self.proj.title_max(n, i)
        if tmax is None:
            self.status.set(self.status_text)
        else:
            hid = self.proj.title_hidden(n, i, text)
            if prob:
                self.status.set(tr("te.titlemax", tmax))
            elif hid:
                self.status.set(tr("te.titlehidden", tmax - hid, hid, tmax))
                warns.append("i:" + tr("te.titlehidden", tmax - hid, hid, tmax))
            else:
                self.status.set(self.status_text)
        buf = io.BytesIO()
        im.save(buf, "PNG")
        self._img = tk.PhotoImage(data=base64.b64encode(buf.getvalue()))
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, image=self._img, anchor="nw")
        self.canvas.configure(scrollregion=(0, 0, im.width, im.height))
        items = [("info" if w.startswith("i:") else "err", w[2:] if w.startswith("i:") else w) for w in warns]
        base = self.proj.wfm(n)
        if len(wc.glyphs) > len(base.glyphs):
            items.append(("info", tr("te.glyphs", len(wc.glyphs) - len(base.glyphs))))
        if not any(k == "err" for k, _ in items):
            items.insert(0, ("ok", tr("te.nowarn")))
        self.show_warnings(items)
        self.info.set(tr("te.wordsinfo", len(words), 2 * len(words) + 2, base.dialogues[i].font[0]))

    def show_warnings(self, items):
        self.warn.configure(state="normal")
        self.warn.delete("1.0", "end")
        for k, m in items:
            self.warn.insert("end", m + "\n", k)
        self.warn.configure(state="disabled")

    # ---- proyecto
    def need_proj(self):
        if not self.proj:
            messagebox.showinfo(tr("te.nodisc"), tr("te.opendiscfirst"))
            return False
        return True

    def save_project(self):
        if not self.need_proj():
            return False
        p = filedialog.asksaveasfilename(title=tr("te.saveproj"), defaultextension=".json", initialfile="tombi-textos.json",
                                         filetypes=[(tr("te.projjson"), "*.json")])
        if not p:
            return False
        json.dump(self.proj.to_json(), open(p, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        self.dirty = False
        self.say(tr("te.projsaved", p, self.proj.n_edits()))
        return True

    def load_project(self):
        if not self.need_proj():
            return
        p = filedialog.askopenfilename(title=tr("te.loadproj"), filetypes=[(tr("te.projjson"), "*.json"), (tr("te.all"), "*.*")])
        if p:
            self.apply_data(lambda: self.proj.load_edits(json.load(open(p, encoding="utf-8")), replace=True), tr("te.projloaded"))

    def export_all(self):
        if not self.need_proj():
            return
        p = filedialog.asksaveasfilename(title=tr("te.exportall"), defaultextension=".txt", initialfile="tombi-textos.txt",
                                         filetypes=[(tr("te.text"), "*.txt"), ("JSON", "*.json")])
        if not p:
            return
        self.say(tr("te.exporting"))
        self.root.update_idletasks()
        if p.lower().endswith(".json"):
            json.dump(self.proj.export_json(), open(p, "w", encoding="utf-8"), ensure_ascii=False, indent=1)
        else:
            open(p, "w", encoding="utf-8").write(self.proj.export_txt())
        self.say(tr("te.exported", p))

    def import_any(self):
        if not self.need_proj():
            return
        p = filedialog.askopenfilename(title=tr("te.import"), filetypes=[(tr("te.textjson"), "*.txt *.json"), (tr("te.all"), "*.*")])
        if not p:
            return
        if p.lower().endswith(".json"):
            self.apply_data(lambda: self.proj.load_edits(json.load(open(p, encoding="utf-8")), replace=False), tr("te.imported"))
        else:
            self.apply_data(lambda: self.proj.import_txt(open(p, encoding="utf-8").read()), tr("te.imported"))

    def apply_data(self, fn, what):
        try:
            applied, problems = fn()
        except Exception as e:
            messagebox.showerror(tr("error"), "%s" % e)
            return
        self.dirty = True
        self.build_langs()
        self.fill_tree()
        if self.cur and not self.proj.selected(self.cur[0]):
            self.cur = None
            self.set_editor("", False)
        if self.cur:
            self.set_editor(self.proj.text(*self.cur), True)
            self.refresh_preview()
        self.say(tr("te.applied", what, applied))
        if problems:
            messagebox.showwarning(tr("te.warnings"), "\n".join(problems[:25]) + ("\n…" if len(problems) > 25 else ""))

    # ---- ISO
    def generate(self):
        if not self.need_proj():
            return
        if not self.proj.n_edits():
            messagebox.showinfo(tr("te.nochanges"), tr("te.nodialogues"))
            return
        files, errors = self.proj.encode_edited()
        if errors:
            messagebox.showerror(tr("te.cantgen"), tr("te.fixtexts") + "\n\n" + "\n".join(errors[:15]))
            return
        out = filedialog.asksaveasfilename(title=tr("te.saveiso"), defaultextension=".bin", initialfile="Tombi-ES.bin",
                                           filetypes=[(tr("te.discimg"), "*.bin")])
        if not out:
            return
        work = os.path.join(os.path.dirname(os.path.abspath(out)), "tombi-text-trabajo")
        self.lock(True)
        self.say(tr("te.generating"))
        res = {}

        def job():
            try:
                if not C.have_mkpsxiso() or not self.proj.src.image:
                    d = os.path.splitext(out)[0] + "_archivos"
                    res["files"] = C.write_patched(self.proj, d)
                    res["dir"] = d
                else:
                    res.update(C.build_report(self.proj, out, work))
                    res["files"] = res["files"]
            except Exception as e:
                res["err"] = str(e) if isinstance(e, (C.CoreError, W.WfmError)) else traceback.format_exc()
            self.root.after(0, done)

        def done():
            self.lock(False)
            if "err" in res:
                self.say(tr("te.isofail"))
                messagebox.showerror(tr("error"), res["err"])
            elif "dir" in res:
                self.say(tr("te.writtenin", res["dir"]))
                messagebox.showinfo(tr("te.modfiles"), tr("te.fallback", len(res["files"]), res["dir"]))
            else:
                self.say(tr("te.isodone", res["out"]))
                messagebox.showinfo(tr("te.done"), tr("te.isook", res["out"], ", ".join(res["files"]), work, res["size"], len(res["files"])))
        threading.Thread(target=job, daemon=True).start()


def main():
    root = tk.Tk()
    App(root, sys.argv[1] if len(sys.argv) > 1 else None)
    root.mainloop()


if __name__ == "__main__":
    main()
