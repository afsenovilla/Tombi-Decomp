/* The widescreen generators' work tables on the heap, only while a room uses them: see ws_heap.h and docs/PORT.md
 * "Widescreen tables on the heap".  (Before, ws_backdrop.c and the other generators kept them in bss: 43 MB, too
 * much for a 3DS title, whose code, data and bss must fit the console's application memory.)
 *
 * PORT_WS_HEAP_POISON=1 (research): new tables filled with 0xA5 instead of zeroes; identical frames then show that no
 * code reads a table before writing it (so a table freed and made again behaves as the static one did). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ws_heap.h"
#ifndef WS_BACKDROP_MAIN
#include "port_runtime.h"
#endif

typedef struct { void **slot; size_t size; int owner; } WsEnt;
#define WS_MAXENT 384
static WsEnt ent[WS_MAXENT]; static int nent;
static void (*hooks[16])(void); static int nhooks;
int ws_heap_failed;
size_t ws_heap_used, ws_heap_peak;
static int poison = -1;

int ws_need(int owner, const WsTab *t, int n)
{
    if (poison < 0) { const char *e = getenv("PORT_WS_HEAP_POISON"); poison = e && *e == '1'; }
    for (int i = 0; i < n; i++) {
        void **slot = (void **)t[i].slot;
        if (*slot) {                                     /* there: a WS_GEN table asked for by a WS_ROOM list stays */
            if (owner == WS_ROOM)
                for (int k = 0; k < nent; k++) if (ent[k].slot == slot) ent[k].owner = WS_ROOM;
            continue;
        }
        void *p = nent < WS_MAXENT ? (poison ? malloc(t[i].size) : calloc(1, t[i].size)) : 0;
        if (!p) {
            if (!ws_heap_failed) {
#ifndef WS_BACKDROP_MAIN
                port_printf("[ws] heap: out of memory (a table of %u KB, %u KB in use): no widescreen extension here\n",
                            (unsigned)(t[i].size >> 10), (unsigned)(ws_heap_used >> 10));
#else
                fprintf(stderr, "ws heap: out of memory\n");
#endif
            }
            ws_heap_failed = 1;
            return 0;
        }
        if (poison) memset(p, 0xA5, t[i].size);
        *slot = p;
        ent[nent].slot = slot; ent[nent].size = t[i].size; ent[nent].owner = owner; nent++;
        ws_heap_used += t[i].size;
        if (ws_heap_used > ws_heap_peak) ws_heap_peak = ws_heap_used;
    }
    return 1;
}

void ws_free(int owner)
{
    int k = 0;
    for (int i = 0; i < nent; i++) {
        if (ent[i].owner == owner) { free(*ent[i].slot); *ent[i].slot = 0; ws_heap_used -= ent[i].size; }
        else ent[k++] = ent[i];
    }
    nent = k;
}

void ws_on_room(void (*reset)(void))
{
    for (int i = 0; i < nhooks; i++) if (hooks[i] == reset) return;
    if (nhooks < (int)(sizeof hooks / sizeof hooks[0])) hooks[nhooks++] = reset;
}

/* The room's tables are given back a few vblanks late: the ordering table the game built in the last frame of the
   old room is drawn after the vblank that saw the change, and its packets may still point into them (the top-down
   map's widened picture, the model blocks).  Their pointers are cleared at once, so nothing new reaches them. */
#define WS_LATE 4
static struct { void *p; int left; } late[WS_MAXENT]; static int nlate;
static void late_age(void)
{
    int k = 0;
    for (int i = 0; i < nlate; i++) {
        if (--late[i].left <= 0) free(late[i].p);
        else late[k++] = late[i];
    }
    nlate = k;
}
void ws_room_change(void)
{
    int k = 0;
    for (int i = 0; i < nhooks; i++) hooks[i]();
    ws_free(WS_GEN);
    for (int i = 0; i < nent; i++) {
        if (ent[i].owner != WS_ROOM) { ent[k++] = ent[i]; continue; }
        if (nlate < WS_MAXENT) { late[nlate].p = *ent[i].slot; late[nlate].left = WS_LATE; nlate++; } else free(*ent[i].slot);
        *ent[i].slot = 0; ws_heap_used -= ent[i].size;
    }
    nent = k;
    ws_heap_failed = 0;
    ws_heap_peak = ws_heap_used;
}

#ifndef WS_BACKDROP_MAIN
void port_ws_heap_vblank(void)
{
    static int last_a = -1, last_s = -1;
    const unsigned char *g = port_ram_ptr(PORT_A_GAME);
    int a = *(const unsigned short *)(g + 0), s = *(const unsigned short *)(g + 2);
    if (nlate) late_age();
    if (a == last_a && s == last_s) return;
    if (ws_heap_peak && port_verbose)
        port_printf("[ws] heap: AREA%02d s%d: peak %u KB of tables; freed\n", last_a, last_s, (unsigned)(ws_heap_peak >> 10));
    last_a = a; last_s = s;
    ws_room_change();
}
#endif
