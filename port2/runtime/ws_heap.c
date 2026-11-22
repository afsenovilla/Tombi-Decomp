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

/* one allocated table: the pointer it was stored in, its size and its owner (WS_ROOM / WS_GEN) */
typedef struct { void **slot; size_t size; int owner; } WsEnt;
#define WS_MAXENT 384
static WsEnt entries[WS_MAXENT];
static int entry_count;
static void (*hooks[16])(void);                          /* the modules' reset hooks (ws_on_room) */
static int hook_count;
int ws_heap_failed;
size_t ws_heap_used, ws_heap_peak;
static int poison = -1;

int ws_need(int owner, const WsTab *table_list, int n)
{
    if (poison < 0) {
        const char *env = getenv("PORT_WS_HEAP_POISON");
        poison = env && *env == '1';
    }
    for (int i = 0; i < n; i++) {
        void **slot = (void **)table_list[i].slot;
        if (*slot) {                                     /* there: a WS_GEN table asked for by a WS_ROOM list stays */
            if (owner == WS_ROOM)
                for (int k = 0; k < entry_count; k++)
                    if (entries[k].slot == slot)
                        entries[k].owner = WS_ROOM;
            continue;
        }
        void *p = entry_count < WS_MAXENT ? (poison ? malloc(table_list[i].size) : calloc(1, table_list[i].size)) : 0;
        if (!p) {
            if (!ws_heap_failed) {
#ifndef WS_BACKDROP_MAIN
                port_printf("[ws] heap: out of memory (a table of %u KB, %u KB in use): no widescreen extension here\n",
                            (unsigned)(table_list[i].size >> 10), (unsigned)(ws_heap_used >> 10));
#else
                fprintf(stderr, "ws heap: out of memory\n");
#endif
            }
            ws_heap_failed = 1;
            return 0;
        }
        if (poison) memset(p, 0xA5, table_list[i].size);
        *slot = p;
        entries[entry_count].slot = slot;
        entries[entry_count].size = table_list[i].size;
        entries[entry_count].owner = owner;
        entry_count++;
        ws_heap_used += table_list[i].size;
        if (ws_heap_used > ws_heap_peak) ws_heap_peak = ws_heap_used;
    }
    return 1;
}

/* frees every table of this owner and closes the gaps in the list */
void ws_free(int owner)
{
    int k = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].owner == owner) {
            free(*entries[i].slot);
            *entries[i].slot = 0;
            ws_heap_used -= entries[i].size;
        }
        else entries[k++] = entries[i];
    }
    entry_count = k;
}

void ws_on_room(void (*reset)(void))
{
    for (int i = 0; i < hook_count; i++)
        if (hooks[i] == reset) return;
    if (hook_count < (int)(sizeof hooks / sizeof hooks[0])) hooks[hook_count++] = reset;
}

/* The room's tables are given back a few vblanks late: the ordering table the game built in the last frame of the
   old room is drawn after the vblank that saw the change, and its packets may still point into them (the top-down
   map's widened picture, the model blocks).  Their pointers are cleared at once, so nothing new reaches them. */
#define WS_LATE 4
static struct { void *p; int left; } late[WS_MAXENT];
static int late_count;
static void late_age(void)
{
    int k = 0;
    for (int i = 0; i < late_count; i++) {
        if (--late[i].left <= 0) free(late[i].p);
        else late[k++] = late[i];
    }
    late_count = k;
}
void ws_room_change(void)
{
    int k = 0;
    for (int i = 0; i < hook_count; i++) hooks[i]();
    ws_free(WS_GEN);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].owner != WS_ROOM) {
            entries[k++] = entries[i];
            continue;
        }
        if (late_count < WS_MAXENT) {
            late[late_count].p = *entries[i].slot;
            late[late_count].left = WS_LATE;
            late_count++;
        } else free(*entries[i].slot);
        *entries[i].slot = 0;
        ws_heap_used -= entries[i].size;
    }
    entry_count = k;
    ws_heap_failed = 0;
    ws_heap_peak = ws_heap_used;
}

#ifndef WS_BACKDROP_MAIN
/* each vblank: ages the late frees, and on a change of the GAME area / section frees the room's tables */
void port_ws_heap_vblank(void)
{
    static int last_area = -1, last_sec = -1;
    const unsigned char *game = port_ram_ptr(PORT_A_GAME);
    int area = *(const unsigned short *)(game + 0),
        sec = *(const unsigned short *)(game + 2);
    if (late_count) late_age();
    if (area == last_area && sec == last_sec) return;
    if (ws_heap_peak && port_verbose)
        port_printf("[ws] heap: AREA%02d s%d: peak %u KB of tables; freed\n", last_area, last_sec,
                    (unsigned)(ws_heap_peak >> 10));
    last_area = area;
    last_sec = sec;
    ws_room_change();
}
#endif
