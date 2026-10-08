// FUNC 801224c0 612 X000
/* score 62 (b19, was 106): case 0 and tail match (switch on u8 t kept in a2: case 2 is o->b04 = t + 1; tail is a
   static __inline__ fin(o, n) giving move a0,s0). Left: case 1 head: game loads d30/D186/d34 first, sh b, computes a and y,
   stores a then y, then reloads y (lhu 0x16) after lw D_800A4570; m/n not coalesced (andi a0; move a3,a0). Tried: all
   orders of the 7 statements x scalar/[0] for D186/D4570/D4574 (perm search), inline pos(o,a,y) setter, volatile/raw y store. */
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

static __inline__ void fin(TObj *o, int m)
{
    o->visible = 1;
    o->w08 = (o->w08 & 0x803f) | m;
    o->timer--;
    ObjListPush_1F80022C(o);
}

void func_801224C0(TObj *o)
{
    unsigned char t;
    int m, n;
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
        o->y.p.whole = (short)(o->d34 - D_1F800186) >> 1;
        o->a.p.whole = (short)(o->d30 - x) >> 1;
        o->y.p.whole -= (D_800A4570[0] >> 8) << 2;
        o->b.p.whole = 0;
        m = o->w08 & 0x7fc0;
        n = m;
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
