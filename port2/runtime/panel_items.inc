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

/* ---- D0SPSUB3 / USA DSPSUB3 addresses (panel_addrs.h; checked against the file before use) */
#define S3_ORDER   PV_S3_ORDER     /* u8[4][5] category order per sort mode */
#define S3_PAIRS   PV_S3_PAIRS     /* {category, item} x 0x8F */
#define S3_CONDIDX PV_S3_CONDIDX   /* u8[item] condition script of the item */
#define S3_CONDS   PV_S3_CONDS     /* Cond *[] */
#define S3_FNS     PV_S3_FNS       /* the 18 condition ops */
#define S3_DEST    PV_S3_DEST      /* u8 *[9]: {area, section} per region bit */
#define S3_MSG0    PV_S3_MSG0      /* items answering message 0 (-1 ended s16) */
#define S3_MSG1    PV_S3_MSG1      /* message 1 */
#define S3_MSG3    PV_S3_MSG3      /* message 3 (2 once D_8009D0E6 >= 5) */
#define S3_NCLUT   PV_S3_NCLUT     /* u8[item] name clut, 0x80 = counted ("x NN") */
#define S3_END     PV_S3_END

#define G_INV_ITEM(i) G8(PV_GAME_INV_ITEM + (i))
#define G_INV_COUNT   G16(PV_GAME_INV_COUNT)
#define G_INV_MODE    G16(PV_GAME_INV_MODE)
#define G_QTY(i)      G8(PV_GAME_QTY + (i))

typedef struct { s16 next; u8 fn, a; s16 b, c; } ItCond;
static u8 *it_ovl, *it_bank; static unsigned it_ovl_n, it_bank_n;
static int it_state;   /* 0 not loaded, 1 ok, -1 file missing / another overlay */
static u8 it_cat[256]; /* category of an item (0xFF none) */
static char it_reason_txt[4][96];

static const u8 *s3(u32 a) { return it_ovl + (a - OVL_BASE); }
static void it_load(void) {
    it_state = -1;
    it_ovl = port_cd_read_named(PA_F_SUB3, &it_ovl_n);
    if (!it_ovl || it_ovl_n < S3_END - OVL_BASE) { port_printf("[panel] items: %s not readable\n", PA_F_SUB3); return; }
#if PORT_VERSION_USA
    static const u32 fns[18] = { 0x800ED2D4, 0x800ED2DC, 0x800ED2E4, 0x800ED3EC, 0x800ED4F0, 0x800ED5F8, 0x800ED6FC, 0x800ED800, 0x800ED818,
                                 0x800ED830, 0x800ED8F8, 0x800ED9C0, 0x800ED9E0, 0x800EDAF4, 0x800EDB04, 0x800EDB70, 0x800EDB88, 0x800EDB98 };
#else
    static const u32 fns[18] = { 0x800EE12C, 0x800EE134, 0x800EE13C, 0x800EE244, 0x800EE348, 0x800EE450, 0x800EE554, 0x800EE658, 0x800EE670,
                                 0x800EE688, 0x800EE750, 0x800EE818, 0x800EE838, 0x800EE94C, 0x800EE95C, 0x800EE9C8, 0x800EE9E0, 0x800EE9F0 };
#endif
    for (int i = 0; i < 18; i++) if (rd32(s3(S3_FNS), 4 * i) != fns[i]) { port_printf("[panel] items: %s is not the overlay this build knows (op %d)\n", PA_F_SUB3, i); return; }
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
    if (!(w[2] == 1 || w[2] == 2 || w[2] == 4 || w[2] == 5 || w[2] == 6) || w[3] != 1) return T("Not now (menu, loading or scene)", "Ahora no (men\xFA, carga o escena)");
    if (SPAD8(0x3CE) | SPAD8(0x1CC) || SPAD16(0x1C6)) return T("The game is paused", "El juego est\xE1 en pausa");
    if (G8(PV_GAME_FADE) != 0 && G8(PV_GAME_FADE) != 0x10) return T("The screen is fading", "La pantalla est\xE1 en transici\xF3n");
    if (G8(PV_GCD_NO_MENU)) return T("Not available in this scene", "No disponible en esta escena");
    if (!G8(PV_GVARS_LANDED)) return T(PANEL_HERO " must be on the ground", "Tombi tiene que estar en el suelo");
    if (G8(PV_GAME_PLAYER_MODE) != 1 || G8(PV_GVARS_C93F)) return T(PANEL_HERO " must stand still", "Tombi tiene que estar quieto");
    if (G8(PV_OBJ_5462) != G8(PV_GAME_PLAYER_MODE) && (G8(PV_PLAYER_STATE) != G8(PV_GAME_PLAYER_MODE) || G8(PV_PLAYER_SUBSTATE) >= 0x20)) return T(PANEL_HERO " must stand still", "Tombi tiene que estar quieto");
    if (SPAD8(0x1B8)) return T("Not available right now", "No disponible ahora");
    return NULL;
}
/* the native ops: USA builds some of them from another op's template (port/tools/gen_port.py: name__ADDR) */
#if PORT_VERSION_USA
#define D0SPSUB3_subScreenCondFalse D0SPSUB3_stubReturnZero8004F344
#define D0SPSUB3_subScreenCondPlayerD D0SPSUB3_subScreenCondPlayerH__800ED4F0
#define D0SPSUB3_subScreenCondArea D0SPSUB3_subScreenCondPlayerY__800ED5F8
#define D0SPSUB3_subScreenCondSection D0SPSUB3_subScreenCondPlayerY__800ED6FC
#define D0SPSUB3_subScreenCondFlag544 D0SPSUB3_subScreenCondEventFlag__800ED8F8
#define D0SPSUB3_func_800E8950 D0SPSUB3_func_800E89F8   /* the item use handler (USA DSPSUB3 0x800E7CB0) */
#endif
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
    if (in_list(S3_MSG3, item)) return G8(PV_GAME_D0E6) < 5 ? 3 : 2;
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
    int suit = G8(PV_GAME_SUIT), pants_ok = G8(PV_GAME_PANTS_OK) == 0xFF, weap_ok = (unsigned)(suit - 1) >= 2, wp = G8(PV_GAME_WEAPON), pn = G8(PV_GAME_PANTS);
    switch (item) {
    case 10: return G8(PV_GAME_CEF7) == 1;
    case 12: return G8(PV_GAME_CFF8) == 1;
    case 20: return G8(PV_GAME_CEF8) == 1;
    case 24: return pants_ok && pn == 2;
    case 28: return pants_ok && pn == 3;
    case 29: return pants_ok && pn == 1;
    case 32: return pants_ok && pn == 0;
    case 98: return suit == 1;
    case 99: return suit == 2;
    case 100: return suit == 3;
    case 96: return G8(PV_GAME_D006) != 0;
    case 132: return G8(PV_GAME_CFFB) != 0;
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
static int it_cols = 2, it_follow, it_wfollow;
static Scroll it_sc, it_wsc;   /* the grid, the destinations (panel.c scroller) */
static int it_sel, it_warp = -1, it_wsel;   /* it_warp: the item asking for a destination */
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
        G16(PV_GCD_WARP_AREA) = (u16)area;
        G8(PV_PLAYER_SUBSTATE) = 0x30;
        G16(PV_GCD_CDA0) = 0;
        G8(PV_GAME_PLAYER_MODE) = 3;
        G8(PV_PLAYER_STATE) = 1;
        G8(PV_PLAYER_STEP) = 0;
        G8(PV_PLAYER) = 3;
        G8(PV_GVARS_USE_ITEM) = 1;
        G8(PV_GVARS_USE_ITEM_ID) = (u8)item;
        G16(PV_GCD_WARP_SECTION) = (u16)sec;
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
    { char t[96]; snprintf(t, sizeof t, T("Completed: %s", "Completado: %s"), it_name(item, b, sizeof b)); it_say(0, t); }
    port_printf("[panel] frame %d: item %d used (handler result %d): weapon %d pants %d\n", port_frame, item, r, G8(PV_GAME_WEAPON), G8(PV_GAME_PANTS));
}

/* ---- drawing */
#define IT_RH 18
enum { HS_ITEM = 0x700, HS_ITOP = 0x800 };   /* HS_ITOP + 2 sort, 3 action, 4 cancel, 0x10.. destinations */
static void it_confirm(int k) {
    if (k < 0 || k >= it_n) return;
    int item = it_items[k]; const char *why = it_reason(item);
    if (why) { it_say(1, why); return; }
    if (item == 7 || item == 14) { it_warp = item; it_wsel = 0; scr_reset(&it_wsc); return; }
    it_rq_item = item; it_rq = RQ_USE;
}
/* the destinations of Barón / the warp item, one thing per line as in the event list: the area (8 px) and under it the
   room (16 px), both wrapped, never cut; rows of different heights scrolled as one strip (drag / fling, cut at the
   window's edges), the D-pad's selection kept on screen */
static int warp_row(const u8 *dd, int w, const char **an, const char **rn) {
    int area = 0, sec = 0; dest_of(dd[0], dd[1], &area, &sec);
    *an = place_area(area, sec); *rn = place_room(area, sec);
    if (!*an) *an = "?";
    return 4 + text_lines(8, w, 3, *an) * 10 + (*rn ? text_lines(16, w, 3, *rn) * 16 : 0);
}
static void it_page_warp(int x, int y, int w, int h) {
    u8 d[81][3]; int n = it_dests(d), rh[81]; const char *an, *rn;
    window_(x, y, w, h);
    pa_text(x + 10, y + 6, 16, T("Where to?", "\xBF" "Ad\xF3nde?"), C_RED, C_REDE);
    int ly = y + 26, tw = w - 24;
    for (int i = 0; i < n; i++) rh[i] = warp_row(d[i], tw, &an, &rn);
    if (it_wsel >= n) it_wsel = n - 1; if (it_wsel < 0) it_wsel = 0;
    int cy[82]; cy[0] = 0; for (int i = 0; i < n; i++) cy[i + 1] = cy[i] + rh[i];
    int vy = ly - 2, vh = y + h - 8 - vy;
    scr_view(&it_wsc, x + 6, vy, w - 12, vh, n ? cy[n] + 2 : 0);
    if (it_wfollow && n) { scr_show(&it_wsc, cy[it_wsel], cy[it_wsel + 1] + 2); it_wfollow = 0; }
    if (!n) pa_text(x + 10, ly, 16, T("No place visited yet", "Ning\xFAn lugar visitado"), C_MUTED, C_EDGE);
    int so = scr_y(&it_wsc), rw = w - 12 - (it_wsc.total > vh ? 8 : 0);
    int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
    pa_clip_x0 = x + 6; pa_clip_y0 = vy; pa_clip_x1 = x + w - 6; pa_clip_y1 = vy + vh;
    for (int i = 0; i < n; i++) {
        int ry = vy + 2 + cy[i] - so, sel = i == it_wsel;
        if (ry + rh[i] <= vy || ry - 1 >= vy + vh) continue;
        warp_row(d[i], tw, &an, &rn);
        if (sel) pa_fill(x + 6, ry - 1, rw, rh[i] - 1, C_SEL);
        int l = text_wrap(x + 12, ry, 8, tw, 10, 3, 0, an, sel ? C_WHITE : C_RED8, sel ? RGB(60, 110, 50) : RGB(230, 200, 190));
        if (rn) text_wrap(x + 12, ry + l * 10, 16, tw, 16, 3, 0, rn, sel ? C_WHITE : C_DARK, sel ? C_SEL : C_EDGE);
        int hy0 = ry - 1 < vy ? vy : ry - 1, hy1 = ry - 2 + rh[i] > vy + vh ? vy + vh : ry - 2 + rh[i];
        if (hy1 - hy0 > 6) hot(x + 6, hy0, rw, hy1 - hy0, HS_ITOP + 0x10 + i);
    }
    pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
    scr_bar(&it_wsc, x + w - 13);
}
/* ---- drawing: a showcase of the selected item on the left (its icon at 4x in a stone frame, name, category,
   description, one status line, Ordenar and the action plank), every item as a grid of icons on stone on the right,
   scrolled by touch (drag / fling) and following the controller's cursor */
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
/* a plank button: st 0 normal (cream text), 1 ready (yellow), 2 not available (grey) */
static void it_btn(int x, int y, int w, const char *s, int st, int id) {
    plank(x, y, w); hot(x, y, w, 24, id);
    pa_text_ol(x + w / 2 - pa_text_w(16, s) / 2, y + 4, 16, s, st == 1 ? C_YELLOW : st == 2 ? RGB(176, 160, 144) : C_WHITE, C_WOODE, C_DARK);
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
/* the action: a plank twice as high, its label in the 24 px event-title letters when they have all of it (their own
   colours; shaded when the item cannot be used now), else the 16 px font pixel doubled with the dark rim */
#define IT_BIG 48
static void it_btn_big(int x, int y, int w, const char *s, int st, int id) {
    plank_h(x, y, w, IT_BIG); hot(x, y, w, IT_BIG, id);
    if (pa_has_glyphs(24, s)) {
        int tw = pa_text_w(24, s);
        pa_text_scaled(x + w / 2 - tw / 2, y + (IT_BIG - 24) / 2, 24, s, C_WHITE, C_WOODE, 1, st == 2);
    } else {
        int tw = 2 * pa_text_w(16, s);
        u32 o = pa_outline; pa_outline = C_DARK;
        pa_text_scaled(x + w / 2 - tw / 2, y + (IT_BIG - 32) / 2, 16, s, st == 1 ? C_YELLOW : st == 2 ? RGB(176, 160, 144) : C_WHITE, C_WOODE, 2, 0);
        pa_outline = o;
    }
}
static void it_frame(int x, int y, int w, int h, int t, u32 c) { pa_fill(x, y, w, t, c); pa_fill(x, y + h - t, w, t, c); pa_fill(x, y, t, h, c); pa_fill(x + w - t, y, t, h, c); }
static void it_selframe(int x, int y, int w, int h) {   /* the game's blinking yellow cursor frame */
    it_frame(x - 1, y - 1, w + 2, h + 2, 1, C_DARK);
    it_frame(x, y, w, h, 2, (port_frame >> 4) & 1 ? C_YELLOW : RGB(255, 168, 48));
}
/* the count: the HUD's digits on a red plate with a dark rim (right / bottom aligned) */
static void it_badge_n(int rx, int by, int n) {
    char d[8]; int nd = snprintf(d, sizeof d, "%d", n > 999 ? 999 : n), w = nd * 8 + 5, h = 13;
    pa_fill(rx - w + 1, by - h, w - 2, h, C_DARK); pa_fill(rx - w, by - h + 1, w, h - 2, C_DARK);
    pa_fill(rx - w + 1, by - h + 1, w - 2, h - 2, RGB(176, 40, 30)); pa_fill(rx - w + 2, by - h + 1, w - 4, 1, RGB(230, 110, 90));
    for (int i = 0; i < nd; i++) pa_spr(rx - w + 3 + 8 * i, by - h + 3, HUD, 8 * (d[i] - '0'), 64, 8, 8, 368, 481, 0);
}
/* in use: 15 x 15, green, a thick white tick */
static void it_badge_use(int x, int y) {
    pa_fill(x + 2, y, 11, 15, C_DARK); pa_fill(x, y + 2, 15, 11, C_DARK); pa_fill(x + 1, y + 1, 13, 13, C_DARK);
    pa_fill(x + 2, y + 1, 11, 13, RGB(56, 156, 56)); pa_fill(x + 1, y + 2, 13, 11, RGB(56, 156, 56)); pa_fill(x + 3, y + 1, 9, 2, RGB(120, 210, 110));
    line(x + 3, y + 7, x + 6, y + 10, 2, C_WHITE); line(x + 6, y + 10, x + 11, y + 4, 2, C_WHITE);
}
static void it_badges(int x, int y, int s, int item) {   /* on an icon drawn at scale s at (x, y) */
    if (it_in_use(item)) it_badge_use(x + 16 * s - 11, y - 4);
    if (s3(S3_NCLUT)[item] & 0x80) it_badge_n(x + 16 * s + 4, y + 16 * s + 3, G_QTY(item));
}
static void page_items(int play) {
    int W = pa_cw, H = pa_ch, top = 56; char b[200];
    if (!it_state) it_load();
    if (!play || it_state != 1) {
        window_(8, top, W - 16, H - top - 8);
        pa_text_fit(20, top + 14, 16, W - 40, !play ? T("Waiting for the game...", "Esperando a la partida...") : T("The item sub-screen data is not on this disc", "Faltan los datos del men\xFA de objetos"), C_MUTED, C_EDGE);
        return;
    }
    it_build();
    if (it_sel >= it_n) it_sel = it_n - 1; if (it_sel < 0) it_sel = 0;
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
        y += 17 * text_wrap(lx + 8, y, 16, lw - 16, 17, 2, 1, it_name(item, b, sizeof b), C_DARK, C_EDGE);
        if (it_warp < 0) { ctext(cx, y, 16, cat_name(it_icat[it_sel]), C_MUTED, C_EDGE); y += 18; }
        pa_fill(lx + 12, y + 1, lw - 24, 1, RGB(230, 200, 180)); y += 4;
        int lim = oy - 4 - (st ? 2 : 0) * 15;
        if (it_desc(item, b, sizeof b)) { int ml = (lim - y) / 15; if (ml > 0) text_wrap(lx + 10, y, 16, lw - 20, 15, ml, 0, b, C_MUTED8, C_EDGE); }
        if (st) text_wrap(lx + 10, lim, 16, lw - 20, 15, 2, 0, st, bad ? C_RED : C_GREEN, C_REDE);
        it_btn_big(lx + 10, ay, bw, it_act_name(it_sel, item), why ? 2 : 1, HS_ITOP + 3);
    }
    /* the grid: icons on stone, scrolled (drag / fling, the scroll bar's halves, or following the cursor) */
    if (it_warp >= 0) { it_page_warp(rx, top, rw, h); return; }
    stone(rx, top, rw, h);
    frame_(rx, top, rw, h);
    int cols = 3, ch = 62, vx = rx + 6, vy = top + 6, vw = rw - 12 - 6, vh = h - 12, cw = vw / cols, gx = vx + (vw - cols * cw) / 2;
    for (int k = 0; k < it_n; k++) { it_lc[k] = (short)(k % cols); it_ly[k] = (short)(k / cols * ch); }
    int total = (it_n + cols - 1) / cols * ch;
    it_cols = cols;
    scr_view(&it_sc, vx, vy, vw, vh, total);
    if (it_follow && it_n) { scr_show(&it_sc, it_ly[it_sel], it_ly[it_sel] + ch); it_follow = 0; }   /* the controller moved the cursor */
    int so = scr_y(&it_sc);
    int sx0 = pa_clip_x0, sy0 = pa_clip_y0, sx1 = pa_clip_x1, sy1 = pa_clip_y1;
    pa_clip_x0 = vx; pa_clip_y0 = vy; pa_clip_x1 = vx + vw; pa_clip_y1 = vy + vh;
    for (int k = 0; k < it_n; k++) {
        int x = gx + it_lc[k] * cw, y = vy + it_ly[k] - so;
        if (y + ch <= vy || y >= vy + vh) continue;
        int item = it_items[k], ok = !it_reason(item), sel = k == it_sel, ix = x + (cw - 48) / 2, iy = y + (ch - 48) / 2;
        if (sel) { window_(ix - 7, iy - 7, 62, 62); it_selframe(ix - 7, iy - 7, 62, 62); }
        pa_item_icon(ix, iy, item, PA_X3 | PA_OUTLINE | (ok ? 0 : PA_SHADE));
        it_badges(ix, iy, 3, item);
        int hy0 = y < vy ? vy : y, hy1 = y + ch > vy + vh ? vy + vh : y + ch;
        if (hy1 - hy0 > 8) hot(x, hy0, cw, hy1 - hy0, HS_ITEM + (k & 0xFF));
    }
    pa_clip_x0 = sx0; pa_clip_y0 = sy0; pa_clip_x1 = sx1; pa_clip_y1 = sy1;
    if (!it_n) pa_text_ol(vx + 6, vy + 6, 16, T("No items", "Sin objetos"), C_WHITE, C_WOODE, C_DARK);
    scr_bar(&it_sc, rx + rw - 10);   /* the thin scroll indicator on the grid's right edge */
}

/* ---- input (panel_events.inc routes the tab's keys and touches here) */
static void it_warp_go(void) {
    u8 d[81][3]; int n = it_dests(d);
    if (it_wsel < 0 || it_wsel >= n) return;
    it_rq_item = it_warp; it_rq_dest = d[it_wsel][0] << 8 | d[it_wsel][1]; it_rq = RQ_WARP; it_warp = -1;
}
static void items_action(int a) {
    if (it_warp >= 0) {
        if (a == IN_UP) { it_wsel--; it_wfollow = 1; } else if (a == IN_DOWN) { it_wsel++; it_wfollow = 1; }
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
    it_follow = 1;
}
static void items_hit(int id) {
    if ((id & ~0xFF) == HS_ITEM) { int k = id & 0xFF; if (k == it_sel) it_confirm(k); else it_sel = k; return; }
    int k = id - HS_ITOP;
    if (k == 2) it_rq = RQ_SORT;
    else if (k == 3) { if (it_warp >= 0) it_warp_go(); else it_confirm(it_sel); }
    else if (k == 4) it_warp = -1;
    else if (k >= 0x10) { if (it_wsel == k - 0x10) it_warp_go(); else it_wsel = k - 0x10; }
}
void items_action_(int a) { items_action(a); }   /* named from panel_events.inc, which comes first */
void items_hit_(int id) { items_hit(id); }
