/* Second screen panel: alternative layouts of the Status tab (included by panel.c after panel_items.inc), behind
 * PORT_PANEL_STATUS_STYLE (trial): 0 the current page (map graph, stats, in progress), 1 character sheet, 2 "what
 * next" (the in-progress events as cards, nearest first), 3 dashboard (2 x 2 windows), 4 showcase like the item tab
 * (portrait and stats on the left, the in-progress events scrolling on the right).  Only the game's own pieces: the
 * HUD's fruit / lives / digits at whole scales, the sub-screen's stone, planks, cream windows and Tomba's face, the
 * item and event icons, the WFM fonts. */

static int st_style_v = -1;
static int st_style(void) {
    if (st_style_v < 0) { const char *s = getenv("PORT_PANEL_STATUS_STYLE"); st_style_v = s ? atoi(s) : 0; if (st_style_v < 0 || st_style_v > 4) st_style_v = 0; }
    return st_style_v;
}
static int st_xf(int sc) { return sc >= 4 ? PA_X4 : sc == 3 ? PA_X3 : sc == 2 ? PA_X2 : 0; }

/* ---- the HUD's pieces at a whole scale sc (every texel an sc x sc block) */
static void fruit_s(int x, int y, int hp, int max, int sc) {
    static const u8 seg[4][4] = { {16, 152, 22, 33}, {32, 152, 14, 26}, {48, 152, 14, 13}, {64, 152, 22, 6} };
    int F = st_xf(sc);
    if (max > 8) max = 8;
    for (int i = 0; i < max; i++) {
        int k = i & 3, right = i >= 4, sx = right ? 72 - seg[k][2] - 16 : seg[k][2];
        pa_spr(x + sx * sc, y + seg[k][3] * sc, HUD, seg[k][0], seg[k][1], 16, 16, 368, 502, (right ? PA_FLIPX : 0) | (i < hp ? 0 : PA_SHADE) | F);
    }
    pa_spr(x + 24 * sc, y + 16 * sc, HUD, 144, 200, 24, 24, 368, 483, F);
    if (hp >= 0 && hp <= 8) pa_spr(x + 28 * sc, y + 19 * sc, HUD, fruit_digit_u[hp], hp == 8 ? 116 : 120, 16, 16, 368, 489, F);
}
static void lives_s(int x, int y, int stored, int sc) {
    int n = stored - 1, F = st_xf(sc);
    pa_spr(x, y, HUD, 128, 56, 32, 24, 368, 481, F);
    if (n >= 10 && n <= 99) { pa_spr(x + 7 * sc, y + 12 * sc, HUD, 8 * (n / 10), 64, 8, 8, 368, 481, F); pa_spr(x + 17 * sc, y + 12 * sc, HUD, 8 * (n % 10), 64, 8, 8, 368, 481, F); }
    else if (n >= 0 && n <= 9) pa_spr(x + 12 * sc, y + 12 * sc, HUD, 8 * n, 64, 8, 8, 368, 481, F);
}
static void ap_badge_s(int x, int y, int s) {
    if (s <= 1) { ap_badge(x, y); return; }
    pa_fill(x + s, y, 28 * s, 14 * s, RGB(200, 176, 64)); pa_fill(x, y + s, 30 * s, 12 * s, RGB(200, 176, 64));
    pa_fill(x + s, y + s, 28 * s, 12 * s, RGB(96, 96, 140)); pa_fill(x + 2 * s, y + 2 * s, 26 * s, 4 * s, RGB(130, 130, 176));
    u32 o = pa_outline; pa_outline = C_DARK;
    pa_text_scaled(x + (15 - pa_text_w(8, "AP") / 2) * s, y + 3 * s, 8, "AP", C_WHITE, RGB(40, 30, 70), s, 0);
    pa_outline = o;
}
/* a number in the HUD's digits (8 x 8) at scale sc; returns the width */
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
    int f = den > 0 ? (w - 2) * num / den : 0; if (f > w - 2) f = w - 2; if (f < 0) f = 0;
    if (f) { pa_fill(x + 1, y + 1, f, h - 2, c); pa_fill(x + 1, y + 1, f, 1, hi); }
}
static void st_time(char *b, int n) { u32 t = G_TIME / 50; snprintf(b, n, "%02u:%02u:%02u", t / 3600, t / 60 % 60, t % 60); }

/* ---- what the layouts show */
static int st_weapon_item(void) {   /* the item of the equipped weapon (it_in_use's table), -1 none */
    static const short wi[10] = { 27, -1, -1, -1, -1, 21, 22, 23, 33, 34 };
    int suit = G8(0x8009C990u), wp = G_WEAPON;
    return (unsigned)(suit - 1) >= 2 && wp < 10 ? wi[wp] : -1;
}
static int st_pants_item(void) {
    static const short pi[4] = { 32, 29, 24, 28 };
    int suit = G8(0x8009C990u);
    if (suit >= 1 && suit <= 3) return 97 + suit;
    return G8(0x8009D090u) == 0xFF && G_PANTS < 4 ? pi[G_PANTS] : -1;
}
static int st_accessories(int *ids) {   /* the other in-use marks of the item menu: umbrella, fish, jewels... */
    static const u8 acc[] = { 10, 12, 20, 96, 132 }; int n = 0;
    for (unsigned i = 0; i < sizeof acc; i++) if (it_in_use(acc[i])) ids[n++] = acc[i];
    return n;
}
static int st_regions(int *total) {
    int v = 0, t = 0;
    for (int r = 0; r < 12; r++) if (nroutes[r]) { t++; if (G_MAPREG(r)) v++; }
    *total = t; return v;
}
static int st_menu_done(int *total) {   /* the menu's events: completed / all */
    int d = 0, n = ev_count();
    for (int k = 0; k < n; k++) if (ev_state(ev_id(k)) == F_DONE) d++;
    *total = n; return d;
}
/* how far an event is: 0 this room, 1 this area, 2 another area, 3 anywhere (an item), 4 not known; the grandfather's
   bracelet (event 0: in progress all game long) last */
static int st_dist(int e) {
    const PanelEventLoc *L = ev_loc(e);
    if (e == 0) return 6;
    if (!L) return 4;
    if (L->area == 254) return 3;
    if (L->area != G_AREA) return 2;
    return L->sec == G_SECTION ? 0 : 1;
}
static int st_events(int *ids, int *tracked) {   /* tracked first (not completed), then the in-progress ones, nearest first */
    int n = 0, pins[4], np = ev_pins(pins);
    for (int i = 0; i < np; i++) if (ev_state(pins[i]) != F_DONE && !ev_hidden_now(pins[i])) ids[n++] = pins[i];
    *tracked = n;
    for (int d = 0; d <= 6; d++) {
        int s = n;
        for (int k = 0; k < ev_count(); k++) {
            int e = ev_id(k), p = 0;
            for (int i = 0; i < *tracked; i++) p |= ids[i] == e;
            if (!p && ev_state(e) == F_PROGRESS && !ev_hidden_now(e) && st_dist(e) == d) ids[n++] = e;
        }
        if (d == 0)   /* this room: by distance to Tomba */
            for (int i = s; i < n; i++) for (int j = i + 1; j < n; j++) {
                const PanelEventLoc *a = ev_loc(ids[i]), *b = ev_loc(ids[j]);
                int da = a->x == -32768 ? 99999 : abs(a->x - P_X), db = b->x == -32768 ? 99999 : abs(b->x - P_X);
                if (db < da) { int t = ids[i]; ids[i] = ids[j]; ids[j] = t; }
            }
    }
    return n;
}
static const char *st_dist_name(int d) {
    switch (d) {
    case 0: return T("Here", "Aqu\xED");
    case 1: return T("This area", "Esta zona");
    case 2: return T("Elsewhere", "Otra zona");
    case 3: return T("Anywhere", "Cualquiera");
    default: return NULL;
    }
}
static void st_place(int e, char *b, int n) {
    const PanelEventLoc *L = ev_loc(e);
    const char *ar = L ? place_area(L->area, L->sec) : NULL, *rm = L && L->area != 254 ? place_room(L->area, L->sec) : NULL;
    if (rm && ar && !strcmp(rm, ar)) rm = NULL;
    else if (rm && L->area == G_AREA) ar = NULL;   /* this area (the title): the room only */
    if (!ar && rm) { snprintf(b, n, "%s", rm); return; }
    snprintf(b, n, "%s%s%s", ar ? ar : T("zone not extracted yet", "zona a\xFAn sin extraer"), rm ? " - " : "", rm ? rm : "");
}
static const char *st_hint(int e) {
    const PanelEventLoc *L = ev_loc(e);
    return ev_giver(e) ? ev_giver(e) : L ? kind_name(L->kind) : NULL;
}
/* an event icon at scale sc centred in a box of bw x bh */
static void st_icon(int x, int y, int bw, int bh, int e, int sc) {
    int w = pa_event_icon_w(e), h = pa_event_icon_h(e);
    if (w <= 0) w = 16;
    if (h <= 0) h = 16;
    pa_event_icon(x + (bw - w * sc) / 2, y + (bh - h * sc) / 2, e, PA_OUTLINE | st_xf(sc));
}
static void st_item(int x, int y, int item, int sc) { if (item >= 0) pa_item_icon(x, y, item, PA_OUTLINE | st_xf(sc)); }
static void st_label(int x, int y, const char *s) { pa_text(x, y, 16, s, C_RED, C_REDE); }
static const char *st_room(int play, int link) {
    const char *an, *rn; room_names(play, G_AREA, G_SECTION, link, &an, &rn); return rn;
}
/* a line of light text with the game's dark outline (over stone / wood) */
static void st_ol(int x, int y, int h, const char *s, u32 c) { pa_text_ol(x, y, h, s, c, C_WOODE, C_DARK); }

/* ---- 1: character sheet */
static void st_sheet(int play, int link) {
    int W = pa_cw, H = pa_ch, top = 56; char b[160];
    window_(8, top, W - 16, 118);
    fruit_s(14, top + 6, G_HEALTH, G_MAXHEALTH, 2);
    int c2 = 170, c3 = 252;
    lives_s(c2, top + 12, G_LIVES, 2);
    pa_text(c2 + 32 - pa_text_w(8, T("Lives", "Vidas")) / 2, top + 64, 8, T("Lives", "Vidas"), C_MUTED8, C_EDGE);
    ap_badge_s(c3, top + 10, 2);
    fmt_ap(b, sizeof b, G_AP);
    { u32 o = pa_outline; pa_outline = 0; int tw = 2 * pa_text_w(16, b); if (tw > W - 16 - c3) tw = W - 16 - c3; pa_text_scaled(c3, top + 44, 16, b, C_DARK, C_EDGE, 2, 0); pa_outline = o; (void)tw; }
    pa_text(c2, top + 92, 16, T("Time", "Tiempo"), C_RED, C_REDE);
    st_time(b, sizeof b); pa_text(c2 + pa_text_w(16, T("Time", "Tiempo")) + 10, top + 92, 16, b, C_DARK, C_EDGE);
    { extern int panel_weapon_icon(int, int); panel_weapon_icon(W - 40, top + 86); }
    /* equipment */
    int y = top + 122, h = H - 8 - y, lw = 190;
    window_(8, y, lw, h);
    st_label(16, y + 5, T("Equipped", "Equipo"));
    int wi = st_weapon_item(), pn = st_pants_item(), acc[8], na = st_accessories(acc);
    const char *lab[2] = { T("Weapon", "Arma"), T("Clothes", "Ropa") }; int its[2] = { wi, pn };
    for (int r = 0; r < 2; r++) {
        int ry = y + 24 + r * 42;
        frame_(14, ry, 40, 40); stone(18, ry + 4, 32, 32); st_item(18, ry + 4, its[r], 2);
        pa_text(60, ry + 2, 8, lab[r], C_MUTED8, C_EDGE);
        text_wrap(60, ry + 11, 16, lw - 60, 14, 2, 0, its[r] >= 0 ? it_name(its[r], b, sizeof b) : T("None", "Nada"), its[r] >= 0 ? C_DARK : C_MUTED, C_EDGE);
    }
    int ay = y + 112;
    pa_text(16, ay, 8, T("Accessories", "Accesorios"), C_MUTED8, C_EDGE);
    if (!na) pa_text(16, ay + 12, 16, T("None", "Nada"), C_MUTED, C_EDGE);
    for (int i = 0; i < na && i < 5; i++) { frame_(14 + i * 38, ay + 10, 36, 36); stone(18 + i * 38, ay + 14, 28, 28); st_item(16 + i * 38, ay + 12, acc[i], 2); }
    /* progress */
    int px = 8 + lw + 4, pw2 = W - 8 - px, tot, d = st_menu_done(&tot), s, dd, rt, rv = st_regions(&rt);
    count_events(&s, &dd);
    window_(px, y, pw2, h);
    st_label(px + 8, y + 5, T("Progress", "Progreso"));
    int ly = y + 24, bw = pw2 - 16;
    snprintf(b, sizeof b, "%d / %d", d, tot);
    pa_text(px + 8, ly, 8, T("Completed", "Completados"), C_MUTED8, C_EDGE); pa_text(px + pw2 - 8 - pa_text_w(8, b), ly, 8, b, C_DARK, C_EDGE);
    st_bar(px + 8, ly + 10, bw, 8, d, tot, RGB(56, 156, 56), RGB(120, 210, 110)); ly += 24;
    snprintf(b, sizeof b, "%d", s);
    pa_text(px + 8, ly, 8, T("In progress", "En curso"), C_MUTED8, C_EDGE); pa_text(px + pw2 - 8 - pa_text_w(8, b), ly, 8, b, C_DARK, C_EDGE);
    st_bar(px + 8, ly + 10, bw, 8, s, tot, RGB(232, 140, 40), RGB(255, 200, 110)); ly += 24;
    snprintf(b, sizeof b, "%d / %d", rv, rt);
    pa_text(px + 8, ly, 8, T("Regions", "Regiones"), C_MUTED8, C_EDGE); pa_text(px + pw2 - 8 - pa_text_w(8, b), ly, 8, b, C_DARK, C_EDGE);
    st_bar(px + 8, ly + 10, bw, 8, rv, rt, RGB(70, 120, 200), RGB(150, 190, 240)); ly += 24;
    snprintf(b, sizeof b, "%d", G_INV_COUNT);
    pa_text(px + 8, ly, 8, T("Items carried", "Objetos"), C_MUTED8, C_EDGE); pa_text(px + pw2 - 8 - pa_text_w(8, b), ly, 8, b, C_DARK, C_EDGE); ly += 14;
    pa_fill(px + 10, ly, pw2 - 20, 1, RGB(230, 200, 180)); ly += 5;
    const char *rn = st_room(play, link);
    pa_text(px + 8, ly, 8, T("You are in", "Lugar"), C_MUTED8, C_EDGE);
    text_wrap(px + 8, ly + 10, 16, pw2 - 16, 15, 2, 0, rn ? rn : "-", C_DARK, C_EDGE);
}

/* an event card: icon at 2x on stone, title, where (red, 8 px), the hint (16 px) */
static void st_card(int x, int y, int w, int h, int e, int tag) {
    char b[160], t[96]; int d = st_dist(e);
    window_(x, y, w, h);
    frame_(x + 6, y + (h - 44) / 2, 44, 44); stone(x + 10, y + (h - 44) / 2 + 4, 36, 36);
    st_icon(x + 10, y + (h - 44) / 2 + 4, 36, 36, e, 2);
    int tx = x + 56, tw = w - 64;
    const char *dn = tag ? T("Tracked", "Siguiendo") : st_dist_name(d);
    int dw = dn ? pa_text_w(8, dn) + 8 : 0, sq = dn && !tag && d && tw < 220;   /* narrow card: a coloured square but for "here" */
    if (sq) dw = 10;
    pa_text_fit(tx, y + 4, 16, tw, ev_title(e, t, sizeof t), C_DARK, C_EDGE);
    if (dn) {   /* a small plate before the place: green here, orange this area, brown elsewhere, red tracked */
        u32 c = tag ? RGB(176, 40, 30) : d == 0 ? RGB(56, 140, 56) : d == 1 ? RGB(200, 110, 30) : RGB(130, 96, 80);
        pa_fill(tx, y + 21, dw, 12, C_DARK); pa_fill(tx + 1, y + 22, dw - 2, 10, c);
        if (!sq) pa_text_ol(tx + 4, y + 23, 8, dn, C_WHITE, C_WOODE, 0);
    }
    st_place(e, b, sizeof b);
    pa_text_fit(tx + (dw ? dw + 5 : 0), y + 23, 8, tw - (dw ? dw + 5 : 0), b, C_RED8, C_REDE);
    const char *hn = st_hint(e);
    if (hn && h >= 52) text_wrap(tx, y + 35, 16, tw, 15, (h - 37) / 15, 0, hn, C_MUTED, C_EDGE);
}

/* ---- 2: what next */
static void st_next(int play, int link) {
    int W = pa_cw, H = pa_ch, top = 56; char b[160];
    int ids[EVENTS], tr, n = st_events(ids, &tr);
    /* where you are */
    window_(8, top, W - 16, 44);
    face(14, top + 10);
    const char *rn = st_room(play, link);
    if (!rn) rn = "-";
    int tw = W - 16 - 46 - 80;
    if (pa_has_glyphs(24, rn) && pa_text_w(24, rn) <= tw) pa_text(44, top + 4, 24, rn, C_WHITE, C_DARK);
    else pa_text_fit(44, top + 8, 16, tw, rn, C_DARK, C_EDGE);
    pa_text(44, top + 30, 8, T("You are here", "Est\xE1s aqu\xED"), C_MUTED8, C_EDGE);
    snprintf(b, sizeof b, T("%d in progress", "%d en curso"), n);
    pa_text(W - 16 - pa_text_w(16, b), top + 6, 16, b, C_RED, C_REDE);
    int s, d; count_events(&s, &d); (void)s;
    snprintf(b, sizeof b, T("%d completed", "%d completados"), d);
    pa_text(W - 16 - pa_text_w(8, b), top + 26, 8, b, C_MUTED8, C_EDGE);
    /* the cards */
    int sy = H - 8 - 56, cy = top + 48, ch = (sy - 4 - cy - 2 * 4) / 3;
    if (!n) { window_(8, cy, W - 16, sy - 4 - cy); pa_text(20, cy + 12, 16, T("Nothing in progress right now", "Nada en curso ahora mismo"), C_MUTED, C_EDGE); }
    for (int i = 0; i < n && i < 3; i++) {
        int y = cy + i * (ch + 4);
        if (i == 2 && n > 3) {   /* the third slot: one more card, shorter, and the count of the rest */
            st_card(8, y, W - 16, ch, ids[i], i < tr);
            snprintf(b, sizeof b, T("+ %d more in Events", "+ %d m\xE1s en Eventos"), n - 3);
            pa_text(W - 16 - pa_text_w(8, b), y + ch - 12, 8, b, C_MUTED8, C_EDGE);
        } else st_card(8, y, W - 16, ch, ids[i], i < tr);
    }
    /* the stats strip */
    stone(8, sy, W - 16, 56); frame_(8, sy, W - 16, 56);
    fruit_s(12, sy + 3, G_HEALTH, G_MAXHEALTH, 1);
    lives_s(92, sy + 14, G_LIVES, 1);
    ap_badge(136, sy + 12); fmt_ap(b, sizeof b, G_AP); st_ol(170, sy + 11, 16, b, C_WHITE);
    st_ol(136, sy + 31, 8, T("Time", "Tiempo"), C_YELLOW);
    st_time(b, sizeof b); st_ol(136 + pa_text_w(8, T("Time", "Tiempo")) + 6, sy + 28, 16, b, C_WHITE);
    { extern int panel_weapon_icon(int, int); int wi = st_weapon_item(); if (wi >= 0) { frame_(W - 54, sy + 6, 44, 44); stone(W - 50, sy + 10, 36, 36); st_item(W - 48, sy + 12, wi, 2); } else panel_weapon_icon(W - 40, sy + 20); }
}

/* ---- 3: dashboard, 2 x 2 */
static void st_dash(int play, int link) {
    int W = pa_cw, H = pa_ch, top = 56, g = 4, cw = (W - 16 - g) / 2, ch = (H - 8 - top - g) / 2; char b[160];
    int x0 = 8, x1 = 8 + cw + g, y0 = top, y1 = top + ch + g, w1 = W - 8 - x1, h1 = H - 8 - y1;
    /* vitality */
    window_(x0, y0, cw, ch);
    st_label(x0 + 8, y0 + 5, T("Vitality", "Vitalidad"));
    fruit_s(x0 + 4, y0 + 20, G_HEALTH, G_MAXHEALTH, 2);
    lives_s(x0 + cw - 46, y0 + 8, G_LIVES, 1);
    ap_badge(x0 + cw - 38, y0 + 64);
    fmt_ap(b, sizeof b, G_AP); pa_text(x0 + cw - 8 - pa_text_w(16, b), y0 + 80, 16, b, C_DARK, C_EDGE);
    pa_text(x0 + cw - 30 - pa_text_w(8, T("Lives", "Vidas")) / 2, y0 + 34, 8, T("Lives", "Vidas"), C_MUTED8, C_EDGE);
    /* equipped */
    window_(x1, y0, w1, ch);
    st_label(x1 + 8, y0 + 5, T("Equipped", "Equipado"));
    int wi = st_weapon_item(), pn = st_pants_item(), acc[8], na = st_accessories(acc), its[2] = { wi, pn };
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
    /* the current objective: the first tracked / nearest in-progress event */
    int ids[EVENTS], tr, n = st_events(ids, &tr);
    window_(x1, y1, w1, h1);
    st_label(x1 + 8, y1 + 5, tr ? T("Tracking", "Siguiendo") : T("Next goal", "Objetivo"));
    if (n > 1) { snprintf(b, sizeof b, T("+%d in progress", "+%d en curso"), n - 1); pa_text(x1 + w1 - 8 - pa_text_w(8, b), y1 + 9, 8, b, C_MUTED8, C_EDGE); }
    if (!n) { pa_text(x1 + 10, y1 + 30, 16, T("Nothing in progress", "Nada en curso"), C_MUTED, C_EDGE); return; }
    {   int e = ids[0], dd = st_dist(e); char t[96];
        frame_(x1 + 6, y1 + 24, 44, 44); stone(x1 + 10, y1 + 28, 36, 36); st_icon(x1 + 10, y1 + 28, 36, 36, e, 2);
        text_wrap(x1 + 56, y1 + 24, 16, w1 - 62, 15, 2, 0, ev_title(e, t, sizeof t), C_DARK, C_EDGE);
        const char *dn = st_dist_name(dd); if (dn) pa_text(x1 + 56, y1 + 58, 8, dn, dd == 0 ? C_GREEN : C_MUTED8, C_EDGE);
        st_place(e, b, sizeof b);
        int nl = text_wrap(x1 + 8, y1 + 74, 8, w1 - 16, 10, 2, 0, b, C_RED8, C_REDE);
        const char *hn = st_hint(e);
        if (hn) text_wrap(x1 + 8, y1 + 76 + nl * 10, 16, w1 - 16, 15, (h1 - 80 - nl * 10) / 15, 0, hn, C_MUTED, C_EDGE);
    }
}

/* ---- 4: showcase like the item tab */
static float st_scroll; static int st_drag, st_dy0, st_vy, st_vh, st_vx, st_vw, st_total; static float st_s0, st_vel; static int st_dlast;
static void st_show(int play, int link) {
    int W = pa_cw, H = pa_ch, top = 56, lw = 168, lx = 8, h = H - 8 - top, rx = lx + lw + 4, rw = W - 8 - rx; char b[160];
    (void)play; (void)link;
    window_(lx, top, lw, h);
    int cx = lx + lw / 2;
    frame_(cx - 41, top + 6, 82, 82);
    stone(cx - 36, top + 11, 72, 72);
    face_f(cx - 36, top + 11, PA_X3);
    int y = top + 80;
    fruit_s(lx + 12, y, G_HEALTH, G_MAXHEALTH, 2);
    y += 106;
    lives_s(lx + lw - 44, y - 6, G_LIVES, 1);
    ap_badge(lx + 10, y + 1); fmt_ap(b, sizeof b, G_AP); pa_text_fit(lx + 46, y, 16, lw - 96, b, C_DARK, C_EDGE);
    y += 20;
    pa_text(lx + 10, y, 16, T("Time", "Tiempo"), C_RED, C_REDE);
    st_time(b, sizeof b); pa_text(lx + lw - 10 - pa_text_w(16, b), y, 16, b, C_DARK, C_EDGE);
    y += 20;
    pa_fill(lx + 12, y, lw - 24, 1, RGB(230, 200, 180)); y += 4;
    pa_text(lx + 10, y, 8, T("Equipped", "Equipo"), C_MUTED8, C_EDGE); y += 11;
    {   int its[8], n = 0, wi = st_weapon_item(), pn = st_pants_item();
        if (wi >= 0) its[n++] = wi;
        if (pn >= 0) its[n++] = pn;
        n += st_accessories(its + n);
        for (int i = 0; i < n && i < 4; i++) { frame_(lx + 8 + i * 38, y, 36, 36); stone(lx + 12 + i * 38, y + 4, 28, 28); st_item(lx + 10 + i * 38, y + 2, its[i], 2); }
        (void)b;
    }
    /* the right column: completed bar, then the in-progress events scrolling */
    stone(rx, top, rw, h);
    frame_(rx, top, rw, h);
    int tot, d = st_menu_done(&tot);
    snprintf(b, sizeof b, T("Completed %d of %d", "Completados %d de %d"), d, tot);
    st_ol(rx + 10, top + 7, 16, b, C_WHITE);
    st_bar(rx + 10, top + 26, rw - 20, 8, d, tot, RGB(56, 156, 56), RGB(120, 210, 110));
    int ids[EVENTS], tr, n = st_events(ids, &tr), ch = 64;
    int vx = rx + 6, vy = top + 40, vw = rw - 12 - 6, vh = h - 46;
    st_vx = vx; st_vy = vy; st_vw = vw; st_vh = vh; st_total = n * ch;
    if (!st_drag) { st_scroll += st_vel; st_vel *= .9f; if (st_vel > -.3f && st_vel < .3f) st_vel = 0; }
    float mx = st_total > vh ? (float)(st_total - vh) : 0;
    if (st_scroll > mx) { st_scroll = mx; st_vel = 0; } if (st_scroll < 0) { st_scroll = 0; st_vel = 0; }
    int so = (int)st_scroll;
    int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
    pa_clip_x0 = vx - 2; pa_clip_y0 = vy - 1; pa_clip_x1 = vx + vw + 2; pa_clip_y1 = vy + vh;
    for (int i = 0; i < n; i++) {
        int cy = vy + i * ch - so + 2;
        if (cy + ch <= vy || cy >= vy + vh) continue;
        st_card(vx + 1, cy, vw - 2, ch - 4, ids[i], i < tr);
        if (i == 0) it_selframe(vx + 1, cy, vw - 2, ch - 4);
    }
    pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
    if (!n) st_ol(vx + 6, vy + 6, 16, T("Nothing in progress", "Nada en curso"), C_WHITE);
    if (st_total > vh) {
        int bx = rx + rw - 10, bh = vh * vh / st_total; if (bh < 16) bh = 16;
        pa_fill(bx - 1, vy - 1, 5, vh + 2, C_DARK);
        pa_fill(bx, vy, 3, vh, RGB(110, 84, 70));
        pa_fill(bx, vy + (int)((vh - bh) * (st_scroll / mx)), 3, bh, C_YELLOW);
    }
}
/* style 4: drag / fling the event list (canvas pixels; action 0 down, 1 up, 2 move); 1 = taken */
int st_touch_(int x, int y, int action) {
    if (st_style() != 4) return 0;
    if (action == 0) {
        if (x >= st_vx && x < st_vx + st_vw + 12 && y >= st_vy && y < st_vy + st_vh) { st_drag = 1; st_dy0 = y; st_s0 = st_scroll; st_dlast = y; st_vel = 0; return 1; }
        return 0;
    }
    if (!st_drag) return 0;
    if (action == 2) { st_scroll = st_s0 - (y - st_dy0); st_vel = (float)(st_dlast - y); st_dlast = y; }
    else st_drag = 0;
    return 1;
}
void st_action_(int a) {   /* style 4: up / down scroll the list a card at a time */
    if (st_style() != 4) return;
    if (a == IN_UP) st_scroll -= 64; else if (a == IN_DOWN) st_scroll += 64;
}

/* the Status tab in style 1..4; 0 = not handled (the current page) */
static int page_status_alt(int play, int link) {
    int s = st_style();
    if (!s || !play) return 0;
    if (s == 1) st_sheet(play, link);
    else if (s == 2) st_next(play, link);
    else if (s == 3) st_dash(play, link);
    else st_show(play, link);
    return 1;
}
