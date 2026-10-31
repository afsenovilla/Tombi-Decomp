/* Second screen panel: the event browser (included by panel.c).  Every event of SCRIPT_FLAGS 0..0x81 with its icon and
 * title from the user's disc, filtered by state (in progress 1..0xFE, done 0xFF, not started 0), and a detail page:
 * where (area / room names and the map graph with a marker on the room), who / what starts it, its state.  Where an
 * event happens comes from panel_eventmap.h, generated from the decomp by port2/tools/gen_panel_events.py (ids and
 * coordinates only; docs/EVENTS_MAP.md lists the coverage); unknown places show "location unknown".
 * Input: touch (Android: nativePanelTouch from the Presentation), the mouse in the PC window, and the keyboard of the
 * PC window (arrows, Enter, Backspace, Q / E: tabs). */
#include "panel_eventmap.h"

enum { F_PROGRESS, F_DONE, F_PENDING, NFILTERS };
static int side_zoom;
static int ev_filter, ev_sel, ev_top, ev_detail = -1;

static const PanelEventLoc *ev_loc(int e) {
    for (unsigned i = 0; i < sizeof panel_event_locs / sizeof panel_event_locs[0]; i++) if (panel_event_locs[i].ev == e) return &panel_event_locs[i];
    return NULL;
}
static int ev_state(int e) { int v = G_EVENT(e); return v == 0xFF ? F_DONE : v ? F_PROGRESS : F_PENDING; }
static int ev_list(int *ids) {
    int n = 0;
    for (int e = 0; e < EVENTS; e++) if (ev_state(e) == ev_filter && (ev_name[e] || ev_loc(e))) ids[n++] = e;
    return n;
}
static const char *filter_name(int f) {
    return f == F_PROGRESS ? T("In progress", "En curso") : f == F_DONE ? T("Done", "Hechos") : T("Not started", "Pendientes");
}
static const char *state_name(int f) {
    return f == F_PROGRESS ? T("In progress", "En curso") : f == F_DONE ? T("Completed", "Completado") : T("Not started", "Sin empezar");
}
/* the area name of the map screen for (area, section) */
static const char *place_area(int area, int sec) {
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

static void event_list(void) {
    int ids[EVENTS], n = ev_list(ids), W = pa_cw, H = pa_ch;
    char b[96];
    filters(56);
    int y0 = 84, rh = 30, rows = (H - y0 - 26) / rh;
    window_(8, y0 - 2, W - 16, rows * rh + 24);
    if (ev_sel >= n) ev_sel = n - 1; if (ev_sel < 0) ev_sel = 0;
    if (ev_sel < ev_top) ev_top = ev_sel; if (ev_sel >= ev_top + rows) ev_top = ev_sel - rows + 1;
    if (ev_top > n - rows) ev_top = n - rows; if (ev_top < 0) ev_top = 0;
    if (!n) pa_text(20, y0 + 8, 16, T("No events here", "No hay eventos"), C_MUTED, C_EDGE);
    for (int r = 0; r < rows && ev_top + r < n; r++) {
        int e = ids[ev_top + r], y = y0 + 4 + r * rh, sel = ev_top + r == ev_sel, st = ev_state(e);
        const PanelEventLoc *L = ev_loc(e);
        if (sel) pa_fill(12, y - 2, W - 24, rh - 1, C_SEL);
        hot(12, y - 2, W - 24, rh - 1, HS_ROW + ev_top + r);
        int pend = st == F_PENDING;
        pa_event_icon(16, y + 5, e, pend ? PA_DIM : 0);
        pa_text_fit(46, y - 1, 16, W - 60, pend ? "? ? ?" : ev_title(e, b, sizeof b), sel ? C_WHITE : st == F_DONE ? C_GREEN : C_DARK, sel ? RGB(60, 110, 50) : C_EDGE);
        const char *ar = L ? place_area(L->area, L->sec) : NULL;
        snprintf(b, sizeof b, "%s  -  %s", state_name(st), ar ? ar : T("location unknown", "ubicaci\xF3n desconocida"));
        pa_text_fit(46, y + 16, 8, W - 60, b, sel ? C_WHITE : C_MUTED8, sel ? RGB(60, 110, 50) : RGB(230, 200, 190));
    }
    int by = y0 + rows * rh + 4;
    snprintf(b, sizeof b, "%d / %d", n ? ev_sel + 1 : 0, n);
    pa_text(W - 24 - pa_text_w(16, b), by, 16, b, C_DARK, C_EDGE);
    int s, d; count_events(&s, &d);
    snprintf(b, sizeof b, T("%d started, %d done", "%d empezados, %d hechos"), s + d, d);
    pa_text(20, by + 4, 8, b, C_MUTED8, RGB(230, 200, 190));
    if (ev_top > 0) { pa_text(W - 40, y0 + 2, 16, "^", C_DARK, C_EDGE); }
    hot(8, y0 - 2, W - 16, 20, HS_PREV); hot(8, by - 4, W / 2, 24, HS_NEXT);
}

static void event_detail(int e) {
    char b[160]; int W = pa_cw, H = pa_ch, st = ev_state(e), pend = st == F_PENDING;
    const PanelEventLoc *L = ev_loc(e);
    filters(56);
    /* title: the icon x2 and the 24 px event-title letters */
    int y = 84;
    window_(8, y, W - 16, 38);
    hot(8, y, W - 16, 38, HS_BACK);
    {   int cx0 = pa_clip_x0; (void)cx0;
        /* icon scaled x2 by drawing it into the canvas twice as big */
        int x0 = 16, y0 = y + 7;
        pa_event_icon(x0, y0 + 4, e, pend ? PA_DIM : 0);
        const char *t = pend ? "? ? ?" : ev_title(e, b, sizeof b);
        if (pa_text_w(24, t) <= W - 60) pa_text(48, y + 7, 24, t, C_WHITE, C_DARK);
        else pa_text_fit(48, y + 11, 16, W - 64, t, C_DARK, C_EDGE);
    }
    y += 42;
    int ww = W * 58 / 100, wh = H - y - 70;
    if (wh < 110) wh = 110;
    /* where */
    window_(8, y, ww, wh);
    pa_text(16, y + 5, 16, T("Where", "D\xF3nde"), C_DARK, C_EDGE);
    const char *ar = L ? place_area(L->area, L->sec) : NULL, *rm = L ? place_room(L->area, L->sec) : NULL;
    if (ar) {
        snprintf(b, sizeof b, "%s%s%s", ar, rm ? "  -  " : "", rm ? rm : "");
        pa_text_fit(16, y + 22, 8, ww - 20, b, C_RED8, C_REDE);
        int l = L->area < 20 && L->sec < 24 ? roomlink[L->area][L->sec] : -1;
        map_mark_link = l; map_mark_ev = e;
        draw_map(12, y + 32, ww - 8, wh - 36, -1);
        map_mark_link = -1;
    } else {
        pa_text(16, y + 26, 16, T("Location unknown", "Ubicaci\xF3n desconocida"), C_MUTED, C_EDGE);
        pa_text_fit(16, y + 46, 8, ww - 20, T("Not found in the decomp yet", "A\xFAn no localizada en la decomp"), C_MUTED8, RGB(230, 200, 190));
    }
    /* who */
    int wx = 8 + ww + 6, wwid = W - 8 - wx;
    window_(wx, y, wwid, wh);
    pa_text(wx + 8, y + 5, 16, T("Who", "Qui\xE9n"), C_DARK, C_EDGE);
    const char *kn = L ? kind_name(L->kind) : NULL;
    if (kn && !pend) {
        pa_text_fit(wx + 8, y + 26, 16, wwid - 16, kn, C_DARK, C_EDGE);
        if (L->x != -32768) { snprintf(b, sizeof b, "x %d  y %d", L->x, L->y); pa_text(wx + 8, y + 44, 8, b, C_RED8, C_REDE); }
    } else pa_text(wx + 8, y + 26, 16, "? ? ?", C_MUTED, C_EDGE);
    face(wx + wwid - 34, y + wh - 34);
    /* state */
    int sy = y + wh + 4;
    window_(8, sy, W - 16, H - sy - 6);
    pa_text(16, sy + 6, 16, T("State", "Estado"), C_DARK, C_EDGE);
    pa_text(16 + pa_text_w(16, T("State", "Estado")) + 16, sy + 6, 16, state_name(st), st == F_PROGRESS ? C_ORANGE : st == F_DONE ? C_GREEN : C_MUTED, C_REDE);
    pa_text_fit(16, sy + 26, 8, W - 32, T("Back: tap the title or press Backspace", "Volver: toca el t\xEDtulo o pulsa Retroceso"), C_MUTED8, RGB(230, 200, 190));
}

static void page_events(int play) {
    if (!play) { window_(8, 56, pa_cw - 16, pa_ch - 64); pa_text(20, 70, 16, T("Waiting for the game...", "Esperando a la partida..."), C_MUTED, C_EDGE); return; }
    if (ev_detail >= 0) event_detail(ev_detail); else event_list();
}

/* ---------------------------------------------------------------- input */
enum { IN_UP = 1, IN_DOWN, IN_LEFT, IN_RIGHT, IN_OK, IN_BACK, IN_TAB_L, IN_TAB_R };
static void panel_action(int a) {
    if (a == IN_TAB_L) { page = (page + NPAGES - 1) % NPAGES; return; }
    if (a == IN_TAB_R) { page = (page + 1) % NPAGES; return; }
    if (page == PAGE_SIDE && (a == IN_LEFT || a == IN_RIGHT || a == IN_OK)) { side_zoom ^= 1; return; }
    if (page == PAGE_ITEMS) { void items_action_(int); items_action_(a); return; }
    if (page != PAGE_EVENTS) return;
    if (ev_detail >= 0) { if (a == IN_BACK || a == IN_OK) ev_detail = -1; return; }
    int ids[EVENTS], n = ev_list(ids);
    if (a == IN_UP && ev_sel > 0) ev_sel--;
    else if (a == IN_DOWN && ev_sel < n - 1) ev_sel++;
    else if (a == IN_LEFT) { ev_filter = (ev_filter + NFILTERS - 1) % NFILTERS; ev_sel = ev_top = 0; }
    else if (a == IN_RIGHT) { ev_filter = (ev_filter + 1) % NFILTERS; ev_sel = ev_top = 0; }
    else if (a == IN_OK && n) ev_detail = ids[ev_sel];
}
static void panel_hit(int id) {
    int k = id & 0xFF;
    if (id >= 0x700) { void items_hit_(int); items_hit_(id); return; }
    switch (id & ~0xFF) {
    case HS_PAGE: page = k; ev_detail = -1; break;
    case HS_FILTER: if (k >= 16) { side_zoom = k - 16; break; } ev_filter = k; ev_sel = ev_top = 0; ev_detail = -1; break;
    case HS_ROW: { int ids[EVENTS], n = ev_list(ids); if (k < n) { if (ev_sel == k || 1) { ev_sel = k; ev_detail = ids[k]; } } break; }
    case HS_BACK: ev_detail = -1; break;
    case HS_PREV: ev_sel -= 5; break;
    case HS_NEXT: ev_sel += 5; break;
    }
}
/* touch / click in canvas pixels: act on release over the target pressed */
static int touch_id = -1;
static void panel_touch_canvas(int x, int y, int action) {   /* action: 0 down, 1 up, 2 move */
    int id = -1;
    for (int i = nhs - 1; i >= 0; i--) if (x >= hs[i].x && y >= hs[i].y && x < hs[i].x + hs[i].w && y < hs[i].y + hs[i].h) { id = hs[i].id; break; }
    if (action == 0) touch_id = id;
    else if (action == 1) { if (id >= 0 && id == touch_id) panel_hit(id); touch_id = -1; }
}
