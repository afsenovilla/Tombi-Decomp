#!/usr/bin/env python3
"""Tombi! graphics editor (Windows exe: tombi-gfx.exe). Opens the PAL disc image, shows screens, interface sprites, VRAM pages, sprite frames and
palettes with their real colours, imports/exports indexed PNG, edits palettes, keeps the project as a mod (tools/mod.py format) and builds a
new ISO.  The logic is in gfx_core.py; this file is only the window.  Interface language: tools/i18n.py (menu "Idioma / Language").

  python tools/gfx_editor.py [disc.bin | extracted_fs_folder]
"""
import os, sys, threading, traceback

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import i18n
from i18n import tr
import gfx_core as G

try:
    import tkinter as tk
    from tkinter import colorchooser, filedialog, messagebox, ttk
except ImportError:      # pragma: no cover
    sys.exit("tkinter is missing (Linux: apt install python3-tk).")
try:
    from PIL import Image, ImageTk
except ImportError:      # pragma: no cover
    sys.exit("Pillow is missing (pip install pillow).")

i18n.add({
    "es": {
        "title": "Tombi! – editor de gráficos", "b.opendisc": "Abrir disco…", "b.openfolder": "Abrir carpeta…", "b.exppage": "Exportar PNG",
        "b.expsel": "Exportar sel.", "b.imppage": "Importar PNG", "b.impsel": "Importar sel.", "b.undo": "Deshacer",
        "b.preview": "Vista previa", "b.savemod": "Guardar mod", "b.loadmod": "Cargar mod", "b.iso": "Generar ISO", "b.help": "Ayuda",
        "b.transparent": "Transparente (0x0000)", "b.revert": "Restaurar original",
        "l.zoom": "Zoom:", "l.rects": "Mostrar rectángulos", "l.palette": "Paleta de la imagen", "l.edit": "Editar paleta", "l.sel": "Selección: {0}",
        "l.nosel": "Selección: toda la imagen", "l.nodisc": "Abre la imagen del disco (Tombi.bin) o la carpeta con los archivos extraídos.",
        "l.opened": "Disco abierto: {0}", "l.edits": "{0} recurso(s) modificado(s)", "l.colour": "Color {0}: palabra 0x{1:04X}  RGB {2}  STP={3}",
        "l.stpnote": "El bit STP (bit 15) de cada color se conserva; un color nuevo hereda el del que sustituye. Un color 0x0000 es transparente.",
        "l.rowof": "Fila {0} de la paleta", "l.loading": "Abriendo… (se extraen los archivos necesarios)", "l.busy": "Trabajando…",
        "d.opendisc": "Imagen del disco", "d.pngfiles": "Imagen PNG", "d.allfiles": "Todos los archivos", "d.savepng": "Guardar PNG", "d.openpng": "Importar PNG",
        "d.shared": "Esta paleta ({0}) la comparten otras imágenes. Al cambiarla también cambian:\n\n{1}\n\nListas de carga que la cargan: {2}\n\n¿Continuar?",
        "d.imported": "PNG importado en {0}.\nColores usados: {1} (límite {2}). {3}", "d.direct": "Índices usados tal cual (paleta idéntica).",
        "d.quant": "Convertido a la paleta actual (color más cercano); píxeles transparentes: {0}.", "d.nonehere": "No hay nada seleccionado.",
        "d.nochanges": "No hay cambios que guardar.", "d.modsaved": "Mod guardado en:\n{0}\n\nArchivos: {1}\n\nSe aplica con: python tools/mod.py build --mods {2}",
        "d.modloaded": "Mod cargado: {0} recurso(s) modificado(s).", "d.geniso": "Guardar la ISO nueva como", "d.isoimg": "Imagen de disco",
        "d.generating": "Generando la ISO (puede tardar varios minutos; necesita ~1,5 GB libres)…", "d.isodone": "ISO generada: {0}",
        "d.isook": "ISO generada:\n{0}\n\nArchivos cambiados: {1}\n\nSe puede borrar la carpeta de trabajo:\n{2}", "d.isofail": "Error al generar la ISO.",
        "d.fallback": "No se pudo montar la ISO (falta mkpsxiso o solo se abrió una carpeta).\n\nSe escribieron {0} archivo(s) en:\n{1}\n\nCópialos sobre la carpeta extraída del disco y reconstruye con mkpsxiso: python tools/mkdisc.py build --relayout (ver README).",
        "d.fallbacktitle": "Archivos modificados", "d.unsaved": "Hay {0} recurso(s) modificado(s) sin guardar como mod. ¿Salir igualmente?",
        "d.preview": "Vista previa en contexto (320x240)", "d.nopreview": "La vista previa en contexto existe para los sprites de la interfaz (TRY / AGAIN) y la pantalla try_again.",
        "d.openfirst": "Abre primero un disco.", "d.pickfirst": "Elige primero una imagen en el árbol.", "d.revertq": "¿Descartar los cambios de {0}?",
        "d.modname": "Carpeta del mod (se crea; el nombre de la carpeta es el nombre del mod)",
        "help": "Árbol de la izquierda: Pantallas, Interfaz/HUD (tabla UiSpriteDef de MAIN0), páginas por lista de carga, sprites WPP y paletas.\n\n"
                "Centro: la imagen con su paleta. Elige otra paleta en la lista de la derecha. Con «Mostrar rectángulos» se dibujan los sprites; haz clic en uno para seleccionarlo (o arrastra para marcar una región).\n\n"
                "Importar PNG: un PNG indexado con la misma paleta se usa tal cual; un PNG RGB/RGBA se convierte al color más cercano de la paleta (la transparencia es el índice 0, palabra 0x0000). "
                "Avisa si hay más colores de los que caben (16 o 256).\n\nEditar paleta: haz clic en un color. Cambiar una paleta cambia todas las imágenes que la usan (se avisa).\n\n"
                "Guardar mod escribe mods/<nombre>/mod.json + PNG (formato de tools/mod.py). Generar ISO aplica los cambios al disco original.",
    },
    "en": {
        "title": "Tombi! – graphics editor", "b.opendisc": "Open disc…", "b.openfolder": "Open folder…", "b.exppage": "Export PNG",
        "b.expsel": "Export sel.", "b.imppage": "Import PNG", "b.impsel": "Import sel.", "b.undo": "Undo",
        "b.preview": "Preview", "b.savemod": "Save mod", "b.loadmod": "Load mod", "b.iso": "Build ISO", "b.help": "Help",
        "b.transparent": "Transparent (0x0000)", "b.revert": "Restore original",
        "l.zoom": "Zoom:", "l.rects": "Show rectangles", "l.palette": "Image palette", "l.edit": "Edit palette", "l.sel": "Selection: {0}",
        "l.nosel": "Selection: whole image", "l.nodisc": "Open the disc image (Tombi.bin) or the folder with the extracted files.",
        "l.opened": "Disc opened: {0}", "l.edits": "{0} resource(s) modified", "l.colour": "Colour {0}: word 0x{1:04X}  RGB {2}  STP={3}",
        "l.stpnote": "The STP bit (bit 15) of each colour is kept; a new colour inherits the one it replaces. Colour 0x0000 is transparent.",
        "l.rowof": "Palette row {0}", "l.loading": "Opening… (extracting the needed files)", "l.busy": "Working…",
        "d.opendisc": "Disc image", "d.pngfiles": "PNG image", "d.allfiles": "All files", "d.savepng": "Save PNG", "d.openpng": "Import PNG",
        "d.shared": "This palette ({0}) is shared with other images. Changing it also changes:\n\n{1}\n\nLoad lists that load it: {2}\n\nContinue?",
        "d.imported": "PNG imported into {0}.\nColours used: {1} (limit {2}). {3}", "d.direct": "Indices used as they are (identical palette).",
        "d.quant": "Converted to the current palette (nearest colour); transparent pixels: {0}.", "d.nonehere": "Nothing is selected.",
        "d.nochanges": "There are no changes to save.", "d.modsaved": "Mod saved in:\n{0}\n\nFiles: {1}\n\nApply it with: python tools/mod.py build --mods {2}",
        "d.modloaded": "Mod loaded: {0} resource(s) modified.", "d.geniso": "Save the new ISO as", "d.isoimg": "Disc image",
        "d.generating": "Building the ISO (may take several minutes; needs ~1.5 GB free)…", "d.isodone": "ISO built: {0}",
        "d.isook": "ISO built:\n{0}\n\nChanged files: {1}\n\nThe work folder can be deleted:\n{2}", "d.isofail": "Error building the ISO.",
        "d.fallback": "The ISO could not be built (mkpsxiso is missing or only a folder was opened).\n\n{0} file(s) were written to:\n{1}\n\nCopy them over the extracted disc folder and rebuild with mkpsxiso: python tools/mkdisc.py build --relayout (see README).",
        "d.fallbacktitle": "Modified files", "d.unsaved": "{0} modified resource(s) are not saved as a mod. Quit anyway?",
        "d.preview": "Preview in context (320x240)", "d.nopreview": "The in-context preview exists for the interface sprites (TRY / AGAIN) and the try_again screen.",
        "d.openfirst": "Open a disc first.", "d.pickfirst": "Pick an image in the tree first.", "d.revertq": "Discard the changes of {0}?",
        "d.modname": "Mod folder (created; the folder name is the mod name)",
        "help": "Left tree: Screens, Interface/HUD (the UiSpriteDef table of MAIN0), pages by load list, WPP sprites and palettes.\n\n"
                "Centre: the image with its palette. Pick another palette in the list on the right. \"Show rectangles\" draws the sprites; click one to select it (or drag to mark a region).\n\n"
                "Import PNG: an indexed PNG with the same palette is used as it is; an RGB/RGBA PNG is converted to the nearest palette colour (transparency is index 0, word 0x0000). "
                "It warns when there are more colours than fit (16 or 256).\n\nEdit palette: click a colour. Changing a palette changes every image that uses it (you are warned).\n\n"
                "Save mod writes mods/<name>/mod.json + PNGs (tools/mod.py format). Build ISO applies the changes to the original disc.",
    },
    "fr": {"title": "Tombi! – éditeur graphique", "b.opendisc": "Ouvrir un disque…", "b.openfolder": "Ouvrir un dossier…", "b.exppage": "Exporter PNG (tout)",
           "b.expsel": "Exporter PNG (sélection)", "b.imppage": "Importer PNG (tout)", "b.impsel": "Importer PNG (sélection)", "b.undo": "Annuler",
           "b.preview": "Aperçu en contexte", "b.savemod": "Enregistrer le mod", "b.loadmod": "Charger un mod", "b.iso": "Générer l'ISO", "b.help": "Aide",
           "b.transparent": "Transparent (0x0000)", "b.revert": "Restaurer l'original", "l.zoom": "Zoom :", "l.rects": "Afficher les rectangles",
           "l.palette": "Palette de l'image", "l.edit": "Modifier la palette", "l.nosel": "Sélection : toute l'image", "t.screens": "Écrans",
           "t.ui": "Interface / HUD", "t.pages": "Pages par liste de chargement", "t.sprites": "Sprites (WPP)", "t.palettes": "Palettes (CLUT)",
           "l.nodisc": "Ouvrez l'image du disque (Tombi.bin) ou le dossier des fichiers extraits.", "d.openfirst": "Ouvrez d'abord un disque."},
    "de": {"title": "Tombi! – Grafikeditor", "b.opendisc": "Disc öffnen…", "b.openfolder": "Ordner öffnen…", "b.exppage": "PNG exportieren (alles)",
           "b.expsel": "PNG exportieren (Auswahl)", "b.imppage": "PNG importieren (alles)", "b.impsel": "PNG importieren (Auswahl)", "b.undo": "Rückgängig",
           "b.preview": "Vorschau im Kontext", "b.savemod": "Mod speichern", "b.loadmod": "Mod laden", "b.iso": "ISO erzeugen", "b.help": "Hilfe",
           "b.transparent": "Transparent (0x0000)", "b.revert": "Original wiederherstellen", "l.zoom": "Zoom:", "l.rects": "Rechtecke anzeigen",
           "l.palette": "Bildpalette", "l.edit": "Palette bearbeiten", "l.nosel": "Auswahl: ganzes Bild", "t.screens": "Bildschirme",
           "t.ui": "Oberfläche / HUD", "t.pages": "Seiten nach Ladeliste", "t.sprites": "Sprites (WPP)", "t.palettes": "Paletten (CLUT)",
           "l.nodisc": "Öffne das Disc-Image (Tombi.bin) oder den Ordner mit den extrahierten Dateien.", "d.openfirst": "Öffne zuerst eine Disc."},
    "it": {"title": "Tombi! – editor grafico", "b.opendisc": "Apri disco…", "b.openfolder": "Apri cartella…", "b.exppage": "Esporta PNG (tutto)",
           "b.expsel": "Esporta PNG (selezione)", "b.imppage": "Importa PNG (tutto)", "b.impsel": "Importa PNG (selezione)", "b.undo": "Annulla",
           "b.preview": "Anteprima nel contesto", "b.savemod": "Salva mod", "b.loadmod": "Carica mod", "b.iso": "Genera ISO", "b.help": "Aiuto",
           "b.transparent": "Trasparente (0x0000)", "b.revert": "Ripristina originale", "l.zoom": "Zoom:", "l.rects": "Mostra rettangoli",
           "l.palette": "Palette dell'immagine", "l.edit": "Modifica palette", "l.nosel": "Selezione: intera immagine", "t.screens": "Schermate",
           "t.ui": "Interfaccia / HUD", "t.pages": "Pagine per lista di caricamento", "t.sprites": "Sprite (WPP)", "t.palettes": "Palette (CLUT)",
           "l.nodisc": "Apri l'immagine del disco (Tombi.bin) o la cartella dei file estratti.", "d.openfirst": "Apri prima un disco."},
    "pt": {"title": "Tombi! – editor de gráficos", "b.opendisc": "Abrir disco…", "b.openfolder": "Abrir pasta…", "b.exppage": "Exportar PNG (tudo)",
           "b.expsel": "Exportar PNG (seleção)", "b.imppage": "Importar PNG (tudo)", "b.impsel": "Importar PNG (seleção)", "b.undo": "Desfazer",
           "b.preview": "Pré-visualização no contexto", "b.savemod": "Guardar mod", "b.loadmod": "Carregar mod", "b.iso": "Gerar ISO", "b.help": "Ajuda",
           "b.transparent": "Transparente (0x0000)", "b.revert": "Restaurar original", "l.zoom": "Zoom:", "l.rects": "Mostrar retângulos",
           "l.palette": "Paleta da imagem", "l.edit": "Editar paleta", "l.nosel": "Seleção: imagem inteira", "t.screens": "Ecrãs",
           "t.ui": "Interface / HUD", "t.pages": "Páginas por lista de carga", "t.sprites": "Sprites (WPP)", "t.palettes": "Paletas (CLUT)",
           "l.nodisc": "Abra a imagem do disco (Tombi.bin) ou a pasta com os ficheiros extraídos.", "d.openfirst": "Abra primeiro um disco."},
})


def set_icon(root, name):
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


def checker(w, h, s=8):
    im = Image.new("RGB", (w, h), (200, 200, 200))
    px = Image.new("RGB", (s, s), (160, 160, 160))
    for y in range(0, h, s):
        for x in range(0, w, s):
            if (x // s + y // s) & 1:
                im.paste(px, (x, y))
    return im


class App:
    def __init__(self, root, path=None):
        self.root = root
        self.core = None
        self.item = None
        self.surf = None
        self.scale = 2
        self.show_rects = True
        self.region = None          # custom (x, y, w, h) of a drag
        self.warned = set()
        self.busy = False
        self.node_of = {}
        self._photo = None
        self._pal_imgs = []
        self.status_text = tr("l.nodisc")
        self.saved_edits = 0
        set_icon(root, "gfx")
        root.geometry("1360x820")
        self._build()
        root.protocol("WM_DELETE_WINDOW", self.on_close)
        if path:
            root.after(100, lambda: self.open_path(path))

    # ---- window
    def rebuild(self):
        for w in self.root.winfo_children():
            w.destroy()
        self._build()
        if self.core:
            self.fill_tree()
            if self.item:
                self.show(self.item, keep=True)

    def _build(self):
        r = self.root
        r.title(tr("title"))
        bar = ttk.Frame(r, padding=4)
        bar.pack(fill="x")
        self.buttons = []
        for item in (("b.opendisc", self.ask_open_image), ("b.openfolder", self.ask_open_folder), None,
                     ("b.exppage", lambda: self.export(False)), ("b.expsel", lambda: self.export(True)),
                     ("b.imppage", lambda: self.import_(False)), ("b.impsel", lambda: self.import_(True)), None,
                     ("b.undo", self.undo), ("b.preview", self.preview), None,
                     ("b.savemod", self.save_mod), ("b.loadmod", self.load_mod), ("b.iso", self.generate), None,
                     ("b.composedall", self.composed_all)):
            if item is None:
                ttk.Separator(bar, orient="vertical").pack(side="left", fill="y", padx=5)
                continue
            b = ttk.Button(bar, text=tr(item[0]), command=item[1])
            b.pack(side="left", padx=1)
            self.buttons.append(b)
        ttk.Button(bar, text=tr("lb.export"), command=self.export_labels).pack(side="left", padx=(8, 1))
        ttk.Button(bar, text=tr("lb.import"), command=self.import_labels).pack(side="left", padx=1)
        ttk.Button(bar, text=tr("b.help"), command=self.help).pack(side="left", padx=(8, 1))
        i18n.selector(bar, self.rebuild).pack(side="right")
        self.status = tk.StringVar(value=self.status_text)
        ttk.Label(r, textvariable=self.status, anchor="w", relief="sunken", padding=3).pack(side="bottom", fill="x")
        pw = ttk.PanedWindow(r, orient="horizontal")
        pw.pack(fill="both", expand=True)
        left = ttk.Frame(pw)
        pw.add(left, weight=1)
        self.tree = ttk.Treeview(left, show="tree", selectmode="browse")
        sb = ttk.Scrollbar(left, command=self.tree.yview)
        self.tree.configure(yscrollcommand=sb.set)
        self.tree.pack(side="left", fill="both", expand=True)
        sb.pack(side="right", fill="y")
        self.tree.bind("<<TreeviewOpen>>", self.on_open_node)
        self.tree.bind("<F2>", self.rename)
        menu = tk.Menu(self.root, tearoff=0)
        menu.add_command(label=tr("lb.rename") + "  (F2)", command=self.rename)
        self.tree.bind("<Button-3>", lambda ev: (self.tree.selection_set(self.tree.identify_row(ev.y)), menu.tk_popup(ev.x_root, ev.y_root)))
        self.tree.bind("<<TreeviewSelect>>", self.on_select)

        mid = ttk.Frame(pw)
        pw.add(mid, weight=4)
        self.mid = mid
        tb = self.tb = ttk.Frame(mid, padding=(4, 2))
        tb.pack(fill="x")
        ttk.Label(tb, text=tr("l.zoom")).pack(side="left")
        self.zoom = tk.StringVar(value="x%d" % self.scale)
        zb = ttk.Combobox(tb, textvariable=self.zoom, values=["x%d" % i for i in range(1, 9)], width=4, state="readonly")
        zb.pack(side="left", padx=4)
        zb.bind("<<ComboboxSelected>>", lambda e: self.set_zoom())
        self.rects_var = tk.BooleanVar(value=self.show_rects)
        ttk.Checkbutton(tb, text=tr("l.rects"), variable=self.rects_var, command=self.toggle_rects).pack(side="left", padx=8)
        self.sel_var = tk.StringVar(value=tr("l.nosel"))
        ttk.Label(tb, textvariable=self.sel_var).pack(side="left", padx=8)
        cf = self.cf = ttk.Frame(mid)
        cf.pack(fill="both", expand=True)
        self.canvas = tk.Canvas(cf, bg="#404050", highlightthickness=0)
        hs = ttk.Scrollbar(cf, orient="horizontal", command=self.canvas.xview)
        vs = ttk.Scrollbar(cf, orient="vertical", command=self.canvas.yview)
        self.canvas.configure(xscrollcommand=hs.set, yscrollcommand=vs.set)
        hs.pack(side="bottom", fill="x")
        vs.pack(side="right", fill="y")
        self.canvas.pack(side="left", fill="both", expand=True)
        self.canvas.bind("<ButtonPress-1>", self.on_press)
        self.canvas.bind("<B1-Motion>", self.on_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_release)
        self.canvas.bind("<MouseWheel>", lambda e: self.canvas.yview_scroll(-1 if e.delta > 0 else 1, "units"))

        right = ttk.Frame(pw, width=330)
        pw.add(right, weight=1)
        ttk.Label(right, text=tr("l.palette"), font=("TkDefaultFont", 9, "bold")).pack(anchor="w", padx=4, pady=(4, 0))
        pf = ttk.Frame(right)
        pf.pack(fill="both", expand=True, padx=4)
        self.pal_now = tk.Canvas(right, height=14, bg="#808080", highlightthickness=0)
        self.pal_now.pack(fill="x", padx=4, pady=(2, 0))
        self.pal_name = tk.StringVar()
        ttk.Label(right, textvariable=self.pal_name, wraplength=320, justify="left").pack(anchor="w", padx=4)
        ttk.Label(right, text=tr("l.palhint"), foreground="#555").pack(anchor="w", padx=4)
        self.pal_list = ttk.Treeview(pf, show="tree", selectmode="browse", height=8)
        ps = ttk.Scrollbar(pf, command=self.pal_list.yview)
        self.pal_list.configure(yscrollcommand=ps.set)
        self.pal_list.pack(side="left", fill="both", expand=True)
        ps.pack(side="right", fill="y")
        self.pal_list.bind("<<TreeviewSelect>>", self.on_pick_palette)
        self.pal_list.bind("<<TreeviewOpen>>", self.on_open_group)
        self.canvas.bind("<Up>", lambda e: self.cycle_palette(-1))
        self.canvas.bind("<Down>", lambda e: self.cycle_palette(1))
        self.canvas.bind("<Enter>", lambda e: self.canvas.focus_set())
        self.pal_refs = {}
        ttk.Label(right, text=tr("l.edit"), font=("TkDefaultFont", 9, "bold")).pack(anchor="w", padx=4, pady=(6, 0))
        self.swatch = tk.Canvas(right, width=16 * 18 + 2, height=16 * 18 + 2, bg="#808080", highlightthickness=0)
        self.swatch.pack(padx=4, pady=2)
        self.swatch.bind("<Button-1>", self.on_swatch)
        self.col_var = tk.StringVar()
        ttk.Label(right, textvariable=self.col_var, wraplength=320, justify="left").pack(anchor="w", padx=4)
        row = ttk.Frame(right)
        row.pack(fill="x", padx=4, pady=2)
        ttk.Button(row, text=tr("b.transparent"), command=self.make_transparent).pack(side="left")
        ttk.Button(row, text=tr("b.revert"), command=self.revert).pack(side="left", padx=4)
        ttk.Label(right, text=tr("l.stpnote"), wraplength=320, justify="left", foreground="#555").pack(anchor="w", padx=4, pady=4)
        self.sw_sel = 0
        self.refresh_status()

    def say(self, t):
        self.status_text = t
        self.status.set(t)

    def refresh_status(self):
        if self.core:
            n = len(self.core.edits)
            self.say((tr("l.opened", os.path.basename(self.core.disc.path)) + "   " + tr("l.edits", n)) if n else tr("l.opened", os.path.basename(self.core.disc.path)))

    def lock(self, on):
        self.busy = on
        for b in self.buttons:
            b.state(["disabled"] if on else ["!disabled"])
        self.root.config(cursor="watch" if on else "")

    # ---- opening
    def ask_open_image(self):
        p = filedialog.askopenfilename(title=tr("b.opendisc"), filetypes=[(tr("d.opendisc"), "*.bin *.iso *.img"), (tr("d.allfiles"), "*.*")])
        if p:
            self.open_path(p)

    def ask_open_folder(self):
        p = filedialog.askdirectory(title=tr("b.openfolder"))
        if p:
            self.open_path(p)

    def open_path(self, path):
        self.say(tr("l.loading"))
        self.lock(True)
        self.root.update_idletasks()
        res = {}

        def job():
            try:
                res["core"] = G.Core(path)
            except Exception as e:
                res["err"] = str(e) if isinstance(e, G.CoreError) else traceback.format_exc()
            res['done'] = True

        def done():
            self.lock(False)
            if "err" in res:
                self.say(res["err"].splitlines()[0] if res["err"] else "")
                messagebox.showerror(tr("error"), res["err"])
                return
            if self.core:
                self.core.disc.close()
            self.core = res["core"]
            self.item = self.surf = None
            self.fill_tree()
            self.refresh_status()
        threading.Thread(target=job, daemon=True).start()
        self.poll(res, done)

    def poll(self, res, done):
        """Runs done() in the main thread when the worker thread has set res['done'] (no after() calls from the thread)."""
        if res.get("done"):
            done()
        else:
            self.root.after(100, lambda: self.poll(res, done))

    # ---- tree
    def fill_tree(self):
        self.tree.delete(*self.tree.get_children())
        self.node_of = {}
        for n in self.core.tree():
            self._insert("", n)

    def _insert(self, parent, n):
        iid = self.tree.insert(parent, "end", text=self.node_text(n), open=False)
        self.node_of[iid] = n
        if n["children"]:
            for c in n["children"]:
                self._insert(iid, c)
        elif n["lazy"]:
            n["_loaded"] = False
            self.tree.insert(iid, "end", text="…")

    def node_text(self, n):
        import labels
        return labels.show(n["id"], n["text"]) if n.get("id") else n["text"]

    def rename(self, _=None):
        """F2 / context menu: name + note of the selected tree item (tools/labels.py)."""
        import labels
        sel = self.tree.selection()
        n = self.node_of.get(sel[0]) if sel else None
        if not n or not n.get("id"):
            return
        cur = labels.get(n["id"]) or {"name": "", "note": ""}
        top = tk.Toplevel(self.root)
        top.title(tr("lb.rename"))
        top.transient(self.root)
        ttk.Label(top, text=n["id"], foreground="#555").grid(row=0, column=0, columnspan=2, padx=8, pady=(8, 2), sticky="w")
        ttk.Label(top, text=tr("lb.name")).grid(row=1, column=0, padx=8, sticky="w")
        nv = tk.StringVar(value=cur["name"])
        ent = ttk.Entry(top, textvariable=nv, width=40)
        ent.grid(row=1, column=1, padx=8, pady=2)
        ttk.Label(top, text=tr("lb.note")).grid(row=2, column=0, padx=8, sticky="nw")
        note = tk.Text(top, width=40, height=4)
        note.insert("1.0", cur["note"])
        note.grid(row=2, column=1, padx=8, pady=2)

        def ok(_=None):
            labels.set(n["id"], nv.get(), note.get("1.0", "end"))
            self.tree.item(sel[0], text=self.node_text(n))
            top.destroy()
        row = ttk.Frame(top)
        row.grid(row=3, column=0, columnspan=2, pady=8)
        ttk.Button(row, text="OK", command=ok).pack(side="left", padx=4)
        ttk.Button(row, text=tr("lb.cancel"), command=top.destroy).pack(side="left", padx=4)
        ent.focus_set()
        top.bind("<Return>", ok)
        top.bind("<Escape>", lambda e: top.destroy())

    def export_labels(self):
        import labels
        p = filedialog.asksaveasfilename(title=tr("lb.export"), defaultextension=".json", initialfile="labels.json", filetypes=[("JSON", "*.json")])
        if p:
            self.say(tr("lb.exported", labels.export_user(p), p))

    def import_labels(self):
        import labels
        p = filedialog.askopenfilename(title=tr("lb.import"), filetypes=[("JSON", "*.json")])
        if p:
            n = labels.import_file(p)
            self.say(tr("lb.imported", n))
            if self.core:
                self.fill_tree()

    def on_open_node(self, _):
        iid = self.tree.focus()
        n = self.node_of.get(iid)
        if n and n["lazy"] and not n.get("_loaded"):
            n["_loaded"] = True
            self.tree.delete(*self.tree.get_children(iid))
            try:
                kids = n["lazy"]()
            except Exception as e:
                messagebox.showerror(tr("error"), str(e))
                return
            for c in kids:
                self._insert(iid, c)

    def show_anim(self, aid):
        """Tree node "Animaciones": Tombi's animation in its own player window (tools/anim_player.py)."""
        import anim_player
        if not getattr(self, "anim_win", None) or not self.anim_win.alive():
            self.anim_win = anim_player.AnimWindow(self.root, self.core.anim_data())
        self.anim_win.show(aid)

    def show_model(self, item):
        """Tree node "Modelos 3D": interactive 3D viewport in the centre panel (tools/model_viewer.py)."""
        import model_viewer
        if getattr(self, "mview", None) is None:
            self.mview = model_viewer.ModelViewer(self.mid, say=self.say)
        if not self.mview.winfo_ismapped():
            self.tb.pack_forget()
            self.cf.pack_forget()
            self.mview.pack(fill="both", expand=True)
        self.mview.show(*item[1:])

    def hide_model(self):
        if getattr(self, "mview", None) is not None and self.mview.winfo_ismapped():
            self.mview.pack_forget()
            self.tb.pack(fill="x")
            self.cf.pack(fill="both", expand=True)

    def on_select(self, _):
        sel = self.tree.selection()
        n = self.node_of.get(sel[0]) if sel else None
        if n and n["item"] and n["item"][0] == "model3d":
            try:
                self.show_model(n["item"])
            except Exception as e:
                messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())
            return
        self.hide_model()
        if n and n["item"] and n["item"][0] == "anim":
            try:
                self.show_anim(n["item"][1])
            except Exception as e:
                messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())
        elif n and n["item"] and n["item"][0] == "composed":
            self.show_composed(n["item"])
        elif n and n["item"]:
            try:
                self.show(n["item"])
            except Exception as e:
                self.say(str(e))
                messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())

    # ---- composed room views (tools/gfx_composed.py: the game runs in the MIPS interpreter, results cached)
    def show_composed(self, item):
        import gfx_composed as GC
        _, area, sec, kind = item
        img = self.core.composed_image()
        if not img:
            messagebox.showinfo(tr("info"), tr("d.composedimg"))
            return
        if GC.cached(img, area, sec, "composed"):
            if kind == "sky" and not GC.cached(img, area, sec, "sky"):
                self.say(tr("l.nosky"))
                return
            self.show(item)
        else:
            self.generate_composed(area, sec, then=lambda: self.show_composed(item))

    def composed_all(self):
        import gfx_composed as GC
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return
        img = self.core.composed_image()
        if not img:
            messagebox.showinfo(tr("info"), tr("d.composedimg"))
            return
        todo = GC.missing(img)
        if not todo:
            self.say(tr("l.composeddone", 0, GC.cache_dir(img)))
            return
        if messagebox.askyesno(tr("info"), tr("d.composedall", len(todo), GC.cache_dir(img))):
            self.generate_composed(None, None)

    def generate_composed(self, area, sec, then=None):
        """Captures one room (area, sec) or all the missing ones in a worker thread, with progress in the status bar."""
        import gfx_composed as GC
        try:
            import numpy  # noqa: F401
        except ImportError as e:
            messagebox.showerror(tr("error"), tr("d.composednp", e))
            return
        img, exe = self.core.composed_image(), self.core.composed_exe()
        res, prog = {}, [0, 1]
        self.lock(True)

        def job():
            try:
                res["r"] = GC.generate(img, exe, area, sec, jobs=1 if area is not None else min(4, os.cpu_count() or 1),
                                       progress=lambda d, t: prog.__setitem__(slice(0, 2), [d, t]))
            except Exception:
                res["err"] = traceback.format_exc()
            res["done"] = True

        def tick():
            if not res.get("done"):
                self.say(tr("l.composing", *prog))
                self.root.after(300, tick)
                return
            self.lock(False)
            bad = [r for r in res.get("r", []) if r[2] == "fail"]
            if "err" in res or bad:
                messagebox.showerror(tr("error"), res["err"] if "err" in res else tr("d.composedfail", len(bad), "\n".join("AREA%02d:%d %s" % (a, s, e) for a, s, _, e in bad[:8])))
            else:
                self.say(tr("l.composeddone", len(res.get("r", [])), GC.cache_dir(img)))
            if then and "err" not in res:
                then()
        threading.Thread(target=job, daemon=True).start()
        tick()

    # ---- showing
    def show(self, item, keep=False):
        self.item = item
        self.surf = self.core.surface_for(item)
        if not keep:
            self.region = None
        s = self.surf
        if s.selected and not keep:
            self.region = None
        self.fill_palettes()
        self.redraw()
        self.draw_swatches()
        self.update_sel()
        if s.pal_note:
            self.say(s.pal_note)

    def edit_ref(self):
        s = self.surf
        if not s:
            return None
        if s.is_clut:
            p = s.pageinfo
            row = getattr(s, "row", 0)
            return G.PalRef(p["name"], row * p["w"], p["w"] if p["w"] <= 256 else 16)
        return s.pal

    def fill_palettes(self):
        self.pal_list.delete(*self.pal_list.get_children())
        self.pal_refs, self._pal_imgs, self._lazy = {}, [], {}
        s = self.surf
        self.update_pal_now()
        if s.bpp == 16:
            return
        cur = None
        for gl, items in self.core.palette_groups(s):
            g = self.pal_list.insert("", "end", text=gl, open=False)
            if callable(items):
                self._lazy[g] = items
                self.pal_list.insert(g, "end", text="…")
                continue
            for label, ref in items:
                iid = self.pal_list.insert(g, "end", text=label, image=self.strip(ref))
                self.pal_refs[iid] = ref
                if ref == s.pal and cur is None:
                    cur = iid
        if s.pal and cur is None:
            g = self.pal_list.insert("", 0, text=tr("pg.inferred"), open=True)
            cur = self.pal_list.insert(g, "end", text=self.core._ref_label(s.pal), image=self.strip(s.pal))
            self.pal_refs[cur] = s.pal
        if cur:
            self.pal_list.item(self.pal_list.parent(cur), open=True)
            self._select_quiet(cur)

    def _select_quiet(self, iid):
        self._quiet = True
        self.pal_list.selection_set(iid)
        self.pal_list.see(iid)
        self.root.after(50, lambda: setattr(self, "_quiet", False))

    def on_open_group(self, _):
        g = self.pal_list.focus()
        fn = self._lazy.pop(g, None)
        if not fn:
            return
        self.pal_list.delete(*self.pal_list.get_children(g))
        for item in fn():
            if isinstance(item[1], list):       # (file, rows): a sub-group filled when opened
                f = self.pal_list.insert(g, "end", text=item[0], open=False)
                self._lazy[f] = (lambda rows=item[1]: rows)
                self.pal_list.insert(f, "end", text="…")
            else:
                label, ref = item
                iid = self.pal_list.insert(g, "end", text=label, image=self.strip(ref))
                self.pal_refs[iid] = ref

    def strip(self, ref):
        cols = self.core.pal_rgb(ref)
        step = 1 if len(cols) <= 16 else 4
        cols = cols[::step]
        im = Image.new("RGB", (len(cols) * 6, 10))
        for i, c in enumerate(cols):
            im.paste(tuple(c), (i * 6, 0, i * 6 + 6, 10))
        ph = ImageTk.PhotoImage(im)
        self._pal_imgs.append(ph)
        return ph

    def update_pal_now(self):
        s = self.surf
        self.pal_now.delete("all")
        if not s or s.bpp == 16 or not s.pal:
            self.pal_name.set("")
            return
        cols = self.core.pal_rgb(s.pal)[:: (1 if s.pal.n <= 16 else 16)]
        self.pal_now.update_idletasks()
        w = max(self.pal_now.winfo_width(), 100) / len(cols)
        for i, c in enumerate(cols):
            self.pal_now.create_rectangle(i * w, 0, (i + 1) * w, 14, fill="#%02x%02x%02x" % tuple(c), outline="")
        self.pal_name.set(tr("l.nowpal", self.core._ref_label(s.pal) + (tr("l.autotag") if s.pal == s.inferred else "")))

    def on_pick_palette(self, _):
        if getattr(self, "_quiet", False) or not self.surf:
            return
        sel = self.pal_list.selection()
        if not sel:
            return
        if sel[0] in self.pal_refs:
            self.surf.choose(self.pal_refs[sel[0]])
            self.redraw()
            self.draw_swatches()
            self.update_pal_now()

    def cycle_palette(self, d):
        if not self.surf or self.surf.bpp == 16 or not self.pal_refs:
            return "break"
        order = [i for i in self._leaves() if i in self.pal_refs]
        sel = self.pal_list.selection()
        k = order.index(sel[0]) if sel and sel[0] in order else -1
        iid = order[(k + d) % len(order)]
        self.pal_list.item(self.pal_list.parent(iid), open=True)
        self.pal_list.selection_set(iid)
        self.pal_list.see(iid)
        return "break"

    def _leaves(self, p=""):
        out = []
        for c in self.pal_list.get_children(p):
            out += self._leaves(c) if self.pal_list.get_children(c) else [c]
        return out

    def redraw(self):
        s = self.surf
        if not s:
            return
        im = s.render()
        W, H = im.size
        bg = checker(W * self.scale, H * self.scale, 8).convert("RGBA") if not s.opaque else Image.new("RGBA", (W * self.scale, H * self.scale))
        im = im.resize((W * self.scale, H * self.scale), Image.NEAREST)
        bg.alpha_composite(im)
        self._photo = ImageTk.PhotoImage(bg.convert("RGB"))
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, image=self._photo, anchor="nw")
        self.canvas.configure(scrollregion=(0, 0, W * self.scale, H * self.scale))
        self.draw_overlay()

    def draw_overlay(self):
        self.canvas.delete("ov")
        s = self.surf
        if not s:
            return
        k = self.scale
        if self.show_rects:
            for r in s.rects:
                sel = r is s.selected
                self.canvas.create_rectangle(r.x * k, r.y * k, (r.x + r.w) * k, (r.y + r.h) * k, outline="#ffee00" if sel else "#00e0ff",
                                             width=2 if sel else 1, tags="ov")
                self.canvas.create_text(r.x * k + 2, r.y * k + 1, text=r.label, anchor="nw", fill="#ffffff", font=("TkDefaultFont", 7), tags="ov")
        if self.region:
            x, y, w, h = self.region
            self.canvas.create_rectangle(x * k, y * k, (x + w) * k, (y + h) * k, outline="#ff40ff", dash=(4, 3), width=2, tags="ov")

    def update_sel(self):
        s = self.surf
        if self.region:
            self.sel_var.set(tr("l.sel", "%d,%d %dx%d" % tuple(self.region)))
        elif s and s.selected:
            r = s.selected
            self.sel_var.set(tr("l.sel", "%s  %d,%d %dx%d" % (r.label, r.x, r.y, r.w, r.h)))
        else:
            self.sel_var.set(tr("l.nosel"))

    def set_zoom(self):
        self.scale = int(self.zoom.get()[1:])
        self.redraw()

    def toggle_rects(self):
        self.show_rects = self.rects_var.get()
        self.draw_overlay()

    # ---- canvas mouse
    def _px(self, e):
        return int(self.canvas.canvasx(e.x) // self.scale), int(self.canvas.canvasy(e.y) // self.scale)

    def on_press(self, e):
        s = self.surf
        if not s:
            return
        x, y = self._px(e)
        self._drag0 = (x, y)
        if s.is_clut:
            s.row = max(0, min(s.H - 1, y))
            self.sw_sel = max(0, min(s.W - 1, x))
            self.draw_swatches()
            self.show_colour()
            return
        r = s.hit(x, y) if self.show_rects else None
        if r:
            s.selected = r
            self.region = None
            if r.pal:
                s.pal = r.pal
                self.fill_palettes()
                self.redraw()
                self.draw_swatches()
        else:
            s.selected = None
            self.region = None
        self.draw_overlay()
        self.update_sel()

    def on_drag(self, e):
        s = self.surf
        if not s or s.is_clut or getattr(self, "_drag0", None) is None or (s.selected and self.show_rects):
            return
        x0, y0 = self._drag0
        x1, y1 = self._px(e)
        x1, y1 = max(0, min(s.W - 1, x1)), max(0, min(s.H - 1, y1))
        a, b = min(x0, x1), min(y0, y1)
        self.region = (a, b, abs(x1 - x0) + 1, abs(y1 - y0) + 1)
        self.draw_overlay()
        self.update_sel()

    def on_release(self, e):
        self._drag0 = None

    # ---- palette editing
    def draw_swatches(self):
        c = self.swatch
        c.delete("all")
        ref = self.edit_ref()
        if not ref or not self.core:
            self.col_var.set("")
            return
        cols = self.core.pal_rgb(ref)
        per = 16
        for i, rgb in enumerate(cols[:256]):
            x, y = (i % per) * 18, (i // per) * 18
            c.create_rectangle(x + 1, y + 1, x + 17, y + 17, fill="#%02x%02x%02x" % tuple(rgb), outline="#000" if i != self.sw_sel else "#ff0", width=1 if i != self.sw_sel else 2)
            if self.core.pal_words(G.PalRef(ref.file, ref.off + i, 1))[0] == 0:
                c.create_line(x + 2, y + 16, x + 16, y + 2, fill="#f00")
        self.show_colour()

    def show_colour(self):
        ref = self.edit_ref()
        if not ref:
            return
        i = min(self.sw_sel, ref.n - 1)
        w = self.core.pal_words(ref)[i]
        self.col_var.set(tr("l.colour", i, w, "%d,%d,%d" % G.word_to_rgb(w), 1 if w & 0x8000 else 0))

    def on_swatch(self, e):
        ref = self.edit_ref()
        if not ref:
            return
        i = (e.y // 18) * 16 + e.x // 18
        if i >= ref.n:
            return
        self.sw_sel = i
        self.show_colour()
        self.draw_swatches()
        old = self.core.pal_rgb(ref)[i]
        res = colorchooser.askcolor(color="#%02x%02x%02x" % tuple(old), title=tr("l.edit"))
        if not res or not res[0] or not self.confirm_shared(ref):
            return
        self.core.set_color(ref, i, tuple(int(v) for v in res[0]))
        self.after_edit()

    def make_transparent(self):
        ref = self.edit_ref()
        if ref and self.confirm_shared(ref):
            self.core.set_word(ref, min(self.sw_sel, ref.n - 1), 0)
            self.after_edit()

    def confirm_shared(self, ref):
        """Warns (once per palette file) when other images use the same CLUT file."""
        if ref.file in self.warned:
            return True
        lists, pages = self.core.clut_users(ref.file)
        cur = self.item[1] if self.item and self.item[0] == "page" else None
        others = [p.split("/")[-1] for p in pages if p != cur]
        if not others and len(lists) <= 1:
            return True
        names = ", ".join(others[:15]) + (" …" if len(others) > 15 else "")
        ll = ", ".join("%s:%d" % (self.core.area_name(a), b) for a, b in lists[:12]) + (" …" if len(lists) > 12 else "")
        if not messagebox.askokcancel(tr("warning"), tr("d.shared", ref.file, names or "-", ll)):
            return False
        self.warned.add(ref.file)
        return True

    def after_edit(self):
        self.redraw()
        self.draw_swatches()
        self.fill_palettes()
        self.refresh_status()

    def revert(self):
        ref = self.edit_ref()
        if not self.core or not ref:
            return
        if messagebox.askyesno(tr("warning"), tr("d.revertq", ref.file)):
            self.core.revert(ref.file)
            self.after_edit()

    # ---- actions
    def need_surface(self):
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return None
        if not self.surf:
            messagebox.showinfo(tr("info"), tr("d.pickfirst"))
            return None
        return self.surf

    def target_region(self, sel):
        s = self.surf
        if not sel:
            return None, tr("l.nosel").split(": ")[-1]
        if self.region:
            return tuple(self.region), "%d,%d %dx%d" % tuple(self.region)
        if s.selected:
            r = s.selected
            return (r.x, r.y, r.w, r.h), r.label
        return "none", ""

    def export(self, sel):
        s = self.need_surface()
        if not s:
            return
        reg, label = self.target_region(sel)
        if reg == "none":
            messagebox.showinfo(tr("info"), tr("d.nonehere"))
            return
        base = os.path.basename(s.title).replace("/", "_").rsplit(".", 1)[0]
        p = filedialog.asksaveasfilename(title=tr("d.savepng"), defaultextension=".png", initialfile=base + ".png", filetypes=[(tr("d.pngfiles"), "*.png")])
        if p:
            try:
                s.export_png(p, reg, s.pal)
                self.say(p)
            except Exception as e:
                messagebox.showerror(tr("error"), str(e))

    def import_(self, sel):
        s = self.need_surface()
        if not s:
            return
        reg, label = self.target_region(sel)
        if reg == "none":
            messagebox.showinfo(tr("info"), tr("d.nonehere"))
            return
        p = filedialog.askopenfilename(title=tr("d.openpng"), filetypes=[(tr("d.pngfiles"), "*.png")])
        if not p:
            return
        try:
            ref = s.selected.pal if (sel and s.selected and not self.region and s.selected.pal) else s.pal
            rep = s.import_png(p, reg, ref)
        except Exception as e:
            messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())
            return
        extra = tr("d.direct") if rep["direct"] else tr("d.quant", rep["transparent"])
        msg = tr("d.imported", s.title, rep["colours"], rep["limit"], extra)
        if rep["warnings"]:
            messagebox.showwarning(tr("warning"), msg + "\n\n" + "\n".join(rep["warnings"]))
        self.say(msg.replace("\n", " "))
        self.redraw()
        self.refresh_status()

    def undo(self):
        if self.core and self.core.undo():
            if self.surf:
                self.show(self.item, keep=True)
            self.refresh_status()

    def preview(self):
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return
        it = self.item
        uid = None
        if it and it[0] == "ui":
            uid = it[1]
        elif not (it and it[0] == "screen" and it[1] == "try_again"):
            messagebox.showinfo(tr("info"), tr("d.nopreview"))
            return
        im = self.core.preview_context(uid if uid in G.UI_SCREEN_POS else None)
        top = tk.Toplevel(self.root)
        top.title(tr("d.preview"))
        im = im.resize((640, 480), Image.NEAREST).convert("RGB")
        top._ph = ImageTk.PhotoImage(im)
        tk.Label(top, image=top._ph).pack()

    def help(self):
        messagebox.showinfo(tr("b.help"), tr("help"))

    # ---- mods and disc
    def save_mod(self):
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return
        if not self.core.edits:
            messagebox.showinfo(tr("info"), tr("d.nochanges"))
            return
        mods = os.path.join(os.getcwd(), "mods")
        p = filedialog.asksaveasfilename(title=tr("d.modname"), initialdir=mods if os.path.isdir(mods) else None, initialfile="mymod",
                                         confirmoverwrite=False)
        if not p:
            return
        try:
            files = self.core.save_mod(p, os.path.basename(p))
        except Exception as e:
            messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())
            return
        self.saved_edits = len(self.core.edits)
        messagebox.showinfo(tr("info"), tr("d.modsaved", p, ", ".join(files), os.path.basename(p)))

    def load_mod(self):
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return
        p = filedialog.askdirectory(title=tr("b.loadmod"))
        if not p:
            return
        try:
            n = self.core.load_mod(p)
        except Exception as e:
            messagebox.showerror(tr("error"), str(e) if isinstance(e, G.CoreError) else traceback.format_exc())
            return
        if self.surf:
            self.show(self.item, keep=True)
        self.refresh_status()
        messagebox.showinfo(tr("info"), tr("d.modloaded", n))

    def generate(self):
        if not self.core:
            messagebox.showinfo(tr("info"), tr("d.openfirst"))
            return
        if not self.core.edits:
            messagebox.showinfo(tr("info"), tr("d.nochanges"))
            return
        try:
            self.core.changed_files()
        except G.CoreError as e:
            messagebox.showerror(tr("error"), str(e))
            return
        out = filedialog.asksaveasfilename(title=tr("d.geniso"), defaultextension=".bin", initialfile="Tombi-mod.bin", filetypes=[(tr("d.isoimg"), "*.bin")])
        if not out:
            return
        work = os.path.join(os.path.dirname(os.path.abspath(out)), "tombi-gfx-work")
        self.lock(True)
        self.say(tr("d.generating"))
        res = {}

        def job():
            try:
                if not self.core.have_mkpsxiso() or not self.core.disc.image:
                    d = os.path.splitext(out)[0] + "_files"
                    res["files"] = self.core.write_files(d)
                    res["dir"] = d
                else:
                    res["files"] = self.core.build_disc(out, work)
            except Exception as e:
                res["err"] = str(e) if isinstance(e, G.CoreError) else traceback.format_exc()
            res['done'] = True

        def done():
            self.lock(False)
            if "err" in res:
                self.say(tr("d.isofail"))
                messagebox.showerror(tr("error"), res["err"])
            elif "dir" in res:
                self.say(res["dir"])
                messagebox.showinfo(tr("d.fallbacktitle"), tr("d.fallback", len(res["files"]), res["dir"]))
            else:
                self.say(tr("d.isodone", out))
                messagebox.showinfo(tr("info"), tr("d.isook", out, ", ".join(res["files"]), work))
        threading.Thread(target=job, daemon=True).start()
        self.poll(res, done)

    def on_close(self):
        if self.core and len(self.core.edits) and len(self.core.edits) != self.saved_edits:
            if not messagebox.askyesno(tr("warning"), tr("d.unsaved", len(self.core.edits))):
                return
        if self.core:
            self.core.disc.close()
        self.root.destroy()


def main():
    import multiprocessing
    multiprocessing.freeze_support()      # the composed room views use worker processes (frozen exe: tools/gfx_composed.py)
    root = tk.Tk()
    App(root, sys.argv[1] if len(sys.argv) > 1 else None)
    root.mainloop()


if __name__ == "__main__":
    main()
