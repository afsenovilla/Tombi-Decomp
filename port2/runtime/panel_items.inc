/* Second screen panel: the item tab "Objetos" (included by panel.c).  The game's Triangle item sub-screen (D0SPSUB3:
 * subScreenUpdate -> subScreenItemListStep) on the second screen, usable while the game runs.
 *
 * What the game does (src/updatePauseMenu.c, src/inventoryScreenHandler.c, src/d0spsub3/subScreenItemListStep.c):
 *   - Triangle in gameplay (updatePauseMenu, called at the top of the gameplay tick of task modes 1 / 2 / 4 / 5) opens
 *     menu screen 7 only when: no pause (1F8001C6 == 0, 1F8003CE | 1F8001CC == 0), fade 0 / 0x10, GAME.disableSelectMenu 0,
 *     the player landed, GAME.playerMode 1, D_8009BCA7 0, the player idle (state 1, substate < 0x20, unless D_800A5462
 *     is 1) and 1F8001B8 0.  Gameplay then stops: the task runs the menu (mode 3) instead of the gameplay tick, the area
 *     overlay is swapped for D0SPSUB3 and reloaded on the way out; nothing of the room moves while the menu is open.
 *   - Confirm on an item: its condition script (CONDS[COND_OF[item]], ops subScreenCond*) must pass, else the message
 *     box 0..3 of the sub-screen's bank (keys: "No need using this", weapons / pants: "already equipped", the Bunk
 *     flower: "Not enough" while D_8009D0E6 < 5, the rest: "Can't use this here").  Items 7 and 14 (Baron, Wings) ask
 *     for a destination among the revealed regions (D_8009D2C4) and start the flight; every other item goes through
 *     the use handler func_800E8950 (equip a weapon, change pants, toggle the umbrella / fish / jewels, eat, ring a bell,
 *     set GVARS.useItemPending for the room's objects: the chicks, the tornado...) and its result picks the item popup.
 *     The menu then closes.
 * Here: the same checks are evaluated live to grey an item with the reason, and a confirmed item is queued; the game
 * loop runs it between two frames (port/fixes.txt gameMainTask -> port_panel_tick, where the debug menu's actions run),
 * re-checking everything first, through the game's own native functions (D0SPSUB3_func_800E8950, the D0SPSUB3_*Cond
 * ops, printInfoMessage, allocObjectLayer2): exactly the menu's confirm path, minus opening and closing the menu.
 * The tables (sort order, categories, condition scripts, flight destinations, item flags) are read from the user's
 * SYSTEM/D0SPSUB3.BIN, the messages from DnSPSUB3's own WFM3 bank, the icons / names as the other tabs do.
 * Grouping: the categories of the sub-screen's sort (D_800F67C8 pairs, order D_800F68E8[GAME.inventory.sortMode]),
 * i.e. the list the menu shows once sorted; "Sort" runs the menu's sort entry (cursor 2). */

#include <stdint.h>

/* ---- D0SPSUB3 addresses (the PAL overlay this decomp matches; checked against the file before use) */
#define S3_ORDER   0x800F68E8u   /* u8[4][5] category order per sort mode */
#define S3_PAIRS   0x800F67C8u   /* {category, item} x 0x8F */
#define S3_CONDIDX 0x800F584Cu   /* u8[item] condition script of the item */
#define S3_CONDS   0x800F665Cu   /* Cond *[] */
#define S3_FNS     0x800F6714u   /* the 18 condition ops */
#define S3_DEST    0x800F67A0u   /* u8 *[9]: {area, section} per region bit */
#define S3_MSG0    0x801008ECu   /* items answering message 0 (-1 ended s16) */
#define S3_MSG1    0x801008FCu   /* message 1 */
#define S3_MSG3    0x80100914u   /* message 3 (2 once D_8009D0E6 >= 5) */
#define S3_NCLUT   0x8010065Cu   /* u8[item] name clut, 0x80 = counted ("x NN") */
#define S3_END     0x80100B88u

#define G_INV_ITEM(i) G8(0x8009D1A4u + (i))
#define G_INV_COUNT   G16(0x8009D2A4u)
#define G_INV_MODE    G16(0x8009D2A6u)
#define G_QTY(i)      G8(0x8009D0A4u + (i))

typedef struct { s16 next; u8 fn, a; s16 b, c; } ItCond;
static u8 *it_ovl, *it_bank; static unsigned it_ovl_n, it_bank_n;
static int it_state;   /* 0 not loaded, 1 ok, -1 file missing / another overlay */
static u8 it_cat[256]; /* category of an item (0xFF none) */
static char it_reason_txt[4][96];

static const u8 *s3(u32 a) { return it_ovl + (a - OVL_BASE); }
static void it_load(void) {
    it_state = -1;
    it_ovl = port_cd_read_named("SYSTEM/D0SPSUB3.BIN", &it_ovl_n);
    if (!it_ovl || it_ovl_n < S3_END - OVL_BASE) { port_printf("[panel] items: SYSTEM/D0SPSUB3.BIN not readable\n"); return; }
    static const u32 fns[18] = { 0x800EE12C, 0x800EE134, 0x800EE13C, 0x800EE244, 0x800EE348, 0x800EE450, 0x800EE554, 0x800EE658, 0x800EE670,
                                 0x800EE688, 0x800EE750, 0x800EE818, 0x800EE838, 0x800EE94C, 0x800EE95C, 0x800EE9C8, 0x800EE9E0, 0x800EE9F0 };
    for (int i = 0; i < 18; i++) if (rd32(s3(S3_FNS), 4 * i) != fns[i]) { port_printf("[panel] items: D0SPSUB3 is not the overlay this build knows (op %d)\n", i); return; }
    memset(it_cat, 0xFF, sizeof it_cat);
    for (int i = 0; i < 0x11E; i += 2) if (it_cat[s3(S3_PAIRS)[i + 1]] == 0xFF) it_cat[s3(S3_PAIRS)[i + 1]] = s3(S3_PAIRS)[i];
    /* the messages: the language's sub-screen overlay carries its own bank */
    char name[32]; unsigned n = 0; u8 *f = NULL;
    if (port_lang > 0 && port_lang < 9) { snprintf(name, sizeof name, "SYSTEM/D%dSPSUB3.BIN", port_lang); f = port_cd_read_named(name, &n); }
    if (!f) { f = it_ovl; n = it_ovl_n; }
    for (unsigned o = 0; o + 16 < n; o += 4) if (!memcmp(f + o, "WFM3", 4)) { it_bank = f + o; it_bank_n = n - o; break; }
    for (int w = 0; w < 4; w++) {
        char *s = it_reason_txt[w];
        if (!it_bank || pa_wfm_dialogue(it_bank, it_bank_n, rd16(it_bank, 0x10) + w, s, sizeof it_reason_txt[w]) <= 0) s[0] = 0;
        for (char *p = s; *p; p++) if (*p == '\n') *p = ' ';
    }
    it_state = 1;
    port_printf("[panel] items: D0SPSUB3 tables read, message 2 \"%s\"\n", it_reason_txt[2]);
}

/* ---- the game's checks */
extern void gameMainTask(void);
static u16 *it_task(void) {   /* the task running gameMainTask (its state words +0x48..+0x4E) */
    for (int i = 0; i < 3; i++) {
        u8 *t = (u8 *)port_hiram + i * 0x70;
        if (*(u32 *)(t + 0xC) == (u32)(uintptr_t)gameMainTask) return (u16 *)(t + 0x48);
    }
    return NULL;
}
/* updatePauseMenu's conditions for Triangle (the press aside); NULL = the menu could open now */
static const char *it_open_reason(void) {
    u16 *w = it_task();
    if (!in_gameplay() || !w || w[0] != 1 || w[1] != 1) return T("Not in a game", "Fuera de la partida");
    if (!(w[2] == 1 || w[2] == 2 || w[2] == 4 || w[2] == 5) || w[3] != 1) return T("Not now (menu, loading or scene)", "Ahora no (men\xFA, carga o escena)");
    if (SPAD8(0x3CE) | SPAD8(0x1CC) || SPAD16(0x1C6)) return T("The game is paused", "El juego est\xE1 en pausa");
    if (G8(0x8009C975u) != 0 && G8(0x8009C975u) != 0x10) return T("The screen is fading", "La pantalla est\xE1 en transici\xF3n");
    if (G8(0x8009CDA2u)) return T("Not available in this scene", "No disponible en esta escena");
    if (!G8(0x8009C93Au)) return T("Tomba is not on the ground", "Tomba no est\xE1 en el suelo");
    if (G8(0x8009D2B0u) != 1 || G8(0x8009C93Fu)) return T("Tomba is busy", "Tomba est\xE1 ocupado");
    if (G8(0x800A5462u) != G8(0x8009D2B0u) && (G8(0x800A603Cu) != G8(0x8009D2B0u) || G8(0x800A603Du) >= 0x20)) return T("Tomba is busy", "Tomba est\xE1 ocupado");
    if (SPAD8(0x1B8)) return T("Not available right now", "No disponible ahora");
    return NULL;
}
extern int D0SPSUB3_subScreenCondFalse(void), D0SPSUB3_subScreenCondTrue(void), D0SPSUB3_subScreenCondPlayerH(unsigned char, short),
    D0SPSUB3_subScreenCondPlayerY(int, short), D0SPSUB3_subScreenCondPlayerD(unsigned char, short), D0SPSUB3_subScreenCondArea(int, short),
    D0SPSUB3_subScreenCondSection(int, short), D0SPSUB3_subScreenCondUnk6Is1(void), D0SPSUB3_subScreenCondFlag100(void),
    D0SPSUB3_subScreenCondEventFlag(int, short, short), D0SPSUB3_subScreenCondFlag544(int, short, short), D0SPSUB3_subScreenCondHealthSettled(void),
    D0SPSUB3_subScreenCondNearType0A(void), D0SPSUB3_subScreenCondNoWeapon(void), D0SPSUB3_subScreenCondRefreshingDrinkSpot(void),
    D0SPSUB3_subScreenCondFlag40(void), D0SPSUB3_subScreenCondVarD0E6Under5(void), D0SPSUB3_subScreenCondVar6102Set(void);
static int it_op(const ItCond *c) {   /* D_800F6714[c->fn](c->a, c->b, c->c) with each op's own prototype */
    switch (c->fn) {
    case 0: return D0SPSUB3_subScreenCondFalse();
    case 1: return D0SPSUB3_subScreenCondTrue();
    case 2: return D0SPSUB3_subScreenCondPlayerH(c->a, c->b);
    case 3: return D0SPSUB3_subScreenCondPlayerY(c->a, c->b);
    case 4: return D0SPSUB3_subScreenCondPlayerD(c->a, c->b);
    case 5: return D0SPSUB3_subScreenCondArea(c->a, c->b);
    case 6: return D0SPSUB3_subScreenCondSection(c->a, c->b);
    case 7: return D0SPSUB3_subScreenCondUnk6Is1();
    case 8: return D0SPSUB3_subScreenCondFlag100();
    case 9: return D0SPSUB3_subScreenCondEventFlag(c->a, c->b, c->c);
    case 10: return D0SPSUB3_subScreenCondFlag544(c->a, c->b, c->c);
    case 11: return D0SPSUB3_subScreenCondHealthSettled();
    case 12: return D0SPSUB3_subScreenCondNearType0A();
    case 13: return D0SPSUB3_subScreenCondNoWeapon();
    case 14: return D0SPSUB3_subScreenCondRefreshingDrinkSpot();
    case 15: return D0SPSUB3_subScreenCondFlag40();
    case 16: return D0SPSUB3_subScreenCondVarD0E6Under5();
    case 17: return D0SPSUB3_subScreenCondVar6102Set();
    }
    return 0;
}
/* the item's condition script as subScreenItemListStep state 2 walks it */
static int it_cond(int item) {
    u32 p = rd32(s3(S3_CONDS), 4 * s3(S3_CONDIDX)[item]);
    if (p < OVL_BASE || p + 8 > S3_END) return 0;
    const u8 *base = s3(p), *q = base; int r = 0;
    for (int guard = 0; guard < 64; guard++) {
        ItCond c = { rd16(q, 0), q[2], q[3], rd16(q, 4), rd16(q, 6) };
        r = it_op(&c);
        if (c.next == -1) break;
        q = r ? base + 8 * c.next : q + 8;
        if (q < it_ovl || q + 8 > it_ovl + it_ovl_n) return 0;
    }
    return r;
}
static int in_list(u32 a, int item) { for (const u8 *q = s3(a); rd16(q, 0) != -1 && q < s3(S3_END); q += 2) if (rd16(q, 0) == item) return 1; return 0; }
static int it_fail_msg(int item) {    /* which message box the menu opens when the script fails */
    if (in_list(S3_MSG0, item)) return 0;
    if (in_list(S3_MSG1, item)) return 1;
    if (in_list(S3_MSG3, item)) return G8(0x8009D0E6u) < 5 ? 3 : 2;
    return 2;
}
static const char *it_reason(int item) {   /* NULL: confirming it now does what the menu does */
    const char *r = it_open_reason();
    if (r) return r;
    if (it_cond(item)) return NULL;
    int w = it_fail_msg(item);
    if (it_reason_txt[w][0]) return it_reason_txt[w];
    static const char *en[4] = { "No need using this.", "Already equipped.", "Can't use this here.", "Not enough." }, *sp[4] = { "No hace falta usarlo.", "Ya equipado.", "No se puede usar aqu\xED.", "No es suficiente." };
    return es() ? sp[w] : en[w];
}
/* the in-use mark of subScreenDrawItemGrid */
static int it_in_use(int item) {
    int suit = G8(0x8009C990u), pants_ok = G8(0x8009D090u) == 0xFF, weap_ok = (unsigned)(suit - 1) >= 2, wp = G8(0x8009D2B2u), pn = G8(0x8009D2B3u);
    switch (item) {
    case 10: return G8(0x8009CEF7u) == 1;
    case 12: return G8(0x8009CFF8u) == 1;
    case 20: return G8(0x8009CEF8u) == 1;
    case 24: return pants_ok && pn == 2;
    case 28: return pants_ok && pn == 3;
    case 29: return pants_ok && pn == 1;
    case 32: return pants_ok && pn == 0;
    case 98: return suit == 1;
    case 99: return suit == 2;
    case 100: return suit == 3;
    case 96: return G8(0x8009D006u) != 0;
    case 132: return G8(0x8009CFFBu) != 0;
    case 27: return weap_ok && wp == 0;
    case 33: return weap_ok && wp == 8;
    case 34: return weap_ok && wp == 9;
    case 21: return weap_ok && wp == 5;
    case 22: return weap_ok && wp == 6;
    case 23: return weap_ok && wp == 7;
    }
    return 0;
}

/* ---- the list: the inventory grouped as the menu's sort lays it out */
static u8 it_items[256], it_icat[256]; static int it_n;
static void it_build(void) {
    int md = G_INV_MODE & 0x7FFF, cnt = G_INV_COUNT, have[256] = { 0 };
    if (md > 3) md = 0;
    if (cnt > 256) cnt = 256;
    for (int i = 0; i < cnt; i++) have[G_INV_ITEM(i)] = 1;
    it_n = 0;
    for (int j = 0; j < 5; j++) {
        int c = s3(S3_ORDER)[md * 5 + j];
        for (int i = 0; i < 0x11E; i += 2) {
            int it = s3(S3_PAIRS)[i + 1];
            if (s3(S3_PAIRS)[i] == c && have[it] == 1) { have[it] = 2; it_icat[it_n] = (u8)c; it_items[it_n++] = (u8)it; }
        }
    }
    for (int i = 0; i < cnt; i++) { int it = G_INV_ITEM(i); if (have[it] == 1) { have[it] = 2; it_icat[it_n] = 5; it_items[it_n++] = (u8)it; } }
}
static const char *cat_name(int c) {
    switch (c) {
    case 0: return T("Keys", "Llaves");
    case 1: return T("Weapons", "Armas");
    case 2: return T("Clothes and gear", "Ropa y equipo");
    case 3: return T("Key items", "Objetos importantes");
    case 4: return T("Consumables", "Consumibles");
    default: return T("Other", "Otros");
    }
}
static const char *it_name(int item, char *b, int n) {
    if (item >= 0 && item < 200 && item_name[item]) return item_name[item];
    snprintf(b, n, T("Item %d", "Objeto %d"), item); return b;
}

/* ---- requests (panel input) -> the game loop */
enum { RQ_NONE, RQ_USE, RQ_WARP, RQ_SORT };
static volatile int it_rq, it_rq_item, it_rq_dest;
static short it_lc[256], it_ly[256];   /* grid column / y (list pixels) of each entry, from the last draw */
static int it_cols = 2, it_vx, it_vy, it_vw, it_vh, it_follow, it_drag, it_dy0, it_dlast;
static float it_scroll, it_vel, it_s0;
static int it_sel, it_warp = -1, it_wsel, it_wtop;   /* it_warp: the item asking for a destination */
static char it_msg[96]; static int it_msg_t, it_msg_bad;
static void it_say(int bad, const char *s) { snprintf(it_msg, sizeof it_msg, "%s", s); it_msg_t = port_frame; it_msg_bad = bad; }

/* destinations of Baron / the Wings: the popup lists of state 3 (regions with D_8009D2C4 set, then their bits) */
static int it_dests(u8 (*d)[3]) {
    int n = 0;
    for (int r = 0; r < 9; r++) {
        unsigned bits = G_MAPREG(r);
        for (int b = 0; b < 9; b++, bits >>= 1) if (bits & 1) { d[n][0] = (u8)r; d[n][1] = (u8)b; n++; }
    }
    return n;
}
static int dest_of(int r, int b, int *area, int *sec) {
    u32 p = rd32(s3(S3_DEST), 4 * r) + 2 * b;
    if (p < OVL_BASE || p + 2 > S3_END) return 0;
    *area = s3(p)[0]; *sec = s3(p)[1];
    return 1;
}

extern int D0SPSUB3_func_800E8950(int item);
extern void *printInfoMessage(int, int);
extern int allocObjectLayer2(void);
extern int SfxPlay2(int, int);
/* the menu's confirm path, between two frames of the game loop */
void port_panel_tick(void) {
    int rq = it_rq, item = it_rq_item, dest = it_rq_dest; char b[64];
    if (!port_panel_on || rq == RQ_NONE) return;
    it_rq = RQ_NONE;
    if (it_state != 1) return;
    const char *why = it_open_reason();
    if (why) { it_say(1, why); return; }
    if (rq == RQ_SORT) {   /* cursor 2 of state 1 */
        unsigned md = G_INV_MODE; int k = 0;
        if (md & 0x8000) { md &= 0x7FFF; G_INV_MODE = (u16)md; }
        else { if (++md >= 4) md = 0; G_INV_MODE = (u16)md; }
        if (md > 3) md = 0;
        for (int j = 0; j < 5; j++) {
            int c = s3(S3_ORDER)[md * 5 + j];
            for (int i = 0; i < 0x11E && k < 256; i += 2) if (s3(S3_PAIRS)[i] == c && G_QTY(s3(S3_PAIRS)[i + 1])) G_INV_ITEM(k++) = s3(S3_PAIRS)[i + 1];
        }
        SfxPlay2(10, 10);
        port_printf("[panel] frame %d: items sorted (mode %d)\n", port_frame, md);
        return;
    }
    int held = 0;
    for (int i = 0; i < G_INV_COUNT && i < 256; i++) held |= G_INV_ITEM(i) == item;
    if (!held) { it_say(1, T("No longer in the inventory", "Ya no est\xE1 en el inventario")); return; }
    if (!it_cond(item)) { it_say(1, it_reason(item)); return; }
    if (rq == RQ_WARP) {   /* state 3 sub 2 */
        int area, sec;
        if ((item != 7 && item != 14) || !dest_of(dest >> 8, dest & 0xFF, &area, &sec)) return;
        G16(0x8009CD94u) = (u16)area;
        G8(0x800A603Du) = 0x30;
        G16(0x8009CDA0u) = 0;
        G8(0x8009D2B0u) = 3;
        G8(0x800A603Cu) = 1;
        G8(0x800A603Eu) = 0;
        G8(0x800A6038u) = 3;
        G8(0x8009C940u) = 1;
        G8(0x8009C941u) = (u8)item;
        G16(0x8009CD96u) = (u16)sec;
        if (item == 14) {
            u8 *o = (u8 *)(uintptr_t)allocObjectLayer2();
            if (o) { o[0] = 1; o[2] = 0x54; o[3] = 1; }
            printInfoMessage(14, 1);
        } else printInfoMessage(7, 1);
        SfxPlay2(10, 10);
        snprintf(b, sizeof b, "%s", place_room(area, sec) ? place_room(area, sec) : place_area(area, sec) ? place_area(area, sec) : "?");
        { char t[96]; snprintf(t, sizeof t, T("Flying to %s", "Volando a %s"), b); it_say(0, t); }
        port_printf("[panel] frame %d: item %d flies to area %d section %d\n", port_frame, item, area, sec);
        return;
    }
    if (item == 7 || item == 14) return;   /* they ask for a destination first */
    int r = D0SPSUB3_func_800E8950(item);
    switch (r) {   /* subScreenItemListStep's result switch */
    case 1: {
        int n = 4, a = item;
        if (a != 0x14 && a != 0x15 && a != 0x16 && a != 0x17 && a != 0x1B && a != 0x21 && a != 0x22 && a != 0x20 && a != 0x1C && a != 0x1D) n = a == 0x18 ? 4 : 1;
        printInfoMessage(a, n);
        break; }
    case 3: printInfoMessage(0x1E, 2); break;
    case 4: printInfoMessage(0x1F, 2); break;
    case 5: printInfoMessage(0x20, 2); break;
    }
    { char t[96]; snprintf(t, sizeof t, T("Done: %s", "Hecho: %s"), it_name(item, b, sizeof b)); it_say(0, t); }
    port_printf("[panel] frame %d: item %d used (handler result %d): weapon %d pants %d\n", port_frame, item, r, G8(0x8009D2B2u), G8(0x8009D2B3u));
}

/* ---- drawing */
#define IT_RH 18
enum { HS_ITEM = 0x700, HS_ITOP = 0x800 };   /* HS_ITOP + 0 up, 1 down, 2 sort, 3 action, 4 cancel, 0x10.. destinations */
static void it_confirm(int k) {
    if (k < 0 || k >= it_n) return;
    int item = it_items[k]; const char *why = it_reason(item);
    if (why) { it_say(1, why); return; }
    if (item == 7 || item == 14) { it_warp = item; it_wsel = it_wtop = 0; return; }
    it_rq_item = item; it_rq = RQ_USE;
}
static void it_page_warp(int x, int y, int w, int h) {
    u8 d[81][3]; int n = it_dests(d); char b[96];
    window_(x, y, w, h);
    pa_text(x + 10, y + 6, 16, T("Where to?", "\xBF" "Ad\xF3nde?"), C_RED, C_REDE);
    int ly = y + 26, vis = (h - 34) / IT_RH;
    if (it_wsel >= n) it_wsel = n - 1; if (it_wsel < 0) it_wsel = 0;
    if (it_wsel < it_wtop) it_wtop = it_wsel; if (it_wsel >= it_wtop + vis) it_wtop = it_wsel - vis + 1;
    if (!n) pa_text(x + 10, ly, 16, T("No place visited yet", "Ning\xFAn lugar visitado"), C_MUTED, C_EDGE);
    for (int i = it_wtop; i < n && i < it_wtop + vis; i++, ly += IT_RH) {
        int area = 0, sec = 0; dest_of(d[i][0], d[i][1], &area, &sec);
        const char *an = place_area(area, sec), *rn = place_room(area, sec);
        snprintf(b, sizeof b, "%s%s%s", an ? an : "?", rn ? " - " : "", rn ? rn : "");
        if (i == it_wsel) pa_fill(x + 6, ly - 1, w - 12, IT_RH, C_SEL);
        pa_text_fit(x + 12, ly, 16, w - 24, b, i == it_wsel ? C_WHITE : C_DARK, i == it_wsel ? C_SEL : C_EDGE);
        hot(x + 6, ly - 1, w - 12, IT_RH, HS_ITOP + 0x10 + i);
    }
}
static void page_items(int play) {
    int W = pa_cw, H = pa_ch, top = 56; char b[96];
    if (!it_state) it_load();
    if (!play || it_state != 1) {
        window_(8, top, W - 16, H - top - 8);
        pa_text_fit(20, top + 14, 16, W - 40, !play ? T("Waiting for the game...", "Esperando a la partida...") : T("The item sub-screen data is not on this disc", "Faltan los datos del men\xFA de objetos"), C_MUTED, C_EDGE);
        return;
    }
    it_build();
    if (it_sel >= it_n) it_sel = it_n - 1; if (it_sel < 0) it_sel = 0;
    const char *gate = it_open_reason();
    /* top row: the sort entry, then what is worn */
    plank(8, top, 92); hot(8, top, 92, 24, HS_ITOP + 2); plank_text(54, top + 4, T("Sort", "Ordenar"), 0);
    {
        int x = 108;
        for (int k = 0; k < it_n; k++) if (it_in_use(it_items[k]) && x < W - 30) { pa_item_icon(x, top + 4, it_items[k], PA_OUTLINE); x += 20; }
        if (gate) pa_text_fit(x + 4, top + 4, 16, W - x - 12, gate, C_ORANGE, C_REDE);
    }
    int dh = 74, ly0 = top + 28, lh = H - 8 - dh - 4 - ly0;
    if (it_warp >= 0) it_page_warp(8, ly0, W - 16, lh);
    else {
        window_(8, ly0, W - 16, lh);
        /* a touch grid: icons at 3x (2x on a narrow canvas), the name under them, a header per category; scrolls
           (drag / fling, the arrows, or follows the controller's cursor) */
        int sc = W >= 400 ? 3 : 2, icon = 16 * sc, gap = 6, avail = W - 16 - 12 - 10;
        int cols = avail / (icon + 64); if (cols < 2) cols = 2; if (cols > 6) cols = 6;
        int cw = (avail - (cols - 1) * gap) / cols, ch = icon + 32, hh = 24;
        int vx = 14, vy = ly0 + 6, vh = lh - 12, yy = 0, col = 0;
        for (int k = 0; k < it_n; k++) {
            if (!k || it_icat[k] != it_icat[k - 1]) { if (col) yy += ch + gap; col = 0; yy += hh; }
            it_lc[k] = (short)col; it_ly[k] = (short)yy;
            if (++col == cols) { col = 0; yy += ch + gap; }
        }
        int total = yy + (col ? ch : 0) + 4;
        it_cols = cols; it_vx = vx; it_vy = vy; it_vw = avail; it_vh = vh;
        if (it_follow && it_n) {   /* the controller moved the cursor: keep it in view */
            int t = it_ly[it_sel] - (it_sel == 0 || it_icat[it_sel] != it_icat[it_sel - 1] ? hh : 0);
            if (t < it_scroll) it_scroll = (float)t;
            if (it_ly[it_sel] + ch > it_scroll + vh) it_scroll = (float)(it_ly[it_sel] + ch - vh);
            it_follow = 0;
        }
        if (!it_drag) { it_scroll += it_vel; it_vel *= .9f; if (it_vel > -.3f && it_vel < .3f) it_vel = 0; }
        float mx = total > vh ? (float)(total - vh) : 0;
        if (it_scroll > mx) { it_scroll = mx; it_vel = 0; } if (it_scroll < 0) { it_scroll = 0; it_vel = 0; }
        int so = (int)it_scroll;
        int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
        pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + avail; pa_clip_y1 = vy + vh;
        if (!it_n) pa_text(20, vy + 4, 16, T("No items", "Sin objetos"), C_MUTED, C_EDGE);
        for (int k = 0; k < it_n; k++) {
            int y = vy + it_ly[k] - so, x = vx + it_lc[k] * (cw + gap);
            if (!k || it_icat[k] != it_icat[k - 1]) {
                int hy = y - hh + 3; const char *cn = cat_name(it_icat[k]);
                if (hy + hh > vy && hy < vy + vh) { pa_text(vx + 2, hy, 16, cn, C_RED, C_REDE); pa_fill(vx + 8 + pa_text_w(16, cn), hy + 9, avail - 12 - pa_text_w(16, cn), 1, C_EDGE); }
            }
            if (y + ch <= vy || y >= vy + vh) continue;
            int item = it_items[k], ok = !it_reason(item), use = it_in_use(item), sel = k == it_sel;
            if (sel) { pa_fill(x - 2, y - 2, cw + 4, ch + 4, RGB(250, 200, 60)); pa_fill(x, y, cw, ch, C_SEL); }
            else { pa_fill(x, y, cw, ch, C_EDGE); pa_fill(x + 1, y + 1, cw - 2, ch - 2, use ? RGB(250, 214, 150) : RGB(250, 236, 226)); }
            pa_item_icon(x + (cw - icon) / 2, y + 4, item, (ok ? 0 : PA_SHADE) | (sc == 3 ? PA_X3 : PA_X2));
            const char *nm = it_name(item, b, sizeof b); char t[112];
            if (s3(S3_NCLUT)[item] & 0x80) { snprintf(t, sizeof t, "%s x%d", nm, G_QTY(item)); nm = t; }
            text_wrap(x + 3, y + icon + 8, 8, cw - 6, 10, 2, 1, nm, sel ? C_WHITE : ok ? C_MUTED8 : C_MUTED, sel ? C_SEL : RGB(230, 200, 190));
            int hy0 = y < vy ? vy : y, hy1 = y + ch > vy + vh ? vy + vh : y + ch;
            if (hy1 - hy0 > 8) hot(x, hy0, cw, hy1 - hy0, HS_ITEM + (k & 0xFF));
        }
        pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
        if (total > vh) {   /* the scroll bar, with arrow hit areas at its ends */
            int bx = W - 8 - 14, bh = vh * vh / total; if (bh < 20) bh = 20;
            pa_fill(bx, vy, 6, vh, RGB(230, 200, 190));
            pa_fill(bx, vy + (int)((vh - bh) * (it_scroll / mx)), 6, bh, C_RED);
            hot(bx - 12, vy, 24, vh / 2, HS_ITOP); hot(bx - 12, vy + vh / 2, 24, vh - vh / 2, HS_ITOP + 1);
        }
    }
    /* the detail: the selected item, its description, why it is grey, the action */
    int dy = H - 8 - dh;
    window_(8, dy, W - 16, dh);
    if (it_n) {
        int item = it_warp >= 0 ? it_warp : it_items[it_sel]; const char *why = it_reason(item);
        pa_item_icon(16, dy + 8, item, why ? PA_SHADE : 0);
        pa_text_fit(38, dy + 7, 16, W - 150, it_name(item, b, sizeof b), C_DARK, C_EDGE);
        char desc[200]; int ly = dy + 27;
        if (it_bank && pa_wfm_dialogue(it_bank, it_bank_n, rd16(it_bank, 0x12) + item, desc, sizeof desc) > 0) {
            for (char *p = desc, *e; *p && ly < dy + 27 + 20; p = e + (*e != 0), ly += 10) {
                e = strchr(p, '\n'); if (!e) e = p + strlen(p);
                char c = *e; *e = 0; pa_text_fit(16, ly, 8, W - 40, p, C_MUTED8, RGB(230, 200, 190)); *e = c;
            }
        }
        const char *st = why ? why : NULL;
        if (it_msg_t && port_frame - it_msg_t < 200) st = it_msg;
        if (st) pa_text_fit(16, dy + dh - 21, 16, W - 140, st, st == it_msg ? (it_msg_bad ? C_RED : C_GREEN) : C_RED, C_REDE);
        const char *act = it_warp >= 0 ? T("Fly", "Volar") : it_icat[it_sel] == 1 ? T("Equip", "Equipar") : it_icat[it_sel] == 2 ? (it_in_use(item) ? T("Take off", "Quitar") : T("Wear", "Poner")) : T("Use", "Usar");
        int bw = 96, bx = W - 16 - bw;
        plank(bx, dy + 8, bw); hot(bx, dy + 8, bw, 24, HS_ITOP + 3); plank_text(bx + bw / 2, dy + 12, act, !why);
        if (it_warp >= 0) { plank(bx, dy + 38, bw); hot(bx, dy + 38, bw, 24, HS_ITOP + 4); plank_text(bx + bw / 2, dy + 42, T("Cancel", "Cancelar"), 0); }
    }
}

/* ---- input (panel_events.inc routes the tab's keys and touches here) */
static void it_warp_go(void) {
    u8 d[81][3]; int n = it_dests(d);
    if (it_wsel < 0 || it_wsel >= n) return;
    it_rq_item = it_warp; it_rq_dest = d[it_wsel][0] << 8 | d[it_wsel][1]; it_rq = RQ_WARP; it_warp = -1;
}
static void items_action(int a) {
    if (it_warp >= 0) {
        if (a == IN_UP) it_wsel--; else if (a == IN_DOWN) it_wsel++;
        else if (a == IN_OK) it_warp_go(); else if (a == IN_BACK) it_warp = -1;
        return;
    }
    if (a == IN_LEFT) it_sel--; else if (a == IN_RIGHT) it_sel++;
    else if ((a == IN_UP || a == IN_DOWN) && it_n) {   /* the grid row above / below, the nearest column */
        int y = it_ly[it_sel], ty = -1, best = -1;
        for (int k = 0; k < it_n; k++) if (a == IN_UP ? it_ly[k] < y && it_ly[k] > ty : it_ly[k] > y && (ty < 0 || it_ly[k] < ty)) ty = it_ly[k];
        if (ty >= 0) for (int k = 0; k < it_n; k++) if (it_ly[k] == ty && (best < 0 || abs(it_lc[k] - it_lc[it_sel]) < abs(it_lc[best] - it_lc[it_sel]))) best = k;
        if (best >= 0) it_sel = best;
    }
    else if (a == IN_OK) it_confirm(it_sel);
    if (it_sel < 0) it_sel = 0; if (it_sel >= it_n) it_sel = it_n - 1;
    it_follow = 1; it_vel = 0;
}
static void items_hit(int id) {
    if ((id & ~0xFF) == HS_ITEM) { int k = id & 0xFF; if (k == it_sel) it_confirm(k); else it_sel = k; return; }
    int k = id - HS_ITOP;
    if (k == 0) { it_scroll -= it_vh * .8f; it_vel = 0; }   /* a page of the grid (clamped when drawn) */
    else if (k == 1) { it_scroll += it_vh * .8f; it_vel = 0; }
    else if (k == 2) it_rq = RQ_SORT;
    else if (k == 3) { if (it_warp >= 0) it_warp_go(); else it_confirm(it_sel); }
    else if (k == 4) it_warp = -1;
    else if (k >= 0x10) { if (it_wsel == k - 0x10) it_warp_go(); else it_wsel = k - 0x10; }
}
void items_action_(int a) { items_action(a); }   /* named from panel_events.inc, which comes first */
void items_hit_(int id) { items_hit(id); }
/* a drag in the grid scrolls it and flings on release (1: the touch was a drag, not a tap) */
int items_touch_(int x, int y, int action) {
    if (it_warp >= 0) return 0;
    if (action == 0) {
        it_drag = -1; it_vel = 0;
        if (x >= it_vx && y >= it_vy && x < it_vx + it_vw && y < it_vy + it_vh) { it_drag = 0; it_dy0 = it_dlast = y; it_s0 = it_scroll; }
        return 0;
    }
    if (it_drag < 0) return 0;
    if (action == 2) {
        if (!it_drag && abs(y - it_dy0) > 10) it_drag = 1;
        if (it_drag) { it_scroll = it_s0 - (y - it_dy0); it_vel = it_vel * .5f + (it_dlast - y) * .5f; it_dlast = y; return 1; }
        return 0;
    }
    int was = it_drag == 1; it_drag = -1;
    if (!was) it_vel = 0;
    return was;
}
