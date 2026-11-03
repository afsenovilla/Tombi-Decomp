/* Second screen panel, item tab: alternative layouts (included by panel_items.inc), picked with the environment variable
 * PORT_PANEL_ITEMS_STYLE (0 / unset: the touch grid of page_items0; 1, 2, 3 below).  Same data, checks and requests as
 * style 0; only the drawing and the layout differ.  Every style shows the selected item's name once, its description in
 * the 16 px font and one status line (the result of the last action, else why it cannot be used now) next to a single
 * action plank.
 *   1  game-like grid: category tabs as small planks (+ Ordenar), the icons at 2x in a dense grid, the selected one in a
 *      blinking yellow frame, badges for in use / count, a detail card under it.
 *   2  the Triangle screen's list: a dark list (icon, name, count, in-use mark; category headers inline) with the game's
 *      green selection bar, the detail and the buttons in a side panel.
 *   3  big cards: the detail on top, 5 x 2 cards with the icons at 3x a page at a time (the game's 10 per page), the
 *      sub-screen's pointing hand on the selected one, a button bar at the bottom (pages, Ordenar, the action). */

#define IT3_PAGE 10
static void it_filter(void) {
    if (it_get_style() != 1 || !it_tab) return;
    int n = 0;
    for (int k = 0; k < it_n; k++) if (it_tab_has(it_tab, it_icat[k])) { it_items[n] = it_items[k]; it_icat[n] = it_icat[k]; n++; }
    it_n = n;
}
/* the one status line: the last action's result for a while, else why the item cannot be used now (NULL: none) */
static const char *it_status(int item, int *bad) {
    if (it_msg_t && port_frame - it_msg_t < 200) { *bad = it_msg_bad; return it_msg; }
    *bad = 1;
    return it_reason(item);
}
static const char *it_act_name(int k, int item) {
    return it_warp >= 0 ? T("Fly", "Volar") : it_icat[k] == 1 ? T("Equip", "Equipar") : it_icat[k] == 2 ? (it_in_use(item) ? T("Take off", "Quitar") : T("Wear", "Poner")) : T("Use", "Usar");
}
static int it_desc(int item, char *d, int n) {   /* the sub-screen's description, its line breaks as spaces */
    if (!it_bank || pa_wfm_dialogue(it_bank, it_bank_n, rd16(it_bank, 0x12) + item, d, n) <= 0) { d[0] = 0; return 0; }
    for (char *p = d; *p; p++) if (*p == '\n') *p = ' ';
    return 1;
}
static const char *it_label(int item, char *b, int n) {   /* the name, "x NN" for counted items */
    char t[96]; const char *nm = it_name(item, t, sizeof t);
    if (s3(S3_NCLUT)[item] & 0x80) snprintf(b, n, "%s x%d", nm, G_QTY(item)); else snprintf(b, n, "%s", nm);
    return b;
}
/* a plank button: st 0 normal (cream text), 1 ready (yellow), 2 not available (grey) */
static void it_btn(int x, int y, int w, const char *s, int st, int id) {
    plank(x, y, w); hot(x, y, w, 24, id);
    pa_text_ol(x + w / 2 - pa_text_w(16, s) / 2, y + 4, 16, s, st == 1 ? C_YELLOW : st == 2 ? RGB(176, 160, 144) : C_WHITE, C_WOODE, C_DARK);
}
/* the count on a dark plate with the HUD's own digits, right / bottom aligned */
static void it_count(int rx, int by, int n) {
    char d[8]; int nd = snprintf(d, sizeof d, "%d", n > 999 ? 999 : n), w = nd * 8 + 3;
    pa_fill(rx - w, by - 11, w, 11, C_DARK); pa_fill(rx - w + 1, by - 10, w - 2, 9, RGB(70, 44, 30));
    for (int i = 0; i < nd; i++) pa_spr(rx - w + 2 + 8 * i, by - 10, HUD, 8 * (d[i] - '0'), 64, 8, 8, 368, 481, 0);
}
/* in use: a green badge with a tick */
static void it_mark(int x, int y) {
    pa_fill(x + 1, y, 9, 11, C_DARK); pa_fill(x, y + 1, 11, 9, C_DARK);
    pa_fill(x + 1, y + 1, 9, 9, RGB(56, 156, 56)); pa_fill(x + 2, y + 1, 7, 1, RGB(110, 200, 100));
    line(x + 2, y + 5, x + 4, y + 7, 1, C_WHITE); line(x + 4, y + 7, x + 8, y + 3, 1, C_WHITE);
}
/* the sub-screen's pointing hand (SYSTEM/A10301 u 124 v 70, the white glove CLUT) */
static void it_hand(int x, int y) { pa_spr(x, y, SUB, 124, 70, 23, 15, 288, 483, 0); }
static void it_frame(int x, int y, int w, int h, int t, u32 c) { pa_fill(x, y, w, t, c); pa_fill(x, y + h - t, w, t, c); pa_fill(x, y, t, h, c); pa_fill(x + w - t, y, t, h, c); }
static u32 it_blink(void) { return (port_frame >> 4) & 1 ? C_YELLOW : RGB(255, 168, 48); }

/* scrolling shared by styles 1 and 2: follow the cursor (hdr: the header above the first of a category), fling, clamp */
static int it_scroll_step(int total, int vh, int ih, int hdr) {
    if (it_follow && it_n) {
        int t = it_ly[it_sel] - (hdr && (it_sel == 0 || it_icat[it_sel] != it_icat[it_sel - 1]) ? hdr : 0);
        if (t < it_scroll) it_scroll = (float)t;
        if (it_ly[it_sel] + ih > it_scroll + vh) it_scroll = (float)(it_ly[it_sel] + ih - vh);
        it_follow = 0;
    }
    if (!it_drag) { it_scroll += it_vel; it_vel *= .9f; if (it_vel > -.3f && it_vel < .3f) it_vel = 0; }
    float mx = total > vh ? (float)(total - vh) : 0;
    if (it_scroll > mx) { it_scroll = mx; it_vel = 0; } if (it_scroll < 0) { it_scroll = 0; it_vel = 0; }
    return (int)it_scroll;
}
static void it_scrollbar(int bx, int vy, int vh, int total, u32 track, u32 thumb) {
    if (total <= vh) return;
    int bh = vh * vh / total; if (bh < 16) bh = 16;
    pa_fill(bx, vy, 4, vh, track);
    pa_fill(bx, vy + (int)((vh - bh) * (it_scroll / (float)(total - vh))), 4, bh, thumb);
    hot(bx - 12, vy, 20, vh / 2, HS_ITOP); hot(bx - 12, vy + vh / 2, 20, vh - vh / 2, HS_ITOP + 1);
}
static void it_clip(int x, int y, int w, int h, int *save) {
    save[0] = pa_clip_x0; save[1] = pa_clip_y0; save[2] = pa_clip_x1; save[3] = pa_clip_y1;
    pa_clip_x0 = x; pa_clip_y0 = y; pa_clip_x1 = x + w; pa_clip_y1 = y + h;
}
static void it_unclip(const int *s) { pa_clip_x0 = s[0]; pa_clip_y0 = s[1]; pa_clip_x1 = s[2]; pa_clip_y1 = s[3]; }

/* ---------------------------------------------------------------- style 1: game-like grid */
static void page_items1(void) {
    int W = pa_cw, H = pa_ch, top = 56, sv[4]; char b[200];
    /* the tab bar: the categories, then Ordenar at the right */
    static const char *en[5] = { "All", "Weapons", "Clothes", "Items", "Misc" }, *sp[5] = { "Todo", "Armas", "Ropa", "Objetos", "Varios" };
    const char *sn = T("Sort", "Ordenar");
    int sw = pa_text_w(16, sn) + 18, tw[5], sum = 0, avail = W - 16 - sw - 6;
    for (int i = 0; i < 5; i++) { tw[i] = pa_text_w(16, T(en[i], sp[i])) + 14; sum += tw[i]; }
    int extra = (avail - sum - 4 * 2) / 5; if (extra < 0) extra = 0;
    for (int i = 0, x = 8; i < 5; i++) { int w = tw[i] + extra; it_btn(x, top, w, T(en[i], sp[i]), i == it_tab ? 1 : 0, HS_ITOP + 0xE0 + i); x += w + 2; }
    it_btn(W - 8 - sw, top, sw, sn, 0, HS_ITOP + 2);
    int dh = 78, gy = top + 28, gh = H - 8 - dh - 3 - gy;
    if (it_warp >= 0) it_page_warp(8, gy, W - 16, gh);
    else {
        window_(8, gy, W - 16, gh);
        int cell = 38, vx = 16, vw = W - 32 - 8, vy = gy + 7, vh = gh - 14, cols = vw / cell;
        int ox = vx + (vw - cols * cell) / 2;
        for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * cell); }
        int total = (it_n + cols - 1) / cols * cell;
        it_cols = cols; it_vx = vx; it_vy = vy; it_vw = vw; it_vh = vh;
        int so = it_scroll_step(total, vh, cell, 0);
        it_clip(vx - 4, vy - 3, vw + 8, vh + 6, sv);
        if (!it_n) pa_text(vx + 4, vy + 4, 16, T("No items", "Sin objetos"), C_MUTED, C_EDGE);
        for (int k = 0; k < it_n; k++) {
            int x = ox + it_lc[k] * cell + 3, y = vy + it_ly[k] - so + 3;
            if (y + cell <= vy || y - 6 >= vy + vh) continue;
            int item = it_items[k], ok = !it_reason(item);
            pa_item_icon(x, y, item, PA_X2 | (ok ? 0 : PA_SHADE));
            if (it_in_use(item)) it_mark(x + 23, y - 2);
            if (s3(S3_NCLUT)[item] & 0x80) it_count(x + 34, y + 34, G_QTY(item));
            if (k == it_sel) { it_frame(x - 3, y - 3, 38, 38, 2, it_blink()); it_frame(x - 4, y - 4, 40, 40, 1, C_DARK); }
            int hy0 = y - 3 < vy ? vy : y - 3, hy1 = y + 35 > vy + vh ? vy + vh : y + 35;
            if (hy1 - hy0 > 8) hot(x - 3, hy0, cell, hy1 - hy0, HS_ITEM + (k & 0xFF));
        }
        it_unclip(sv);
        it_scrollbar(W - 8 - 12, vy, vh, total, RGB(230, 200, 190), C_RED);
    }
    /* the detail card */
    int dy = H - 8 - dh;
    window_(8, dy, W - 16, dh);
    if (!it_n && it_warp < 0) return;
    int item = it_warp >= 0 ? it_warp : it_items[it_sel], bad; const char *st = it_status(item, &bad);
    int bw = 100, tx = 56, tw2 = W - 16 - tx - bw - 6;
    pa_item_icon(18, dy + 10, item, PA_X2 | (it_reason(item) ? PA_SHADE : 0));
    pa_text_fit(tx, dy + 6, 16, tw2, it_label(item, b, sizeof b), C_DARK, C_EDGE);
    if (it_desc(item, b, sizeof b)) text_wrap(tx, dy + 24, 16, tw2, 16, st ? 2 : 3, 0, b, C_MUTED8, C_EDGE);
    if (st) pa_text_fit(18, dy + dh - 22, 16, W - 16 - 18 - bw - 6, st, bad ? C_RED : C_GREEN, C_REDE);
    it_btn(W - 16 - bw, dy + (dh - 24) / 2, bw, it_act_name(it_sel, item), it_reason(item) ? 2 : 1, HS_ITOP + 3);
    if (it_warp >= 0) it_btn(W - 16 - bw, dy + dh - 30, bw, T("Cancel", "Cancelar"), 0, HS_ITOP + 4);
}

/* ---------------------------------------------------------------- style 2: the Triangle screen's list */
static void darkwin(int x, int y, int w, int h) { pa_nine(x, y, w, h, SUB, 0, 216, 248, 32, 8, 288, 480, 0); }
static void page_items2(void) {
    int W = pa_cw, H = pa_ch, top = 56, sv[4]; char b[200];
    int lw = (W - 16) * 3 / 5, lx = 8, lh = H - 8 - top, RH = 18;
    u32 bg = pa_clut_color(288, 480, 1), dim = RGB(176, 150, 136);
    if (it_warp >= 0) it_page_warp(lx, top, lw, lh);
    else {
        darkwin(lx, top, lw, lh);
        int vx = lx + 6, vw = lw - 12 - 6, vy = top + 6, vh = lh - 12, yy = 0;
        for (int k = 0; k < it_n; k++) {
            if (!k || it_icat[k] != it_icat[k - 1]) yy += RH + (k ? 2 : 0);
            it_lc[k] = 0; it_ly[k] = (short)yy; yy += RH;
        }
        it_cols = 1; it_vx = vx; it_vy = vy; it_vw = vw; it_vh = vh;
        int so = it_scroll_step(yy, vh, RH, RH);
        it_clip(vx, vy, vw, vh, sv);
        if (!it_n) pa_text(vx + 4, vy + 2, 16, T("No items", "Sin objetos"), dim, bg);
        for (int k = 0; k < it_n; k++) {
            int y = vy + it_ly[k] - so;
            if (!k || it_icat[k] != it_icat[k - 1]) {
                const char *cn = cat_name(it_icat[k]); int hy = y - RH;
                if (hy + RH > vy && hy < vy + vh) {
                    pa_text_ol(vx + 2, hy + 1, 16, cn, C_YELLOW, RGB(150, 90, 30), C_DARK);
                    int ex = vx + 8 + pa_text_w(16, cn); pa_fill(ex, hy + 9, vx + vw - 4 - ex, 1, RGB(150, 100, 70));
                }
            }
            if (y + RH <= vy || y >= vy + vh) continue;
            int item = it_items[k], ok = !it_reason(item);
            if (k == it_sel) for (int j = 0; j < RH; j++) for (int i = 0; i < vw; i++) pa_blend(vx + i, y + j, ok ? RGB(40, 230, 40) : RGB(20, 110, 20), ok ? 120 : 150);
            u32 rowbg = k == it_sel ? (ok ? RGB(56, 140, 40) : RGB(46, 70, 30)) : bg;
            pa_item_icon(vx + 3, y + 1, item, ok ? 0 : PA_SHADE);
            int rx = vx + vw - 4;
            if (s3(S3_NCLUT)[item] & 0x80) { snprintf(b, sizeof b, "x%d", G_QTY(item)); rx -= pa_text_w(16, b); pa_text(rx, y + 1, 16, b, C_YELLOW, rowbg); rx -= 4; }
            if (it_in_use(item)) { rx -= 11; it_mark(rx, y + 4); rx -= 4; }
            char t[96];
            pa_text_fit(vx + 24, y + 1, 16, rx - vx - 26, it_name(item, t, sizeof t), ok ? C_WHITE : dim, rowbg);
            int hy0 = y < vy ? vy : y, hy1 = y + RH > vy + vh ? vy + vh : y + RH;
            if (hy1 - hy0 > 6) hot(vx, hy0, vw, hy1 - hy0, HS_ITEM + (k & 0xFF));
        }
        it_unclip(sv);
        it_scrollbar(lx + lw - 12, vy, vh, yy, RGB(90, 60, 50), RGB(220, 180, 120));
    }
    /* the side panel: the selected item, its description, the status line, the buttons */
    int px = lx + lw + 4, pw2 = W - 8 - px;
    window_(px, top, pw2, lh);
    int bw = pw2 - 16, by = top + lh - 8 - 24;
    if (it_warp >= 0) it_btn(px + 8, by - 28, bw, T("Cancel", "Cancelar"), 0, HS_ITOP + 4);
    else it_btn(px + 8, by - 28, bw, T("Sort", "Ordenar"), 0, HS_ITOP + 2);
    if (!it_n && it_warp < 0) return;
    int item = it_warp >= 0 ? it_warp : it_items[it_sel], bad; const char *st = it_status(item, &bad);
    it_btn(px + 8, by, bw, it_act_name(it_sel, item), it_reason(item) ? 2 : 1, HS_ITOP + 3);
    int cx = px + pw2 / 2, y = top + 10;
    pa_item_icon(cx - 24, y, item, PA_X3 | (it_reason(item) ? PA_SHADE : 0));
    if (it_warp < 0 && it_in_use(item)) it_mark(cx + 18, y - 3);
    y += 54;
    y += 18 * text_wrap(px + 8, y, 16, pw2 - 16, 17, 2, 1, it_label(item, b, sizeof b), C_DARK, C_EDGE) + 4;
    int stl = st ? 2 : 0, lim = by - 28 - 4;
    if (it_desc(item, b, sizeof b)) {
        int maxl = (lim - y - stl * 16 - (st ? 4 : 0)) / 16; if (maxl < 1) maxl = 1;
        y += 16 * text_wrap(px + 8, y, 16, pw2 - 16, 16, maxl, 0, b, C_MUTED8, C_EDGE) + 4;
    }
    if (st) text_wrap(px + 8, lim - 32, 16, pw2 - 16, 16, 2, 0, st, bad ? C_RED : C_GREEN, C_REDE);
}

/* ---------------------------------------------------------------- style 3: big cards, a page at a time */
static void page_items3(void) {
    int W = pa_cw, H = pa_ch, top = 56; char b[200];
    int dh = 76, by = H - 8 - 24, gy = top + dh + 4, gh = by - 4 - gy, cols = 5, rows = 2;
    int cw = (W - 16) / cols, ch = gh / rows, gx = 8 + (W - 16 - cols * cw) / 2;
    int pages = (it_n + IT3_PAGE - 1) / IT3_PAGE, pg = it_n ? it_sel / IT3_PAGE : 0;
    if (pages < 1) pages = 1;
    it_cols = cols; it_vw = 0; it_vh = 0;   /* no drag scrolling: pages */
    for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * ch); }
    /* the detail on top */
    window_(8, top, W - 16, dh);
    if (it_n || it_warp >= 0) {
        int item = it_warp >= 0 ? it_warp : it_items[it_sel], bad; const char *st = it_status(item, &bad);
        const char *cn = it_warp >= 0 ? "" : cat_name(it_icat[it_sel]); int cnw = pa_text_w(16, cn);
        pa_text_fit(18, top + 7, 16, W - 52 - cnw, it_label(item, b, sizeof b), C_DARK, C_EDGE);
        pa_text(W - 18 - cnw, top + 7, 16, cn, C_MUTED, C_EDGE);
        pa_fill(18, top + 24, W - 36, 1, RGB(230, 200, 180));
        if (it_desc(item, b, sizeof b)) text_wrap(18, top + 27, 16, W - 36, 15, st ? 2 : 3, 0, b, C_MUTED8, C_EDGE);
        if (st) pa_text_fit(18, top + dh - 21, 16, W - 36, st, bad ? C_RED : C_GREEN, C_REDE);
    }
    /* the cards */
    if (it_warp >= 0) it_page_warp(8, gy, W - 16, gh);
    else {
        int hx = -1, hy = 0;
        for (int s = 0; s < IT3_PAGE; s++) {
            int k = pg * IT3_PAGE + s, x = gx + s % cols * cw, y = gy + s / cols * ch;
            if (k >= it_n) { frame_(x + 2, y + 2, cw - 4, ch - 4); continue; }
            int item = it_items[k], ok = !it_reason(item), sel = k == it_sel;
            window_(x + 2, y + 2, cw - 4, ch - 4);
            if (sel) { pa_fill(x + 7, y + 7, cw - 14, ch - 14, RGB(255, 236, 160)); it_frame(x + 1, y + 1, cw - 2, ch - 2, 2, it_blink()); hx = x - 9; hy = y + ch / 2 - 8; }
            pa_item_icon(x + (cw - 48) / 2, y + (ch - 48) / 2, item, PA_X3 | (ok ? 0 : PA_SHADE));
            if (it_in_use(item)) it_mark(x + cw - 18, y + 6);
            if (s3(S3_NCLUT)[item] & 0x80) it_count(x + cw - 7, y + ch - 6, G_QTY(item));
            hot(x, y, cw, ch, HS_ITEM + (k & 0xFF));
        }
        if (hx >= 0) it_hand(hx < 0 ? 0 : hx, hy);
        if (!it_n) pa_text(gx + 10, gy + 10, 16, T("No items", "Sin objetos"), C_MUTED, C_EDGE);
    }
    /* the button bar: pages, Ordenar, the action */
    int x = 8;
    plank(x, by, 40); hot(x, by, 40, 24, HS_ITOP + 5);
    pa_spr(x + 9, by + 7, SUB, 56, 206, 22, 9, 288, 500, PA_OUTLINE | (pg > 0 ? 0 : PA_SHADE));
    x += 42;
    snprintf(b, sizeof b, T("%d of %d", "%d de %d"), pg + 1, pages);   /* the 16 px font has no slash */
    { int pw = pa_text_w(16, b) + 16; plank(x, by, pw); plank_text(x + pw / 2, by + 4, b, 0); x += pw + 2; }
    plank(x, by, 40); hot(x, by, 40, 24, HS_ITOP + 6);
    pa_spr(x + 8, by + 7, SUB, 80, 206, 24, 9, 288, 500, PA_OUTLINE | (pg < pages - 1 ? 0 : PA_SHADE));
    x += 48;
    const char *sn = it_warp >= 0 ? T("Cancel", "Cancelar") : T("Sort", "Ordenar"); int sw = pa_text_w(16, sn) + 20;
    it_btn(x, by, sw, sn, 0, it_warp >= 0 ? HS_ITOP + 4 : HS_ITOP + 2); x += sw + 6;
    if (it_n || it_warp >= 0) {
        int item = it_warp >= 0 ? it_warp : it_items[it_sel];
        it_btn(x, by, W - 8 - x, it_act_name(it_sel, item), it_reason(item) ? 2 : 1, HS_ITOP + 3);
    }
}

static void page_items(int play) {
    int s = it_get_style();
    if (!s) { page_items0(play); return; }
    if (!it_state) it_load();
    if (!play || it_state != 1) { page_items0(play); return; }
    it_build();
    if (it_sel >= it_n) it_sel = it_n - 1; if (it_sel < 0) it_sel = 0;
    if (s == 1) page_items1(); else if (s == 2) page_items2(); else page_items3();
}
