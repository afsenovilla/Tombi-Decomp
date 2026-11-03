/* Second screen panel: the event browser (included by panel.c).  Every event of the pause menu's Events list (SUBSCREEN_EVENT_ORDER, ids up to 0xBF, in its order) with its icon and
 * title from the user's disc, filtered by state (in progress 1..0xFE, done 0xFF, not started 0), and a detail page:
 * where (area / room names and the map graph with a marker on the room), who / what starts it, its state.  Where an
 * event happens comes from panel_eventmap.h, generated from the decomp by port2/tools/gen_panel_events.py (ids and
 * coordinates only; docs/EVENTS_MAP.md lists the coverage); unknown places show "location unknown".
 * Input: touch (Android: nativePanelTouch from the Presentation), the mouse in the PC window, and the keyboard of the
 * PC window (arrows, Enter, Backspace, Q / E: tabs, H: hide / show the selected event, V: list the hidden ones).
 * Hidden events (the Ocultar plank, a long press on a row, H): left out of the lists, the Status tab's "in progress" box and
 * the map markers until "Ocultos: s\xED" lists them again (dimmed); kept in panel_hidden.txt next to the memory cards
 * (the event ids, one line). */
#include "panel_eventmap.h"
#include "panel_givers.h"

enum { F_PROGRESS, F_DONE, F_PENDING, NFILTERS };
static int map_action(int a), map_touch(int x, int y, int action); static void map_hit(int k);   /* panel_side.inc */
static int ev_filter, ev_sel, ev_top, ev_detail = -1;

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
    int px = (s16)G16(0x800A604Au), key[EVENTS];
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
    return f == F_PROGRESS ? T("In progress", "En curso") : f == F_DONE ? T("Completed", "Completado") : T("Not started", "Sin empezar");
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

/* one row of the list: the title (16 px) and under it the plate and the place (in progress) or the state and the area
   (8 px), both wrapped, never cut: the place's next lines start under its first letter.  A row is 30 high with one
   line of each, 16 more for each further title line and 10 for each further place line. */
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
        R->pw = dist_plate_w(e); if (R->pw) R->pw += 5;
        ev_place(e, pl, sizeof pl);
        snprintf(R->p, sizeof R->p, "%s%s", hid ? T("Hidden - ", "Oculto - ") : "", pl);
        char *d = NULL;   /* too long for one line: the room starts the next line ("area -" / "room") */
        for (char *q = strstr(R->p + (strlen(R->p) - strlen(pl)), " - "); q; q = strstr(q + 1, " - ")) d = q;
        if (d && text_lines(8, W - 60 - R->tr - R->pw, 2, R->p) > 1) d[2] = '\n';
    } else snprintf(R->p, sizeof R->p, "%s%s  -  %s", hid ? T("Hidden - ", "Oculto - ") : "", state_name(st), ar ? ar : T("zone not extracted yet", "zona a\xFAn sin extraer"));
    R->tl = text_lines(16, W - 60 - R->tr, 4, R->t);
    R->pl = text_lines(8, W - 60 - R->tr - R->pw, 6, R->p);
    R->h = 30 + (R->tl - 1) * 16 + (R->pl - 1) * 10;
}
static int ev_vis = 5;   /* rows on the page drawn last (the page up / down strips move by it) */
static void event_list(void) {
    int ids[EVENTS], n = ev_list(ids), W = pa_cw, H = pa_ch, rh[EVENTS];
    char b[96]; static EvRow R;
    filters(56);
    int y0 = 84, rows = (H - y0 - 56) / 30, avail = rows * 30;
    window_(8, y0 - 2, W - 16, avail + 24);
    for (int i = 0; i < n; i++) { ev_row(ids[i], &R); rh[i] = R.h; }
    if (ev_sel >= n) ev_sel = n - 1; if (ev_sel < 0) ev_sel = 0;
    if (ev_top > ev_sel) ev_top = ev_sel; if (ev_top < 0) ev_top = 0;
    /* whole rows only: the selected one on the page, and the last page full */
    for (;;) { int u = 0; for (int i = ev_top; i <= ev_sel; i++) u += rh[i]; if (u <= avail || ev_top >= ev_sel) break; ev_top++; }
    for (;;) { int u = 0; for (int i = ev_top - 1; i < n; i++) u += rh[i]; if (ev_top <= 0 || u > avail) break; ev_top--; }
    /* page up / down strips first: the rows registered after them win where they overlap (the top of row 1) */
    hot(8, y0 - 2, W - 16, 20, HS_PREV); hot(8, y0 + avail, W / 2, 24, HS_NEXT);
    if (!n) pa_text(20, y0 + 8, 16, T("No events here", "No hay eventos"), C_MUTED, C_EDGE);
    int used = 0, r = ev_top;
    for (; r < n && used + rh[r] <= avail; used += rh[r], r++) {
        int e = ids[r], y = y0 + 4 + used, h = rh[r], sel = r == ev_sel, st = ev_state(e);
        ev_row(e, &R);
        if (sel) pa_fill(12, y - 2, W - 24, h - 1, C_SEL);
        hot(12, y - 2, W - 24, h - 1, HS_ROW + r);
        int pend = st == F_PENDING, hid = ev_hidden(e), py = y + (R.tl - 1) * 16;
        pa_event_icon(16, y + 5, e, pend ? PA_DIM : hid ? PA_SHADE : 0);
        text_wrap(46, y - 1, 16, W - 60 - R.tr, 16, 4, 0, R.t, sel ? C_WHITE : hid ? C_MUTED : st == F_DONE ? C_GREEN : C_DARK, sel ? RGB(60, 110, 50) : C_EDGE);
        if (st == F_PROGRESS) dist_plate(46, py + 15, e);
        if (R.tr) pin_mark(W - 30, y + 2, sel ? C_WHITE : C_RED);
        text_wrap(46 + R.pw, py + (st == F_PROGRESS ? 17 : 16), 8, W - 60 - R.tr - R.pw, 10, 6, 0, R.p, sel ? C_WHITE : st == F_PROGRESS ? C_RED8 : C_MUTED8, sel ? RGB(60, 110, 50) : RGB(230, 200, 190));
    }
    ev_vis = r - ev_top > 1 ? r - ev_top : 1;
    int by = y0 + avail + 4;
    snprintf(b, sizeof b, T("%d of %d", "%d de %d"), n ? ev_sel + 1 : 0, n);   /* the 16 px font has no '/' */
    pa_text(W - 24 - pa_text_w(16, b), by, 16, b, C_DARK, C_EDGE);
    int s, d; count_events(&s, &d);
    snprintf(b, sizeof b, T("%d started, %d completed", "%d empezados, %d completados"), s + d, d);
    pa_text(20, by + 4, 8, b, C_MUTED8, RGB(230, 200, 190));
    if (ev_top > 0) { pa_text(W - 40, y0 + 2, 16, "^", C_DARK, C_EDGE); }
    /* [Ocultar / Mostrar] the selected event   [Ocultos: no / s\xED (n)] */
    int py = y0 - 2 + avail + 28, pw2 = (W - 20) / 2, cur = n ? ids[ev_sel] : -1;
    plank(8, py, pw2); hot(8, py, pw2, 24, HS_FILTER + 8);
    plank_text(8 + pw2 / 2, py + 4, cur >= 0 && ev_hidden(cur) ? T("Show", "Mostrar") : T("Hide", "Ocultar"), 0);
    plank(W - 8 - pw2, py, pw2); hot(W - 8 - pw2, py, pw2, 24, HS_FILTER + 9);
    snprintf(b, sizeof b, ev_show_hidden ? T("Hidden: shown (%d)", "Ocultos: s\xED (%d)") : T("Hidden: off (%d)", "Ocultos: no (%d)"), ev_hidden_count());
    plank_text(W - 8 - pw2 / 2, py + 4, b, ev_show_hidden);
}

static void event_detail(int e) {
    char b[160]; int W = pa_cw, H = pa_ch, st = ev_state(e), pend = st == F_PENDING;
    const PanelEventLoc *L = ev_loc(e);
    filters(56);
    /* title: the icon x2 and the 24 px event-title letters */
    int y = 84, th = 38;   /* the title box grows when the title needs more than two lines (never cut) */
    const char *t = pend ? "? ? ?" : ev_title(e, b, sizeof b);
    int tl = pa_text_w(16, t) <= W - 64 ? 1 : text_lines(16, W - 64, 5, t);
    if (tl > 2) th = tl * 17 + 4;
    window_(8, y, W - 16, th);
    hot(8, y, W - 16, th, HS_BACK);
    {   pa_event_icon(16, y + th / 2 - 8, e, pend ? PA_DIM : 0);
        if (pa_text_w(24, t) <= W - 60) pa_text(48, y + 7, 24, t, C_WHITE, C_DARK);
        else if (tl == 1) pa_text(48, y + 11, 16, t, C_DARK, C_EDGE);
        else text_wrap(48, y + 2, 16, W - 64, 17, tl, 0, t, C_DARK, C_EDGE);
    }
    y += th + 4;
    int ww = W * 58 / 100, wh = H - y - 70;
    if (wh < 110) wh = 110;
    /* where */
    window_(8, y, ww, wh);
    pa_text(16, y + 5, 16, T("Where", "D\xF3nde"), C_DARK, C_EDGE);
    const char *ar = L ? place_area(L->area, L->sec) : NULL, *rm = L ? place_room(L->area, L->sec) : NULL;
    if (ar) {
        snprintf(b, sizeof b, "%s%s%s", ar, rm ? "  -  " : "", rm ? rm : "");
        if (rm && text_lines(8, ww - 20, 2, b) > 1) snprintf(b, sizeof b, "%s  -\n%s", ar, rm);   /* the room on its own line */
        int nl = text_wrap(16, y + 22, 8, ww - 20, 10, 6, 0, b, C_RED8, C_REDE);
        int l = L->area < 20 && L->sec < 24 ? roomlink[L->area][L->sec] : -1;
        for (int s = 0; L->area < 20 && s < 24 && l < 0; s++) l = roomlink[L->area][s];   /* room not known: the area */
        map_mark_link = l; map_mark_ev = e;
        draw_map(12, y + 22 + nl * 10, ww - 8, wh - 26 - nl * 10, -1);
        map_mark_link = -1;
    } else {
        text_wrap(16, y + 26, 16, ww - 20, 18, 5, 0, T("The zone of this event is not extracted from the game data yet", "La zona de este evento a\xFAn no se ha extra\xED""do de los datos del juego"), C_MUTED, C_EDGE);
    }
    /* who */
    int wx = 8 + ww + 6, wwid = W - 8 - wx;
    window_(wx, y, wwid, wh);
    pa_text(wx + 8, y + 5, 16, T("Who", "Qui\xE9n"), C_DARK, C_EDGE);
    const char *kn = ev_giver(e) ? ev_giver(e) : L ? kind_name(L->kind) : NULL;
    if (kn && !pend) {
        text_wrap(wx + 8, y + 26, 16, wwid - 16, 17, 5, 0, kn, C_DARK, C_EDGE);
    } else pa_text(wx + 8, y + 26, 16, "? ? ?", C_MUTED, C_EDGE);
    face(wx + wwid - 34, y + wh - 34);
    /* state */
    int sy = y + wh + 4;
    window_(8, sy, W - 16, H - sy - 6);
    pa_text(16, sy + 6, 16, T("State", "Estado"), C_DARK, C_EDGE);
    pa_text(16 + pa_text_w(16, T("State", "Estado")) + 16, sy + 6, 16, state_name(st), st == F_PROGRESS ? C_ORANGE : st == F_DONE ? C_GREEN : C_MUTED, C_REDE);
    {   int bw = 124, bx = W - 16 - bw, pn = ev_pinned(e);
        plank(bx, sy + 4, bw); hot(bx, sy + 4, bw, 24, HS_FILTER + 10);
        plank_text(bx + bw / 2 + 8, sy + 8, pn ? T("Unpin", "Quitar") : T("Track", "Seguir"), pn);
        pin_mark(bx + 12, sy + 9, pn ? C_YELLOW : C_WHITE);
        if (ev_hidden(e)) pa_text(bx - 10 - pa_text_w(16, T("Hidden", "Oculto")), sy + 6, 16, T("Hidden", "Oculto"), C_MUTED, C_REDE);
        if (pin_msg_t && port_frame - pin_msg_t < 150) text_wrap(16, sy + 26, 8, W - 32, 10, 2, 0, pin_msg, C_RED8, C_REDE);
    }
    if (!(pin_msg_t && port_frame - pin_msg_t < 150))
        text_wrap(16, sy + 26, 8, W - 32, 10, (H - 6 - sy - 26) / 10, 0, T("Back: tap the title or press Backspace.  Hide / show: H.  Track: P, or a long press on its row in the list", "Volver: toca el t\xEDtulo o pulsa Retroceso.  Ocultar / mostrar: H.  Seguir: P, o mant\xE9n pulsada su fila en la lista"), C_MUTED8, RGB(230, 200, 190));
}

static void page_events(int play) {
    if (!play) { window_(8, 56, pa_cw - 16, pa_ch - 64); pa_text(20, 70, 16, T("Waiting for the game...", "Esperando a la partida..."), C_MUTED, C_EDGE); return; }
    if (ev_detail >= 0) event_detail(ev_detail); else event_list();
}

/* ---------------------------------------------------------------- input */
enum { IN_UP = 1, IN_DOWN, IN_LEFT, IN_RIGHT, IN_OK, IN_BACK, IN_TAB_L, IN_TAB_R, IN_ZOOM_OUT, IN_ZOOM_IN, IN_HIDE, IN_SHOW_HIDDEN, IN_PIN };
static void panel_action(int a) {
    if (a == IN_TAB_L) { page = (page + NPAGES - 1) % NPAGES; return; }
    if (a == IN_TAB_R) { page = (page + 1) % NPAGES; return; }
    if (page == PAGE_SIDE) { map_action(a); return; }
    if (page == PAGE_ITEMS) { void items_action_(int); items_action_(a); return; }
    if (page != PAGE_EVENTS && page != PAGE_MAP) return;
    if (a == IN_SHOW_HIDDEN) { ev_show_hidden = !ev_show_hidden; ev_sel = ev_top = 0; return; }
    if (page != PAGE_EVENTS) return;
    if (ev_detail >= 0) { if (a == IN_BACK || a == IN_OK) ev_detail = -1; else if (a == IN_HIDE) ev_hide_toggle(ev_detail); else if (a == IN_PIN) ev_pin_toggle(ev_detail); return; }
    int ids[EVENTS], n = ev_list(ids);
    if (a == IN_HIDE) { if (n && ev_sel < n) ev_hide_toggle(ids[ev_sel]); return; }
    if (a == IN_PIN) { if (n && ev_sel < n) ev_pin_toggle(ids[ev_sel]); return; }
    if (a == IN_UP && ev_sel > 0) ev_sel--;
    else if (a == IN_DOWN && ev_sel < n - 1) ev_sel++;
    else if (a == IN_LEFT) { ev_filter = (ev_filter + NFILTERS - 1) % NFILTERS; ev_sel = ev_top = 0; }
    else if (a == IN_RIGHT) { ev_filter = (ev_filter + 1) % NFILTERS; ev_sel = ev_top = 0; }
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
        ev_filter = k; ev_sel = ev_top = 0; ev_detail = -1; break;
    case HS_ROW: {   /* a tap opens the event, a long press (0.6 s) pins / unpins it */
        int ids[EVENTS], n = ev_list(ids);
        if (k < n) { ev_sel = k; if (touch_long) ev_pin_toggle(ids[k]); else ev_detail = ids[k]; }
        break;
    }
    case HS_BACK: ev_detail = -1; break;
    case HS_PREV: ev_sel -= ev_vis; break;
    case HS_NEXT: ev_sel += ev_vis; break;
    }
}
/* touch / click in canvas pixels: act on release over the target pressed */
static int touch_id = -1, touch_t0;
static void panel_touch_canvas(int x, int y, int action) {   /* action: 0 down, 1 up, 2 move */
    int id = -1;
    if (page == PAGE_SIDE && map_touch(x, y, action)) { touch_id = -1; return; }
    if (page == PAGE_ITEMS) { int items_touch_(int, int, int); if (items_touch_(x, y, action)) { touch_id = -1; return; } }
    for (int i = nhs - 1; i >= 0; i--) if (x >= hs[i].x && y >= hs[i].y && x < hs[i].x + hs[i].w && y < hs[i].y + hs[i].h) { id = hs[i].id; break; }
    if (action == 0) { touch_id = id; touch_t0 = port_frame; }
    else if (action == 1) { touch_long = port_frame - touch_t0 >= 30; if (id >= 0 && id == touch_id) panel_hit(id); touch_id = -1; touch_long = 0; }
}
