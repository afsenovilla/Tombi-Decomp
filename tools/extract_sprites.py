#!/usr/bin/env python3
"""Extracts every graphic of Tombi! (PAL) as PNG straight from a disc image, with no dependencies beyond
Python 3 (Windows/Linux/macOS).

  python3 tools/extract_sprites.py Tombi.bin OUT_DIR      (command line)
  python3 tools/extract_sprites.py                         (no arguments: a small window to pick the image and folder)

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


def extract(img, out, log=print):
    """Everything: file system, WPP sprites, VRAM pages/snapshots and SPR atlas cuts under `out`."""
    out = os.path.abspath(out)
    print_ = log
    fs = os.path.join(out, "fs")
    if not os.path.isdir(os.path.join(fs, "AREA00")):
        print_("extracting the file system of %s ..." % img)
        run_isoext(img, fs)
    os.environ["TOMBI_FS"] = fs
    os.environ["TOMBI_IMG"] = os.path.abspath(img)
    sys.path.insert(0, HERE)
    import area000 as A
    import wpp
    import vram
    A.FS = fs
    n = 0
    for area in sorted(os.listdir(fs)):
        d = os.path.join(fs, area)
        if not os.path.isdir(d): continue
        for f in sorted(os.listdir(d)):
            if not f.endswith(".000"): continue
            path = os.path.join(d, f)
            if not wpp.is_wpp(A.rel_name(path)): continue
            dst = os.path.join(out, "sprites", area, f[:-4])
            class Args: pass
            a = Args(); a.file, a.out, a.clut, a.row, a.x0, a.auto = path, dst, None, 0, 0, True
            wpp.cmd_unpack(a); n += 1
    print_("done: %d sprite files under %s" % (n, os.path.join(out, "sprites")))
    print_("simulating the VRAM of every load list (SPR atlas cuts, pages, snapshots) ...")
    st = vram.export_all(out, log=lambda m: None)
    print_("done: %d load lists, %d pages, %d SPR tables, %d frames / %d parts (%d identical tables shared, %d frame PNGs + %d part PNGs written); under %s and %s" % (
        st["lists_total"], st["pages"], st["tables"], st["frames_total"], st["parts_total"], st["tables_deduplicated"],
        st["frames_written"], st["parts_written"], os.path.join(out, "spr"), os.path.join(out, "vram")))
    print_("static full screens: %d under %s" % (st.get("static_screens", 0), os.path.join(out, "screens")))
    import room
    print_("rendering the room backgrounds (WMD models with per polygon palettes) ...")
    nr = room.export_rooms(os.path.join(out, "rooms"), log=lambda m: None)
    print_("done: %d rooms under %s" % (nr, os.path.join(out, "rooms")))
    print_("exporting the room models as 3D (glTF .glb, open in Blender / Windows 3D Viewer) ...")
    ng = room.export_glb(os.path.join(out, "rooms3d"), log=lambda m: None)
    print_("done: %d models under %s" % (ng, os.path.join(out, "rooms3d")))


def gui():
    """Small window (tkinter, standard library): pick the disc image and the output folder, press Extract."""
    import threading, queue
    import tkinter as tk
    from tkinter import filedialog, messagebox, ttk
    root = tk.Tk()
    root.title("Tombi! asset extractor")
    root.resizable(True, True)
    img_v, out_v = tk.StringVar(), tk.StringVar()
    frm = ttk.Frame(root, padding=12); frm.grid(sticky="nsew")
    root.columnconfigure(0, weight=1); root.rowconfigure(0, weight=1)
    frm.columnconfigure(1, weight=1); frm.rowconfigure(3, weight=1)
    ttk.Label(frm, text="Disc image (.bin):").grid(row=0, column=0, sticky="w", pady=3)
    ttk.Entry(frm, textvariable=img_v, width=60).grid(row=0, column=1, sticky="ew", padx=6)
    ttk.Button(frm, text="Browse...", command=lambda: img_v.set(filedialog.askopenfilename(
        title="Tombi! disc image", filetypes=[("Disc image", "*.bin *.img *.iso"), ("All files", "*.*")]) or img_v.get())).grid(row=0, column=2)
    ttk.Label(frm, text="Output folder:").grid(row=1, column=0, sticky="w", pady=3)
    ttk.Entry(frm, textvariable=out_v, width=60).grid(row=1, column=1, sticky="ew", padx=6)
    ttk.Button(frm, text="Browse...", command=lambda: out_v.set(filedialog.askdirectory(title="Output folder") or out_v.get())).grid(row=1, column=2)
    btn = ttk.Button(frm, text="Extract")
    btn.grid(row=2, column=0, columnspan=3, pady=8)
    log = tk.Text(frm, height=14, width=90, state="disabled", wrap="word")
    log.grid(row=3, column=0, columnspan=3, sticky="nsew")
    sb = ttk.Scrollbar(frm, command=log.yview); sb.grid(row=3, column=3, sticky="ns"); log["yscrollcommand"] = sb.set
    q = queue.Queue()

    def put(msg):
        q.put(str(msg))

    def pump():
        try:
            while True:
                m = q.get_nowait()
                log.configure(state="normal"); log.insert("end", m + "\n"); log.see("end"); log.configure(state="disabled")
                if m.startswith("__END__"):
                    btn.configure(state="normal")
                    if m != "__END__ok": messagebox.showerror("Extraction failed", m[8:])
                    else: messagebox.showinfo("Done", "Assets written under\n" + out_v.get())
        except queue.Empty:
            pass
        root.after(100, pump)

    def work(img, out):
        try:
            extract(img, out, log=put); put("__END__ok")
        except Exception as e:
            import traceback; put(traceback.format_exc()); put("__END__: " + str(e))

    def start():
        img, out = img_v.get().strip(), out_v.get().strip()
        if not os.path.isfile(img): messagebox.showwarning("Missing", "Pick the disc image (.bin)."); return
        if not out: messagebox.showwarning("Missing", "Pick an output folder."); return
        btn.configure(state="disabled")
        put("Extracting %s -> %s (about 2 minutes, ~100 MB) ..." % (img, out))
        threading.Thread(target=work, args=(img, out), daemon=True).start()

    btn.configure(command=start)
    root.after(100, pump)
    root.mainloop()


def main():
    if len(sys.argv) == 1:
        try:
            return gui()
        except Exception as e:   # no tkinter or no display: fall back to the usage text
            sys.exit("%s\n(no window: %s)" % (__doc__, e))
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    extract(sys.argv[1], sys.argv[2])


if __name__ == "__main__":
    main()
