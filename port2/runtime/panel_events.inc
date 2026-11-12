/* Second screen panel: the event browser (included by panel.c).  Every event of the pause menu's Events list (SUBSCREEN_EVENT_ORDER, ids up to 0xBF, in its order) with its icon and
 * title from the user's disc, filtered by state (in progress 1..0xFE, done 0xFF, not started 0), scrolled by touch as
 * one strip (panel.c Scroll), and a detail page (event_detail: where, who / hint, Track / Hide).  Where an
 * event happens comes from panel_eventmap.h, generated from the decomp by port2/tools/gen_panel_events.py (ids and
 * coordinates only; docs/EVENTS_MAP.md lists the coverage); unknown places show "location unknown".
 * Input: touch (Android: nativePanelTouch from the Presentation), the mouse in the PC window, and the keyboard of the
 * PC window (arrows, Enter, Backspace, Q / E: tabs, H: hide / show the selected event, P: track it, V: list the hidden ones).
 * Hidden events (the Ocultar plank, a long press on a row, H): left out of the lists, the Status tab's "in progress" box and
 * the map markers until "Ocultos: s\xED" lists them again (dimmed); kept in panel_hidden.txt next to the memory cards
 * (the event ids, one line). */
#include "panel_eventmap.h"
#include "panel_givers.h"

enum { F_PROGRESS, F_DONE, F_PENDING, NFILTERS };
static int map_action(int a), map_touch(int x, int y, int action); static void map_hit(int k);   /* panel_side.inc */
static int ev_filter, ev_sel, ev_detail = -1;

/* ---------------------------------------------------------------- hidden events (panel_hidden.txt) */
static u8 ev_hid[EVENTS]; static int ev_hid_loaded, ev_show_hidden;
extern const char *port_save_file(const char *base, char *out, int n);   /* shims/card.c */
static void ev_hid_load(void) {
    char path[1100]; FILE *f;
    if (ev_hid_loaded) return;
    ev_hid_loaded = 1;
    if (!(f = fopen(port_save_file("panel_hidden.txt", path, sizeof path), "r"))) return;
    int e, n = 0;
    while (fscanf(f, "%d", &e) == 1) if (e >= 0 && e < EVENTS) { ev_hid[e] = 1; n++; }
    fclose(f);
    port_printf("[panel] %d hidden events (%s)\n", n, path);
}
static void ev_hid_save(void) {
    char path[1100]; FILE *f = fopen(port_save_file("panel_hidden.txt", path, sizeof path), "w");
    if (!f) { port_printf("[panel] cannot write %s\n", path); return; }
    for (int e = 0; e < EVENTS; e++) if (ev_hid[e]) fprintf(f, "%d ", e);
    fprintf(f, "\n");
    fclose(f);
}
static int ev_hidden(int e) { ev_hid_load(); return e >= 0 && e < EVENTS && ev_hid[e]; }
static int ev_hidden_now(int e) { return !ev_show_hidden && ev_hidden(e); }   /* left out of what is drawn */
static void ev_hide_toggle(int e) {
    if (e < 0 || e >= EVENTS) return;
    ev_hid_load(); ev_hid[e] = !ev_hid[e]; ev_hid_save();
    port_printf("[panel] event %d %s\n", e, ev_hid[e] ? "hidden" : "shown");
}
/* ---------------------------------------------------------------- pinned events (panel_pins.txt, at most 4, in pin order) */
#define NPINS 4
static int ev_pin[NPINS], ev_npin, ev_pin_loaded;
static void ev_pin_load(void) {
    char path[1100]; FILE *f; int e;
    if (ev_pin_loaded) return;
    ev_pin_loaded = 1;
    if (!(f = fopen(port_save_file("panel_pins.txt", path, sizeof path), "r"))) return;
    while (ev_npin < NPINS && fscanf(f, "%d", &e) == 1) if (e >= 0 && e < EVENTS) ev_pin[ev_npin++] = e;
    fclose(f);
}
static int ev_pinned(int e) { ev_pin_load(); for (int i = 0; i < ev_npin; i++) if (ev_pin[i] == e) return 1; return 0; }
static int ev_pins(int *ids) { ev_pin_load(); for (int i = 0; i < ev_npin; i++) ids[i] = ev_pin[i]; return ev_npin; }
static char pin_msg[64]; static int pin_msg_t;
static void ev_pin_toggle(int e) {
    if (e < 0 || e >= EVENTS) return;
    ev_pin_load();
    int i = 0; while (i < ev_npin && ev_pin[i] != e) i++;
    if (i < ev_npin) { memmove(ev_pin + i, ev_pin + i + 1, (ev_npin - i - 1) * sizeof ev_pin[0]); ev_npin--; }
    else if (ev_npin < NPINS) ev_pin[ev_npin++] = e;
    else { snprintf(pin_msg, sizeof pin_msg, "%s", T("4 events tracked already: unpin one first", "Ya sigues 4 eventos: quita uno antes")); pin_msg_t = port_frame; return; }
    char path[1100]; FILE *f = fopen(port_save_file("panel_pins.txt", path, sizeof path), "w");
    if (f) { for (int k = 0; k < ev_npin; k++) fprintf(f, "%d ", ev_pin[k]); fprintf(f, "\n"); fclose(f); }
    port_printf("[panel] event %d %s (%d pinned)\n", e, ev_pinned(e) ? "pinned" : "unpinned", ev_npin);
}
static int ev_hidden_count(void) { int n = 0; ev_hid_load(); for (int e = 0; e < EVENTS; e++) n += ev_hid[e]; return n; }

static const PanelEventLoc *ev_loc(int e) {
    for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) if (panel_event_locs[i].ev == e) return &panel_event_locs[i];
    return NULL;
}
static int ev_state(int e) { int v = G_EVENT(e); return v == 0xFF ? F_DONE : v ? F_PROGRESS : F_PENDING; }
static int ev_link_state(int link) {   /* the menu's events placed at this room link (or its area, when the room is not known) */
    int st = 0;
    for (int k = 0; k < ev_count(); k++) {
        int e = ev_id(k), f = ev_state(e); const PanelEventLoc *L;
        if (f == F_PENDING || !(L = ev_loc(e)) || L->area >= 20) continue;
        int l = L->sec < 24 ? roomlink[L->area][L->sec] : -1;
        for (int s = 0; s < 24 && l < 0; s++) l = roomlink[L->area][s];
        if (l != link) continue;
        if (f == F_PROGRESS) return 1;
        st = 2;
    }
    return st;
}
/* how far an event is from Tomba: 0 this room, 1 this area, 2 another area, 3 anywhere (an item from the menu), 4 not
   known; the grandfather's bracelet (event 0: in progress all game long) last */
enum { D_HERE, D_AREA, D_AWAY, D_ANY, D_UNKNOWN, D_LAST = 6 };
static int ev_dist(int e) {
    const PanelEventLoc *L = ev_loc(e);
    if (e == 0) return D_LAST;
    if (!L) return D_UNKNOWN;
    if (L->area == 254) return D_ANY;
    if (L->area != G_AREA) return D_AWAY;
    return L->sec == G_SECTION ? D_HERE : D_AREA;
}
static int ev_pinned(int e);
/* nearest first (stable): the tracked ones, then this room by distance to Tomba (PLAYER x), this area, other areas */
static void ev_near_sort(int *ids, int n) {
    int px = (s16)G16(PV_PLAYER_X), key[EVENTS];
    for (int i = 0; i < n; i++) {
        const PanelEventLoc *L = ev_loc(ids[i]); int d = ev_dist(ids[i]);
        key[i] = ev_pinned(ids[i]) ? -1 : d * 100000 + (d == D_HERE ? (L->x == -32768 ? 99999 : abs(L->x - px) < 99999 ? abs(L->x - px) : 99999) : 0);
    }
    for (int i = 1; i < n; i++) for (int j = i; j > 0 && key[j] < key[j - 1]; j--) { int t = key[j]; key[j] = key[j - 1]; key[j - 1] = t; t = ids[j]; ids[j] = ids[j - 1]; ids[j - 1] = t; }
}
static int ev_list(int *ids) {
    int n = 0;
    for (int k = 0; k < ev_count(); k++) { int e = ev_id(k); if (ev_state(e) == ev_filter && (ev_norder || ev_name[e] || ev_loc(e)) && !ev_hidden_now(e)) ids[n++] = e; }
    if (ev_filter == F_PROGRESS) ev_near_sort(ids, n);
    return n;
}
static const char *filter_name(int f) {
    return f == F_PROGRESS ? T("In progress", "En curso") : f == F_DONE ? T("Completed", "Completados") : T("Not started", "Pendientes");
}
static const char *state_name(int f) {
    return f == F_PROGRESS ? T("In progress", "En curso") : f == F_DONE ? T("Completed", "Completado") : T("Not started", "Pendiente");
}
/* the area name of the map screen for (area, section) */
static const char *place_area(int area, int sec) {
    if (area == 254) {   /* no room: completed by using an item from the pause menu (panel_eventmap.h x = the item) */
        static char b[96]; int it = -1;
        for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) if (panel_event_locs[i].area == 254 && panel_event_locs[i].sec == sec) it = panel_event_locs[i].x;
        snprintf(b, sizeof b, T("Use %s (anywhere)", "Usar %s (en cualquier lugar)"), it >= 0 && it < 256 && item_name[it] ? item_name[it] : T("an item", "un objeto"));
        return b;
    }
    if (area == 14) return sec + 14 < 24 ? a14_area[sec + 14] : NULL;
    int l = area < 20 && sec < 24 ? roomlink[area][sec] : -1;
    if (l < 0 && area < 20) for (int s = 0; s < 24 && l < 0; s++) l = roomlink[area][s];
    return l >= 0 && links[l][2] < 12 ? area_name[links[l][2]] : NULL;
}
static const char *place_room(int area, int sec) {
    if (area == 14) return sec < 24 ? a14_room[sec] : NULL;
    int l = area < 20 && sec < 24 ? roomlink[area][sec] : -1;
    return l >= 0 && links[l][2] < 12 && links[l][3] < 24 ? room_name[links[l][2]][links[l][3]] : NULL;
}
static const char *dist_name(int d) {
    switch (d) {
    case D_HERE: return T("Here", "Aqu\xED");
    case D_AREA: return T("This area", "Esta zona");
    case D_AWAY: return T("Elsewhere", "Otra zona");
    case D_ANY: return T("Anywhere", "Cualquiera");
    default: return NULL;
    }
}
/* where an in-progress event is: a coloured plate (green here, orange this area, brown elsewhere) at (x, y), 12 high;
   returns its width (0: none) */
static int dist_plate(int x, int y, int e) {
    int d = ev_dist(e); const char *s = dist_name(d);
    if (!s) return 0;
    int w = pa_text_w(8, s) + 8;
    u32 c = d == D_HERE ? RGB(56, 140, 56) : d == D_AREA ? RGB(200, 110, 30) : RGB(130, 96, 80);
    pa_fill(x, y, w, 12, C_DARK); pa_fill(x + 1, y + 1, w - 2, 10, c);
    pa_text_ol(x + 4, y + 2, 8, s, C_WHITE, C_WOODE, 0);
    return w;
}
/* the place of an event: "area - room", the room alone in the area Tomba is in */
static void ev_place(int e, char *b, int n) {
    const PanelEventLoc *L = ev_loc(e);
    const char *ar = L ? place_area(L->area, L->sec) : NULL, *rm = L && L->area != 254 ? place_room(L->area, L->sec) : NULL;
    if (rm && ar && !strcmp(rm, ar)) rm = NULL;
    else if (rm && L->area == G_AREA) ar = NULL;
    if (!ar && rm) { snprintf(b, n, "%s", rm); return; }
    snprintf(b, n, "%s%s%s", ar ? ar : T("zone not extracted yet", "zona a\xFAn sin extraer"), rm ? " - " : "", rm ? rm : "");
}
static void pin_mark(int x, int y, unsigned c) {   /* a push pin, 10 x 14: round head, needle */
    pa_fill(x + 2, y, 7, 7, RGB(60, 20, 10)); pa_fill(x + 3, y + 1, 5, 5, c);
    pa_fill(x + 1, y + 6, 9, 2, RGB(60, 20, 10));
    pa_fill(x + 5, y + 8, 1, 6, RGB(60, 40, 30));
}
static const char *kind_name(int k), *ev_giver(int e);
static const char *place_area(int area, int sec), *place_room(int area, int sec);
static const char *ev_giver(int e) {   /* panel_givers.h, in the panel's language */
    for (unsigned i = 0; i < sizeof panel_givers / sizeof panel_givers[0]; i++) if (panel_givers[i].ev == e) {
        const char *s = T(panel_givers[i].en, panel_givers[i].es);
        if (es() && es_improved)
            for (unsigned j = 0; j < sizeof panel_givers_mej / sizeof panel_givers_mej[0]; j++) if (!strcmp(s, panel_givers_mej[j].disc)) return panel_givers_mej[j].mej;
        return s;
    }
    return NULL;
}
static const char *kind_name(int k) {
    switch (k) {
    case EK_NPC: return T("Someone here", "Alguien de aqu\xED");
    case EK_CHEST: return T("A chest", "Un cofre");
    case EK_ITEM: return T("An item", "Un objeto");
    case EK_SCENE: return T("A scene", "Una escena");
    case EK_OBJECT: return T("An object", "Un objeto del lugar");
    default: return NULL;
    }
}

static void filters(int y) {
    int W = pa_cw, n = NFILTERS, tw = (W - 16 - 4 * (n - 1)) / n;
    for (int i = 0; i < n; i++) {
        int x = 8 + i * (tw + 4);
        plank(x, y, tw); hot(x, y, tw, 24, HS_FILTER + i);
        plank_text(x + tw / 2, y + 4, filter_name(i), i == ev_filter);
    }
}

/* one row of the list, one thing per line: the title (16 px); in progress, the plate (here / this area / elsewhere) on a
   line of its own and the place under it, full width; else the state and the area (8 px).  Everything wraps, never cut.
   A row is 30 high with one line of each, 11 more for the plate line, 16 for each further title line and 10 for each
   further place line. */
typedef struct { char t[96], p[160]; int pw, tr, tl, pl, h; } EvRow;
static int dist_plate_w(int e) { const char *s = dist_name(ev_dist(e)); return s ? pa_text_w(8, s) + 8 : 0; }
static void ev_row(int e, EvRow *R) {
    int W = pa_cw, st = ev_state(e), hid = ev_hidden(e);
    const PanelEventLoc *L = ev_loc(e);
    const char *ar = L ? place_area(L->area, L->sec) : NULL;
    char t[96];
    R->tr = ev_pinned(e) ? 18 : 0; R->pw = 0;
    snprintf(R->t, sizeof R->t, "%s", st == F_PENDING ? "? ? ?" : ev_title(e, t, sizeof t));
    if (st == F_PROGRESS) {   /* in progress: where, as the plate and the place (the room, in this area) */
        char pl[128];
        R->pw = dist_plate_w(e);   /* (only says whether there is a plate line: the place below it is full width) */
        ev_place(e, pl, sizeof pl);
        snprintf(R->p, sizeof R->p, "%s%s", hid ? T("Hidden - ", "Oculto - ") : "", pl);
        char *d = NULL;   /* too long for one line: the room starts the next line ("area -" / "room") */
        for (char *q = strstr(R->p + (strlen(R->p) - strlen(pl)), " - "); q; q = strstr(q + 1, " - ")) d = q;
        if (d && text_lines(8, W - 60 - R->tr, 2, R->p) > 1) d[2] = '\n';
    } else snprintf(R->p, sizeof R->p, "%s%s  -  %s", hid ? T("Hidden - ", "Oculto - ") : "", state_name(st), ar ? ar : T("zone not extracted yet", "zona a\xFAn sin extraer"));
    R->tl = text_lines(16, W - 60 - R->tr, 4, R->t);
    R->pl = text_lines(8, W - 60 - R->tr, 6, R->p);
    R->h = 30 + (R->tl - 1) * 16 + (R->pl - 1) * 10 + (R->pw ? 11 : 0);
}
/* the list scrolls as one strip of rows (panel.c scroller): pixel for pixel under the finger, flung, the rows cut
   cleanly at the window's edges; the D-pad moves the selection and the strip follows it */
static Scroll ev_sc; static int ev_follow;
static void ev_reset(void) { ev_sel = 0; scr_reset(&ev_sc); }
static void event_list(void) {
    int ids[EVENTS], n = ev_list(ids), W = pa_cw, H = pa_ch, ry[EVENTS + 1];
    char b[96]; static EvRow R;
    filters(56);
    int y0 = 84, rows = (H - y0 - 56) / 30, avail = rows * 30;
    window_(8, y0 - 2, W - 16, avail + 24);
    ry[0] = 0;
    for (int i = 0; i < n; i++) { ev_row(ids[i], &R); ry[i + 1] = ry[i] + R.h; }
    if (ev_sel >= n) ev_sel = n - 1; if (ev_sel < 0) ev_sel = 0;
    int vx = 12, vy = y0 + 1, vw = W - 24, vh = avail + 1;
    scr_view(&ev_sc, vx, vy, vw, vh, n ? ry[n] + 2 : 0);
    if (ev_follow && n) { scr_show(&ev_sc, ry[ev_sel], ry[ev_sel + 1] + 2); ev_follow = 0; }
    if (!n) pa_text(20, y0 + 8, 16, T("No events here", "No hay eventos"), C_MUTED, C_EDGE);
    int so = scr_y(&ev_sc), bar = ev_sc.total > vh, rw = W - 24 - (bar ? 8 : 0);
    int cx0 = pa_clip_x0, cy0 = pa_clip_y0, cx1 = pa_clip_x1, cy1 = pa_clip_y1;
    pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + vw; pa_clip_y1 = vy + vh;
    for (int r = 0; r < n; r++) {
        int top = vy + 1 + ry[r] - so, h = ry[r + 1] - ry[r];
        if (top + h <= vy || top >= vy + vh) continue;
        int e = ids[r], y = top + 2, sel = r == ev_sel, st = ev_state(e);
        ev_row(e, &R);
        if (sel) pa_fill(12, y - 2, rw, h - 1, C_SEL);
        {   int hy0 = top < vy ? vy : top, hy1 = top + h - 1 > vy + vh ? vy + vh : top + h - 1;
            if (hy1 - hy0 > 6) hot(12, hy0, rw, hy1 - hy0, HS_ROW + r); }
        int pend = st == F_PENDING, hid = ev_hidden(e), py = y + (R.tl - 1) * 16;
        pa_event_icon(16, y + 5, e, pend ? PA_DIM : hid ? PA_SHADE : 0);
        text_wrap(46, y - 1, 16, W - 60 - R.tr, 16, 4, 0, R.t, sel ? C_WHITE : hid ? C_MUTED : st == F_DONE ? C_GREEN : C_DARK, sel ? RGB(60, 110, 50) : C_EDGE);
        if (st == F_PROGRESS) dist_plate(46, py + 15, e);
        if (R.tr) pin_mark(W - 30 - (bar ? 6 : 0), y + 2, sel ? C_WHITE : C_RED);
        text_wrap(46, py + (R.pw ? 28 : st == F_PROGRESS ? 17 : 16), 8, W - 60 - R.tr, 10, 6, 0, R.p, sel ? C_WHITE : st == F_PROGRESS ? C_RED8 : C_MUTED8, sel ? RGB(60, 110, 50) : RGB(230, 200, 190));
    }
    pa_clip_x0 = cx0; pa_clip_y0 = cy0; pa_clip_x1 = cx1; pa_clip_y1 = cy1;
    scr_bar(&ev_sc, W - 19);
    int by = y0 + avail + 4;
    snprintf(b, sizeof b, T("%d of %d", "%d de %d"), n ? ev_sel + 1 : 0, n);   /* the 16 px font has no '/' */
    pa_text(W - 24 - pa_text_w(16, b), by, 16, b, C_DARK, C_EDGE);
    int s, d; count_events(&s, &d);
    snprintf(b, sizeof b, T("%d started, %d completed", "%d empezados, %d completados"), s + d, d);
    pa_text(20, by + 4, 8, b, C_MUTED8, RGB(230, 200, 190));
    /* [Ocultar / Mostrar] the selected event   [Ocultos: no / s\xED (n)] */
    int py = y0 - 2 + avail + 28, pw2 = (W - 20) / 2, cur = n ? ids[ev_sel] : -1;
    plank(8, py, pw2); hot(8, py, pw2, 24, HS_FILTER + 8);
    plank_text(8 + pw2 / 2, py + 4, cur >= 0 && ev_hidden(cur) ? T("Show", "Mostrar") : T("Hide", "Ocultar"), 0);
    plank(W - 8 - pw2, py, pw2); hot(W - 8 - pw2, py, pw2, 24, HS_FILTER + 9);
    snprintf(b, sizeof b, ev_show_hidden ? T("Hidden: shown (%d)", "Ocultos: s\xED (%d)") : T("Hidden: off (%d)", "Ocultos: no (%d)"), ev_hidden_count());
    plank_text(W - 8 - pw2 / 2, py + 4, b, ev_show_hidden);
}

/* the detail page, laid out as the Items page's showcase: a top window with the icon at 3x in a stone frame, the title
   (16 px, wrapped) and the state under it; "Where" (the place plate, the area in 8 px, the room in 16 px, each on its
   own line, and "Show on the map" when the map tab can show its marker); "Who" (the giver of panel_givers.h) or, when
   that is unknown or only repeats the title, "Hint" (what starts it: a chest, an item, someone or something in the
   place); and three planks twice as high: Back, Track / Stop tracking, Hide / Show.  Nothing is cut: every text wraps
   and the windows take the height they need. */
static void plank_h(int x, int y, int w, int h);                  /* panel_items.inc */
static int map_shows_event(int e); static void map_focus(int e);  /* panel_side.inc */
/* (no tolower: the game's own tolower (src/main0) and the C library's collide when the mingw build pulls both in) */
static int lc_ascii(int c) { return c >= 'A' && c <= 'Z' ? c + 32 : c; }
static int ev_same_text(const char *a, const char *b) {   /* b is a (case aside) or is inside it: it says nothing new */
    int na = (int)strlen(a), nb = (int)strlen(b);
    for (int i = 0; i + nb <= na; i++) {
        int k = 0;
        while (k < nb && lc_ascii((unsigned char)a[i + k]) == lc_ascii((unsigned char)b[k])) k++;
        if (k == nb) return 1;
    }
    return 0;
}
static const char *kind_hint(int k) {   /* what starts an event, when no one is named for it (a scene says nothing) */
    switch (k) {
    case EK_NPC: return T("Someone in this place", "Alguien de este lugar");
    case EK_CHEST: return T("A chest", "Un cofre");
    case EK_ITEM: return T("An item", "Un objeto");
    case EK_OBJECT: return T("Something in this place", "Algo de este lugar");
    default: return NULL;
    }
}
/* a plank twice as high: the label in the 24 px event-title letters when they have all of it and it fits (mode 0), else
   the 16 px font pixel doubled (1), else 16 px, wrapped (2: never cut).  ev_plank_mode: the mode a label needs. */
static int ev_plank_mode(int w, const char *s) { return pa_has_glyphs(24, s) && pa_text_w(24, s) <= w - 16 ? 0 : 2 * pa_text_w(16, s) <= w - 16 ? 1 : 2; }
static void ev_plank(int x, int y, int w, int h, const char *s, int mode, int id) {
    plank_h(x, y, w, h); hot(x, y, w, h, id);
    u32 o = pa_outline; pa_outline = C_DARK;
    if (mode < ev_plank_mode(w, s)) mode = ev_plank_mode(w, s);
    if (!mode) { int tw = pa_text_w(24, s); pa_text_scaled(x + w / 2 - tw / 2, y + (h - 24) / 2, 24, s, C_WHITE, C_WOODE, 1, 0); }
    else if (mode == 1) { int tw = 2 * pa_text_w(16, s); pa_text_scaled(x + w / 2 - tw / 2, y + (h - 32) / 2, 16, s, C_WHITE, C_WOODE, 2, 0); }
    else { int nl = text_lines(16, w - 12, 3, s); text_wrap(x + 6, y + (h - nl * 16) / 2 + 1, 16, w - 12, 16, 3, 1, s, C_WHITE, C_WOODE); }
    pa_outline = o;
}
/* "Where" in (x, y, w, h); h 0: only measures.  Returns the height it needs. */
static int ev_where(int x, int y, int w, int h, int e) {
    const PanelEventLoc *L = ev_loc(e);
    int tw = w - 16, cy = y + 24, map = !port_panel_no_map && map_shows_event(e), plate = dist_name(ev_dist(e)) != NULL;
    if (h) pa_text(x + 8, y + 5, 16, T("Where", "D\xF3nde"), C_RED, C_REDE);
    if (plate) { if (h) dist_plate(x + 8, cy, e); cy += 16; }
    const char *ar = L ? place_area(L->area, L->sec) : NULL, *rm = L && L->area != 254 ? place_room(L->area, L->sec) : NULL;
    if (rm && ar && !strcmp(rm, ar)) rm = NULL;
    wrap_dry = !h;
    if (!ar && !rm) cy += 16 * text_wrap(x + 8, cy, 16, tw, 16, 6, 0, L ? T("Zone not extracted yet", "Zona a\xFAn sin extraer") : T("Not known yet where it happens", "A\xFAn no se sabe d\xF3nde ocurre"), C_MUTED, C_EDGE);
    else if (ar && rm) {
        cy += 10 * text_wrap(x + 8, cy, 8, tw, 10, 6, 0, ar, C_RED8, C_REDE) + 3;
        cy += 16 * text_wrap(x + 8, cy, 16, tw, 16, 6, 0, rm, C_DARK, C_EDGE);
    } else cy += 16 * text_wrap(x + 8, cy, 16, tw, 16, 6, 0, ar ? ar : rm, C_DARK, C_EDGE);
    wrap_dry = 0;
    if (h && map) {
        int py = y + h - 8 - 24;
        plank(x + 8, py, w - 16); hot(x + 8, py, w - 16, 24, HS_FILTER + 11);
        plank_text(x + w / 2, py + 4, T("Show on the map", "Ver en el mapa"), 0);
    }
    return cy - y + 8 + (map ? 30 : 0);
}
static void event_detail(int e) {
    char b[160]; int W = pa_cw, H = pa_ch, st = ev_state(e), pend = st == F_PENDING, pn = ev_pinned(e), hid = ev_hidden(e);
    const PanelEventLoc *L = ev_loc(e);
    int top = 56;
    /* the top window: the icon x3 in a stone frame | the title, the state */
    const char *t = pend ? "? ? ?" : ev_title(e, b, sizeof b);
    int tx = 16 + 62 + 10, tw = W - 18 - tx, tl = text_lines(16, tw, 6, t);
    const char *sn = state_name(st), *hn = hid ? T("Hidden", "Oculto") : NULL;
    int sw = pa_text_w(16, sn), hl = hn && sw + 16 + (pn ? 14 : 0) + pa_text_w(16, hn) > tw;   /* "Oculto" after the state, or under it */
    int blk = tl * 17 + 3 + 16 + (hl ? 17 : 0), th = blk + 16 > 74 ? blk + 16 : 74;
    window_(8, top, W - 16, th);
    frame_(16, top + (th - 62) / 2, 62, 62); stone(21, top + (th - 62) / 2 + 5, 52, 52);
    {   int iw = pa_event_icon_w(e), ih = pa_event_icon_h(e); if (iw <= 0) iw = 16; if (ih <= 0) ih = 16;
        int sc = (iw > ih ? iw : ih) * 3 <= 52 ? 3 : 2;   /* 3x, 2x for the big ones (24 x 24): inside the stone */
        pa_event_icon(47 - iw * sc / 2, top + th / 2 - ih * sc / 2, e, PA_OUTLINE | mk_flag(sc) | (pend ? PA_DIM : 0)); }
    int ty = top + (th - blk) / 2;
    text_wrap(tx, ty, 16, tw, 17, 6, 0, t, pend ? C_MUTED : C_DARK, C_EDGE);
    ty += tl * 17 + 3;
    pa_text(tx, ty, 16, sn, st == F_PROGRESS ? C_ORANGE : st == F_DONE ? C_GREEN : C_MUTED, C_REDE);
    if (pn) pin_mark(tx + sw + 8, ty + 1, C_RED);   /* tracked: the list's red pin after the state */
    if (hn) pa_text(hl ? tx : tx + sw + 16 + (pn ? 14 : 0), hl ? ty + 17 : ty, 16, hn, C_MUTED, C_EDGE);
    /* the planks: Back (small), Track, Hide */
    int bh = 48, by = H - 8 - bh, bk = W >= 400 ? 84 : 72, bw = (W - 16 - bk - 8) / 2;
    const char *l1 = pn ? T("Stop tracking", "Dejar de seguir") : T("Track", "Seguir"), *l2 = hid ? T("Show", "Mostrar") : T("Hide", "Ocultar");
    int m1 = ev_plank_mode(bw, l1), m2 = ev_plank_mode(bw, l2), md = m1 > m2 ? m1 : m2;   /* the pair in one style */
    ev_plank(8, by, bk, bh, T("Back", "Volver"), 2, HS_BACK);
    ev_plank(8 + bk + 4, by, bw, bh, l1, md, HS_FILTER + 10);
    ev_plank(W - 8 - bw, by, bw, bh, l2, md, HS_FILTER + 8);
    /* above them: the last message (4 events tracked already), else on the PC the keys when they fit one line */
    int mb = by - 4;
    if (pin_msg_t && port_frame - pin_msg_t < 150) {
        int nl = text_lines(8, W - 32, 3, pin_msg);
        mb -= nl * 10 + 4; text_wrap(16, mb + 2, 8, W - 32, 10, 3, 0, pin_msg, C_RED8, C_REDE);
    }
#ifndef __ANDROID__
    else {
        const char *k = T("Backspace: back    P: track    H: hide", "Retroceso: volver    P: seguir    H: ocultar");
        if (pa_text_w(8, k) <= W - 32) { mb -= 14; pa_text(16, mb + 3, 8, k, C_MUTED8, RGB(230, 200, 190)); }
    }
#endif
    /* who gives it, unless that only repeats the title; else the hint (what starts it); else no window */
    const char *who = pend ? NULL : ev_giver(e), *lab = T("Who", "Qui\xE9n");
    if (who && ev_same_text(t, who)) who = NULL;
    if (!who && !pend && L) { who = kind_hint(L->kind); lab = T("Hint", "Pista"); }
    int my = top + th + 4, mh = mb - my;
    if (!who) { window_(8, my, W - 16, mh); ev_where(8, my, W - 16, mh, e); return; }
    if (W >= 400) {   /* side by side, unless "Where" would not fit its column */
        int dw = (W - 16 - 4) * 56 / 100, ww = W - 16 - 4 - dw, wx = 8 + dw + 4;
        if (ev_where(8, my, dw, 0, e) <= mh && 24 + text_lines(16, ww - 16, 9, who) * 16 + 6 <= mh) {
            window_(8, my, dw, mh); ev_where(8, my, dw, mh, e);
            window_(wx, my, ww, mh);
            pa_text(wx + 8, my + 5, 16, lab, C_RED, C_REDE);
            text_wrap(wx + 8, my + 24, 16, ww - 16, 16, 9, 0, who, C_DARK, C_EDGE);
            return;
        }
    }
    int wl = text_lines(16, W - 32, 6, who), wh = 24 + wl * 16 + 8, dh = mh - wh - 4;   /* one above the other */
    window_(8, my, W - 16, dh); ev_where(8, my, W - 16, dh, e);
    window_(8, my + dh + 4, W - 16, wh);
    pa_text(16, my + dh + 9, 16, lab, C_RED, C_REDE);
    text_wrap(16, my + dh + 28, 16, W - 32, 16, 6, 0, who, C_DARK, C_EDGE);
}

static void page_events(int play) {
    if (!play) { window_(8, 56, pa_cw - 16, pa_ch - 64); pa_text(20, 70, 16, T("Waiting for the game...", "Esperando a la partida..."), C_MUTED, C_EDGE); return; }
    if (ev_detail >= 0) event_detail(ev_detail); else event_list();
}

/* ---------------------------------------------------------------- input */
enum { IN_UP = 1, IN_DOWN, IN_LEFT, IN_RIGHT, IN_OK, IN_BACK, IN_TAB_L, IN_TAB_R, IN_ZOOM_OUT, IN_ZOOM_IN, IN_HIDE, IN_SHOW_HIDDEN, IN_PIN };
static void panel_action(int a) {
    if (a == IN_TAB_L) { do page = (page + NPAGES - 1) % NPAGES; while (port_panel_no_map && page == PAGE_SIDE); return; }
    if (a == IN_TAB_R) { do page = (page + 1) % NPAGES; while (port_panel_no_map && page == PAGE_SIDE); return; }
    if (page == PAGE_SIDE) { map_action(a); return; }
    if (page == PAGE_ITEMS) { void items_action_(int); items_action_(a); return; }
    if (page != PAGE_EVENTS && page != PAGE_MAP) return;
    if (a == IN_SHOW_HIDDEN) { ev_show_hidden = !ev_show_hidden; ev_reset(); return; }
    if (page != PAGE_EVENTS) return;
    if (ev_detail >= 0) { if (a == IN_BACK || a == IN_OK) ev_detail = -1; else if (a == IN_HIDE) ev_hide_toggle(ev_detail); else if (a == IN_PIN) ev_pin_toggle(ev_detail); return; }
    int ids[EVENTS], n = ev_list(ids);
    if (a == IN_HIDE) { if (n && ev_sel < n) ev_hide_toggle(ids[ev_sel]); return; }
    if (a == IN_PIN) { if (n && ev_sel < n) ev_pin_toggle(ids[ev_sel]); return; }
    if (a == IN_UP && ev_sel > 0) { ev_sel--; ev_follow = 1; }
    else if (a == IN_DOWN && ev_sel < n - 1) { ev_sel++; ev_follow = 1; }
    else if (a == IN_LEFT) { ev_filter = (ev_filter + NFILTERS - 1) % NFILTERS; ev_reset(); }
    else if (a == IN_RIGHT) { ev_filter = (ev_filter + 1) % NFILTERS; ev_reset(); }
    else if (a == IN_OK && n) ev_detail = ids[ev_sel];
}
static int touch_long;
static void panel_hit(int id) {
    int k = id & 0xFF;
    if (id >= 0x700) { void items_hit_(int); items_hit_(id); return; }
    switch (id & ~0xFF) {
    case HS_PAGE: page = k; ev_detail = -1; break;
    case HS_FILTER:
        if (k >= 16) { map_hit(k); break; }
        if (k == 8) { panel_action(IN_HIDE); break; }
        if (k == 9) { panel_action(IN_SHOW_HIDDEN); break; }
        if (k == 10) { if (ev_detail >= 0) ev_pin_toggle(ev_detail); break; }
        if (k == 11) { if (ev_detail >= 0 && !port_panel_no_map) { map_focus(ev_detail); page = PAGE_SIDE; } break; }
        ev_filter = k; ev_reset(); ev_detail = -1; break;
    case HS_ROW: {   /* a tap opens the event, a long press (0.6 s) pins / unpins it */
        int ids[EVENTS], n = ev_list(ids);
        if (k < n) { ev_sel = k; if (touch_long) ev_pin_toggle(ids[k]); else ev_detail = ids[k]; }
        break;
    }
    case HS_BACK: ev_detail = -1; break;
    }
}
/* touch / click in canvas pixels: act on release over the target pressed */
static int touch_id = -1, touch_t0;
static void panel_touch_canvas(int x, int y, int action) {   /* action: 0 down, 1 up, 2 move */
    int id = -1;
    if (page == PAGE_SIDE && map_touch(x, y, action)) { touch_id = -1; return; }
    if (scr_touch(x, y, action)) { touch_id = -1; return; }   /* a list dragged / flung (panel.c scroller) */
    for (int i = nhs - 1; i >= 0; i--) if (x >= hs[i].x && y >= hs[i].y && x < hs[i].x + hs[i].w && y < hs[i].y + hs[i].h) { id = hs[i].id; break; }
    if (action == 0) { touch_id = id; touch_t0 = port_frame; }
    else if (action == 1) { touch_long = port_frame - touch_t0 >= 30; if (id >= 0 && id == touch_id) panel_hit(id); touch_id = -1; touch_long = 0; }
}
