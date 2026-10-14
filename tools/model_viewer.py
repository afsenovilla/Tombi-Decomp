#!/usr/bin/env python3
"""tk widget of the 3D model preview (rooms / object models) of the graphics editor.  Mesh loading + software renderer: tools/model_view.py.

ModelViewer(parent).show(kind, ld, idx, k): kind "room" | "obj".  OpenGL (pyopengltk + PyOpenGL, fixed pipeline: one texture atlas per part,
alpha test for the index-0 cut-out, double sided) when available, else the numpy software renderer (reduced resolution while the mouse
drags, full resolution after 250 ms idle).  TOMBI_NOGL=1 forces the software path.  Controls: left drag orbit, right/middle drag pan, wheel zoom,
double click reset, WASD (fly) / QE (down / up)."""
import math, os, sys
import tkinter as tk
from tkinter import ttk, filedialog, colorchooser, messagebox

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
import model_view as MV
from i18n import tr


def _gl_available():
    if os.environ.get("TOMBI_NOGL"):
        return None
    try:
        import pyopengltk
        from OpenGL import GL
        return pyopengltk
    except Exception:
        return None


class ModelViewer(ttk.Frame):
    def __init__(self, parent, say=None):
        super().__init__(parent)
        self.say = say or (lambda s: None)
        self.mesh = None
        self.cam = MV.Camera()
        self.bg = (40, 40, 56)
        self.v = {k: tk.BooleanVar(value=True) for k in ("tex", "vcol", "sky")}
        self.v["wire"] = tk.BooleanVar(value=False)
        self.v["objects"] = tk.BooleanVar(value=False)
        self.lay = {}                  # layer -> BooleanVar
        self.sel = None
        tb = ttk.Frame(self, padding=(4, 2))
        tb.pack(fill="x")
        for key, lab in (("tex", "m.tex"), ("wire", "m.wire"), ("vcol", "m.vcol"), ("sky", "m.sky")):
            ttk.Checkbutton(tb, text=tr(lab), variable=self.v[key], command=self.redraw).pack(side="left", padx=3)
        for lab, cmd in (("m.bg", self.pick_bg), ("m.reset", self.reset), ("m.glb", self.export_glb), ("m.shot", self.screenshot)):
            ttk.Button(tb, text=tr(lab), command=cmd).pack(side="left", padx=3)
        ttk.Checkbutton(tb, text=tr("m.showobj"), variable=self.v["objects"], command=self._objs_changed).pack(side="left", padx=3)
        self.layfr = ttk.Frame(tb)
        self.layfr.pack(side="left")
        self.info = tk.StringVar()
        ttk.Label(tb, textvariable=self.info).pack(side="left", padx=8)
        ttk.Label(self, text=tr("m.hint"), foreground="#555").pack(side="bottom", anchor="w", padx=4)
        self.side = tk.Text(self, width=34, height=8, wrap="word", state="disabled", bg="#f4f4f4", font=("TkFixedFont", 9))
        self.side.pack(side="right", fill="y")
        self.holder = ttk.Frame(self)
        self.holder.pack(side="left", fill="both", expand=True)
        self.gl = None
        self.surface = None
        self._photo = None
        self._idle = None
        self._fast = False
        self._last = None
        self._keys = set()
        self._shot = None
        self._make_surface(_gl_available())

    # ---- surfaces
    def _make_surface(self, glmod):
        if self.surface is not None:
            self.surface.destroy()
        self.gl = None
        if glmod:
            outer = self

            class GLView(glmod.OpenGLFrame):
                def initgl(s):
                    try:
                        outer._initgl()
                        outer._gl_ready = True
                        outer.after(1, outer.redraw)
                    except Exception as e:
                        outer._gl_failed(e)

                def redraw(s):
                    try:
                        outer._draw_gl(max(1, s.winfo_width()), max(1, s.winfo_height()))
                    except Exception as e:
                        outer._gl_failed(e)
            self.surface = GLView(self.holder, width=640, height=480)
            self.surface.animate = 0
            self.gl = self.surface
            self._gl_ready = False
        else:
            self.surface = tk.Label(self.holder, bg="#202030", bd=0)
        self.surface.pack(fill="both", expand=True)
        self._bind(self.surface)

    def _gl_failed(self, e):
        if self.gl is None:
            return
        self.say("OpenGL: %r -> software" % (e,))
        self.after(10, lambda: (self._make_surface(None), self.redraw()))

    def backend(self):
        return tr("m.gl") if self.gl is not None else tr("m.soft")

    # ---- input
    def _bind(self, w):
        w.bind("<ButtonPress>", self._press)
        w.bind("<B1-Motion>", lambda e: self._motion(e, 1))
        w.bind("<B2-Motion>", lambda e: self._motion(e, 2))
        w.bind("<B3-Motion>", lambda e: self._motion(e, 2))
        w.bind("<ButtonRelease>", self._release)
        w.bind("<Motion>", self._hover)
        w.bind("<MouseWheel>", lambda e: self._zoom(1 if e.delta > 0 else -1))
        w.bind("<Button-4>", lambda e: self._zoom(1))
        w.bind("<Button-5>", lambda e: self._zoom(-1))
        w.bind("<Double-Button-1>", lambda e: self.reset())
        w.bind("<Enter>", lambda e: w.focus_set())
        w.bind("<KeyPress>", self._key)
        w.bind("<Configure>", lambda e: self.redraw())

    def _press(self, e):
        self._pos = (e.x, e.y)
        self._moved = False

    def _motion(self, e, mode):
        dx, dy = e.x - self._pos[0], e.y - self._pos[1]
        self._moved = True
        self._pos = (e.x, e.y)
        if mode == 1:
            self.cam.orbit(dx, dy)
        else:
            self.cam.pan(dx, dy, self.surface.winfo_height())
        self.redraw(fast=True)

    def _release(self, e=None):
        if e is not None and not getattr(self, "_moved", True) and e.num == 1:
            self._click(e.x, e.y)
        self.redraw()

    # ---- placed objects
    def _layers(self):
        return {k for k, v in self.lay.items() if v.get()}

    def _objs_changed(self):
        self.sel = None
        self._say_obj()
        self.redraw()

    def _build_layer_checks(self):
        for w in self.layfr.winfo_children():
            w.destroy()
        self.lay = {}
        for L in MV.object_layers(self.mesh.objs if self.mesh else []):
            self.lay[L] = tk.BooleanVar(value=True)
            c = MV.LAYER_COL.get(L, (.7, .7, .7))
            tk.Checkbutton(self.layfr, text=tr("m.layer", L), variable=self.lay[L], command=self._objs_changed, fg="#%02x%02x%02x" % tuple(int(v * 200) for v in c)).pack(side="left")

    def _say_obj(self):
        self.side.configure(state="normal")
        self.side.delete("1.0", "end")
        if self.v["objects"].get() and self.mesh:
            self.side.insert("end", MV.object_text(self.sel) if self.sel else tr("m.noobj") + "\n" + tr("m.nobjs", len(self.mesh.objs)))
        self.side.configure(state="disabled")

    def _click(self, x, y):
        if not (self.v["objects"].get() and self.mesh and self.mesh.objs):
            return
        w, h = max(8, self.surface.winfo_width()), max(8, self.surface.winfo_height())
        self.sel = MV.pick_object(self.mesh.objs, self.cam, w, h, x, y, self._layers())
        self._say_obj()
        self.redraw()

    def _hover(self, e):
        if self.v["objects"].get() and self.mesh and self.mesh.objs and e.state & 0x700 == 0:
            o = MV.pick_object(self.mesh.objs, self.cam, max(8, self.surface.winfo_width()), max(8, self.surface.winfo_height()), e.x, e.y, self._layers())
            self.info.set(o["label"] if o else tr("m.info", self.mesh.name, self.mesh.tris, self.backend()))

    def _zoom(self, s):
        self.cam.zoom(s)
        self.redraw(fast=True)

    def _key(self, e):
        k = e.keysym.lower()
        d = {"w": (1, 0, 0), "s": (-1, 0, 0), "d": (0, 1, 0), "a": (0, -1, 0), "e": (0, 0, 1), "q": (0, 0, -1)}.get(k)
        if d:
            self.cam.fly(*d)
            self.redraw(fast=True)

    # ---- model
    def show(self, kind, ld, idx, k=None):
        if kind == "room":
            self.mesh = MV.load_room(ld, idx)
        else:
            self.mesh = MV.load_object(ld, idx, k)
        self.sel = None
        self._build_layer_checks()
        self._say_obj()
        self.has_sky = any(p["kind"] == "sky" for p in self.mesh.parts)
        self.cam.reset(self.mesh.center, self.mesh.radius)
        self._glstate = None
        self.info.set(tr("m.info", self.mesh.name, self.mesh.tris, self.backend()))
        self.redraw()

    def reset(self):
        if self.mesh:
            self.cam.reset(self.mesh.center, self.mesh.radius)
            self.redraw()

    def pick_bg(self):
        c = colorchooser.askcolor(color="#%02x%02x%02x" % self.bg, parent=self)[0]
        if c:
            self.bg = tuple(int(x) for x in c)
            self.redraw()

    def opts(self):
        o = {k: v.get() for k, v in self.v.items()}
        o["bg"] = self.bg
        o["layers"] = self._layers()
        o["sel"] = self.sel
        return o

    # ---- drawing
    def redraw(self, fast=False):
        if self.mesh is None or self.surface is None:
            return
        if self.gl is not None:
            if not self._gl_ready:
                return          # drawn once the context exists (initgl -> redraw)
            try:
                self.gl.tkExpose(None)
            except Exception as e:
                self._gl_failed(e)
            return
        w, h = max(8, self.surface.winfo_width()), max(8, self.surface.winfo_height())
        if self._idle:
            self.after_cancel(self._idle)
            self._idle = None
        k = 0.35 if fast else 1.0
        self._draw_soft(w, h, k)
        if fast:
            self._idle = self.after(250, self.redraw)

    def _draw_soft(self, w, h, k):
        from PIL import Image, ImageTk
        rw, rh = max(8, int(w * k)), max(8, int(h * k))
        im = Image.fromarray(MV.render(self.mesh, self.cam, rw, rh, self.opts()))
        if k < 1:
            im = im.resize((w, h), Image.NEAREST)
        self._photo = ImageTk.PhotoImage(im)
        self.surface.configure(image=self._photo)

    def _initgl(self):
        from OpenGL import GL as g
        g.glEnable(g.GL_DEPTH_TEST)
        g.glDisable(g.GL_CULL_FACE)
        g.glEnable(g.GL_ALPHA_TEST)
        g.glAlphaFunc(g.GL_GREATER, 0.5)
        self._glstate = None

    def _upload(self, g):
        for t in getattr(self, "_texs", []):
            g.glDeleteTextures([t])
        self._texs, self._gparts = [], []
        for p in self.mesh.parts:
            tex = p["tex"]
            if tex.shape[1] > 8192:
                from PIL import Image
                tex = np.asarray(Image.fromarray(tex).resize((8192, tex.shape[0]), Image.BILINEAR))
            t = g.glGenTextures(1)
            g.glBindTexture(g.GL_TEXTURE_2D, t)
            g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_MIN_FILTER, g.GL_NEAREST)
            g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_MAG_FILTER, g.GL_NEAREST)
            g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_WRAP_S, g.GL_CLAMP_TO_EDGE)
            g.glTexParameteri(g.GL_TEXTURE_2D, g.GL_TEXTURE_WRAP_T, g.GL_CLAMP_TO_EDGE)
            g.glPixelStorei(g.GL_UNPACK_ALIGNMENT, 1)
            g.glTexImage2D(g.GL_TEXTURE_2D, 0, g.GL_RGBA, tex.shape[1], tex.shape[0], 0, g.GL_RGBA, g.GL_UNSIGNED_BYTE, np.ascontiguousarray(tex))
            uv = p["uv"] / np.array([p["tex"].shape[1], p["tex"].shape[0]], np.float32)
            self._texs.append(t)
            self._gparts.append((t, np.ascontiguousarray(p["pos"], np.float32), np.ascontiguousarray(uv, np.float32),
                                 np.ascontiguousarray(p["col"], np.float32), np.ascontiguousarray(p["idx"].reshape(-1), np.uint32), p["kind"]))
        self._glstate = self.mesh

    def _draw_gl(self, w, h):
        from OpenGL import GL as g
        if self._glstate is not self.mesh:
            if self.mesh is None:
                return
            self._upload(g)
        o = self.opts()
        g.glViewport(0, 0, w, h)
        g.glClearColor(o["bg"][0] / 255, o["bg"][1] / 255, o["bg"][2] / 255, 1)
        g.glClear(g.GL_COLOR_BUFFER_BIT | g.GL_DEPTH_BUFFER_BIT)
        g.glMatrixMode(g.GL_PROJECTION)
        g.glLoadMatrixd(np.ascontiguousarray(self.cam.proj(w / max(1, h)).T))
        g.glMatrixMode(g.GL_MODELVIEW)
        g.glLoadMatrixd(np.ascontiguousarray(self.cam.view().T))
        g.glPolygonMode(g.GL_FRONT_AND_BACK, g.GL_LINE if o["wire"] else g.GL_FILL)
        g.glEnableClientState(g.GL_VERTEX_ARRAY)
        for t, pos, uv, col, idx, kind in self._gparts:
            if kind == "sky" and (not o["sky"] or o["wire"]):
                continue
            tx = o["tex"] and not o["wire"]
            g.glBindTexture(g.GL_TEXTURE_2D, t)
            (g.glEnable if tx else g.glDisable)(g.GL_TEXTURE_2D)
            g.glVertexPointer(3, g.GL_FLOAT, 0, pos)
            if tx:
                g.glEnableClientState(g.GL_TEXTURE_COORD_ARRAY)
                g.glTexCoordPointer(2, g.GL_FLOAT, 0, uv)
            else:
                g.glDisableClientState(g.GL_TEXTURE_COORD_ARRAY)
            if o["vcol"] and not o["wire"]:
                g.glEnableClientState(g.GL_COLOR_ARRAY)
                g.glColorPointer(3, g.GL_FLOAT, 0, col)
            else:
                g.glDisableClientState(g.GL_COLOR_ARRAY)
                g.glColor3f(.75, .9, 1.0) if o["wire"] else g.glColor3f(1, 1, 1)
            g.glDrawElements(g.GL_TRIANGLES, len(idx), g.GL_UNSIGNED_INT, idx)
        if o["objects"] and self.mesh.objs:
            mk = MV.marker_part(self.mesh.objs, o["layers"], o["sel"])
            if len(mk["idx"]):
                g.glDisable(g.GL_TEXTURE_2D)
                g.glDisableClientState(g.GL_TEXTURE_COORD_ARRAY)
                g.glEnableClientState(g.GL_COLOR_ARRAY)
                g.glPolygonMode(g.GL_FRONT_AND_BACK, g.GL_FILL)
                g.glVertexPointer(3, g.GL_FLOAT, 0, np.ascontiguousarray(mk["pos"], np.float32))
                g.glColorPointer(3, g.GL_FLOAT, 0, np.ascontiguousarray(mk["col"], np.float32))
                g.glDrawElements(g.GL_TRIANGLES, mk["idx"].size, g.GL_UNSIGNED_INT, np.ascontiguousarray(mk["idx"].reshape(-1), np.uint32))
        g.glDisableClientState(g.GL_VERTEX_ARRAY)
        g.glDisableClientState(g.GL_TEXTURE_COORD_ARRAY)
        g.glDisableClientState(g.GL_COLOR_ARRAY)

    # ---- output
    def export_glb(self):
        if not self.mesh:
            return
        try:
            data = self.mesh.glb(self.v["objects"].get(), self._layers())
        except Exception as e:
            messagebox.showerror(tr("error"), str(e))
            return
        if not data:
            messagebox.showinfo(tr("info"), tr("m.noglb"))
            return
        p = filedialog.asksaveasfilename(parent=self, defaultextension=".glb", initialfile=self.mesh.name.replace(":", "_").replace(" ", "") + ".glb",
                                         filetypes=[("glTF binary", "*.glb")])
        if p:
            open(p, "wb").write(data)
            self.say(tr("m.savedglb", p))

    def screenshot(self):
        """PNG of the current view (always from the software renderer, at the size of the viewport)."""
        if not self.mesh:
            return
        from PIL import Image
        p = filedialog.asksaveasfilename(parent=self, defaultextension=".png", initialfile=self.mesh.name.replace(":", "_").replace(" ", "") + ".png",
                                         filetypes=[("PNG", "*.png")])
        if p:
            w, h = max(8, self.surface.winfo_width()), max(8, self.surface.winfo_height())
            Image.fromarray(MV.render(self.mesh, self.cam, w, h, self.opts())).save(p)
            self.say(tr("m.savedshot", p))
