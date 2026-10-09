// FUNC 80128180 548 X009
/* score 4: only the w1e=4 store uses v1 instead of v0. The game hoists li 0x10 (box0/box2) early in v1, which
   needs a multi-set variable (k); with k single-set gcc places the li late and d90 lands in v1 instead of a0.
   Tried: k for each other constant store, goto-tail for w08, inline box setter, chained stores, ternary forms. */
#include "TOBJ.H"

typedef struct { Fix16 x, y, z; } V3F;

extern unsigned char D_8009D2C3, D_8009CDC9, D_8009D12E, D_8009D07E, D_8009D083, D_8009D081;
extern unsigned short D_8009D0B0;
extern int D_1F8002F0[];
extern void *D_8012EE80;
extern void AnimLoadDuration(TObj *o);
extern void FUN_8003e3cc(int, int, V3F *, int, int);

void func_80128180(TObj *o)
{
    V3F v;
    int k;

    if (!(D_8009D2C3 & 1)) {
        if (D_8009CDC9 == 0xff) {
        if (D_8009D12E || D_8009D07E) {
            o->b04 = 3;
            return;
        }
        if (o->subtype == 0) {
            v.x.p.whole = 0xd90;
            v.y.p.whole = -0x244;
            v.z.p.whole = 0xb9a;
            FUN_8003e3cc(0x8a, 0, &v, 0, 0);
        }
        o->b04 = 3;
        return;
        }
        o->w08 = 0x78d2;
    } else {
    if (!D_8009D12E && !D_8009D07E && !o->subtype) {
        v.x.p.whole = 0xd90;
        v.y.p.whole = -0x244;
        v.z.p.whole = 0xb9a;
        FUN_8003e3cc(0x8a, 0, &v, 0, 0);
    }
    if (D_8009D083 == 2) {
    if (D_8009D0B0 || D_8009D081 || o->subtype) {
        o->b04 = 3;
        return;
    }
    v.x.p.whole = 0xbb8;
    v.y.p.whole = -0x276;
    v.z.p.whole = 0xb9a;
    FUN_8003e3cc(0xc, 0, &v, 0, 0);
    o->b04 = 3;
    return;
    }
    o->w08 = 0x7912;
    }
    *(signed char *)&o->b0f = -9;
    o->b0d = 1;
    k = 4;
    o->w1e = k;
    o->b04++;
    o->w7a = o->b6b;
    o->d3c = D_1F8002F0[0];
    o->anim = D_8012EE80;
    o->wac = 0;
    AnimLoadDuration(o);
    k = 0x10;
    o->box1 = 0x20;
    o->box3 = 0x20;
    o->box0 = k;
    o->box2 = k;
    o->ba7 = 0;
    o->b69 = 0;
    o->w9a = 0;
    o->w98 = 3;
    o->b6b = 0;
    o->b6a = 0;
    *(int *)&o->wa8 = o->d90 ? o->d90 : o->d94;
}
