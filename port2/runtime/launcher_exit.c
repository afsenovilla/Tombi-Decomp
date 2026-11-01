/* --launcher-exit (port only; docs/PORT.md "Back to the launcher"): a way out of the game that ends it cleanly, given by
 * the launchers (Android TombiActivity, tools/port_launcher.py).  Without the flag nothing here is used.
 *
 *  - The Start pause menu (drawPauseMenu / updatePauseMenu, port/fixes.txt) gets a 4th entry under PAUSE / EXIT GAME /
 *    LOAD GAME: "STOP" (English) / "CERRAR" (--lang 5), with the game's own Yes / No box: "STOP GAME?" /
 *    "¿Cerrar el juego?" (pause menu state 1F8003CD = 4, a port-only value next to the retail 2 = quit, 3 = load).
 *    The words are glyph strings of the language's pause menu strip (tools/pause_text.py), so they are drawn by the
 *    game in its own font; the strip holds only the letters the language's menus use, which is why the words are
 *    STOP / CERRAR (no H for LAUNCHER, no Q in the English strip).  Languages other than English and Spanish keep
 *    their three entries (their strips are not mapped; the title entry below is there for all).  The frame of the
 *    menu, a fixed 3-entry picture of 8x8 tiles, grows 16 px down: border tiles moved, interior tiles stretched.
 *  - The title carousel gets "QUIT" / "CERRAR" right of OPTIONS (runtime/main.c port_title_debug_list).
 *  - Choosing it (port_launcher_quit): nothing is saved; on the PC the process ends with status 0 (port_quit), so
 *    the launcher that started it gets its window back; on Android TombiActivity.quitToLauncher() finishes the
 *    game's activity (its onDestroy sends SDL_QUIT, which ends the :game process with status 0 through port_quit)
 *    and LauncherActivity, under it in the task, shows again. */
#include <string.h>
#include "port_runtime.h"

int port_launcher_exit;   /* --launcher-exit */
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
    if (!asked) port_printf("[port] --launcher-exit: back to the launcher\n");
#ifdef __ANDROID__
    /* the activity finishes on the UI thread; SDL_QUIT then reaches this thread's event pump (runtime/sdl.c) */
    if (!asked) { asked = 1; if (android_quit()) return; }
    else return;
#endif
    asked = 1;
    port_quit(0);
}

/* ---------------------------------------------------------------- the pause menu (drawPauseMenu, port/fixes.txt) */
typedef struct { short x, y; unsigned char *s; } ENT;
typedef struct { int uv0; int uv1; unsigned short uv2; unsigned char w, h; unsigned short uv3; signed char dx, dy; } PT;

/* glyph codes (tools/pause_text.py; English: capitals 01 E 02 G 03 I 04 L 05 M 06 N 07 P 08 S 09 Y 0A ?, small capitals
   0B A 0C D 0D E 0E I 0F M 10 N 11 O 12 P 13 S 14 T 15 U 16 V 17 X) */
static unsigned char en_entry[] = { 0x08, 0x14, 0x11, 0x12, 0xff };                                   /* STOP */
static unsigned char en_quest[] = { 0x08, 0x14, 0x11, 0x12, 0x00, 0x02, 0x0b, 0x0f, 0x0d, 0x0a, 0xff };  /* STOP GAME? */
static unsigned char es_entry[] = { 0x02, 0x03, 0x0a, 0x0a, 0x01, 0x0a, 0xff };                       /* CERRAR */
static unsigned char es_quest[] = { 0x1e, 0x02, 0x0f, 0x19, 0x19, 0x0d, 0x19, 0x00, 0x0f, 0x14, 0x00,
                                    0x13, 0x1c, 0x0f, 0x10, 0x17, 0x1f, 0xff };                       /* ¿Cerrar el juego? */

int port_lx_dy;                  /* > 0 while drawing the 4-entry pause menu: the frame grows by this many px */
static int fy0, fy1;

int port_lx_on(void) { return port_launcher_exit && (port_lang == 0 || port_lang == 5); }

/* the Start menu's list (PAUSE / QUIT / LOAD) with the 4th entry under LOAD */
void *port_lx_pause_list(void *list) {
    static ENT out[8]; ENT *e = (ENT *)list; int n;
    if (!port_lx_on()) return list;
    for (n = 0; n < 6 && e[n].x != -1; n++) out[n] = e[n];
    if (n != 3) return list;
    out[3].y = out[2].y + 16;
    if (port_lang == 5) { out[3].s = es_entry; out[3].x = out[2].x; }   /* CARGAR and CERRAR: 6 glyphs */
    else { out[3].s = en_entry; out[3].x = 160 - 16; }
    out[4] = e[3];
    port_lx_dy = 16;
    return out;
}
/* the Yes / No box of the 4th entry: the language's quit box (its Yes / No line at y 120) with our question */
void *port_lx_confirm_list(void *list) {
    static ENT out[8]; ENT *e = (ENT *)list; int n, k = 0;
    out[k].s = port_lang == 5 ? es_quest : en_quest;
    out[k].x = port_lang == 5 ? 160 - 68 : 120; out[k].y = 96; k++;
    for (n = 0; n < 6 && e[n].x != -1; n++) if (e[n].y != 96) out[k++] = e[n];
    out[k] = e[n];
    return out;
}
/* the frame picture's vertical span (8x8 tiles), before its quads are drawn */
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
