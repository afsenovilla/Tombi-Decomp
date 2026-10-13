#!/usr/bin/env python3
"""Extracts every graphic of Tombi! (PAL) as PNG straight from a disc image, with no dependencies beyond
Python 3 (Windows/Linux/macOS).

  python3 tools/extract_sprites.py Tombi.bin OUT_DIR      (command line)
  python3 tools/extract_sprites.py Tombi.bin OUT_DIR --only sprites,rooms3d    (only some steps)
  python3 tools/extract_sprites.py                         (no arguments: a small window to pick the image, folder and what to extract)

Steps for --only (comma separated, default all; each one makes its own prerequisites, the fs dump is reused if OUT_DIR/fs exists):
  files sprites screens vram rooms rooms3d banks music movie_audio videos      (--video-frames: videos also dumps every frame as PNG)

Steps: the ISO9660 file system of the .bin is extracted to OUT_DIR/fs (tools/isoext.py), then
  OUT_DIR/sprites/<AREA>/<file>/frame_NNNN.png   every WPP sprite frame (AREAxx/*.000 of load kind 0x60/0x62, 40 files, about 13 000
                                                 frames, tools/wpp.py): indexed PNG, index 0 transparent, each frame with the palette the
                                                 game data assigns it (Tombi's palette where the game sets it at run time)
  OUT_DIR/spr/<AREA|SYSTEM>/listNN/sprKK/        every SPR frame definition (load kinds 0x30..0x3F) cut out of the simulated VRAM:
                                                 frame_NNNN_partK.png (one POLY_FT4 part, own palette), frame_NNNN.png (parts composed),
                                                 manifest.json (offsets, palettes, which atlas) (tools/vram.py)
  OUT_DIR/screens/*.png                          the static full screens at their real size and palette: title landscape (320 x 240), Whoopee
                                                 Camp logo, "Sony Computer Entertainment Europe presents" (640 x 512), TOMBI! logo page, menu atlases,
                                                 world map texture, title font sheet (tools/screens.py)
  OUT_DIR/vram/<AREA|SYSTEM>/listNN/             every load list replayed into VRAM: vram.png (1024 x 512, 16 bit view) and every GRPX page
                                                 (room pages, atlases, ...) with its palette, palette blocks as swatches
  OUT_DIR/sound/bank/<SNDxxxx>/                  every VAB sample of the 52 .WVD banks as WAV (+ vab.json with the tone table) (tools/audio.py)
  OUT_DIR/sound/music/*.mid                      the 36 .SEQ sequences as standard MIDI (type 0)
  OUT_DIR/sound/movie_audio/*.wav                the XA ADPCM audio track of each of the 30 .STR movies (37.8 kHz stereo)
  OUT_DIR/video/NAME.png, NAME.mp4               poster frame of each movie (pure Python MDEC, tools/str.py); mp4 only if ffmpeg is in PATH
                                                 (all frames as PNG: python3 tools/str.py frames Tombi.bin NAME OUT_DIR)
The retail disc is only read.  Windows users: dist/windows/tombi-assets.exe (built by .github/workflows/assets-exe.yml) does the same
with no Python:
  double-click it (window), or  tombi-assets.exe Tombi.bin OUT_DIR"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
BUNDLE = getattr(sys, "_MEIPASS", HERE)   # PyInstaller one-file build: the data files live in a temp dir


def run_isoext(img, fs):
    """tools/isoext.py as a module (it reads sys.argv), so it works inside the frozen exe too."""
    src = open(os.path.join(BUNDLE, "isoext.py")).read()
    argv, sys.argv = sys.argv, ["isoext.py", img, fs]
    try:
        exec(compile(src, "isoext.py", "exec"), {"__name__": "__main__", "__file__": os.path.join(BUNDLE, "isoext.py")})
    finally:
        sys.argv = argv


STEPS = [   # (key, group, label es, description)
    ("files", "Archivos", "Archivos del disco (fs)", "file system of the disc image under OUT/fs"),
    ("sprites", "Imagenes", "Sprites", "WPP sprites (OUT/sprites) + SPR frames (OUT/spr)"),
    ("screens", "Imagenes", "Pantallas", "title, logos, menu atlases (OUT/screens)"),
    ("vram", "Imagenes", "Paginas VRAM", "raw VRAM snapshots and pages (OUT/vram)"),
    ("rooms", "3D / Salas", "Salas PNG", "room backgrounds (OUT/rooms)"),
    ("rooms3d", "3D / Salas", "Salas 3D (glb)", "room models as glTF (OUT/rooms3d)"),
    ("banks", "Audio", "Bancos de sonido (WAV)", "VAB samples (OUT/sound/bank)"),
    ("music", "Audio", "Musica (MIDI)", "SEQ sequences (OUT/sound/music)"),
    ("movie_audio", "Audio", "Audio de peliculas (WAV)", "XA tracks (OUT/sound/movie_audio)"),
    ("videos", "Video", "Videos (poster/mp4)", "posters + mp4 if ffmpeg (OUT/video)"),
]
STEP_KEYS = [k for k, _, _, _ in STEPS]


class Extractor:
    """Independent steps; each one computes (and caches) its own prerequisites: the fs dump is skipped when OUT/fs already exists, the
    SPR palette usage (needed by the VRAM page colouring) is computed once per instance."""

    def __init__(self, img, out, log=print, video_frames=False):
        self.img, self.out, self.log = os.path.abspath(img), os.path.abspath(out), log
        self.fs = os.path.join(self.out, "fs")
        self.video_frames = video_frames
        self._env = False
        self._usage = None      # (stats, usage) of vram.export_spr
        self._stats = None

    def _setup(self):
        """fs dump (once) and the module environment (TOMBI_FS / TOMBI_IMG, area000.FS)."""
        if not os.path.isdir(os.path.join(self.fs, "AREA00")):
            self.log("extracting the file system of %s ..." % self.img)
            run_isoext(self.img, self.fs)
        if not self._env:
            os.environ["TOMBI_FS"] = self.fs
            os.environ["TOMBI_IMG"] = self.img
            if HERE not in sys.path: sys.path.insert(0, HERE)
            import area000 as A
            A.FS = self.fs
            if hasattr(A, "IMG"): A.IMG = self.img
            self._env = True

    def files(self):
        self._setup()
        self.log("done: file system under %s" % self.fs)

    def sprites(self):
        self._setup()
        import area000 as A, wpp
        n = 0
        for area in sorted(os.listdir(self.fs)):
            d = os.path.join(self.fs, area)
            if not os.path.isdir(d): continue
            for f in sorted(os.listdir(d)):
                if not f.endswith(".000"): continue
                path = os.path.join(d, f)
                if not wpp.is_wpp(A.rel_name(path)): continue
                class Args: pass
                a = Args(); a.file, a.out, a.clut, a.row, a.x0, a.auto = path, os.path.join(self.out, "sprites", area, f[:-4]), None, 0, 0, True
                wpp.cmd_unpack(a); n += 1
        self.log("done: %d sprite files under %s" % (n, os.path.join(self.out, "sprites")))
        self.log("simulating the VRAM of every load list (SPR atlas cuts) ...")
        self._spr(self.out)
        st = self._stats
        self.log("done: %d SPR tables, %d frames / %d parts (%d identical tables shared) under %s" % (
            st["tables"], st["frames_total"], st["parts_total"], st["tables_deduplicated"], os.path.join(self.out, "spr")))

    def _spr(self, dst):
        import vram
        if not vram.A.load_lists():
            raise RuntimeError("the load lists (SYS/LD0*.BIN) and the disc image are needed")
        self._stats, usage = vram.export_spr(dst, log=lambda m: None)
        self._usage = usage

    def screens(self):
        self._setup()
        import screens
        n = screens.export_screens(os.path.join(self.out, "screens"))
        self.log("static full screens: %d under %s" % (n, os.path.join(self.out, "screens")))

    def vram(self):
        self._setup()
        import vram, tempfile, shutil
        if self._usage is None:   # page colouring needs the palettes the SPR parts use: replay the SPR cuts into a scratch dir
            self.log("computing SPR palette usage (scratch run, not kept) ...")
            tmp = tempfile.mkdtemp(prefix="tombi_spr_")
            try: self._spr(tmp)
            finally: shutil.rmtree(tmp, ignore_errors=True)
        vram.export_screens(self.out, self._usage, self._stats, log=lambda m: None)
        self.log("done: VRAM snapshots and pages under %s" % os.path.join(self.out, "vram"))

    def rooms(self):
        self._setup()
        import room
        self.log("rendering the room backgrounds ...")
        self.log("done: %d rooms under %s" % (room.export_rooms(os.path.join(self.out, "rooms"), log=lambda m: None), os.path.join(self.out, "rooms")))

    def rooms3d(self):
        self._setup()
        import room
        self.log("exporting the room models as 3D (glTF .glb) ...")
        self.log("done: %d models under %s" % (room.export_glb(os.path.join(self.out, "rooms3d"), log=lambda m: None), os.path.join(self.out, "rooms3d")))

    def _audio(self, **kw):
        self._setup()
        import audio
        kw = dict(dict(banks=False, music=False, xa=False), **kw)
        audio.export_all(self.img, self.out, log=self.log, **kw)

    def banks(self): self._audio(banks=True)
    def music(self): self._audio(music=True)
    def movie_audio(self): self._audio(xa=True)

    def videos(self):
        self._setup()
        import importlib
        strmod = importlib.import_module("str")
        self.log("video: poster frame of every movie and, if ffmpeg is in PATH, mp4 ...")
        strmod.export_all(self.img, self.out, log=self.log)
        if self.video_frames:
            for p, (lba, size) in sorted(strmod.str_files(self.img).items()):
                name = os.path.basename(p)[:-4]; d = os.path.join(self.out, "video", name + "_frames"); os.makedirs(d, exist_ok=True)
                for num, w, h, bs in strmod.frames(self.img, lba, size):
                    strmod.write_png(os.path.join(d, "frame_%05d.png" % num), w, h, strmod.decode_frame(bs, w, h))
                self.log("video: all frames of %s under %s" % (name, d))

    def run(self, steps=None):
        for k in (steps or STEP_KEYS):
            if k not in STEP_KEYS: raise ValueError("unknown step %r (valid: %s)" % (k, ", ".join(STEP_KEYS)))
            getattr(self, k)()


def extract(img, out, log=print, only=None, video_frames=False):
    """Runs the chosen steps (names in STEP_KEYS, default: all) under `out`."""
    Extractor(img, out, log, video_frames).run(only)


def gui():
    """Small window (tkinter, standard library): pick the disc image and the output folder, tick what to extract."""
    import threading, queue
    import tkinter as tk
    from tkinter import filedialog, messagebox, ttk
    root = tk.Tk()
    root.title("Tombi! asset extractor")
    root.resizable(True, True)
    img_v, out_v = tk.StringVar(), tk.StringVar()
    frm = ttk.Frame(root, padding=12); frm.grid(sticky="nsew")
    root.columnconfigure(0, weight=1); root.rowconfigure(0, weight=1)
    frm.columnconfigure(1, weight=1); frm.rowconfigure(5, weight=1)
    ttk.Label(frm, text="Imagen de disco (.bin):").grid(row=0, column=0, sticky="w", pady=3)
    ttk.Entry(frm, textvariable=img_v, width=60).grid(row=0, column=1, sticky="ew", padx=6)
    ttk.Button(frm, text="Buscar...", command=lambda: img_v.set(filedialog.askopenfilename(
        title="Tombi! disc image", filetypes=[("Disc image", "*.bin *.img *.iso"), ("All files", "*.*")]) or img_v.get())).grid(row=0, column=2)
    ttk.Label(frm, text="Carpeta de salida:").grid(row=1, column=0, sticky="w", pady=3)
    ttk.Entry(frm, textvariable=out_v, width=60).grid(row=1, column=1, sticky="ew", padx=6)
    ttk.Button(frm, text="Buscar...", command=lambda: out_v.set(filedialog.askdirectory(title="Output folder") or out_v.get())).grid(row=1, column=2)

    # grouped checkboxes with select all / none per group
    boxes = ttk.Frame(frm); boxes.grid(row=2, column=0, columnspan=3, sticky="ew", pady=6)
    vars_ = {}
    groups = []
    for k, g, label, _ in STEPS:
        if g not in groups: groups.append(g)
    for gi, g in enumerate(groups):
        lf = ttk.LabelFrame(boxes, text=g, padding=6); lf.grid(row=0, column=gi, sticky="nsew", padx=4)
        keys = [k for k, gg, _, _ in STEPS if gg == g]
        for k, gg, label, _ in STEPS:
            if gg != g: continue
            vars_[k] = tk.BooleanVar(value=True)
            ttk.Checkbutton(lf, text=label, variable=vars_[k]).pack(anchor="w")
        row = ttk.Frame(lf); row.pack(anchor="w", pady=(4, 0))
        ttk.Button(row, text="Todo", width=6, command=lambda ks=keys: [vars_[k].set(True) for k in ks]).pack(side="left")
        ttk.Button(row, text="Nada", width=6, command=lambda ks=keys: [vars_[k].set(False) for k in ks]).pack(side="left", padx=2)
    frames_v = tk.BooleanVar(value=False)
    ttk.Checkbutton(frm, text="Videos: volcar tambien todos los fotogramas PNG (lento, mucho espacio)", variable=frames_v).grid(row=3, column=0, columnspan=3, sticky="w")

    bf = ttk.Frame(frm); bf.grid(row=4, column=0, columnspan=3, pady=8)
    btn_sel = ttk.Button(bf, text="Extraer seleccionado"); btn_sel.pack(side="left", padx=6)
    btn_all = ttk.Button(bf, text="Extraer todo"); btn_all.pack(side="left", padx=6)
    log = tk.Text(frm, height=14, width=90, state="disabled", wrap="word")
    log.grid(row=5, column=0, columnspan=3, sticky="nsew")
    sb = ttk.Scrollbar(frm, command=log.yview); sb.grid(row=5, column=3, sticky="ns"); log["yscrollcommand"] = sb.set
    q = queue.Queue()

    def put(msg):
        q.put(str(msg))

    def pump():
        try:
            while True:
                m = q.get_nowait()
                log.configure(state="normal"); log.insert("end", m + "\n"); log.see("end"); log.configure(state="disabled")
                if m.startswith("__END__"):
                    btn_sel.configure(state="normal"); btn_all.configure(state="normal")
                    if m != "__END__ok": messagebox.showerror("Extraction failed", m[8:])
                    else: messagebox.showinfo("Listo", "Assets en\n" + out_v.get())
        except queue.Empty:
            pass
        root.after(100, pump)

    def work(img, out, steps, vf):
        try:
            ex = Extractor(img, out, log=put, video_frames=vf)
            for i, k in enumerate(steps, 1):
                put("[%d/%d] %s" % (i, len(steps), next(l for kk, _, l, _ in STEPS if kk == k)))
                getattr(ex, k)()
            put("__END__ok")
        except Exception as e:
            import traceback; put(traceback.format_exc()); put("__END__: " + str(e))

    def start(everything):
        img, out = img_v.get().strip(), out_v.get().strip()
        steps = STEP_KEYS if everything else [k for k in STEP_KEYS if vars_[k].get()]
        if not os.path.isfile(img): messagebox.showwarning("Falta", "Elige la imagen de disco (.bin)."); return
        if not out: messagebox.showwarning("Falta", "Elige una carpeta de salida."); return
        if not steps: messagebox.showwarning("Falta", "Marca al menos una casilla."); return
        btn_sel.configure(state="disabled"); btn_all.configure(state="disabled")
        put("Extrayendo %s -> %s: %s" % (img, out, ", ".join(steps)))
        threading.Thread(target=work, args=(img, out, steps, frames_v.get()), daemon=True).start()

    btn_sel.configure(command=lambda: start(False))
    btn_all.configure(command=lambda: start(True))
    root.after(100, pump)
    root.mainloop()


def main():
    if len(sys.argv) == 1:
        try:
            return gui()
        except Exception as e:   # no tkinter or no display: fall back to the usage text
            sys.exit("%s\n(no window: %s)" % (__doc__, e))
    args, only, frames = [], None, False
    it = iter(sys.argv[1:])
    for x in it:
        if x == "--only": only = next(it, "").split(",")
        elif x.startswith("--only="): only = x[7:].split(",")
        elif x == "--video-frames": frames = True
        else: args.append(x)
    if len(args) != 2 or (only and any(k not in STEP_KEYS for k in only)):
        sys.exit(__doc__)
    extract(args[0], args[1], only=only, video_frames=frames)


if __name__ == "__main__":
    main()
