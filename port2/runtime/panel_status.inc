/* Second screen panel: the Status tab (included by panel.c after panel_items.inc).  A 2 x 2 dashboard of cream windows:
 * vitality (the HUD's fruit at 2x, lives, AP), what is equipped (weapon, clothes, accessories as the item menu marks
 * them in use), progress (events completed of the pause menu's list, regions revealed, items, play time) and the next
 * goal (the first tracked / nearest in-progress event: icon, title, where, the hint).  Only the game's own pieces: the
 * HUD's fruit / lives / digits at whole scales, the sub-screen's stone and windows, item and event icons, WFM fonts.
 * (Chosen among four rendered layouts, 2026-11-03; the map graph page it replaces showed the area's route graph.) */

static int st_xf(int sc) { return sc >= 4 ? PA_X4 : sc == 3 ? PA_X3 : sc == 2 ? PA_X2 : 0; }

/* ---- the HUD's pieces at a whole scale sc (every texel an sc x sc block) */
static void fruit_s(int x, int y, int hp, int max, int sc) {
    static const u8 seg[4][4] = { {16, 152, 22, 33}, {32, 152, 14, 26}, {48, 152, 14, 13}, {64, 152, 22, 6} };
    int F = st_xf(sc);
    if (max > 8) max = 8;
    for (int i = 0; i < max; i++) {
        int k = i & 3, right = i >= 4, sx = right ? 72 - seg[k][2] - 16 : seg[k][2];
        pa_spr(x + sx * sc, y + seg[k][3] * sc, HUD, seg[k][0], seg[k][1], 16, 16, 368, fruit_clut(i, hp), (right ? PA_FLIPX : 0) | F);
    }
    pa_spr(x + 24 * sc, y + 16 * sc, HUD, 144, 200, 24, 24, 368, 483, F);
    { int u, v; if (!fruit_digit(hp, &u, &v)) pa_spr(x + 28 * sc, y + 19 * sc, HUD, u, v, 16, 16, 368, 489, F); }
}
/* a number in the HUD's digits (8 x 8) at scale sc, outlined; returns the width */
static int hud_num(int x, int y, const char *d, int sc) {
    int w = 0;
    for (; *d; d++) {
        if (*d >= '0' && *d <= '9') { pa_spr(x + w, y, HUD, 8 * (*d - '0'), 64, 8, 8, 368, 481, st_xf(sc) | PA_OUTLINE); w += 8 * sc; }
        else w += 4 * sc;
    }
    return w;
}
/* a progress bar: dark rim, brown well, the filled part with a light top line */
static void st_bar(int x, int y, int w, int h, int num, int den, u32 c, u32 hi) {
    pa_fill(x, y, w, h, C_DARK); pa_fill(x + 1, y + 1, w - 2, h - 2, RGB(150, 116, 96));
    int f = den > 0 ? (w - 2) * num / den : 0;
    if (f > w - 2) f = w - 2;
    if (f < 0) f = 0;
    if (f) { pa_fill(x + 1, y + 1, f, h - 2, c); pa_fill(x + 1, y + 1, f, 1, hi); }
}
static void st_time(char *b, int n) { u32 t = G_TIME / 50; snprintf(b, n, "%02u:%02u:%02u", t / 3600, t / 60 % 60, t % 60); }

/* ---- what the page shows */
static int st_weapon_item(void) {   /* the item of the equipped weapon (it_in_use's table), -1 none */
    static const short wi[10] = { 27, -1, -1, -1, -1, 21, 22, 23, 33, 34 };
    int suit = G8(PV_GAME_SUIT), wp = G_WEAPON;
    return (unsigned)(suit - 1) >= 2 && wp < 10 ? wi[wp] : -1;
}
static int st_pants_item(void) {   /* the pants worn, or the suit */
    static const short pi[4] = { 32, 29, 24, 28 };
    int suit = G8(PV_GAME_SUIT);
    if (suit >= 1 && suit <= 3) return 97 + suit;
    return G8(PV_GAME_PANTS_OK) == 0xFF && G_PANTS < 4 ? pi[G_PANTS] : -1;
}
static int st_accessories(int *ids) {   /* the other in-use marks of the item menu: umbrella, fish, jewels... */
    static const u8 acc[] = { 10, 12, 20, 96, 132 }; int n = 0;
    for (unsigned i = 0; i < sizeof acc; i++) if (it_in_use(acc[i])) ids[n++] = acc[i];
    return n;
}
static int st_regions(int *total) {   /* the map's regions revealed (D_8009D2C4) of those with routes */
    int v = 0, t = 0;
    for (int r = 0; r < 12; r++) if (nroutes[r]) { t++; if (G_MAPREG(r)) v++; }
    *total = t; return v;
}
static int st_menu_done(int *total) {   /* the menu's events: completed / all */
    int d = 0, n = ev_count();
    for (int k = 0; k < n; k++) if (ev_state(ev_id(k)) == F_DONE) d++;
    *total = n; return d;
}
static int st_events(int *ids) {   /* the in-progress events (tracked ones too), nearest first as the event list sorts them */
    int n = 0, pins[4], np = ev_pins(pins);
    for (int k = 0; k < ev_count(); k++) { int e = ev_id(k); if (ev_state(e) == F_PROGRESS && !ev_hidden_now(e)) ids[n++] = e; }
    for (int i = 0; i < np; i++) {   /* a tracked event not started yet still leads */
        int e = pins[i], have = 0;
        for (int k = 0; k < n; k++) have |= ids[k] == e;
        if (!have && ev_state(e) == F_PENDING && !ev_hidden_now(e)) ids[n++] = e;
    }
    ev_near_sort(ids, n);
    return n;
}
static const char *st_hint(int e) {
    const PanelEventLoc *L = ev_loc(e);
    return ev_giver(e) ? ev_giver(e) : L ? kind_name(L->kind) : NULL;
}
/* an event icon at scale sc centred in a box of bw x bh */
static void st_icon(int x, int y, int bw, int bh, int e, int sc, int f) {
    int w = pa_event_icon_w(e), h = pa_event_icon_h(e);
    if (w <= 0) w = 16;
    if (h <= 0) h = 16;
    pa_event_icon(x + (bw - w * sc) / 2, y + (bh - h * sc) / 2, e, PA_OUTLINE | st_xf(sc) | f);
}
static void st_item(int x, int y, int item, int sc) { if (item >= 0) pa_item_icon(x, y, item, PA_OUTLINE | st_xf(sc)); }
static void st_label(int x, int y, const char *s) { pa_text(x, y, 16, s, C_RED, C_REDE); }

/* ---- the page: 2 x 2 windows */
static void page_status(void) {
    int W = pa_cw, H = pa_ch, top = 56, g = 4, cw = (W - 16 - g) / 2, ch = (H - 8 - top - g) / 2; char b[160];
    int x0 = 8, x1 = 8 + cw + g, y0 = top, y1 = top + ch + g, w1 = W - 8 - x1, h1 = H - 8 - y1;
    /* vitality */
    window_(x0, y0, cw, ch);
    st_label(x0 + 8, y0 + 5, T("Vitality", "Vitalidad"));
    fruit_s(x0 + 4, y0 + 20, G_HEALTH, G_MAXHEALTH, 2);
    lives(x0 + cw - 46, y0 + 8, G_LIVES);
    pa_text(x0 + cw - 30 - pa_text_w(8, T("Lives", "Vidas")) / 2, y0 + 34, 8, T("Lives", "Vidas"), C_MUTED8, C_EDGE);
    ap_badge(x0 + cw - 38, y0 + 64);
    fmt_ap(b, sizeof b, G_AP); pa_text(x0 + cw - 8 - pa_text_w(16, b), y0 + 80, 16, b, C_DARK, C_EDGE);
    /* equipped */
    window_(x1, y0, w1, ch);
    st_label(x1 + 8, y0 + 5, T("Equipped", "Equipado"));
    int acc[8], na = st_accessories(acc), its[2] = { st_weapon_item(), st_pants_item() };
    for (int r = 0; r < 2; r++) {
        int ry = y0 + 24 + r * 38;
        frame_(x1 + 6, ry, 40, 36); stone(x1 + 10, ry + 4, 32, 28); st_item(x1 + 10, ry + 2, its[r], 2);
        pa_text(x1 + 52, ry + 2, 8, r ? T("Clothes", "Ropa") : T("Weapon", "Arma"), C_MUTED8, C_EDGE);
        text_wrap(x1 + 52, ry + 12, 16, w1 - 58, 13, 2, 0, its[r] >= 0 ? it_name(its[r], b, sizeof b) : T("None", "Nada"), its[r] >= 0 ? C_DARK : C_MUTED, C_EDGE);
    }
    pa_text(x1 + 8, y0 + 104, 8, T("Accessories", "Accesorios"), C_MUTED8, C_EDGE);
    if (!na) pa_text(x1 + 8, y0 + 116, 16, T("None", "Nada"), C_MUTED, C_EDGE);
    for (int i = 0; i < na && i < 5; i++) st_item(x1 + 8 + i * 22, y0 + 116, acc[i], 1);
    /* progress */
    window_(x0, y1, cw, h1);
    st_label(x0 + 8, y1 + 5, T("Progress", "Progreso"));
    int tot, d = st_menu_done(&tot), rt, rv = st_regions(&rt), ly = y1 + 26;
    snprintf(b, sizeof b, "%d", d);
    int bx = x0 + 10 + hud_num(x0 + 10, ly, b, 3) + 6;
    snprintf(b, sizeof b, T("of %d events", "de %d eventos"), tot); pa_text(bx, ly + 2, 8, b, C_MUTED8, C_EDGE);
    pa_text(bx, ly + 13, 8, T("completed", "completados"), C_MUTED8, C_EDGE);
    st_bar(x0 + 8, ly + 30, cw - 16, 8, d, tot, RGB(56, 156, 56), RGB(120, 210, 110)); ly += 46;
    snprintf(b, sizeof b, T("Regions  %d of %d", "Regiones  %d de %d"), rv, rt); pa_text(x0 + 10, ly, 16, b, C_DARK, C_EDGE); ly += 18;
    snprintf(b, sizeof b, T("Items  %d", "Objetos  %d"), G_INV_COUNT); pa_text(x0 + 10, ly, 16, b, C_DARK, C_EDGE); ly += 18;
    st_time(b, sizeof b); pa_text(x0 + 10, ly, 16, T("Time", "Tiempo"), C_RED, C_REDE); pa_text(x0 + cw - 10 - pa_text_w(16, b), ly, 16, b, C_DARK, C_EDGE);
    /* the next goal: the first of the in-progress events, nearest first */
    int ids[EVENTS], n = st_events(ids);
    window_(x1, y1, w1, h1);
    st_label(x1 + 8, y1 + 5, n && ev_pinned(ids[0]) ? T("Tracking", "Siguiendo") : T("Next goal", "Objetivo"));
    if (n > 1) { snprintf(b, sizeof b, T("+%d in progress", "+%d en curso"), n - 1); pa_text(x1 + w1 - 8 - pa_text_w(8, b), y1 + 9, 8, b, C_MUTED8, C_EDGE); }
    if (!n) { pa_text(x1 + 10, y1 + 30, 16, T("Nothing in progress", "Nada en curso"), C_MUTED, C_EDGE); return; }
    {   int e = ids[0], pend = ev_state(e) == F_PENDING; char t[96];
        frame_(x1 + 6, y1 + 24, 44, 44); stone(x1 + 10, y1 + 28, 36, 36); st_icon(x1 + 10, y1 + 28, 36, 36, e, 2, pend ? PA_DIM : 0);
        text_wrap(x1 + 56, y1 + 24, 16, w1 - 62, 15, 2, 0, pend ? "? ? ?" : ev_title(e, t, sizeof t), C_DARK, C_EDGE);
        dist_plate(x1 + 56, y1 + 56, e);
        ev_place(e, b, sizeof b);
        int nl = text_wrap(x1 + 8, y1 + 74, 8, w1 - 16, 10, 2, 0, b, C_RED8, C_REDE);
        const char *hn = pend ? NULL : st_hint(e);
        if (hn) text_wrap(x1 + 8, y1 + 76 + nl * 10, 16, w1 - 16, 15, (h1 - 80 - nl * 10) / 15, 0, hn, C_MUTED, C_EDGE);
    }
}
