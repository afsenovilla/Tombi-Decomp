#!/usr/bin/env python3
"""Tombi Save Editor: a PKHeX-style editor for Tombi! / Tomba! memory card saves (docs/SAVE_EDITOR.md).

Open a card image (.mcd/.mcr/.gme/.vgs/.mcs, the native port's tombi_mc0.mcd), pick a save, edit, "Save as".  Item
and event names are read at run time from the user's own disc image or extracted data folder (button "Game data"):
nothing of the game is shipped with the editor.  Without it the editor shows ids.  The library is tools/tombi_save.py.
"""
import json, os, sys
import tkinter as tk
from tkinter import ttk, filedialog, messagebox

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tombi_save as T

APP = "Tombi Save Editor"
CFG = os.path.join(os.path.expanduser("~"), ".tombi_save_editor.json")
AREAS = {0: "Village of All Beginnings", 1: "Forest / Watch Tower", 2: "Caves / tunnels", 3: "Phoenix Mountain",
         4: "Trick Village area", 5: "Baccus Village", 6: "Dirt Motocross", 7: "(7)", 8: "Baccus Lake",
         9: "Mushroom Village", 10: "Deep Jungle", 11: "Lumberjack Town / Factory", 12: "(12)", 13: "Pig Island",
         14: "Evil Pigs", 15: "(15)", 16: "Stonetown / Clock Tower", 17: "Iron Town / Iron Castle",
         18: "Civilization Town", 19: "Dwarf Village"}
STATES = ["0 not started", "started", "255 completed"]

UI = {
    "en": dict(open="Open card…", saveas="Save as…", data="Game data…", lang="Names", slot="Save", player="Player",
               items="Items", events="Events", misc="Misc", location="Location", nodata="No game data: names shown as ids",
               cut="cut/unused", untitled="no title", warnloc="Changing the area/section moves where Continue starts;"
                                                                  " pick a room you already reached.",
               state="State", title="Title", saved="Written: {0}", err="Error", nosave="No Tombi save on this card",
               qty="Qty", have="Have", filter="Filter", hint="Double-click an event to cycle its state; type a raw value below.",
               raw="Raw value", setraw="Set", time="Play time (h:m:s)", checksum="Checksum"),
    "es": dict(open="Abrir tarjeta…", saveas="Guardar como…", data="Datos del juego…", lang="Nombres", slot="Partida",
               player="Jugador", items="Objetos", events="Eventos", misc="Varios", location="Lugar",
               nodata="Sin datos del juego: se muestran ids", cut="recortado/sin uso", untitled="sin título",
               warnloc="Cambiar área/sección mueve dónde empieza Continuar; elige una sala ya visitada.",
               state="Estado", title="Título", saved="Guardado: {0}", err="Error", nosave="No hay partidas de Tombi en esta tarjeta",
               qty="Cant.", have="Tiene", filter="Filtro", hint="Doble clic en un evento cambia su estado; abajo, valor en bruto.",
               raw="Valor", setraw="Poner", time="Tiempo de juego (h:m:s)", checksum="Suma"),
}
PLAYER_FIELDS = ["ap", "max_health", "health", "lives", "level", "red_level", "green_level", "blue_level", "red_bar",
                 "green_bar", "blue_bar", "golden_bowl", "bonus_health"]
LABELS = {"ap": "AP", "max_health": "Max HP / vitalidad", "health": "HP", "lives": "Lives / vidas", "level": "Level (title)",
          "red_level": "Red exp level", "green_level": "Green exp level", "blue_level": "Blue exp level",
          "red_bar": "Red exp bar", "green_bar": "Green exp bar", "blue_bar": "Blue exp bar",
          "golden_bowl": "Golden bowl (HP>8)", "bonus_health": "Bonus hearts", "weapon": "Equipped weapon (raw)",
          "pants": "Equipped pants (raw)", "area": "Area", "section": "Section", "cur_area": "Current area",
          "cur_section": "Current section", "cur_spawn": "Current spawn point", "purified_areas": "Purified areas"}


def load_cfg():
    try:
        return json.load(open(CFG, encoding="utf-8"))
    except Exception:
        return {}


class Editor(tk.Tk):
    def __init__(self, card=None):
        super().__init__()
        self.cfg = load_cfg()
        self.ui = self.cfg.get("ui", "es")
        self.L = UI[self.ui]
        self.title(APP)
        self.geometry("900x640")
        self.card = self.save = None
        self.names = {"items": {}, "events": {}, "untitled": set()}
        self.vars = {}
        self._build()
        if self.cfg.get("data"):
            self.load_names(self.cfg["data"], quiet=True)
        if card:
            self.open_card(card)

    # ---- layout
    def _build(self):
        L = self.L
        top = ttk.Frame(self, padding=4); top.pack(fill="x")
        ttk.Button(top, text=L["open"], command=self.ask_open).pack(side="left")
        ttk.Button(top, text=L["saveas"], command=self.ask_save).pack(side="left", padx=4)
        ttk.Label(top, text=L["slot"]).pack(side="left", padx=(12, 2))
        self.slot_cb = ttk.Combobox(top, state="readonly", width=60)
        self.slot_cb.pack(side="left")
        self.slot_cb.bind("<<ComboboxSelected>>", lambda e: self.pick(self.slot_cb.current()))
        bar = ttk.Frame(self, padding=(4, 0)); bar.pack(fill="x")
        ttk.Button(bar, text=L["data"], command=self.ask_data).pack(side="left")
        ttk.Label(bar, text=L["lang"]).pack(side="left", padx=(8, 2))
        self.lang_cb = ttk.Combobox(bar, state="readonly", width=12, values=["%s %s" % kv for kv in T.LANGS.items()])
        lang = self.cfg.get("lang", "S")
        self.lang_cb.current(list(T.LANGS).index(lang) if lang in T.LANGS else 0)
        self.lang_cb.pack(side="left")
        self.lang_cb.bind("<<ComboboxSelected>>", lambda e: self.cfg.get("data") and self.load_names(self.cfg["data"]))
        self.status = ttk.Label(bar, text=L["nodata"], foreground="#666"); self.status.pack(side="left", padx=8)
        self.nb = ttk.Notebook(self); self.nb.pack(fill="both", expand=True, padx=4, pady=4)
        self.tab_player = ttk.Frame(self.nb, padding=8); self.nb.add(self.tab_player, text=L["player"])
        self.tab_items = ttk.Frame(self.nb, padding=4); self.nb.add(self.tab_items, text=L["items"])
        self.tab_events = ttk.Frame(self.nb, padding=4); self.nb.add(self.tab_events, text=L["events"])
        self.tab_misc = ttk.Frame(self.nb, padding=8); self.nb.add(self.tab_misc, text=L["misc"])
        self._build_player(); self._build_items(); self._build_events(); self._build_misc()

    def _spin(self, parent, key, row, col=0):
        lo, hi = T.FIELDS[key][2:4]
        ttk.Label(parent, text=LABELS.get(key, key)).grid(row=row, column=col, sticky="w", padx=4, pady=2)
        v = tk.StringVar()
        ttk.Spinbox(parent, from_=lo, to=hi, textvariable=v, width=12).grid(row=row, column=col + 1, sticky="w")
        ttk.Label(parent, text="%d..%d" % (lo, hi), foreground="#888").grid(row=row, column=col + 2, sticky="w", padx=4)
        self.vars[key] = v

    def _build_player(self):
        f = self.tab_player
        for i, k in enumerate(PLAYER_FIELDS):
            self._spin(f, k, i % 7, (i // 7) * 3)
        r = 8
        for k in ("weapon", "pants"):
            self._spin(f, k, r)
            r += 1
        lf = ttk.LabelFrame(f, text=self.L["location"], padding=6)
        lf.grid(row=r + 1, column=0, columnspan=6, sticky="we", pady=8)
        ttk.Label(lf, text=LABELS["area"]).grid(row=0, column=0, sticky="w")
        self.area_cb = ttk.Combobox(lf, state="readonly", width=34, values=["%02d %s" % kv for kv in AREAS.items()])
        self.area_cb.grid(row=0, column=1, sticky="w")
        self._spin(lf, "section", 1)
        self._spin(lf, "cur_spawn", 2)
        ttk.Label(lf, text=self.L["warnloc"], foreground="#a40", wraplength=600).grid(row=3, column=0, columnspan=4, sticky="w")

    def _build_items(self):
        top = ttk.Frame(self.tab_items); top.pack(fill="x")
        ttk.Label(top, text=self.L["filter"]).pack(side="left")
        self.item_filter = tk.StringVar()
        e = ttk.Entry(top, textvariable=self.item_filter, width=30); e.pack(side="left", padx=4)
        self.item_filter.trace_add("write", lambda *a: self.fill_items())
        self.show_cut = tk.BooleanVar(value=True)
        ttk.Checkbutton(top, text=self.L["cut"], variable=self.show_cut, command=self.fill_items).pack(side="left", padx=8)
        cv = tk.Canvas(self.tab_items, highlightthickness=0)
        sb = ttk.Scrollbar(self.tab_items, orient="vertical", command=cv.yview)
        self.items_frame = ttk.Frame(cv)
        self.items_frame.bind("<Configure>", lambda e: cv.configure(scrollregion=cv.bbox("all")))
        cv.create_window((0, 0), window=self.items_frame, anchor="nw")
        cv.configure(yscrollcommand=sb.set)
        cv.pack(side="left", fill="both", expand=True); sb.pack(side="right", fill="y")
        cv.bind_all("<MouseWheel>", lambda e: cv.yview_scroll(int(-e.delta / 120), "units"))
        self.item_vars = {i: (tk.BooleanVar(), tk.StringVar(value="0")) for i in range(T.ITEM_COUNT)}

    def item_name(self, i):
        n = self.names["items"].get(i)
        return n if n else "#%d" % i

    def fill_items(self):
        for w in self.items_frame.winfo_children():
            w.destroy()
        L, flt = self.L, self.item_filter.get().lower()
        n_items = max(len(self.names["items"]), 0xAB)
        ttk.Label(self.items_frame, text="id").grid(row=0, column=0)
        ttk.Label(self.items_frame, text=L["have"]).grid(row=0, column=1)
        ttk.Label(self.items_frame, text=L["qty"]).grid(row=0, column=2)
        r = 1
        for i in range(n_items):
            nm = self.item_name(i)
            cut = i in T.UNUSED_ITEMS
            if (flt and flt not in nm.lower() and flt != str(i)) or (cut and not self.show_cut.get()):
                continue
            have, qty = self.item_vars[i]
            ttk.Label(self.items_frame, text=str(i), width=4).grid(row=r, column=0, sticky="e")
            ttk.Checkbutton(self.items_frame, variable=have).grid(row=r, column=1)
            ttk.Spinbox(self.items_frame, from_=0, to=255, textvariable=qty, width=5).grid(row=r, column=2)
            ttk.Label(self.items_frame, text=nm + ("   (%s)" % L["cut"] if cut else ""),
                      foreground="#a40" if cut else "").grid(row=r, column=3, sticky="w", padx=6)
            r += 1

    def _build_events(self):
        f = self.tab_events
        cols = ("id", "title", "state")
        self.ev = ttk.Treeview(f, columns=cols, show="headings", height=20)
        for c, w, t in zip(cols, (50, 520, 180), ("id", self.L["title"], self.L["state"])):
            self.ev.heading(c, text=t); self.ev.column(c, width=w, anchor="w")
        sb = ttk.Scrollbar(f, orient="vertical", command=self.ev.yview)
        self.ev.configure(yscrollcommand=sb.set)
        bot = ttk.Frame(f); bot.pack(side="bottom", fill="x")
        self.ev.pack(side="left", fill="both", expand=True); sb.pack(side="left", fill="y")
        ttk.Label(bot, text=self.L["hint"]).pack(side="left")
        self.ev_raw = tk.StringVar()
        ttk.Button(bot, text=self.L["setraw"], command=self.set_event_raw).pack(side="right")
        ttk.Entry(bot, textvariable=self.ev_raw, width=6).pack(side="right")
        ttk.Label(bot, text=self.L["raw"]).pack(side="right", padx=4)
        for s in STATES:
            ttk.Button(bot, text=s.split(" ", 1)[-1], command=lambda s=s: self.set_event_state(s)).pack(side="right", padx=2)
        self.ev.bind("<Double-1>", lambda e: self.cycle_event())
        self.ev.tag_configure("cut", foreground="#a40")
        self.ev.tag_configure("done", foreground="#070")

    def ev_text(self, i):
        v = self.save.event(i)
        return "%d %s" % (v, "completed" if v == 255 else "started" if v else "not started")

    def fill_events(self):
        self.ev.delete(*self.ev.get_children())
        if not self.save:
            return
        for i in range(T.EVENT_USED):
            t = self.names["events"].get(i)
            cut = i in T.UNUSED_EVENTS
            if not t:
                t = "#%d" % i if not self.names["events"] else "(%s)" % self.L["untitled"]
            if cut:
                t += "  [%s]" % self.L["cut"]
            v = self.save.event(i)
            self.ev.insert("", "end", iid=str(i), values=(i, t, self.ev_text(i)),
                           tags=("cut",) if cut else ("done",) if v == 255 else ())

    def _sel_events(self):
        return [int(x) for x in self.ev.selection()]

    def cycle_event(self):
        for i in self._sel_events():
            v = self.save.event(i)
            self.save.set_event(i, 1 if v == 0 else 255 if v != 255 else 0)
            self.ev.set(str(i), "state", self.ev_text(i))

    def set_event_state(self, s):
        v = {0: 0, 1: 1, 2: 255}[STATES.index(s)]
        for i in self._sel_events():
            self.save.set_event(i, v)
            self.ev.set(str(i), "state", self.ev_text(i))

    def set_event_raw(self):
        try:
            v = int(self.ev_raw.get(), 0)
            assert 0 <= v <= 255
        except Exception:
            messagebox.showerror(self.L["err"], "0..255"); return
        for i in self._sel_events():
            self.save.set_event(i, v)
            self.ev.set(str(i), "state", self.ev_text(i))

    def _build_misc(self):
        f = self.tab_misc
        ttk.Label(f, text=self.L["time"]).grid(row=0, column=0, sticky="w")
        self.time_var = tk.StringVar()
        ttk.Entry(f, textvariable=self.time_var, width=14).grid(row=0, column=1, sticky="w")
        ttk.Label(f, text="max 999:59:59", foreground="#888").grid(row=0, column=2, sticky="w", padx=4)
        self._spin(f, "purified_areas", 1)
        self._spin(f, "cur_area", 2)
        self._spin(f, "cur_section", 3)
        self.info = tk.Text(f, height=12, width=100, font=("TkFixedFont", 9))
        self.info.grid(row=5, column=0, columnspan=4, sticky="we", pady=8)

    # ---- data
    def ask_data(self):
        p = filedialog.askopenfilename(title="Tombi disc image (.bin/.cue/.iso) — Cancel to pick an extracted folder",
                                       filetypes=[("Disc image", "*.bin *.cue *.iso *.img"), ("All", "*.*")])
        if not p:
            p = filedialog.askdirectory(title="Extracted game folder (with AREA00, EXE)")
        if p:
            self.load_names(p)

    def lang(self):
        return list(T.LANGS)[self.lang_cb.current()]

    def load_names(self, path, quiet=False):
        try:
            self.names = T.names_from_disc(path, self.lang())
            self.cfg["data"], self.cfg["lang"] = path, self.lang()
            self.save_cfg()
            self.status.config(text="%s  (%d items, %d events)" % (os.path.basename(path), len(self.names["items"]),
                                                                   len(self.names["events"])))
        except Exception as e:
            self.names = {"items": {}, "events": {}, "untitled": set()}
            self.status.config(text=self.L["nodata"])
            if not quiet:
                messagebox.showerror(self.L["err"], str(e))
        self.refresh_names()

    def refresh_names(self):
        self.fill_items()
        self.fill_events()
        if self.save:
            self.load_fields()

    def save_cfg(self):
        try:
            json.dump(self.cfg, open(CFG, "w", encoding="utf-8"), indent=1)
        except Exception:
            pass

    # ---- card
    def ask_open(self):
        p = filedialog.askopenfilename(filetypes=[("Memory card", "*.mcd *.mcr *.mc *.srm *.gme *.vgs *.mem *.mcs *.mcb *.ps"),
                                                  ("All", "*.*")])
        if p:
            self.open_card(p)

    def open_card(self, path):
        try:
            self.card = T.Card(path)
            saves = self.card.saves()
        except Exception as e:
            messagebox.showerror(self.L["err"], str(e)); return
        if not saves:
            messagebox.showwarning(APP, self.L["nosave"]); return
        self.saves = saves
        self.slot_cb["values"] = ["%02d  %s  [%s]" % (s.slot, s.title(), s.region) for s in saves]
        self.slot_cb.current(0)
        self.title("%s — %s" % (APP, os.path.basename(path)))
        self.pick(0)

    def pick(self, k):
        if self.save:
            try:
                self.apply()
            except T.SaveError:
                pass
        self.save = self.saves[k]
        self.load_fields()
        self.fill_events()

    def load_fields(self):
        s = self.save
        for k, v in self.vars.items():
            v.set(str(s.get(k)))
        a = s.get("area")
        self.area_cb.current(a if a < len(AREAS) else 0)
        inv = s.inventory()
        for i, (have, qty) in self.item_vars.items():
            have.set(i in inv)
            qty.set(str(s.item(i)))
        t = s.get("time") // 50
        self.time_var.set("%d:%02d:%02d" % (t // 3600, t // 60 % 60, t % 60))
        self.show_info()

    def show_info(self):
        s = self.save
        self.info.delete("1.0", "end")
        self.info.insert("end", "%s\nfile %s  block %d  %s %s\ninventory (%d): %s\n" % (
            s.title(), s.name, s.block, self.L["checksum"], "ok" if s.checksum_ok() else "BAD", len(s.inventory()),
            ", ".join(self.item_name(i) for i in s.inventory())))

    def apply(self):
        """Puts the widgets into the save; raises SaveError with the first bad value."""
        s = self.save
        for k, v in self.vars.items():
            txt = v.get().split(" ")[0]
            try:
                val = int(txt, 0)
            except ValueError:
                raise T.SaveError("%s: '%s' is not a number" % (LABELS.get(k, k), v.get()))
            if k == "max_health":
                continue
            s.set(k, val) if k != "health" else None
        s.set("golden_bowl", int(self.vars["golden_bowl"].get()))
        s.set("max_health", int(self.vars["max_health"].get()))
        if int(self.vars["health"].get()) > s.get("max_health"):
            raise T.SaveError("HP > max HP")
        s.set("health", int(self.vars["health"].get()))
        s.set("area", int(self.area_cb.get().split()[0]))
        try:
            h, m, sec = (int(x) for x in self.time_var.get().split(":"))
            assert 0 <= m < 60 and 0 <= sec < 60 and 0 <= h <= 999
        except Exception:
            raise T.SaveError("time: h:mm:ss")
        cur = s.get("time")
        if cur // 50 != (h * 3600 + m * 60 + sec):
            s.set("time", (h * 3600 + m * 60 + sec) * 50)
        for i, (have, qty) in self.item_vars.items():
            try:
                q = int(qty.get())
            except ValueError:
                raise T.SaveError("item %d: quantity" % i)
            if have.get() and q == 0:
                q = 1
            if not have.get():
                q = 0
            if q != s.item(i) or (q > 0) != (i in s.inventory()):
                s.set_item(i, q)

    def ask_save(self):
        if not self.save:
            return
        try:
            self.apply()
        except T.SaveError as e:
            messagebox.showerror(self.L["err"], str(e)); return
        base, ext = os.path.splitext(self.card.path)
        p = filedialog.asksaveasfilename(initialfile=os.path.basename(base) + "_edit" + ext,
                                         initialdir=os.path.dirname(self.card.path), defaultextension=ext)
        if not p:
            return
        try:
            self.card.save(p)
        except Exception as e:
            messagebox.showerror(self.L["err"], str(e)); return
        self.show_info()
        messagebox.showinfo(APP, self.L["saved"].format(p))


def main():
    Editor(sys.argv[1] if len(sys.argv) > 1 else None).mainloop()


if __name__ == "__main__":
    main()
