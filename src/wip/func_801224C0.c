// FUNC 801224c0 612 X000
/* score 16 (b36, was 62): short n + fin(o, short m) + `short k = o->w08 & 0x803f; o->w08 = k | m;` fixed m/n copies
   and the tail; locals pa/py for the a/y header. Left: case-1 header schedule: game loads d30, D186, d34, stores b=0 first,
   and loads o->step (lbu a1) late after lhu w08, so x sits in a1; ours hoists lbu step to the top (x in a0).
   Tried: all dependency-valid orders of the 8 header statements (+ st = o->step local), raw-offset stores, volatile step (81). */
#include "TOBJ.H"

extern void *D_8013B208[];
extern int D_1F8002D4[];
extern short D_1F800176[];
extern unsigned short D_1F800186;
extern int D_800A4570[];
extern int D_800A4574[];
short GetClut(int x, int y);
void ObjListPush_1F80022C(TObj *o);
void FUN_80018934(TObj *o);

static __inline__ void fin(TObj *o, short m)
{
    o->visible = 1;
    { short k = o->w08 & 0x803f; o->w08 = k | m; }
    o->timer--;
    ObjListPush_1F80022C(o);
}

void func_801224C0(TObj *o)
{
    unsigned char t;
    int m;
    short n;
    short pa, py;
    short x;
    t = o->b04;
    switch (t) {
    case 0:
        o->w1e = 8;
        o->w08 = GetClut(0xc0, 0x1e7);
        o->b0d = 1;
        o->anim = D_8013B208[0];
        o->d3c = D_1F8002D4[0];
        o->w22 = 0;
        o->step = 0;
        o->b0f = 2;
        o->b04++;
        o->timer = 0xf;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        break;
    case 1:
        x = D_1F800176[0];
        if (x >= 0x35d) break;
        pa = (short)(o->d30 - x) >> 1;
        py = (short)(o->d34 - D_1F800186) >> 1;
        o->y.p.whole = py;
        o->a.p.whole = pa;
        m = o->w08 & 0x7fc0;
        n = m;
        o->y.p.whole -= (D_800A4570[0] >> 8) << 2;
        o->b.p.whole = 0;
        o->a.p.whole -= D_800A4574[0] >> 10;
        switch (o->step) {
        case 0:
            if (o->timer == 0) {
                if (o->w22 == 6) {
                    n = m - 0x40;
                    o->w22--;
                    o->step++;
                } else {
                    n = m + 0x40;
                    o->w22++;
                }
                o->timer = 0xf;
            }
            break;
        case 1:
            if (o->timer == 0) {
                if (o->w22 == 0) {
                    n = m + 0x40;
                    o->w22++;
                    o->step--;
                } else {
                    n = m - 0x40;
                    o->w22--;
                }
                o->timer = 0xf;
            }
            break;
        }
        fin(o, n);
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
