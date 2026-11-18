/* --launcher-exit (port only; docs/PORT.md "Back to the launcher"): a way out of the game that ends it cleanly, given by
 * the launchers (Android TombiActivity, tools/port_launcher.py).  Without the flag nothing here is used.
 *
 *  - The Start pause menu (drawPauseMenu / updatePauseMenu, port/fixes.txt) gets a 4th entry under PAUSE / EXIT GAME /
 *    LOAD GAME in every language ("VOLVER AL LAUNCHER", "Back to Launcher", "RETOUR AU LAUNCHER"...), with the game's
 *    own Yes / No box ("¿Volver al launcher?", "Return to the launcher?"...: pause menu state 1F8003CD = 4, a port-only
 *    value next to the retail 2 = quit, 3 = load).  The menu frame, a fixed picture of 8x8 tiles, grows 16 px down and
 *    as wide as the text (border tiles moved, interior tiles stretched; the Spanish widening of port/fixes.txt).
 *  - The title carousel gets the same words right of OPTIONS (runtime/main.c port_title_debug_list; the title font has
 *    A-Z), and choosing it opens the same question in a Yes / No box drawn here (the HUD page of the pause frame is not
 *    in VRAM on the title).
 *  - Choosing Yes (port_launcher_quit): nothing is saved; on the PC the process ends with status 0 (port_quit), so the
 *    launcher that started it gets its window back; on Android TombiActivity.quitToLauncher() finishes the game's
 *    activity (its onDestroy sends SDL_QUIT, which ends the :game process with status 0 through port_quit) and
 *    LauncherActivity, under it in the task, shows again.
 *
 * Glyphs.  The pause menu font is a strip of 8x16 glyphs on HUD page 1 (SYSTEM/A{n+1}0003.GAM, v 144) holding only the
 * letters language n's menus use, addressed by the UV table of MAINn.EXE (D_80080300; tools/pause_text.py); no strip
 * has all the letters these words need.  A letter of the language's own strip is drawn by the game as a sprite (its
 * code); any other is drawn here as flat TILE runs (code 0x40 + library index) from a glyph built at run time from the
 * user's disc, in the strip's own palette (AREA00/CLUT01.GAM row 483 at x 352, the CLUT drawPauseMenu uses):
 *   1. the same letter in another language's strip (the nine strips together have A-W, Y, Z and a-x except q);
 *   2. a diacritic: the base letter plus the marks of the Swedish 'ä' over its 'a' (ü);
 *   3. 'W': the 'M' turned upside down within its rows; a small letter: the capital resampled to the x-height of 'x'.
 * Nothing of the game's pixels is in the repository. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "port_runtime.h"

int port_launcher_exit;   /* --launcher-exit */
int port_clean_exit;     /* set once the player chose to leave (android/loader.c: no crash marker after it) */
extern int port_lang;

#ifdef __ANDROID__
#include <jni.h>
#include <SDL2/SDL_system.h>
static int android_quit(void) {
    JNIEnv *env = (JNIEnv *)SDL_AndroidGetJNIEnv(); jobject act = (jobject)SDL_AndroidGetActivity();
    int ok = 0;
    if (!env || !act) return 0;
    jclass cls = (*env)->GetObjectClass(env, act);
    jmethodID mid = (*env)->GetStaticMethodID(env, cls, "quitToLauncher", "()V");
    if (mid) { (*env)->CallStaticVoidMethod(env, cls, mid); ok = 1; }
    if ((*env)->ExceptionCheck(env)) { (*env)->ExceptionClear(env); ok = 0; }
    (*env)->DeleteLocalRef(env, cls); (*env)->DeleteLocalRef(env, act);
    return ok;
}
#endif

void port_launcher_quit(void) {
    static int asked;
    port_clean_exit = 1;
    if (!asked) port_printf("[port] --launcher-exit: back to the launcher\n");
#ifdef __ANDROID__
    /* the activity finishes on the UI thread; SDL_QUIT then reaches this thread's event pump (runtime/sdl.c) */
    if (!asked) { asked = 1; if (android_quit()) return; }
    else return;
#elif defined(__wasm__)
    /* port2/wasm: the native host does what the Android branch above does (host_sdl.c) */
    if (!asked) { asked = 1; if (port_host_launcher_quit()) return; }
    else return;
#endif
    asked = 1;
    port_quit(0);
}

/* ---------------------------------------------------------------- texts (Latin-1) */
/* the letters of each language's strip, code = index + 1 (read off the strips; '\x01' = a glyph not used here) */
static const char *const STRIP[9] = {
    "EGILMNPSY?adeimnopstuvx",                       /* 0 English */
    "ACEFGHIMNOPQRSTUabeghijnortu?",                 /* 1 French */
    "ABDEIJKLMNOPSU\xdc" "abdegijklnprstu?",          /* 2 German */
    "ACEIMNOPRSUacdegi\x01lnoprstv?",                /* 3 Italian */
    "ADEGIJKLMNPRSTUVZabdeilnoprstuvw?",             /* 4 Dutch */
    "ACEGILMNPRSUadegi\xedjlmnoprstuv\xbf?",         /* 5 Spanish */
    "ACEGIJMNPRSUa\xe3" "degimnoprstv?",              /* 6 Portuguese */
    "ABEHJKMNOPRSTUVYa\xe4" "bceijkmnorstuv?y",       /* 7 Swedish */
    "ADEFHIJKL\x01NOPSTU\xc6" "abdefgijklnoprstu",    /* 8 Danish */
};
static const unsigned UVTAB[9] = { 0x70b02, 0x70b02, 0x70b02, 0x70b02, 0x70b02, 0x70aea, 0x70b02, 0x70b02, 0x70b02 };
static const char *const ENTRY[9] = {     /* "quit game"; Swedish writes the small ä in capitals as its HäMTA does */
    "Quit Game", "QUITTER LE JEU", "SPIEL BEENDEN", "CHIUDI GIOCO", "SPEL AFSLUITEN",
    "CERRAR JUEGO", "FECHAR JOGO", "ST\xe4NG SPELET", "LUK SPILLET" };
/* the first entry (PAUSE: it resumes the game) as "continue"; Swedish writes the small ä in capitals as its HäMTA does */
static const char *const CONT[9] = {
    "Continue", "CONTINUER", "WEITER", "CONTINUA", "VERDER", "CONTINUAR", "CONTINUAR", "FORTS\xe4TT", "FORTS\xc6T" };
static const char *const QUEST[9] = {
    "Quit the game?", "Quitter le jeu?", "Spiel beenden?", "Chiudere il gioco?", "Spel afsluiten?",
    "\xbf" "Cerrar el juego?", "Fechar o jogo?", "St\xe4ng spelet?", "Luk spillet?" };
static int lang(void) { return port_lang >= 0 && port_lang < 9 ? port_lang : 0; }

/* the title carousel's label: the entry in capitals (the title font is ASCII) */
const char *port_lx_title_label(void) {
    static char b[40]; const char *s = ENTRY[lang()]; int i;
    for (i = 0; s[i] && i < 39; i++) { unsigned char c = (unsigned char)s[i]; b[i] = c >= 'a' && c <= 'z' ? c - 32 : c == 0xe4 ? 0xc4 : c == 0xf6 ? 0xd6 : c == 0xfc ? 0xdc : c;   /* Ä Ö Ü: port_title_dots */ }
    b[i] = 0;
    return b;
}

/* ---------------------------------------------------------------- the glyph library */
extern unsigned char *pa_read(const char *name, unsigned *n);   /* runtime/panel_assets.c: a disc file, GAM unpacked */
typedef struct { unsigned char px[16][8]; unsigned char ch; } GLY;
static GLY lib[64]; static int nlib;
static unsigned char *page[9], *exe[9]; static unsigned exen[9], pagen[9]; static signed char tried[9];
static unsigned short clut[16]; static int clut_ok = -1;

static int load(int L) {
    if (!tried[L]) {
        char nm[32]; tried[L] = 1;
        snprintf(nm, sizeof nm, "EXE/MAIN%d.EXE", L); exe[L] = pa_read(nm, &exen[L]);
        snprintf(nm, sizeof nm, "SYSTEM/A%d0003.GAM", L + 1); page[L] = pa_read(nm, &pagen[L]);
        if (!exe[L] || !page[L] || pagen[L] < 128 * 256 || exen[L] < UVTAB[L] + 80) {
            port_printf("[port] --launcher-exit: the pause glyphs of language %d are not readable\n", L);
            free(exe[L]); free(page[L]); exe[L] = page[L] = 0;
        }
    }
    return page[L] != 0;
}
static void load_clut(void) {
    unsigned n; unsigned char *d;
    if (clut_ok >= 0) return;
    clut_ok = 0;
    if ((d = pa_read("AREA00/CLUT01.GAM", &n)) && n >= ((3 * 256 + 224) + 16) * 2) {
        for (int i = 0; i < 16; i++) clut[i] = (unsigned short)(d[(3 * 256 + 224 + i) * 2] | d[(3 * 256 + 224 + i) * 2 + 1] << 8);
        clut_ok = 1;
    }
    free(d);
}
/* glyph of code c (>= 1) of language L's strip */
static int from_strip(int L, int code, GLY *g) {
    if (!load(L)) return 0;
    const unsigned char *t = exe[L] + UVTAB[L] + 2 * (code - 1);
    int u = t[0], v = t[1];
    if (v != 0x70 && v != 0x80 && v != 0x90) return 0;
    for (int y = 0; y < 16; y++)
        for (int x = 0; x < 8; x++) {
            int X = (u + x) & 255;
            g->px[y][x] = (page[L][(v + y) * 128 + X / 2] >> (4 * (X & 1))) & 15;
        }
    return 1;
}
static int strip_code(int L, int ch) {
    const char *p = strchr(STRIP[L], ch);
    return ch > 1 && p ? (int)(p - STRIP[L]) + 1 : 0;
}
static int any_strip(int ch, GLY *g) {
    static const int order[9] = { 0, 5, 1, 2, 3, 4, 6, 7, 8 };
    int c = strip_code(lang(), ch);
    if (c && from_strip(lang(), c, g)) return 1;
    for (int i = 0; i < 9; i++) { int L = order[i]; if ((c = strip_code(L, ch)) && from_strip(L, c, g)) return 1; }
    return 0;
}
static void rows(const GLY *g, int *top, int *bot) {
    *top = 16; *bot = -1;
    for (int y = 0; y < 16; y++) for (int x = 0; x < 8; x++) if (g->px[y][x]) { if (y < *top) *top = y; if (y > *bot) *bot = y; }
}
static int build(int ch, GLY *g) {
    GLY a, b;
    memset(g, 0, sizeof *g);
    if (any_strip(ch, g)) return 1;
    if (ch == 0xfc || ch == 0xf6 || ch == 0xeb || ch == 0xef) {        /* ü ö ë ï: base + the marks of ä over a */
        int base = ch == 0xfc ? 'u' : ch == 0xf6 ? 'o' : ch == 0xeb ? 'e' : 'i';
        if (!any_strip(base, g) || !from_strip(7, strip_code(7, 0xe4), &a) || !from_strip(7, strip_code(7, 'a'), &b)) return 0;
        int t, bt; rows(&b, &bt, &t);
        for (int y = 0; y < bt; y++) for (int x = 0; x < 8; x++) if (a.px[y][x] && !b.px[y][x]) g->px[y][x] = a.px[y][x];
        return 1;
    }
    if (ch == 'W' && any_strip('M', &a)) {                              /* M upside down */
        int t, bt; rows(&a, &t, &bt);
        for (int y = t; y <= bt; y++) memcpy(g->px[y], a.px[t + bt - y], 8);
        return 1;
    }
    if (ch >= 'a' && ch <= 'z' && any_strip(ch - 32, &a) && any_strip('x', &b)) {   /* the capital at the x-height */
        int t, bt, xt, xb; rows(&a, &t, &bt); rows(&b, &xt, &xb);
        if (bt > t && xb > xt)
            for (int y = xt; y <= xb; y++) memcpy(g->px[y], a.px[t + (y - xt) * (bt - t) / (xb - xt)], 8);
        return 1;
    }
    return 0;
}
/* library index of Latin-1 letter ch, -1: none */
static int glyph(int ch) {
    for (int i = 0; i < nlib; i++) if (lib[i].ch == ch) return i;
    if (nlib >= 64 || !build(ch, &lib[nlib])) {
        static unsigned char said[256];   /* (once a letter: asked every frame the menu is open, and a log line is a write to the SD card on a 3DS) */
        if (!said[ch & 255]) { said[ch & 255] = 1; port_printf("[port] --launcher-exit: no glyph for %c\n", ch); }
        return -1;
    }
    lib[nlib].ch = (unsigned char)ch;
    return nlib++;
}
/* text -> glyph codes of the current strip (letters it has) or 0x40 + library index; 0xff ended; returns the length */
static int encode(const char *s, unsigned char *out, int all_lib) {
    int n = 0;
    for (; *s && n < 40; s++) {
        int ch = (unsigned char)*s, c = 0;
        if (ch != ' ') {
            if (!all_lib) c = strip_code(lang(), ch);
            if (!c) { int g = glyph(ch); c = g >= 0 ? 0x40 + g : 0; }
        }
        out[n++] = (unsigned char)c;
    }
    out[n] = 0xff;
    return n;
}

/* ---------------------------------------------------------------- drawing: flat TILE runs into the game's OT */
extern char *NEXT_PRIM; extern int CURRENT_OT;
extern void AddPrim(void *, void *);
extern void SetDrawMode(void *, int, int, int, void *);
static void tile(int ot, int x, int y, int w, int h, unsigned rgb, int semi) {
    unsigned char *t = (unsigned char *)NEXT_PRIM;
    t[3] = 3; t[4] = rgb & 255; t[5] = rgb >> 8 & 255; t[6] = rgb >> 16 & 255; t[7] = semi ? 0x62 : 0x60;
    ((short *)t)[4] = (short)x; ((short *)t)[5] = (short)y; ((short *)t)[6] = (short)w; ((short *)t)[7] = (short)h;
    AddPrim((void *)(CURRENT_OT + ot), t);
    NEXT_PRIM += 0x10;
}
static unsigned rgb15(unsigned short v) { return (v & 31) << 3 | ((v >> 5) & 31) << 11 | ((v >> 10) & 31) << 19; }
static void draw_lib(int ot, int k, int x, int y) {
    load_clut();
    if (k < 0 || k >= nlib || !clut_ok) return;
    for (int r = 0; r < 16; r++)
        for (int i = 0; i < 8;) {
            int c = lib[k].px[r][i], j = i + 1;
            while (j < 8 && lib[k].px[r][j] == c) j++;
            if (clut[c]) tile(ot, x + i, y + r, j - i, 1, rgb15(clut[c]), 0);
            i = j;
        }
}
/* drawPauseMenu: a code >= 0x40 of an entry string (port/fixes.txt) */
void port_lx_glyph(int c, int x, int y) { draw_lib(4, c - 0x40, x, y); }

/* ---------------------------------------------------------------- the pause menu (drawPauseMenu, port/fixes.txt) */
typedef struct { short x, y; unsigned char *s; } ENT;
int port_lx_on(void) { return port_launcher_exit; }
int port_lx_dy;                  /* > 0 while drawing the 4-entry pause menu: the frame grows by this many px */
static int fy0, fy1, ent_x0, ent_x1;
static int span(const ENT *e, int n, int *x0, int *x1) {
    *x0 = 0x7fff; *x1 = -0x7fff;
    for (int i = 0; i < n; i++) {
        int k = 0, a = 0x7fff, b = -1;
        for (int j = 0; e[i].s[j] != 0xff && j < 40; j++) if (e[i].s[j]) { if (j < a) a = j; b = j; }
        if (b >= 0) { if (e[i].x + 8 * a < *x0) *x0 = e[i].x + 8 * a; if (e[i].x + 8 * b + 8 > *x1) *x1 = e[i].x + 8 * b + 8; }
        (void)k;
    }
    return *x1 > *x0;
}
/* the Start menu's list (PAUSE / QUIT / LOAD): PAUSE reads "continue", the 4th entry under LOAD, centred on the others */
void *port_lx_pause_list(void *list) {
    static ENT out[8]; static unsigned char txt[44]; ENT *e = (ENT *)list; int n, a, b, len;
    ent_x0 = ent_x1 = 0;
    for (n = 0; n < 6 && e[n].x != -1; n++) out[n] = e[n];
    if (n != 3 || !span(e, n, &a, &b)) return list;
    out[3] = e[3];
    {   /* entry 0 reads "continue", centred where the original was (every platform) */
        static unsigned char ct[44]; int c0, c1, cl = encode(CONT[lang()], ct, 0);
        if (span(e, 1, &c0, &c1)) { out[0].s = ct; out[0].x = (short)((c0 + c1) / 2 - 4 * cl); }
    }
    if (!port_lx_on()) return out;   /* the 4th entry (quit to the launcher): --launcher-exit only */
    len = encode(ENTRY[lang()], txt, 0);
    out[3].s = txt; out[3].y = out[2].y + 16; out[3].x = (short)((a + b) / 2 - 4 * len);
    out[4] = e[3];
    ent_x0 = out[3].x; ent_x1 = out[3].x + 8 * len;
    port_lx_dy = 16;
    return out;
}
/* a highlight bar of the pause / Select menus reaches 6 px past the text of the menu's entries (x0..x1, from port_es_pause) */
void port_lx_bar(short *x, short *w, int x0, int x1) {
    if (ent_x1 > ent_x0) { if (ent_x0 < x0) x0 = ent_x0; if (ent_x1 > x1) x1 = ent_x1; }
    if (x1 <= x0) return;
    int a = *x, b = *x + *w;
    if (x0 - 6 < a) a = x0 - 6;
    if (x1 + 6 > b) b = x1 + 6;
    *x = (short)a; *w = (short)(b - a);
}
/* the Yes / No box of the 4th entry: the language's quit box (its Yes / No line) with our question over it */
static ENT confirm[8]; static unsigned char quest[44]; static int nconfirm;
void *port_lx_confirm_list(void *list) {
    ENT *e = (ENT *)list; int n, k = 0, a, b, len, qy = 0x7fff, ly = -1;
    for (n = 0; n < 6 && e[n].x != -1; n++) { if (e[n].y < qy) qy = e[n].y; if (e[n].y > ly) ly = e[n].y; }
    for (n = 0; n < 6 && e[n].x != -1; n++) if (e[n].y == ly) confirm[1 + k++] = e[n];
    if (!k || !span(confirm + 1, k, &a, &b)) return list;
    len = encode(QUEST[lang()], quest, 0);
    confirm[0].s = quest; confirm[0].y = (short)qy; confirm[0].x = (short)((a + b) / 2 - 4 * len);
    confirm[1 + k] = e[n];
    nconfirm = 1 + k;
    return confirm;
}
/* the frame picture's vertical span (8x8 tiles), before its quads are drawn */
typedef struct { int uv0; int uv1; unsigned short uv2; unsigned char w, h; unsigned short uv3; signed char dx, dy; } PT;
void port_lx_frame(const void *pts, int n, int oy) {
    const PT *pt = (const PT *)pts;
    fy0 = 0x7fff; fy1 = -0x7fff;
    for (; n > 0; n--, pt++) {
        if (oy + pt->dy < fy0) fy0 = oy + pt->dy;
        if (oy + pt->dy + pt->h > fy1) fy1 = oy + pt->dy + pt->h;
    }
    if (fy1 - fy0 <= 16) port_lx_dy = 0;
}
static int vmap(int y) {
    int a = fy0 + 8, b = fy1 - 8;
    if (y <= a) return y;
    if (y >= b) return y + port_lx_dy;
    return a + (y - a) * (b - a + port_lx_dy) / (b - a);
}
void port_lx_vmap(short *y0, short *y1, short *y2, short *y3) {
    *y0 = (short)vmap(*y0); *y1 = (short)vmap(*y1); *y2 = (short)vmap(*y2); *y3 = (short)vmap(*y3);
}

/* ---------------------------------------------------------------- the title's Yes / No box (loopTitleScreen) */
extern unsigned short JOYPAD_STATE;
extern void playSFX(int);
extern ENT D_8008039c[];
extern void *port_lang_item(void *);
#define TOT 0                           /* the title's OT slot 0: drawn last, over the logo (slot 1) */
static int title_open, title_sel;      /* title_sel: 0 Yes, 1 No */
void port_lx_title_open(void) { title_open = 1; title_sel = 1; }
static void title_text(const unsigned char *s, int x, int y) {
    for (; *s != 0xff; s++, x += 8) if (*s >= 0x40) draw_lib(TOT, *s - 0x40, x, y);
}
/* every title frame (after NEXT_PRIM is set): returns 1 while the box is open (it takes the pad) */
int port_lx_title_tick(void) {
    if (!title_open) return 0;
    if (JOYPAD_STATE & 0x80) { if (title_sel) { title_sel = 0; playSFX(8); } }
    else if (JOYPAD_STATE & 0x20) { if (!title_sel) { title_sel = 1; playSFX(8); } }
    else if (JOYPAD_STATE & 0x4008) { if (!title_sel) { port_launcher_quit(); } title_open = 0; return 1; }
    else if (JOYPAD_STATE & 0x2000) { title_open = 0; return 1; }
    return 1;
}
/* loopTitleScreen, after the carousel is drawn: the box, over the logo */
void port_lx_title_draw(void) {
    static unsigned char q[44], yn[2][24]; static int yx[2], ny;
    int len, x0, x1, y0 = 84, y1 = 140, i;
    if (!title_open) return;
    /* the words: the question, and the Yes / No of the language's quit box, all as library glyphs */
    len = encode(QUEST[lang()], q, 1);
    if (!ny) {                         /* the cells of the quit box's last line ("Yes    No"), word by word */
        ENT *e = (ENT *)port_lang_item(D_8008039c); int ly = -1, nst = (int)strlen(STRIP[lang()]);
        for (i = 0; i < 6 && e[i].x != -1; i++) if (e[i].y > ly) ly = e[i].y;
        for (i = 0; i < 6 && e[i].x != -1 && ny < 2; i++) if (e[i].y == ly) {
            const unsigned char *s = e[i].s; int j = 0;
            while (ny < 2) {
                int k = 0;
                while (s[j] == 0) j++;
                if (s[j] == 0xff) break;
                yx[ny] = e[i].x + 8 * j;
                for (; s[j] != 0 && s[j] != 0xff && k < 22; j++) {
                    int g = s[j] <= nst ? glyph((unsigned char)STRIP[lang()][s[j] - 1]) : -1;
                    yn[ny][k++] = (unsigned char)(g >= 0 ? 0x40 + g : 0);
                }
                yn[ny++][k] = 0xff;
            }
        }
    }
    x0 = 160 - 4 * len - 12; x1 = 160 + 4 * len + 12;
    if (x0 > 104) { x0 = 104; x1 = 216; }
    title_text(q, 160 - 4 * len, 96);
    for (i = 0; i < ny; i++) title_text(yn[i], yx[i], 120);
    tile(TOT, 0x74 + 0x30 * title_sel, 0x78, 0x28, 16, 0x00ffff, 1);   /* the highlight, as drawPauseMenu's Yes / No */
    /* the frame: a gold border (2 px) round a light translucent field, as the pause menu's */
    tile(TOT, x0, y0, x1 - x0, 2, 0x58c8e0, 0); tile(TOT, x0, y1 - 2, x1 - x0, 2, 0x58c8e0, 0);
    tile(TOT, x0, y0, 2, y1 - y0, 0x58c8e0, 0); tile(TOT, x1 - 2, y0, 2, y1 - y0, 0x58c8e0, 0);
    tile(TOT, x0 + 2, y0 + 2, x1 - x0 - 4, y1 - y0 - 4, 0x6890a8, 1);
    tile(TOT, x0 + 2, y0 + 2, x1 - x0 - 4, y1 - y0 - 4, 0x5878a0, 1);   /* twice: the logo shows through less */
    { void *r = NEXT_PRIM; SetDrawMode(r, 0, 0, 0, 0); AddPrim((void *)(CURRENT_OT + TOT), r); NEXT_PRIM += 0xc; }
}

/* the title carousel: two dots over glyph c (Ä Ö Ü of a label, runtime/main.c port_title_dots) drawn flat at cell idx
   (titleDrawCarouselGlyph's front window: x = 0xA0 + w * idx - MulNegSin(0xE0, 0x5A), y 0xB8), each a 1-px dot of the
   glyph's brightest colour in a 3x3 of its darkest (its CLUT row 0x1E0 + pal at x 336, its texels on the page at 384, 0) */
typedef struct { unsigned char u, v, w, h, big; } TGLY;
extern TGLY TITLE_FONT[];
extern int MulNegSin(int a, int b);
void port_lx_title_dots(int c, int idx, int pal) {
    extern unsigned short *port_vram(void); extern int GetGraphType(void);
    const unsigned short *vr = port_vram(); const TGLY *g = &TITLE_FONT[c];
    int b8 = GetGraphType() == 1 || GetGraphType() == 2;          /* tpage 0x86 (8 bpp) instead of 0x26 (4 bpp) */
    int used[256] = {0}, x, y, X, gw = g->w, edge = -1, fill = -1, le = 1 << 30, lf = -1;
    for (y = 0; y < g->h; y++) for (x = 0; x < gw; x++) {
        int u = g->u + x, w = vr[(g->v + y) * 1024 + 384 + (b8 ? u / 2 : u / 4)];
        used[b8 ? (w >> (8 * (u & 1))) & 255 : (w >> (4 * (u & 3))) & 15] = 1;
    }
    for (int i = 1; i < 256; i++) if (used[i]) {
        unsigned short v = vr[(0x1E0 + pal) * 1024 + 336 + i];
        int l = (v & 31) * 3 + ((v >> 5) & 31) * 6 + ((v >> 10) & 31);
        if (!v) continue;
        if (l < le) { le = l; edge = v; }
        if (l > lf) { lf = l; fill = v; }
    }
    if (edge < 0 || fill < 0 || edge == fill) return;
    X = 0xA0 + gw * idx - MulNegSin(0xE0, 0x5A);
    for (int d = 0; d < 2; d++) {                                  /* 3x3: a 1-px dot of the fill in the outline */
        int dx = X + (d ? 4 : 1), dy = 0xB8 - 3;
        tile(0, dx + 1, dy + 1, 1, 1, rgb15((unsigned short)fill), 0);
        tile(0, dx, dy, 3, 3, rgb15((unsigned short)edge), 0);
    }
}
