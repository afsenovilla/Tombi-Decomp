#!/usr/bin/env python3
"""Launcher of the native-pointer port (tombi-port.exe, docs/PORT.md "M3 phase 3" / "Mods"): a window where the
player picks their own disc image (.bin/.cue/.iso of Tombi! PAL SCES-01331), a save folder, the window options and
the mods, and a Play button that runs tombi2.exe --window with those options.  Same Tk toolkit and tools/i18n.py
catalogs as tombi-gfx / tombi-text.  No game data is bundled: the disc is checked (MAIN0.EXE extracted next to
tombi-port.json, the saved settings, and compared by sha1) before the first run.

  python3 tools/port_launcher.py             # from a checkout: uses build/port2-win/tombi2.exe (Windows) or build/port2/tombi2 (Linux)
  pyinstaller --onefile --windowed --name tombi-port ... tools/port_launcher.py   (.github/workflows/port2-exe.yml)
"""
import hashlib, json, os, struct, subprocess, sys, threading
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

import i18n
from i18n import tr
import port_mods as M

MAIN0_SHA1 = "bec8dc5f4116f5e497f2337eef8c523ff6dee8fc"     # EXE/MAIN0.EXE of SCES-01331 (.github/workflows/report.yml)
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

i18n.add({
    "es": {
        "title": "Tombi! - port nativo", "tab.play": "Jugar", "tab.mods": "Mods", "tab.controls": "Controles",
        "l.disc": "Imagen del disco (.bin / .cue / .iso):", "b.browse": "Elegir...", "l.savedir": "Carpeta de partidas guardadas:",
        "l.scale": "Tamaño de la ventana:", "l.fullscreen": "Pantalla completa (F11 en el juego)", "l.hz": "Velocidad:",
        "l.pad": "Mando: cualquier mando compatible con SDL2 (Xbox, PS4/PS5, Switch Pro) se usa automáticamente; teclado: cursores, X = Cruz, Z = Cuadrado, S = Triángulo, D = Círculo, Q/W = L1/R1, A/E = L2/R2, Intro = Start, Retroceso = Select.",
        "b.play": "Jugar", "b.stop": "Cerrar el juego", "b.copylog": "Copiar el registro", "s.copied": "Registro copiado al portapapeles ({0} líneas).", "l.autostart": "Empezar partida nueva automáticamente (pulsa Start en el título)",
        "s.nodisc": "Elige tu imagen del disco.", "s.checking": "Comprobando el disco...", "s.ok": "Disco correcto: MAIN0.EXE verificado (sha1).",
        "s.badsha": "Aviso: MAIN0.EXE no coincide con SCES-01331 (sha1 {0}); el port puede no funcionar.",
        "s.noexe": "No se encuentra EXE/MAIN0.EXE en la imagen: no es un disco de Tombi! PAL.", "s.notiso": "No es una imagen ISO9660: {0}",
        "s.running": "Jugando... (cerrar la ventana del juego termina)", "s.ended": "El juego terminó (código {0}). Registro: {1}",
        "s.nobin": "No se encuentra el binario del port ({0}). Compílalo (docs/PORT.md) o usa el exe distribuido.",
        "l.log": "Últimas líneas del registro del port:", "l.modshelp": "Cada mod es una opción del port (--poke, --hz...); se guardan en tombi-port.json.",
        "l.key": "Tecla", "b.defaults": "Valores por defecto", "l.cmd": "Comando:",
    },
    "en": {
        "title": "Tombi! - native port", "tab.play": "Play", "tab.mods": "Mods", "tab.controls": "Controls",
        "l.disc": "Disc image (.bin / .cue / .iso):", "b.browse": "Browse...", "l.savedir": "Save folder (memory cards):",
        "l.scale": "Window size:", "l.fullscreen": "Fullscreen (F11 in game)", "l.hz": "Speed:",
        "l.pad": "Controller: any SDL2-compatible pad (Xbox, PS4/PS5, Switch Pro) is used automatically; keyboard: arrows, X = Cross, Z = Square, S = Triangle, D = Circle, Q/W = L1/R1, A/E = L2/R2, Enter = Start, Backspace = Select.",
        "b.play": "Play", "b.stop": "Close the game", "b.copylog": "Copy log", "s.copied": "Log copied to the clipboard ({0} lines).", "l.autostart": "Start a new game automatically (presses Start on the title)",
        "s.nodisc": "Pick your disc image.", "s.checking": "Checking the disc...", "s.ok": "Disc OK: MAIN0.EXE verified (sha1).",
        "s.badsha": "Warning: MAIN0.EXE does not match SCES-01331 (sha1 {0}); the port may not work.",
        "s.noexe": "EXE/MAIN0.EXE not found in the image: not a Tombi! PAL disc.", "s.notiso": "Not an ISO9660 image: {0}",
        "s.running": "Playing... (closing the game window ends it)", "s.ended": "The game ended (code {0}). Log: {1}",
        "s.nobin": "Port binary not found ({0}). Build it (docs/PORT.md) or use the distributed exe.",
        "l.log": "Last lines of the port's log:", "l.modshelp": "Each mod is a port option (--poke, --hz...); they are saved in tombi-port.json.",
        "l.key": "Key", "b.defaults": "Defaults", "l.cmd": "Command:",
    },
    "fr": {"title": "Tombi! - port natif", "tab.play": "Jouer", "tab.mods": "Mods", "tab.controls": "Commandes", "b.play": "Jouer",
           "l.disc": "Image du disque (.bin / .cue / .iso) :", "b.browse": "Parcourir...", "l.savedir": "Dossier des sauvegardes :"},
    "de": {"title": "Tombi! - nativer Port", "tab.play": "Spielen", "tab.mods": "Mods", "tab.controls": "Steuerung", "b.play": "Spielen",
           "l.disc": "Disc-Image (.bin / .cue / .iso):", "b.browse": "Durchsuchen...", "l.savedir": "Speicherordner:"},
    "it": {"title": "Tombi! - port nativo", "tab.play": "Gioca", "tab.mods": "Mod", "tab.controls": "Controlli", "b.play": "Gioca",
           "l.disc": "Immagine del disco (.bin / .cue / .iso):", "b.browse": "Sfoglia...", "l.savedir": "Cartella dei salvataggi:"},
    "pt": {"title": "Tombi! - port nativo", "tab.play": "Jogar", "tab.mods": "Mods", "tab.controls": "Controlos", "b.play": "Jogar",
           "l.disc": "Imagem do disco (.bin / .cue / .iso):", "b.browse": "Procurar...", "l.savedir": "Pasta dos jogos guardados:"},
})


# ---------------------------------------------------------------------------------------------- files
def app_dir():
    """Folder of the launcher: next to the exe when frozen (settings, MAIN0.EXE, log), else the repo's build/."""
    if getattr(sys, "frozen", False):
        return os.path.dirname(os.path.abspath(sys.executable))
    d = os.path.join(ROOT, "build", "launcher")
    os.makedirs(d, exist_ok=True)
    return d


def bin_dir():
    """Where tombi2(.exe) and SDL2.dll live: the PyInstaller bundle, else the build tree."""
    if getattr(sys, "_MEIPASS", None):
        return sys._MEIPASS
    if os.name == "nt":
        return os.path.join(ROOT, "build", "port2-win")
    return os.path.join(ROOT, "build", "port2")


def game_binary():
    return os.path.join(bin_dir(), "tombi2.exe" if os.name == "nt" else "tombi2")


def settings_file():
    return os.path.join(app_dir(), "tombi-port.json")


def load_settings():
    s = M.default_settings()
    s.update({"disc": "", "savedir": "", "scale": 2, "fullscreen": False})
    try:
        s.update(json.load(open(settings_file(), encoding="utf-8")))
    except (OSError, ValueError):
        pass
    return s


def save_settings(s):
    try:
        json.dump(s, open(settings_file(), "w", encoding="utf-8"), indent=1)
    except OSError:
        pass


def bin_of_cue(path):
    """The data track file named by a .cue sheet (next to it)."""
    if not path.lower().endswith(".cue"):
        return path
    try:
        for line in open(path, encoding="utf-8", errors="replace"):
            line = line.strip()
            if line.upper().startswith("FILE "):
                name = line[5:].rsplit(" ", 1)[0].strip().strip('"')
                return os.path.join(os.path.dirname(path), name)
    except OSError:
        pass
    return path


def extract_main0(image, out):
    """EXE/MAIN0.EXE from the ISO9660 directory of a raw (2352) or cooked (2048) image -> out; returns its sha1."""
    f = open(image, "rb")
    f.seek(16 * 2352 + 25)
    raw = f.read(5) == b"CD001"
    ss, uo = (2352, 24) if raw else (2048, 0)

    def sec(n, cnt=1):
        b = []
        for i in range(cnt):
            f.seek((n + i) * ss + uo)
            b.append(f.read(2048))
        return b"".join(b)
    pvd = sec(16)
    if pvd[1:6] != b"CD001":
        f.close()
        raise ValueError(tr("s.notiso", os.path.basename(image)))
    found = {}

    def walk(lba, size, pre):
        data = sec(lba, (size + 2047) // 2048)
        p = 0
        while p < size:
            n = data[p]
            if n == 0:
                p = (p // 2048 + 1) * 2048
                continue
            r = data[p:p + n]
            p += n
            elba, esz = struct.unpack("<I", r[2:6])[0], struct.unpack("<I", r[10:14])[0]
            nm = r[33:33 + r[32]]
            if nm in (b"\0", b"\1"):
                continue
            nm = nm.decode(errors="replace")
            if r[25] & 2:
                walk(elba, esz, pre + nm + "/")
            else:
                found[(pre + nm.split(";")[0]).upper()] = (elba, esz)
    root = pvd[156:190]
    walk(struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "")
    if "EXE/MAIN0.EXE" not in found:
        f.close()
        raise ValueError(tr("s.noexe"))
    lba, size = found["EXE/MAIN0.EXE"]
    data = sec(lba, (size + 2047) // 2048)[:size]
    f.close()
    with open(out, "wb") as o:
        o.write(data)
    return hashlib.sha1(data).hexdigest()


def build_id():
    """Short git sha of the build: build_id.txt bundled by port2-exe.yml, else `git rev-parse` of the checkout."""
    for d in (getattr(sys, "_MEIPASS", None), ROOT):
        if d and os.path.isfile(os.path.join(d, "build_id.txt")):
            return open(os.path.join(d, "build_id.txt"), encoding="utf-8", errors="replace").read().strip() or "unknown"
    try:
        return subprocess.check_output(["git", "-C", ROOT, "rev-parse", "--short", "HEAD"], text=True,
                                       stderr=subprocess.DEVNULL).strip() or "unknown"
    except Exception:
        return "unknown"


BUILD_ID = build_id()


def set_icon(root):
    for d in (os.path.join(getattr(sys, "_MEIPASS", ""), "icons"), os.path.join(ROOT, "tools", "icons")):
        for name in ("port", "gfx"):
            ico, png = os.path.join(d, name + ".ico"), os.path.join(d, name + ".png")
            try:
                if os.name == "nt" and os.path.isfile(ico):
                    root.iconbitmap(ico)
                    return
                if os.path.isfile(png):
                    root._icon_img = tk.PhotoImage(file=png)
                    root.iconphoto(True, root._icon_img)
                    return
            except Exception:
                pass


# ---------------------------------------------------------------------------------------------- window
class App:
    def __init__(self, root):
        self.root = root
        self.s = load_settings()
        self.proc = None
        self.log_path = os.path.join(app_dir(), "tombi-port.log")   # the last run's log (Copy log works before a run too)
        self.vars = {}
        set_icon(root)
        root.geometry("760x600")
        root.protocol("WM_DELETE_WINDOW", self.on_close)
        self._build()

    def rebuild(self):
        self.collect()
        for w in self.root.winfo_children():
            w.destroy()
        self._build()

    def _build(self):
        r = self.root
        r.title(tr("title") + "  [build " + BUILD_ID + "]")
        bar = ttk.Frame(r, padding=4)
        bar.pack(fill="x")
        i18n.selector(bar, self.rebuild).pack(side="right")
        self.nb = ttk.Notebook(r)
        self.nb.pack(fill="both", expand=True, padx=6, pady=4)
        self.play_tab = ttk.Frame(self.nb, padding=8)
        self.mods_tab = ttk.Frame(self.nb, padding=8)
        self.ctrl_tab = ttk.Frame(self.nb, padding=8)
        self.nb.add(self.play_tab, text=tr("tab.play"))
        self.nb.add(self.mods_tab, text=tr("tab.mods"))
        self.nb.add(self.ctrl_tab, text=tr("tab.controls"))
        self._build_play(self.play_tab)
        self._build_mods(self.mods_tab)
        self._build_controls(self.ctrl_tab)
        self.status = tk.StringVar(value=tr("s.nodisc"))
        ttk.Label(r, textvariable=self.status, anchor="w", padding=4).pack(fill="x")
        if self.s.get("disc"):
            self.root.after(200, self.check_disc)

    def var(self, key, kind=tk.StringVar, default=""):
        v = self.vars.get(key)
        if v is None:
            v = self.vars[key] = kind(value=self.s.get(key, default))
        return v

    def _build_play(self, f):
        ttk.Label(f, text=tr("l.disc")).grid(row=0, column=0, sticky="w", pady=(0, 2))
        row = ttk.Frame(f); row.grid(row=1, column=0, sticky="ew", pady=(0, 8))
        ttk.Entry(row, textvariable=self.var("disc")).pack(side="left", fill="x", expand=True)
        ttk.Button(row, text=tr("b.browse"), command=self.pick_disc).pack(side="left", padx=4)
        ttk.Label(f, text=tr("l.savedir")).grid(row=2, column=0, sticky="w", pady=(0, 2))
        row = ttk.Frame(f); row.grid(row=3, column=0, sticky="ew", pady=(0, 8))
        ttk.Entry(row, textvariable=self.var("savedir")).pack(side="left", fill="x", expand=True)
        ttk.Button(row, text=tr("b.browse"), command=self.pick_savedir).pack(side="left", padx=4)
        opts = ttk.Frame(f); opts.grid(row=4, column=0, sticky="w", pady=4)
        ttk.Label(opts, text=tr("l.scale")).pack(side="left")
        sv = self.var("scale", tk.IntVar, 2)
        for n in (1, 2, 3):
            ttk.Radiobutton(opts, text="%dx" % n, variable=sv, value=n).pack(side="left", padx=2)
        ttk.Checkbutton(opts, text=tr("l.fullscreen"), variable=self.var("fullscreen", tk.BooleanVar, False)).pack(side="left", padx=(16, 0))
        hz = ttk.Frame(f); hz.grid(row=5, column=0, sticky="w", pady=4)
        ttk.Label(hz, text=tr("l.hz")).pack(side="left")
        hv = self.var("hz60", tk.BooleanVar, False)
        ttk.Radiobutton(hz, text="50 Hz (PAL)", variable=hv, value=False).pack(side="left", padx=2)
        ttk.Radiobutton(hz, text="60 Hz", variable=hv, value=True).pack(side="left", padx=2)
        ttk.Checkbutton(f, text=tr("l.autostart"), variable=self.var("autostart", tk.BooleanVar, False)).grid(row=6, column=0, sticky="w", pady=4)
        ttk.Label(f, text=tr("l.pad"), wraplength=700, justify="left").grid(row=7, column=0, sticky="w", pady=8)
        btns = ttk.Frame(f); btns.grid(row=8, column=0, sticky="w", pady=8)
        self.play_btn = ttk.Button(btns, text=tr("b.play"), command=self.play, width=16)
        self.play_btn.pack(side="left")
        ttk.Button(btns, text=tr("b.stop"), command=self.stop).pack(side="left", padx=8)
        ttk.Button(btns, text=tr("b.copylog"), command=self.copy_log).pack(side="left", padx=8)
        ttk.Label(f, text=tr("l.cmd")).grid(row=9, column=0, sticky="w")
        self.cmd_text = tk.Text(f, height=3, wrap="word")
        self.cmd_text.grid(row=10, column=0, sticky="ew")
        ttk.Label(f, text=tr("l.log")).grid(row=11, column=0, sticky="w", pady=(8, 0))
        self.log_text = tk.Text(f, height=8, wrap="none")
        self.log_text.grid(row=12, column=0, sticky="nsew")
        f.columnconfigure(0, weight=1)
        f.rowconfigure(12, weight=1)

    def _build_mods(self, f):
        ttk.Label(f, text=tr("l.modshelp"), wraplength=700, justify="left").pack(anchor="w", pady=(0, 6))
        for mid, lk, dk, kind, _fn in M.MODS:
            if mid == "keys_on":
                continue      # on the Controls tab
            box = ttk.Frame(f, padding=(0, 2))
            box.pack(fill="x")
            ttk.Checkbutton(box, text=tr(lk), variable=self.var(mid, tk.BooleanVar, False)).pack(side="left")
            if kind == "check+area":
                av, sv = self.var("area", tk.IntVar, 0), self.var("section", tk.IntVar, 0)
                ttk.Label(box, text=tr("mod.area")).pack(side="left", padx=(12, 2))
                names = ["%02d %s" % (a, M.AREA_NAMES.get(a, "")) for a, _ in M.ROOM_COUNTS]
                self.area_cb = ttk.Combobox(box, values=names, state="readonly", width=34)
                idx = [a for a, _ in M.ROOM_COUNTS]
                self.area_cb.current(idx.index(av.get()) if av.get() in idx else 0)
                self.area_cb.pack(side="left")
                ttk.Label(box, text=tr("mod.section")).pack(side="left", padx=(12, 2))
                self.sec_cb = ttk.Combobox(box, state="readonly", width=4)
                self.sec_cb.pack(side="left")

                def fill_sections(_=None, av=av, sv=sv):
                    a = M.ROOM_COUNTS[self.area_cb.current()][0]
                    av.set(a)
                    secs = [str(k) for k in range(M.ROOM_COUNTS[self.area_cb.current()][1]) if (a, k) not in M.DOOR_ROOMS]
                    self.sec_cb["values"] = secs
                    self.sec_cb.current(secs.index(str(sv.get())) if str(sv.get()) in secs else 0)
                    sv.set(int(self.sec_cb.get()))
                self.area_cb.bind("<<ComboboxSelected>>", fill_sections)
                self.sec_cb.bind("<<ComboboxSelected>>", lambda _e, sv=sv: sv.set(int(self.sec_cb.get())))
                fill_sections()
            elif kind.startswith("check+int:"):
                _, key, lo, hi, step = kind.split(":")
                iv = self.var(key, tk.IntVar, int(lo))
                ttk.Spinbox(box, from_=int(lo), to=int(hi), increment=int(step), textvariable=iv, width=10).pack(side="left", padx=12)
            ttk.Label(f, text=tr(dk), wraplength=680, justify="left", foreground="#555").pack(anchor="w", padx=24, pady=(0, 4))

    def _build_controls(self, f):
        ttk.Checkbutton(f, text=tr("mod.keys"), variable=self.var("keys_on", tk.BooleanVar, False)).pack(anchor="w")
        ttk.Label(f, text=tr("mod.keys.d"), wraplength=680, justify="left", foreground="#555").pack(anchor="w", padx=24, pady=(0, 8))
        grid = ttk.Frame(f); grid.pack(anchor="w")
        keys = self.s.get("keys") or dict(M.DEFAULT_KEYS)
        self.key_vars = {}
        for i, b in enumerate(M.BUTTONS):
            ttk.Label(grid, text=b.capitalize(), width=10).grid(row=i % 7, column=(i // 7) * 2, sticky="w", pady=2)
            v = self.key_vars[b] = tk.StringVar(value=keys.get(b, M.DEFAULT_KEYS[b]))
            ttk.Entry(grid, textvariable=v, width=14).grid(row=i % 7, column=(i // 7) * 2 + 1, sticky="w", padx=(2, 16))
        ttk.Button(f, text=tr("b.defaults"), command=lambda: [v.set(M.DEFAULT_KEYS[b]) for b, v in self.key_vars.items()]).pack(anchor="w", pady=8)
        ttk.Separator(f, orient="horizontal").pack(fill="x", pady=8)
        row = ttk.Frame(f); row.pack(anchor="w")
        ttk.Label(row, text=tr("mod.prompts") + ":").pack(side="left")
        pv = self.var("prompts", tk.StringVar, "playstation")
        for name, label in (("playstation", "PlayStation"), ("xbox", "Xbox"), ("keyboard", tr("tab.controls") + " / Keyboard")):
            ttk.Radiobutton(row, text=label, variable=pv, value=name).pack(side="left", padx=4)
        ttk.Label(f, text=tr("mod.prompts.d"), wraplength=680, justify="left", foreground="#555").pack(anchor="w", padx=24, pady=(2, 4))

    # ---- actions
    def collect(self):
        for k, v in self.vars.items():
            try:
                self.s[k] = v.get()
            except tk.TclError:
                pass
        if hasattr(self, "key_vars"):
            self.s["keys"] = {b: v.get().strip() for b, v in self.key_vars.items()}
        save_settings(self.s)

    def pick_disc(self):
        p = filedialog.askopenfilename(filetypes=[("Disc image", "*.bin *.cue *.iso *.img"), ("All", "*.*")])
        if p:
            self.vars["disc"].set(p)
            if not self.vars["savedir"].get():
                self.vars["savedir"].set(os.path.dirname(p))
            self.check_disc()

    def pick_savedir(self):
        p = filedialog.askdirectory()
        if p:
            self.vars["savedir"].set(p)

    def check_disc(self):
        self.collect()
        image = bin_of_cue(self.s["disc"])
        self.status.set(tr("s.checking"))

        def work():
            try:
                sha = extract_main0(image, os.path.join(app_dir(), "MAIN0.EXE"))
                msg = tr("s.ok") if sha == MAIN0_SHA1 else tr("s.badsha", sha[:8])
            except (OSError, ValueError, struct.error) as e:
                msg = str(e)
            self.root.after(0, self.status.set, msg)
        threading.Thread(target=work, daemon=True).start()

    def command(self):
        self.collect()
        s = self.s
        exe = game_binary()
        cmd = [exe, "--disc", bin_of_cue(s["disc"]), "--window", "--scale", str(s.get("scale", 2)),
               "--show-fps", "2"]     # the measured vblank rate and its histogram in the log
        if s.get("savedir"):
            cmd += ["--save-dir", s["savedir"]]
        if s.get("fullscreen"):
            cmd += ["--fullscreen"]
        cmd += M.mod_args(s, app_dir())
        return cmd

    def play(self):
        cmd = self.command()
        self.cmd_text.delete("1.0", "end")
        self.cmd_text.insert("1.0", subprocess.list2cmdline(cmd))
        if not os.path.isfile(cmd[0]):
            messagebox.showerror(tr("title"), tr("s.nobin", cmd[0]))
            return
        if not os.path.isfile(cmd[2]) or not os.path.isfile(os.path.join(app_dir(), "MAIN0.EXE")):
            self.check_disc()
            messagebox.showerror(tr("title"), tr("s.nodisc"))
            return
        if self.proc and self.proc.poll() is None:
            return
        self.log_path = os.path.join(app_dir(), "tombi-port.log")
        log = open(self.log_path, "w", encoding="utf-8", errors="replace")
        log.write("[launcher] build %s\n" % BUILD_ID)      # the exe then prints its own id: [port2] build <sha> <date>
        log.flush()
        flags = subprocess.CREATE_NO_WINDOW if os.name == "nt" else 0
        self.proc = subprocess.Popen(cmd, cwd=app_dir(), stdout=log, stderr=subprocess.STDOUT, creationflags=flags)
        self.status.set(tr("s.running"))
        self.play_btn.state(["disabled"])
        self.root.after(1000, self.poll)

    def poll(self):
        if not self.proc:
            return
        try:
            self.show_log_tail(open(self.log_path, encoding="utf-8", errors="replace").read().splitlines())
        except OSError:
            pass
        rc = self.proc.poll()
        if rc is None:
            self.root.after(1000, self.poll)
            return
        self.status.set(tr("s.ended", rc, self.log_path))
        self.play_btn.state(["!disabled"])
        self.proc = None

    def show_log_tail(self, lines):
        """The last 8 lines, preceded by the warning / FATAL / access-violation lines from earlier in the log (a
        missing disc, a bad layout) so a problem reported at startup stays visible under the final block."""
        tail = lines[-8:]
        warn = [l for l in lines[:-8] if "warning" in l or "FATAL" in l or "access violation" in l][:6]
        self.log_text.delete("1.0", "end")
        self.log_text.insert("1.0", "\n".join(warn + (["..."] if warn else []) + tail))

    def copy_log(self):
        """Whole log file to the clipboard (what to paste in a bug report)."""
        try:
            text = open(self.log_path, encoding="utf-8", errors="replace").read() if self.log_path else self.log_text.get("1.0", "end")
        except OSError:
            text = self.log_text.get("1.0", "end")
        self.root.clipboard_clear()
        self.root.clipboard_append(text)
        self.status.set(tr("s.copied", len(text.splitlines())))

    def stop(self):
        if self.proc and self.proc.poll() is None:
            self.proc.terminate()

    def on_close(self):
        self.collect()
        self.stop()
        self.root.destroy()


def main():
    root = tk.Tk()
    try:
        ttk.Style().theme_use("clam" if os.name != "nt" else "vista")
    except tk.TclError:
        pass
    App(root)
    root.mainloop()


if __name__ == "__main__":
    main()
