/* ws_heap.c: the widescreen generators' work tables on the heap, only while a room uses them (docs/PORT.md
 * "Widescreen tables on the heap").
 *
 * A table that was `static T name[N][M]` is `static T (*name)[M]` (NULL until needed).  Every entry point of a
 * generator (the gen / take / build functions that the rest of the port calls) first asks for the tables its code
 * touches, as a list:
 *
 *     static const WsTab TW_GEN[] = { WS_T(tw_lab, TW_H), WS_T(tw_rgb, TW_H), ... };
 *     if (!ws_need(WS_GEN, TW_GEN, WS_NT(TW_GEN))) return;      (out of memory: no strips, ws_heap_failed set)
 *
 * Each table not there yet is allocated zeroed (as bss was).  Owners:
 *   WS_GEN   work tables of one generation: freed when the call into the module that made them returns
 *            (ws_free(WS_GEN) at the end of port_ws_backdrop, port_ws_pigs_blocks, ...);
 *   WS_ROOM  tables read again in later frames (the picture taken from VRAM, results): freed when the room (GAME
 *            area / section) changes, after every module's reset hook (ws_on_room) has dropped the caches that
 *            depend on them (strip sums, done flags), so coming back to a room builds again.
 * The same code runs on every platform (PC, Android / wasm2c, 3DS).  tools/ws_heap_check.py checks that every
 * function touching a heap table is reached only from entries that ask for it. */
#ifndef WS_HEAP_H
#define WS_HEAP_H
#include <stddef.h>

enum { WS_ROOM = 1, WS_GEN = 2 };
typedef struct { void *slot; size_t size; } WsTab;      /* slot: the address of the table's pointer */
#define WS_T(p, n) { (void *)&(p), sizeof *(p) * (size_t)(n) }   /* p: the pointer; n: the old first dimension */
#define WS_NT(a) ((int)(sizeof (a) / sizeof (a)[0]))

int ws_need(int owner, const WsTab *t, int n);           /* 1: all there; 0: out of memory (ws_heap_failed = 1) */
void ws_free(int owner);
void ws_on_room(void (*reset)(void));                    /* a module's reset, before the WS_ROOM tables go */
void ws_room_change(void);                               /* resets + frees everything now */
extern int ws_heap_failed;                               /* an ws_need failed since the last ws_room_change */
extern size_t ws_heap_used, ws_heap_peak;                /* bytes now; the most since the last room change */
void port_ws_heap_vblank(void);                          /* shims/psyq.c VSync: the room change */
#endif
