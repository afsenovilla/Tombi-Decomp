/* Second screen panel, item tab: refinements of style 3 (big cards a page at a time), included by panel_items_alt.inc.
 *   4 (3a)  category tabs (Todo / Armas / Ropa / Objetos / Llaves) + Ordenar; 4 x 2 items with the icon at 3x and the
 *           name under it in the 8 px game font, in one cream window with the L1 / R1 arrows on its sides and page dots;
 *           the selection in the game's blinking yellow frame; a detail window (icon, name, description, status, action).
 *   5 (3b)  style 3 cleaned up: a detail window on top (icon, name, category, description, status, action inside), the
 *           5 x 2 cards with the blinking frame (no hand), clear badges, one bottom row: Ordenar left, L1 < n de N > R1
 *           centred.
 *   6 (3c)  split: a showcase on the left (the icon at 4x in a stone frame, name, category, description, status, Ordenar
 *           and the action) and a 3 x 4 grid of icons on stone on the right, paged by a row at its bottom edge. */

/* a clear count badge: the HUD's digits on a red plate with a dark rim (right / bottom aligned) */
static void it_badge_n(int rx, int by, int n) {
    char d[8]; int nd = snprintf(d, sizeof d, "%d", n > 999 ? 999 : n), w = nd * 8 + 5, h = 13;
    pa_fill(rx - w + 1, by - h, w - 2, h, C_DARK); pa_fill(rx - w, by - h + 1, w, h - 2, C_DARK);
    pa_fill(rx - w + 1, by - h + 1, w - 2, h - 2, RGB(176, 40, 30)); pa_fill(rx - w + 2, by - h + 1, w - 4, 1, RGB(230, 110, 90));
    for (int i = 0; i < nd; i++) pa_spr(rx - w + 3 + 8 * i, by - h + 3, HUD, 8 * (d[i] - '0'), 64, 8, 8, 368, 481, 0);
}
/* a clear in-use badge: 15 x 15, green, a thick white tick */
static void it_badge_use(int x, int y) {
    pa_fill(x + 2, y, 11, 15, C_DARK); pa_fill(x, y + 2, 15, 11, C_DARK); pa_fill(x + 1, y + 1, 13, 13, C_DARK);
    pa_fill(x + 2, y + 1, 11, 13, RGB(56, 156, 56)); pa_fill(x + 1, y + 2, 13, 11, RGB(56, 156, 56)); pa_fill(x + 3, y + 1, 9, 2, RGB(120, 210, 110));
    line(x + 3, y + 7, x + 6, y + 10, 2, C_WHITE); line(x + 6, y + 10, x + 11, y + 4, 2, C_WHITE);
}
static void it_badges(int x, int y, int s, int item) {   /* on an icon drawn at scale s at (x, y) */
    if (it_in_use(item)) it_badge_use(x + 16 * s - 11, y - 4);
    if (s3(S3_NCLUT)[item] & 0x80) it_badge_n(x + 16 * s + 4, y + 16 * s + 3, G_QTY(item));
}
/* L1 < n de N > R1, centred on cx (the 16 px font has no slash) */
static void it_pager(int cx, int y, int pg, int pages) {
    char b[32]; snprintf(b, sizeof b, T("%d of %d", "%d de %d"), pg + 1, pages);
    int pw = pa_text_w(16, b) + 16, x = cx - (40 + 2 + pw + 2 + 40) / 2;
    plank(x, y, 40); hot(x, y, 40, 24, HS_ITOP + 5);
    pa_spr(x + 9, y + 7, SUB, 56, 206, 22, 9, 288, 500, PA_OUTLINE | (pg > 0 ? 0 : PA_SHADE));
    x += 42; plank(x, y, pw); plank_text(x + pw / 2, y + 4, b, 0); x += pw + 2;
    plank(x, y, 40); hot(x, y, 40, 24, HS_ITOP + 6);
    pa_spr(x + 8, y + 7, SUB, 80, 206, 24, 9, 288, 500, PA_OUTLINE | (pg < pages - 1 ? 0 : PA_SHADE));
}
static void it_dots(int cx, int y, int pg, int pages) {   /* page dots, each a touch target */
    if (pages < 2) return;
    int step = 12, x0 = cx - (pages - 1) * step / 2;
    for (int i = 0; i < pages; i++) {
        dot(x0 + i * step, y, 3, i == pg ? C_YELLOW : RGB(200, 170, 150));
        hot(x0 + i * step - 6, y - 6, 12, 12, HS_ITOP + 0xC0 + i);
    }
}
/* a tall plank (h >= 28): the game's 24-row plank cut in three, its top 14 and bottom 10 rows kept and its middle rows
   (6..17, plain grain) tiled between them; the end grain pieces likewise.  No texel is scaled. */
static void plank_h(int x, int y, int w, int h) {
    static const struct { int v, n; } seg[3] = { {0, 14}, {6, 12}, {14, 10} };
    int dy[3] = { y, y + 14, y + h - 10 }, dn[3] = { 14, h - 24, 10 };
    for (int i = 0; i < 3; i++) {
        pa_tile(x + 4, dy[i], w - 8, dn[i], SUB, 193, 128 + seg[i].v, 31, seg[i].n, 288, 481);
        pa_tile(x, dy[i], 4, dn[i], SUB, 184, 128 + seg[i].v, 4, seg[i].n, 288, 481);
        pa_tile(x + w - 9, dy[i], 9, dn[i], SUB, 224, 128 + seg[i].v, 9, seg[i].n, 288, 481);
    }
}
#define IT_BIG 48   /* the action plank's height: twice a plank */
static void it_btn_big(int x, int y, int w, const char *s, int st, int id) {
    plank_h(x, y, w, IT_BIG); hot(x, y, w, IT_BIG, id);
    pa_text_ol(x + w / 2 - pa_text_w(16, s) / 2, y + IT_BIG / 2 - 9, 16, s, st == 1 ? C_YELLOW : st == 2 ? RGB(176, 160, 144) : C_WHITE, C_WOODE, C_DARK);
}

/* the detail as a proper window: icon at 3x, name (and category), description, the status line and the action inside */
static void it_detail_win(int x, int y, int w, int h, int with_cat) {
    char b[200];
    window_(x, y, w, h);
    if (!it_n && it_warp < 0) return;
    int item = it_warp >= 0 ? it_warp : it_items[it_sel], bad; const char *st = it_status(item, &bad), *why = it_reason(item);
    pa_item_icon(x + 12, y + 12, item, PA_X3 | (why ? PA_SHADE : 0));
    if (it_warp < 0) it_badges(x + 12, y + 12, 3, item);
    int tx = x + 74, tw = w - 74 - 12, bw = 112, dw = tw - bw - 8;
    const char *cn = with_cat && it_warp < 0 ? cat_name(it_icat[it_sel]) : ""; int cnw = *cn ? pa_text_w(16, cn) : 0;
    pa_text_fit(tx, y + 8, 16, tw - cnw - 8, it_label(item, b, sizeof b), C_DARK, C_EDGE);
    if (*cn) pa_text(x + w - 12 - cnw, y + 8, 16, cn, C_MUTED, C_EDGE);
    pa_fill(tx, y + 26, tw, 1, RGB(230, 200, 180));
    /* the description left of the action plank, the status line under it */
    int lines = (h - 29 - 8 - (st ? 20 : 0)) / 15; if (lines < 1) lines = 1; if (lines > 4) lines = 4;
    if (it_desc(item, b, sizeof b)) text_wrap(tx, y + 29, 16, dw, 15, lines, 0, b, C_MUTED8, C_EDGE);
    if (st) pa_text_fit(x + 12, y + h - 26, 16, w - 24 - bw - 8, st, bad ? C_RED : C_GREEN, C_REDE);
    it_btn_big(x + w - 10 - bw, y + h - 10 - IT_BIG, bw, it_act_name(it_sel, item), why ? 2 : 1, HS_ITOP + 3);
    if (it_warp >= 0) it_btn(x + w - 10 - 2 * bw - 6, y + h - 34, bw, T("Cancel", "Cancelar"), 0, HS_ITOP + 4);
}
static void it_selframe(int x, int y, int w, int h) { it_frame(x - 1, y - 1, w + 2, h + 2, 1, C_DARK); it_frame(x, y, w, h, 2, it_blink()); }

/* ---------------------------------------------------------------- 4 (3a): tabs, 4 x 2 named items, detail window */
static void page_items4(void) {
    int W = pa_cw, H = pa_ch, top = 56; char b[96];
    static const char *en[5] = { "All", "Weapons", "Clothes", "Items", "Keys" }, *sp[5] = { "Todo", "Armas", "Ropa", "Objetos", "Llaves" };
    const char *sn = T("Sort", "Ordenar");
    int sw = pa_text_w(16, sn) + 18, tw[5], sum = 0, avail = W - 16 - sw - 6;
    for (int i = 0; i < 5; i++) { tw[i] = pa_text_w(16, T(en[i], sp[i])) + 14; sum += tw[i]; }
    int extra = (avail - sum - 4 * 2) / 5; if (extra < 0) extra = 0;
    for (int i = 0, x = 8; i < 5; i++) { int w = tw[i] + extra; it_btn(x, top, w, T(en[i], sp[i]), i == it_tab ? 1 : 0, HS_ITOP + 0xE0 + i); x += w + 2; }
    it_btn(W - 8 - sw, top, sw, sn, 0, HS_ITOP + 2);
    int dh = 98, gy = top + 28, gh = H - 8 - dh - 4 - gy, cols = 4, rows = 2;
    it_pp = cols * rows;
    int pages = (it_n + it_pp - 1) / it_pp, pg = it_n ? it_sel / it_pp : 0; if (pages < 1) pages = 1;
    it_cols = cols; it_vw = 0; it_vh = 0;
    for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * 100); }
    if (it_warp >= 0) it_page_warp(8, gy, W - 16, gh);
    else {
        window_(8, gy, W - 16, gh);
        int gx = 14, gw = W - 28, cw = gw / cols, ch = (gh - 26) / rows, y0 = gy + 5;
        /* the L1 / R1 arrows either side of the page dots, on the window's bottom row */
        int ay = gy + gh - 18, dw2 = (pages > 1 ? pages - 1 : 0) * 12;
        pa_spr(W / 2 - dw2 / 2 - 36, ay, SUB, 56, 206, 22, 9, 288, 500, PA_OUTLINE | (pg > 0 ? 0 : PA_SHADE)); hot(W / 2 - dw2 / 2 - 44, ay - 8, 34, 24, HS_ITOP + 5);
        pa_spr(W / 2 + dw2 / 2 + 12, ay, SUB, 80, 206, 24, 9, 288, 500, PA_OUTLINE | (pg < pages - 1 ? 0 : PA_SHADE)); hot(W / 2 + dw2 / 2 + 8, ay - 8, 34, 24, HS_ITOP + 6);
        for (int s = 0; s < it_pp; s++) {
            int k = pg * it_pp + s, x = gx + s % cols * cw, y = y0 + s / cols * ch;
            if (k >= it_n) break;
            int item = it_items[k], ok = !it_reason(item), ix = x + (cw - 48) / 2;
            if (k == it_sel) { pa_fill(x + 3, y + 1, cw - 6, ch - 2, RGB(255, 236, 170)); it_selframe(x + 2, y, cw - 4, ch); }
            pa_item_icon(ix, y + 3, item, PA_X3 | (ok ? 0 : PA_SHADE));
            it_badges(ix, y + 3, 3, item);
            char t[96]; text_wrap(x + 3, y + 52, 8, cw - 6, 9, 2, 1, it_name(item, t, sizeof t), ok ? C_MUTED8 : C_MUTED, k == it_sel ? RGB(255, 236, 170) : RGB(255, 238, 222));
            hot(x, y, cw, ch, HS_ITEM + (k & 0xFF));
        }
        if (!it_n) pa_text(gx + 8, y0 + 8, 16, T("No items", "Sin objetos"), C_MUTED, C_EDGE);
        it_dots(W / 2, ay + 4, pg, pages);
    }
    (void)b;
    it_detail_win(8, H - 8 - dh, W - 16, dh, 1);
}

/* ---------------------------------------------------------------- 5 (3b): style 3 cleaned up */
static void page_items5(void) {
    int W = pa_cw, H = pa_ch, top = 56;
    int dh = 104, by = H - 8 - 24, gy = top + dh + 4, gh = by - 4 - gy, cols = 5, rows = 2;
    int cw = (W - 16) / cols, ch = gh / rows, gx = 8 + (W - 16 - cols * cw) / 2;
    it_pp = cols * rows;
    int pages = (it_n + it_pp - 1) / it_pp, pg = it_n ? it_sel / it_pp : 0; if (pages < 1) pages = 1;
    it_cols = cols; it_vw = 0; it_vh = 0;
    for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * ch); }
    it_detail_win(8, top, W - 16, dh, 1);
    if (it_warp >= 0) it_page_warp(8, gy, W - 16, gh);
    else {
        for (int s = 0; s < it_pp; s++) {
            int k = pg * it_pp + s, x = gx + s % cols * cw, y = gy + s / cols * ch;
            if (k >= it_n) { frame_(x + 2, y + 2, cw - 4, ch - 4); continue; }
            int item = it_items[k], ok = !it_reason(item), ix = x + (cw - 48) / 2, iy = y + (ch - 48) / 2;
            window_(x + 2, y + 2, cw - 4, ch - 4);
            if (k == it_sel) { pa_fill(x + 7, y + 7, cw - 14, ch - 14, RGB(255, 236, 160)); it_selframe(x + 1, y + 1, cw - 2, ch - 2); }
            pa_item_icon(ix, iy, item, PA_X3 | (ok ? 0 : PA_SHADE));
            it_badges(ix, iy, 3, item);
            hot(x, y, cw, ch, HS_ITEM + (k & 0xFF));
        }
        if (!it_n) pa_text(gx + 10, gy + 10, 16, T("No items", "Sin objetos"), C_MUTED, C_EDGE);
    }
    const char *sn = T("Sort", "Ordenar"); int sw = pa_text_w(16, sn) + 16;
    it_btn(8, by, sw, sn, 0, HS_ITOP + 2);
    it_pager(W / 2, by, pg, pages);
}

/* ---------------------------------------------------------------- 6 (3c): showcase left, grid right */
static void page_items6(void) {
    int W = pa_cw, H = pa_ch, top = 56; char b[200];
    int lw = 168, lx = 8, h = H - 8 - top, rx = lx + lw + 4, rw = W - 8 - rx;
    /* the showcase */
    window_(lx, top, lw, h);
    int cx = lx + lw / 2;
    frame_(cx - 41, top + 6, 82, 82);
    stone(cx - 36, top + 11, 72, 72);
    int bw = lw - 20, ay = top + h - 10 - IT_BIG, oy = ay - 28;
    if (it_warp >= 0) it_btn(lx + 10, oy, bw, T("Cancel", "Cancelar"), 0, HS_ITOP + 4);
    else it_btn(lx + 10, oy, bw, T("Sort", "Ordenar"), 0, HS_ITOP + 2);
    if (it_n || it_warp >= 0) {
        int item = it_warp >= 0 ? it_warp : it_items[it_sel], bad; const char *st = it_status(item, &bad), *why = it_reason(item);
        pa_item_icon(cx - 32, top + 15, item, PA_X4 | PA_OUTLINE | (why ? PA_SHADE : 0));
        if (it_warp < 0) { if (it_in_use(item)) it_badge_use(cx + 26, top + 8); if (s3(S3_NCLUT)[item] & 0x80) it_badge_n(cx + 40, top + 86, G_QTY(item)); }
        int y = top + 92;
        char t[96]; const char *nm = it_name(item, t, sizeof t);
        y += 17 * text_wrap(lx + 8, y, 16, lw - 16, 17, 2, 1, nm, C_DARK, C_EDGE);
        if (it_warp < 0) { const char *cn = cat_name(it_icat[it_sel]); ctext(cx, y, 16, cn, C_MUTED, C_EDGE); y += 18; }
        pa_fill(lx + 12, y + 1, lw - 24, 1, RGB(230, 200, 180)); y += 4;
        int stl = st ? 2 : 0, lim = oy - 4 - stl * 15;
        if (it_desc(item, b, sizeof b)) { int ml = (lim - y) / 15; if (ml > 0) text_wrap(lx + 10, y, 16, lw - 20, 15, ml, 0, b, C_MUTED8, C_EDGE); }
        if (st) text_wrap(lx + 10, lim, 16, lw - 20, 15, 2, 0, st, bad ? C_RED : C_GREEN, C_REDE);
        it_btn_big(lx + 10, ay, bw, it_act_name(it_sel, item), why ? 2 : 1, HS_ITOP + 3);
    }
    /* the grid: icons on stone, a pager on its bottom edge */
    int cols = 3, gy = top + 8, ph = 30, gh = h - 16 - ph, cw = (rw - 16) / cols, rows = gh / 62, ch = rows ? gh / rows : 62, gx = rx + 8 + (rw - 16 - cols * cw) / 2;
    it_pp = cols * rows;
    int pages = (it_n + it_pp - 1) / it_pp, pg = it_n ? it_sel / it_pp : 0; if (pages < 1) pages = 1;
    it_cols = cols; it_vw = 0; it_vh = 0;
    for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * ch); }
    if (it_warp >= 0) { it_page_warp(rx, top, rw, h); return; }
    stone(rx, top, rw, h);
    frame_(rx, top, rw, h);
    int y0 = gy + (gh - rows * ch) / 2;
    for (int s = 0; s < it_pp; s++) {
        int k = pg * it_pp + s, x = gx + s % cols * cw, y = y0 + s / cols * ch;
        if (k >= it_n) break;
        int item = it_items[k], ok = !it_reason(item), sel = k == it_sel, ix = x + (cw - 48) / 2, iy = y + (ch - 48) / 2 - (sel ? 2 : 0);
        if (sel) { window_(ix - 7, iy - 7, 62, 62); it_selframe(ix - 7, iy - 7, 62, 62); }
        pa_item_icon(ix, iy, item, PA_X3 | PA_OUTLINE | (ok ? 0 : PA_SHADE));
        it_badges(ix, iy, 3, item);
        hot(x, y, cw, ch, HS_ITEM + (k & 0xFF));
    }
    if (!it_n) pa_text_ol(rx + 12, gy + 8, 16, T("No items", "Sin objetos"), C_WHITE, C_WOODE, C_DARK);
    it_pager(rx + rw / 2, top + h - 8 - 24, pg, pages);
}

static void page_items3v(int s) { if (s == 4) page_items4(); else if (s == 5) page_items5(); else page_items6(); }
