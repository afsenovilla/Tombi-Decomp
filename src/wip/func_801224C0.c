// FUNC 801224c0 612 X000
/* score 124: logic ok; game keeps stores in source order (less scheduling) */
#include "TOBJ.H"

extern void *D_8013B208;
extern int D_800A4570;
extern int D_800A4574;
short GetClut(int x, int y);
void ObjListPush_1F80022C(TObj *o);
void FUN_80018934(TObj *o);

void func_801224C0(TObj *o)
{
    short sx;
    int m, n;
    switch (o->b04) {
    case 0:
        o->w1e = 8;
        o->w08 = GetClut(0xc0, 0x1e7);
        o->b0d = 1;
        o->anim = D_8013B208;
        o->b0f = 2;
        o->timer = 0xf;
        o->b04++;
        o->w22 = 0;
        o->step = 0;
        o->d3c = *(int *)0x1F8002D4;
        o->d34 = o->y.p.whole;
        o->d30 = o->h->p.whole;
        break;
    case 1:
        sx = *(short *)0x1F800176;
        if (sx >= 0x35d) break;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(o->d30 - sx) >> 1;
        o->y.p.whole = (short)(o->d34 - *(unsigned short *)0x1F800186) >> 1;
        o->y.p.whole -= (D_800A4570 >> 8) << 2;
        m = o->w08 & 0x7fc0;
        n = m;
        o->a.p.whole -= D_800A4574 >> 10;
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
        o->visible = 1;
        o->w08 = (o->w08 & 0x803f) | n;
        o->timer--;
        ObjListPush_1F80022C(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
