// FUNC 80124ad0 1112 X014
/* score 16: case 0 only: with ix = wb4 << 2; ix += (int)D (o34) la now comes first and step is in a0 like the game,
   but base/ix are swapped (ours base v0, ix v1; game ix v0, base v1). -dl: local-alloc gives the short-lived
   REG_EQUIV base qty the higher priority; sched1 places the la right before the addu. Tried e set twice, int b
   temp, char or short pointer ix, *e++ reads, all orders of step++.
   Older: score 23 with e = &D[wb4*2]; o15 brute-forced ~800 variants (temps, struct HV table, inline helpers). */
#include "TOBJ.H"

extern short D_80126718[];
extern unsigned short D_8009C962;
extern short D_1F80027E, D_1F800284;
extern int func_8012491C(TObj *o);
extern int func_801249D0(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);
extern short func_8004065C(TObj *o, short x, short y, int k);

static __inline__ int land(TObj *o)
{
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 10)) {
        o->wae = D_1F800284;
        o->wb2 = D_1F80027E;
        return 1;
    }
    if (func_8004065C(o, o->h->p.whole - 8, o->y.p.whole, 1)) return 0;
    func_8004065C(o, o->h->p.whole + 8, o->y.p.whole, 0);
    return 0;
}

void func_80124AD0(TObj *o)
{
    short *e;
    short v;

    switch (o->step) {
    case 0:
        {
        int ix;
        o->step++;
        ix = *(unsigned short *)&o->wb4 << 2;
        ix += (int)D_80126718;
        e = (short *)ix;
        }
        o->velH = e[0];
        o->velV = e[1];
        if (o->animFrame) o->velH = -o->velH;
        break;
    case 1:
        o->a.raw += o->velH << 8;
        if (func_8012491C(o)) o->velH = 0;
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->step++;
        }
        break;
    case 4:
        v = o->velH;
        if (v < 0) {
            o->velH = v + 0x30;
            if (o->velH >= 0) o->velH = 0;
        } else {
            o->velH = v - 0x30;
            if (o->velH <= 0) o->velH = 0;
        }
    case 2:
        o->a.raw += o->velH << 8;
        if (func_8012491C(o)) o->velH = 0;
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->wb2 = 0;
        o->y.raw += o->velV << 8;
        if (land(o)) {
            if (o->wae == 1 || func_801249D0(o)) {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else if (D_8009C962 != 7) {
                o->step = 3;
                o->velH = 0;
                o->velV = 0;
            } else {
                o->active = 2;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            }
        }
        break;
    case 3:
        o->y.p.whole += 3;
        o->wb2 = 0;
        if (land(o)) {
            if (o->wb2) {
                if (o->wb2 > 0) {
                    o->velH += 0x38;
                    if (o->velH > 0x400) o->velH = 0x400;
                } else {
                    o->velH -= 0x38;
                    if (o->velH < -0x400) o->velH = -0x400;
                }
            }
            o->a.raw += o->velH << 8;
            if (func_8012491C(o)) o->velH = 0;
        } else {
            o->step = 4;
            o->y.p.whole -= 3;
        }
        break;
    }
    if (o->velH < 0) o->d8c += 0x10;
    else o->d8c -= 0x10;
    o->d8c = *(unsigned char *)&o->d8c;
}
