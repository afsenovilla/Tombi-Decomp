#!/usr/bin/env python3
"""Extracts every graphic of Tombi! (PAL) as PNG straight from a disc image, with no dependencies beyond
Python 3 (Windows/Linux/macOS).

  python3 tools/extract_sprites.py Tombi.bin OUT_DIR      (command line)
  python3 tools/extract_sprites.py Tombi.bin OUT_DIR --only sprites,rooms3d    (only some steps)
  python3 tools/extract_sprites.py                         (no arguments: a small window to pick the image, folder and what to extract)

Steps for --only (comma separated, default all; each one makes its own prerequisites, the fs dump is reused if OUT_DIR/fs exists):
  files sprites animations screens vram skies rooms rooms3d banks music music_audio movie_audio videos
  (--video-frames: videos also dumps every frame as PNG;  --audio-format wav|mp3: format of banks, music_audio and movie_audio, mp3 needs lameenc,
   falls back to WAV; loop points only in WAV)

Steps: the ISO9660 file system of the .bin is extracted to OUT_DIR/fs (tools/isoext.py), then
  OUT_DIR/sprites/<AREA>/<file>/frame_NNNN.png   every WPP sprite frame (AREAxx/*.000 of load kind 0x60/0x62, 40 files, about 13 000
                                                 frames, tools/wpp.py): indexed PNG, index 0 transparent, each frame with the palette the
                                                 game data assigns it (Tombi's palette where the game sets it at run time)
  OUT_DIR/spr/<AREA|SYSTEM>/listNN/sprKK/        every SPR frame definition (load kinds 0x30..0x3F) cut out of the simulated VRAM:
                                                 frame_NNNN_partK.png (one POLY_FT4 part, own palette), frame_NNNN.png (parts composed),
                                                 manifest.json (offsets, palettes, which atlas) (tools/vram.py)
  OUT_DIR/anim/<owner>/NN[_name].gif|png         Tombi's 82 animation scripts (idle, walk, run, jump...: anim/tombi/NN_<name>) and the scripts of every
  OUT_DIR/anim/catalog.json                      enemy / NPC / object handler (anim/aNN_<handler>/NN) as animated GIF + PNG strip, and the decoded
                                                 catalogue (owner, handler, area, script, frames, durations, loop) (tools/anim.py, tools/animobj.py)
  OUT_DIR/screens/*.png                          the static full screens at their real size and palette: title landscape (320 x 240), Whoopee
                                                 Camp logo, "Sony Computer Entertainment Europe presents" (640 x 512), TOMBI! logo page, menu atlases,
                                                 world map texture, title font sheet (tools/screens.py)
  OUT_DIR/vram/<AREA|SYSTEM>/listNN/             every load list replayed into VRAM: vram.png (1024 x 512, 16 bit view) and every GRPX page
                                                 (room pages, atlases, ...) with its palette, palette blocks as swatches
  OUT_DIR/sound/bank/<SNDxxxx>/                  every VAB sample of the 52 .WVD banks as WAV (+ vab.json with the tone table) (tools/audio.py)
  OUT_DIR/sound/music/*.mid                      the 36 .SEQ sequences as standard MIDI (type 0)
  OUT_DIR/sound/music_audio/*.wav|mp3            every SEQ rendered to stereo 44.1 kHz audio with its own bank (software sequencer in tools/audio.py, needs numpy)
  OUT_DIR/sound/movie_audio/*.wav                the XA ADPCM audio track of each of the 30 .STR movies (37.8 kHz stereo)
  OUT_DIR/video/NAME.avi, NAME.mp4               each movie (pure Python MDEC, tools/str.py); mp4 only if ffmpeg is in PATH
                                                 (all frames as PNG: python3 tools/str.py frames Tombi.bin NAME OUT_DIR)
The retail disc is only read.  Windows users: dist/windows/tombi-assets.exe (built by .github/workflows/assets-exe.yml) does the same
with no Python:
  double-click it (window), or  tombi-assets.exe Tombi.bin OUT_DIR"""
import os, sys

HERE = os.path.dirname(os.path.abspath(__file__))
if HERE not in sys.path:
    sys.path.insert(0, HERE)
import i18n, extract_strings
from i18n import tr
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
    ("files", "files", "Archivos del disco (fs)", "file system of the disc image under OUT/fs"),
    ("sprites", "images", "Sprites", "WPP sprites (OUT/sprites) + SPR frames (OUT/spr)"),
    ("animations", "images", "Animaciones", "animations of Tombi, enemies, NPCs and objects as GIF + catalog.json (OUT/anim/<owner>/)"),
    ("screens", "images", "Pantallas", "title, logos, menu atlases (OUT/screens)"),
    ("vram", "images", "Paginas VRAM", "raw VRAM snapshots and pages (OUT/vram)"),
    ("skies", "rooms", "Cielos de las salas", "sky layers captured with the MIPS interpreter, needs numpy + Pillow, slow (OUT/skies)"),
    ("rooms", "rooms", "Salas PNG", "room backgrounds (OUT/rooms)"),
    ("rooms3d", "rooms", "Salas 3D (glb)", "room models as glTF (OUT/rooms3d)"),
    ("banks", "audio", "Bancos de sonido", "VAB samples (OUT/sound/bank)"),
    ("music", "audio", "Musica (MIDI)", "SEQ sequences (OUT/sound/music)"),
    ("music_audio", "audio", "Musica renderizada", "SEQ + bank rendered to audio (OUT/sound/music_audio)"),
    ("movie_audio", "audio", "Audio de peliculas", "XA tracks (OUT/sound/movie_audio)"),
    ("videos", "video", "Videos (AVI)", "AVI MJPEG+PCM (needs Pillow), mp4 if ffmpeg (OUT/video)"),
]
STEP_KEYS = [k for k, _, _, _ in STEPS]


class Extractor:
    """Independent steps; each one computes (and caches) its own prerequisites: the fs dump is skipped when OUT/fs already exists, the
    SPR palette usage (needed by the VRAM page colouring) is computed once per instance."""

    def __init__(self, img, out, log=print, video_frames=False, audio_format="wav"):
        self.img, self.out, self.log = os.path.abspath(img), os.path.abspath(out), log
        self.fs = os.path.join(self.out, "fs")
        self.video_frames = video_frames
        self.audio_format = audio_format
        self._env = False
        self._usage = None      # (stats, usage) of vram.export_spr
        self._stats = None

    def _setup(self):
        """fs dump (once) and the module environment (TOMBI_FS / TOMBI_IMG, area000.FS)."""
        if not os.path.isdir(os.path.join(self.fs, "AREA00")):
            self.log(tr("ex.extractfs", self.img))
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
        self.log(tr("ex.donefs", self.fs))

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
        self.log(tr("ex.donesprites", n, os.path.join(self.out, "sprites")))
        self.log(tr("ex.simvram"))
        self._spr(self.out)
        st = self._stats
        self.log(tr("ex.donespr",
            st["tables"], st["frames_total"], st["parts_total"], st["tables_deduplicated"], os.path.join(self.out, "spr")))

    def _spr(self, dst):
        import vram
        if not vram.A.load_lists():
            raise RuntimeError("the load lists (SYS/LD0*.BIN) and the disc image are needed")
        self._stats, usage = vram.export_spr(dst, log=lambda m: None)
        self._usage = usage

    def animations(self):
        self._setup()
        import anim
        d = os.path.join(self.out, "anim")
        n = anim.export(d, log=lambda m: None)
        self.log(tr("ex.animations", n, d))

    def screens(self):
        self._setup()
        import screens
        n = screens.export_screens(os.path.join(self.out, "screens"))
        self.log(tr("ex.screens", n, os.path.join(self.out, "screens")))

    def vram(self):
        self._setup()
        import vram, tempfile, shutil
        if self._usage is None:   # page colouring needs the palettes the SPR parts use: replay the SPR cuts into a scratch dir
            self.log(tr("ex.usage"))
            tmp = tempfile.mkdtemp(prefix="tombi_spr_")
            try: self._spr(tmp)
            finally: shutil.rmtree(tmp, ignore_errors=True)
        vram.export_screens(self.out, self._usage, self._stats, log=lambda m: None)
        self.log(tr("ex.donevram", os.path.join(self.out, "vram")))

    def skies(self):
        """Runs the game in tools/psxemu for every room (~15 s each) and keeps room_<AA>_<n>_sky.png under OUT/skies; rooms / rooms3d then use them."""
        self._setup()
        import room, gfx_composed
        self.log(tr("ex.skies"))
        exe = os.path.join(self.fs, "EXE", "MAIN0.EXE")
        try:
            import numpy  # noqa: F401
            from psxemu import capture
        except ImportError as e:
            self.log(tr("ex.skiesnp", e))
            return
        todo = [(a, s) for a, n in gfx_composed.rooms() for s in range(n) if not gfx_composed.unreachable(a, s)]
        dst = os.path.join(self.out, "skies")
        os.makedirs(dst, exist_ok=True)
        done = [0]
        def prog(i, n): self.log(tr("ex.skiesprog", i, n))
        res = capture.rooms(dst, exe=exe, disc=self.img, log=lambda m: None, progress=prog)
        for a, s, st, info in res:      # a room without sky leaves no file: rooms/rooms3d simply have no backdrop there
            if st == "fail": self.log(tr("ex.skiesfail", a, s, info))
        room.SKY_DIR = dst
        self.log(tr("ex.doneskies", sum(1 for f in os.listdir(dst) if f.endswith("_sky.png")), dst))

    def _sky_dir(self):
        """Folder with sky captures for rooms / rooms3d: OUT/skies when the step ran, else the capture cache of the disc image (may not exist)."""
        import room
        d = os.path.join(self.out, "skies")
        room.SKY_DIR = d if os.path.isdir(d) else None

    def rooms(self):
        self._setup()
        import room
        self._sky_dir()
        self.log(tr("ex.rooms"))
        self.log(tr("ex.donerooms", room.export_rooms(os.path.join(self.out, "rooms"), log=lambda m: None), os.path.join(self.out, "rooms")))

    def rooms3d(self):
        self._setup()
        import room
        self._sky_dir()
        self.log(tr("ex.rooms3d"))
        self.log(tr("ex.donerooms3d", room.export_glb(os.path.join(self.out, "rooms3d"), log=lambda m: None), os.path.join(self.out, "rooms3d")))

    def _audio(self, **kw):
        self._setup()
        import audio
        kw = dict(dict(banks=False, music=False, xa=False, fmt=self.audio_format), **kw)
        audio.export_all(self.img, self.out, log=self.log, **kw)

    def banks(self): self._audio(banks=True)
    def music(self): self._audio(music=True)
    def music_audio(self): self._audio(music_audio=True)
    def movie_audio(self): self._audio(xa=True)

    def videos(self):
        self._setup()
        import importlib
        strmod = importlib.import_module("str")
        self.log(tr("ex.video"))
        strmod.export_all(self.img, self.out, log=self.log)
        if self.video_frames:
            for p, (lba, size) in sorted(strmod.str_files(self.img).items()):
                name = os.path.basename(p)[:-4]; d = os.path.join(self.out, "video", name + "_frames"); os.makedirs(d, exist_ok=True)
                for num, w, h, bs in strmod.frames(self.img, lba, size):
                    strmod.write_png(os.path.join(d, "frame_%05d.png" % num), w, h, strmod.decode_frame(bs, w, h))
                self.log(tr("ex.videoframes", name, d))

    def run(self, steps=None):
        for k in (steps or STEP_KEYS):
            if k not in STEP_KEYS: raise ValueError("unknown step %r (valid: %s)" % (k, ", ".join(STEP_KEYS)))
            getattr(self, k)()


def extract(img, out, log=print, only=None, video_frames=False, audio_format="wav"):
    """Runs the chosen steps (names in STEP_KEYS, default: all) under `out`."""
    Extractor(img, out, log, video_frames, audio_format).run(only)


def gui():
    """Small window (tkinter, standard library): pick the disc image and the output folder, tick what to extract.  UI language: tools/i18n.py."""
    import threading, queue
    import tkinter as tk
    from tkinter import filedialog, messagebox, ttk
    root = tk.Tk()
    for d in (os.path.join(getattr(sys, "_MEIPASS", ""), "icons"), os.path.join(os.path.dirname(os.path.abspath(__file__)), "icons")):
        try:
            if os.name == "nt" and os.path.isfile(os.path.join(d, "assets.ico")):
                root.iconbitmap(os.path.join(d, "assets.ico"))
                break
            if os.path.isfile(os.path.join(d, "assets.png")):
                root._icon_img = tk.PhotoImage(file=os.path.join(d, "assets.png"))
                root.iconphoto(True, root._icon_img)
                break
        except Exception:
            pass
    root.resizable(True, True)
    img_v, out_v = tk.StringVar(), tk.StringVar()
    frames_v, fmt_v = tk.BooleanVar(value=False), tk.StringVar(value="WAV")
    vars_ = {k: tk.BooleanVar(value=True) for k in STEP_KEYS}
    q = queue.Queue()
    st = {"busy": False, "log": [], "btns": []}
    groups = []
    for k, g, label, _ in STEPS:
        if g not in groups: groups.append(g)

    def put(msg):
        q.put(str(msg))

    def build():
        for w in root.winfo_children(): w.destroy()
        root.title(tr("ex.title"))
        frm = ttk.Frame(root, padding=12); frm.grid(sticky="nsew")
        root.columnconfigure(0, weight=1); root.rowconfigure(0, weight=1)
        frm.columnconfigure(1, weight=1); frm.rowconfigure(6, weight=1)
        i18n.selector(frm, build).grid(row=0, column=0, columnspan=3, sticky="e")
        ttk.Label(frm, text=tr("ex.image")).grid(row=1, column=0, sticky="w", pady=3)
        ttk.Entry(frm, textvariable=img_v, width=60).grid(row=1, column=1, sticky="ew", padx=6)
        ttk.Button(frm, text=tr("ex.browse"), command=lambda: img_v.set(filedialog.askopenfilename(
            title=tr("ex.imagedlg"), filetypes=[(tr("ex.discimg"), "*.bin *.img *.iso"), (tr("ex.allfiles"), "*.*")]) or img_v.get())).grid(row=1, column=2)
        ttk.Label(frm, text=tr("ex.outdir")).grid(row=2, column=0, sticky="w", pady=3)
        ttk.Entry(frm, textvariable=out_v, width=60).grid(row=2, column=1, sticky="ew", padx=6)
        ttk.Button(frm, text=tr("ex.browse"), command=lambda: out_v.set(filedialog.askdirectory(title=tr("ex.outdlg")) or out_v.get())).grid(row=2, column=2)
        # grouped checkboxes with select all / none per group
        boxes = ttk.Frame(frm); boxes.grid(row=3, column=0, columnspan=3, sticky="ew", pady=6)
        for gi, g in enumerate(groups):
            lf = ttk.LabelFrame(boxes, text=tr("sg." + g), padding=6); lf.grid(row=0, column=gi, sticky="nsew", padx=4)
            keys = [k for k, gg, _, _ in STEPS if gg == g]
            for k in keys:
                ttk.Checkbutton(lf, text=tr("st." + k), variable=vars_[k]).pack(anchor="w")
            row = ttk.Frame(lf); row.pack(anchor="w", pady=(4, 0))
            ttk.Button(row, text=tr("ex.all"), width=6, command=lambda ks=keys: [vars_[k].set(True) for k in ks]).pack(side="left")
            ttk.Button(row, text=tr("ex.none"), width=6, command=lambda ks=keys: [vars_[k].set(False) for k in ks]).pack(side="left", padx=2)
        ttk.Checkbutton(frm, text=tr("ex.frames"), variable=frames_v).grid(row=4, column=0, columnspan=2, sticky="w")
        ff = ttk.Frame(frm); ff.grid(row=4, column=2, sticky="e")
        ttk.Label(ff, text=tr("ex.audiofmt")).pack(side="left")
        ttk.Combobox(ff, textvariable=fmt_v, values=("WAV", "MP3"), width=6, state="readonly").pack(side="left", padx=4)
        bf = ttk.Frame(frm); bf.grid(row=5, column=0, columnspan=3, pady=8)
        btn_sel = ttk.Button(bf, text=tr("ex.extractsel"), command=lambda: start(False)); btn_sel.pack(side="left", padx=6)
        btn_all = ttk.Button(bf, text=tr("ex.extractall"), command=lambda: start(True)); btn_all.pack(side="left", padx=6)
        st["btns"] = [btn_sel, btn_all]
        for b in st["btns"]: b.configure(state="disabled" if st["busy"] else "normal")
        log = tk.Text(frm, height=14, width=90, state="disabled", wrap="word")
        log.grid(row=6, column=0, columnspan=3, sticky="nsew")
        sb = ttk.Scrollbar(frm, command=log.yview); sb.grid(row=6, column=3, sticky="ns"); log["yscrollcommand"] = sb.set
        st["logw"] = log
        log.configure(state="normal"); log.insert("end", "".join(m + "\n" for m in st["log"])); log.see("end"); log.configure(state="disabled")

    def pump():
        try:
            while True:
                m = q.get_nowait()
                if not m.startswith("__END__"):
                    st["log"].append(m)
                    log = st["logw"]
                    log.configure(state="normal"); log.insert("end", m + "\n"); log.see("end"); log.configure(state="disabled")
                else:
                    st["busy"] = False
                    for b in st["btns"]: b.configure(state="normal")
                    if m != "__END__ok": messagebox.showerror(tr("ex.failed"), m[8:])
                    else: messagebox.showinfo(tr("ex.done"), tr("ex.doneat", out_v.get()))
        except queue.Empty:
            pass
        root.after(100, pump)

    def work(img, out, steps, vf, af):
        try:
            ex = Extractor(img, out, log=put, video_frames=vf, audio_format=af)
            for i, k in enumerate(steps, 1):
                put("[%d/%d] %s" % (i, len(steps), tr("st." + k)))
                getattr(ex, k)()
            put("__END__ok")
        except Exception as e:
            import traceback; put(traceback.format_exc()); put("__END__: " + str(e))

    def start(everything):
        img, out = img_v.get().strip(), out_v.get().strip()
        steps = STEP_KEYS if everything else [k for k in STEP_KEYS if vars_[k].get()]
        if not os.path.isfile(img): messagebox.showwarning(tr("ex.missing"), tr("ex.pickimage")); return
        if not out: messagebox.showwarning(tr("ex.missing"), tr("ex.pickout")); return
        if not steps: messagebox.showwarning(tr("ex.missing"), tr("ex.pickstep")); return
        st["busy"] = True
        for b in st["btns"]: b.configure(state="disabled")
        put(tr("ex.extracting", img, out, ", ".join(steps)))
        threading.Thread(target=work, args=(img, out, steps, frames_v.get(), fmt_v.get().lower()), daemon=True).start()

    build()
    root.after(100, pump)
    root.mainloop()


def main():
    if len(sys.argv) == 1:
        try:
            return gui()
        except Exception as e:   # no tkinter or no display: fall back to the usage text
            sys.exit("%s\n(no window: %s)" % (__doc__, e))
    args, only, frames, afmt = [], None, False, "wav"
    it = iter(sys.argv[1:])
    for x in it:
        if x == "--only": only = next(it, "").split(",")
        elif x.startswith("--only="): only = x[7:].split(",")
        elif x == "--video-frames": frames = True
        elif x == "--audio-format": afmt = next(it, "")
        elif x.startswith("--audio-format="): afmt = x.split("=", 1)[1]
        else: args.append(x)
    if len(args) != 2 or (only and any(k not in STEP_KEYS for k in only)) or afmt not in ("wav", "mp3"):
        sys.exit(__doc__)
    extract(args[0], args[1], only=only, video_frames=frames, audio_format=afmt)


if __name__ == "__main__":
    main()
