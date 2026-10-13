#!/usr/bin/env python3
"""Animation preview window of the graphics editor (tools/gfx_editor.py, tree node "Animaciones"): play/pause, step, speed, loop, zoom and
GIF export of one Tombi animation. The data and rendering are in tools/anim.py; this file is only the Tk window."""
import os, sys
import tkinter as tk
from tkinter import filedialog, messagebox, ttk

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
from i18n import tr
import anim
from PIL import ImageTk

i18n.add({
    "es": {"an.title": "Animaciones", "an.play": "Reproducir", "an.pause": "Pausa", "an.prev": "◀ Paso", "an.next": "Paso ▶", "an.speed": "Velocidad:",
           "an.loop": "Bucle", "an.zoom": "Zoom:", "an.gif": "Exportar GIF", "an.entries": "Fotogramas del script (frame · ticks)",
           "an.info": "{0}  (id {1})  ·  script {2}  ·  {3} fotogramas, {4} ticks  ·  {5}", "an.looptype": "bucle al fotograma {0}", "an.holdtype": "termina y mantiene el último",
           "an.now": "Fotograma {0}/{1}  ·  frame WPP {2}  ·  {3} ticks", "an.savegif": "Guardar GIF", "an.gifok": "GIF guardado: {0}", "an.gifs": "Imagen GIF",
           "an.conf": "nombre: confianza {0} (H código, M código + imagen, L solo imagen)", "an.noname": "nombre sin identificar",
           "t.anims": "Animaciones (Tombi)"},
    "en": {"an.title": "Animations", "an.play": "Play", "an.pause": "Pause", "an.prev": "◀ Step", "an.next": "Step ▶", "an.speed": "Speed:",
           "an.loop": "Loop", "an.zoom": "Zoom:", "an.gif": "Export GIF", "an.entries": "Script entries (frame · ticks)",
           "an.info": "{0}  (id {1})  ·  script {2}  ·  {3} frames, {4} ticks  ·  {5}", "an.looptype": "loops to frame {0}", "an.holdtype": "ends and holds the last frame",
           "an.now": "Frame {0}/{1}  ·  WPP frame {2}  ·  {3} ticks", "an.savegif": "Save GIF", "an.gifok": "GIF saved: {0}", "an.gifs": "GIF image",
           "an.conf": "name confidence {0} (H code, M code + strip, L strip only)", "an.noname": "name not identified",
           "t.anims": "Animations (Tombi)"},
    "fr": {"an.title": "Animations", "an.play": "Lecture", "an.pause": "Pause", "an.speed": "Vitesse :", "an.loop": "Boucle", "an.gif": "Exporter GIF",
           "t.anims": "Animations (Tombi)"},
    "de": {"an.title": "Animationen", "an.play": "Abspielen", "an.pause": "Pause", "an.speed": "Tempo:", "an.loop": "Schleife", "an.gif": "GIF exportieren",
           "t.anims": "Animationen (Tombi)"},
    "it": {"an.title": "Animazioni", "an.play": "Riproduci", "an.pause": "Pausa", "an.speed": "Velocità:", "an.loop": "Ciclo", "an.gif": "Esporta GIF",
           "t.anims": "Animazioni (Tombi)"},
    "pt": {"an.title": "Animações", "an.play": "Reproduzir", "an.pause": "Pausa", "an.speed": "Velocidade:", "an.loop": "Ciclo", "an.gif": "Exportar GIF",
           "t.anims": "Animações (Tombi)"},
})

SPEEDS = ["0.25x", "0.5x", "1x", "2x", "4x"]


class AnimWindow:
    """One Toplevel; `show(id)` switches animation. `data` = (catalogue, anim.Sprites)."""

    def __init__(self, root, data):
        self.cat, self.spr = data
        self.root = root
        self.win = tk.Toplevel(root)
        self.win.title(tr("an.title"))
        self.win.protocol("WM_DELETE_WINDOW", self.close)
        self.an = None
        self.k = 0
        self.left = 0
        self.playing = False
        self.job = None
        self._photo = None
        w = self.win
        self.info = tk.StringVar()
        ttk.Label(w, textvariable=self.info, wraplength=620, justify="left").pack(anchor="w", padx=6, pady=(6, 0))
        self.conf = tk.StringVar()
        ttk.Label(w, textvariable=self.conf, foreground="#555").pack(anchor="w", padx=6)
        self.canvas = tk.Canvas(w, bg="#282838", width=420, height=300, highlightthickness=0)
        self.canvas.pack(fill="both", expand=True, padx=6, pady=4)
        self.now = tk.StringVar()
        ttk.Label(w, textvariable=self.now).pack(anchor="w", padx=6)
        bar = ttk.Frame(w, padding=4)
        bar.pack(fill="x")
        self.play_txt = tk.StringVar(value=tr("an.play"))
        ttk.Button(bar, textvariable=self.play_txt, command=self.toggle).pack(side="left")
        ttk.Button(bar, text=tr("lb.rename"), command=self.rename).pack(side="left", padx=(8, 2))
        ttk.Button(bar, text=tr("an.prev"), command=lambda: self.step(-1)).pack(side="left", padx=2)
        ttk.Button(bar, text=tr("an.next"), command=lambda: self.step(1)).pack(side="left")
        ttk.Label(bar, text=tr("an.speed")).pack(side="left", padx=(10, 2))
        self.speed = tk.StringVar(value="1x")
        ttk.Combobox(bar, textvariable=self.speed, values=SPEEDS, width=6, state="readonly").pack(side="left")
        self.loop = tk.BooleanVar(value=True)
        ttk.Checkbutton(bar, text=tr("an.loop"), variable=self.loop).pack(side="left", padx=8)
        ttk.Label(bar, text=tr("an.zoom")).pack(side="left")
        self.zoom = tk.StringVar(value="x3")
        zb = ttk.Combobox(bar, textvariable=self.zoom, values=["x%d" % i for i in range(1, 7)], width=4, state="readonly")
        zb.pack(side="left", padx=2)
        zb.bind("<<ComboboxSelected>>", lambda e: self.draw())
        ttk.Button(bar, text=tr("an.gif"), command=self.export_gif).pack(side="right")
        ttk.Label(w, text=tr("an.entries")).pack(anchor="w", padx=6)
        self.entries = tk.Text(w, height=6, wrap="none", font=("TkFixedFont", 9))
        self.entries.pack(fill="x", padx=6, pady=(0, 6))

    def alive(self):
        try:
            return bool(self.win.winfo_exists())
        except tk.TclError:
            return False

    def close(self):
        self.stop()
        self.win.destroy()

    def zoom_n(self):
        return int(self.zoom.get()[1:])

    def rename(self):
        """Name + note of this animation (tools/labels.py, id anim:tombi:N); the GIF file name uses it."""
        from tkinter import simpledialog
        import labels
        if not self.an:
            return
        lid = "anim:tombi:%d" % self.an["id"]
        cur = labels.get(lid) or {"name": "", "note": ""}
        nm = simpledialog.askstring(tr("lb.rename"), tr("lb.name"), initialvalue=cur["name"] or self.an["name"], parent=self.win)
        if nm is None:
            return
        nt = simpledialog.askstring(tr("lb.rename"), tr("lb.note"), initialvalue=cur["note"], parent=self.win)
        labels.set(lid, nm, cur["note"] if nt is None else nt)
        self.an["name"] = labels.name(lid) or anim.name_of(self.an["id"])
        self.show(self.an["id"])

    def show(self, aid):
        self.an = next(a for a in self.cat if a["id"] == aid)
        an = self.an
        self.box = anim.bounds(self.spr, an["frames"])
        al = an["alias_of"]
        if an["loop"] is not None:
            how = tr("an.looptype", an["loop"])
        else:
            how = tr("an.holdtype")
        self.info.set(tr("an.info", an["name"], aid, an["start"], len(an["entries"]), an["ticks"], how))
        self.conf.set(tr("an.conf", an["confidence"]) if an["confidence"] else tr("an.noname"))
        self.entries.configure(state="normal")
        self.entries.delete("1.0", "end")
        self.entries.insert("end", "  ".join("%d·%d%s" % (e["frame"], e["dur"], {0: "", 0x4000: "↺", 0x8000: "■", 0xC000: "↺■"}[e["flags"]])
                                              for e in an["entries"]))
        self.entries.configure(state="disabled")
        self.k = 0
        self.left = max(an["entries"][0]["dur"], 1)
        self.draw()
        self.play()

    def draw(self):
        if not self.an:
            return
        e = self.an["entries"][self.k]
        z = self.zoom_n()
        im = anim.compose(self.spr, e["frame"], self.box, z, (40, 40, 56, 255))
        self._photo = ImageTk.PhotoImage(im)
        self.canvas.delete("all")
        cw = max(self.canvas.winfo_width(), im.width)
        ch = max(self.canvas.winfo_height(), im.height)
        self.canvas.create_image(cw // 2, ch // 2, image=self._photo)
        self.now.set(tr("an.now", self.k + 1, len(self.an["entries"]), e["frame"], e["dur"]))

    def next_index(self):
        """Index after self.k, or None when the script ends and Loop is off."""
        an = self.an
        n = len(an["entries"])
        if self.k + 1 < n:
            return self.k + 1
        if not self.loop.get():
            return None
        return an["loop"] if an["loop"] is not None else 0

    def step(self, d):
        self.pause()
        n = len(self.an["entries"])
        self.k = (self.k + d) % n
        self.draw()

    def tick(self):
        self.job = None
        if not self.playing or not self.alive():
            return
        j = self.next_index()
        if j is None:
            self.pause()
            return
        self.k = j
        self.draw()
        self.schedule()

    def schedule(self):
        f = float(self.speed.get()[:-1])
        ms = max(int(max(self.an["entries"][self.k]["dur"], 1) * anim.TICK_MS / f), 10)
        self.job = self.win.after(ms, self.tick)

    def play(self):
        self.playing = True
        self.play_txt.set(tr("an.pause"))
        if self.job:
            self.win.after_cancel(self.job)
        self.schedule()

    def pause(self):
        self.playing = False
        self.play_txt.set(tr("an.play"))
        if self.job:
            self.win.after_cancel(self.job)
            self.job = None

    def stop(self):
        self.pause()

    def toggle(self):
        if self.an is None:
            return
        self.pause() if self.playing else self.play()

    def export_gif(self):
        if not self.an:
            return
        p = filedialog.asksaveasfilename(parent=self.win, title=tr("an.savegif"), defaultextension=".gif", initialfile=anim.fname(self.an) + ".gif",
                                         filetypes=[(tr("an.gifs"), "*.gif")])
        if p:
            anim.save_gif(self.spr, self.an, p, self.zoom_n(), int(anim.TICK_MS / float(self.speed.get()[:-1])))
            messagebox.showinfo(tr("an.title"), tr("an.gifok", p), parent=self.win)
