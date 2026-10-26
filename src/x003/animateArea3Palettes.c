// FUNC 80117de4 1184 X003
// MATCHING 80117de4 1184
#include "camera.h"
#include "spad.h"
/* Loops written as macros on ONE function-scope `short i` shared by all four loops: the shared loop variable is
   what keeps i in a1 (it conflicts with k in a0 in cases 2/3) and makes cse extend i rather than a before the loop.
   Covers csv pieces 80117F98/80118040/801180AC. */
#include "gamestate.h"
typedef struct { short x, y, w, h; } RECT;
typedef struct {
    unsigned char p0[3];
    unsigned char count;
    int items[0x58];
    unsigned char b164;
} L;
extern int getScrollOffsetX(void);
extern void FUN_8005f488(RECT *, int, int);
extern void loadArea3ClutRows(L *);

#define FILL() \
    for (i = a; i <= b; i++) { \
        o->items[o->count] = (int)tbl + list[i]; \
        o->count++; \
    }

#define ADDRANGE() \
    for (i = 0; i < 11; i++) { \
        k = base + i; \
        if (k >= n) break; \
        if (k >= 0) { \
            o->items[i] = (int)tbl + list[k]; \
            o->count++; \
        } \
    }

void animateArea3Palettes(L *o, unsigned char *tbl)
{
    unsigned char n = tbl[0];
    int *list = (int *)(tbl + 4);
    short a;
    short b;
    short i, base;
    int k;
    RECT r;

    switch (GAME.selectedSection) {
    case 0:
    case 4:
        if (CAMERA_SCROLL.rotY == 0) {
            a = ((*(short*)((char*)&SPAD.i174 + 2)) + 0x140) / 80;
            b = a + 8;
        } else {
            { int t = getScrollOffsetX() + 0x140; a = ((*(short*)((char*)&SPAD.i174 + 2)) + t) / 80; }
            b = a + 9;
        }
        if (a < 0) a = 0;
        if (b >= n) b = n - 1;
        FILL();
        break;
    case 1:
    case 5:
        if (CAMERA_SCROLL.rotY == 0) {
            a = ((*(short*)((char*)&SPAD.i174 + 2)) - 0xa28) / 80;
            b = a + 8;
        } else {
            { int t = getScrollOffsetX() - 0xa28; a = ((*(short*)((char*)&SPAD.i174 + 2)) + t) / 80; }
            b = a + 9;
        }
        if ((*(short*)((char*)&SPAD.i174 + 2)) >= 0x12c1) a--;
        if (a < 0) a = 0;
        if (b >= n) b = n - 1;
        FILL();
        break;
    case 2:
        base = ((*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX()) / 80;
        ADDRANGE();
        loadArea3ClutRows(o);
        break;
    case 3:
        if ((GAME.purifiedAreas & 2) && o->b164 == 0) {
            r.x = 0xa0;
            r.y = 0x1e0;
            r.w = 0x10;
            r.h = 0x20;
            FUN_8005f488(&r, 0x90, 0x1e0);
            r.x = 0x280;
            r.y = 0;
            r.w = 0x40;
            r.h = 0xe0;
            FUN_8005f488(&r, 0x80, 0x100);
            o->b164++;
        }
        base = ((*(short*)((char*)&SPAD.i174 + 2)) + getScrollOffsetX()) / 80;
        ADDRANGE();
        break;
    }
}
