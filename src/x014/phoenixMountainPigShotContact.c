// FUNC 8011cd6c 248 X014
// MATCHING 8011cd6c 248
// VERSION SLPS_01144 x14 8011A1D8 216
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void playerSetAnim(TObj *, int, int);
void func_8011A1D8(TObj *o, TObj *e)
{
    if (o->active & 2) return;
    if (boxesOverlap(o, e) == 0) return;
    if (*(unsigned char *)&o->wac >= 2) {
        o->active = 2;
        o->animFrame = 1;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 1);
    } else {
        *(unsigned char *)&o->wac = 0;
        e->active = 2;
        e->b6a = 1;
        playerSetAnim(o, 0x10, 0);
        o->active = 6;
        o->b04 = 5;
        o->step = 0x65;
        o->state = 0;
    }
    SPAD.s19e = 0;
}
#else
#include "spad.h"
#define U8(p, o) (*(unsigned char *)((p) + (o)))
#define U16(p, o) (*(unsigned short *)((p) + (o)))
extern short boxesOverlap(unsigned char *, unsigned char *);
extern void decreaseObjectTimer(unsigned char *, int);
extern void playerSetAnim(unsigned char *, int, int);

void phoenixMountainPigShotContact(unsigned char *a, unsigned char *b)
{
    if (SPAD.b1a4 != 0) return;
    if (U8(a, 0xac) == 2) return;
    if (a[0] & 2) return;
    if (boxesOverlap(a, b) == 0) return;
    if (U8(a, 0xac) >= 2) {
        a[0] = 2;
        U16(a, 0x2e) = 1;
        a[4] = 2;
        a[5] = 0;
        a[6] = 0;
        decreaseObjectTimer(a, 1);
    } else {
        U8(a, 0xac) = 0;
        b[0] = 2;
        b[0x6a] = 1;
        playerSetAnim(a, 0x10, 0);
        a[0] = 6;
        a[4] = 5;
        a[5] = 0x65;
        a[6] = 0;
    }
    SPAD.s19e = 0;
}
#endif
