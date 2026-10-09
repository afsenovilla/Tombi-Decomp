/* score 21: start differs: game loads lw d8c then lhu wba after sh velV, keeps the 0xfff value in v0 with two copies (move v1 / move a0 for the sltiu test); ours: lh wba first and an andi 0xffff for the unsigned short inline param. Tried int/short/ushort/uint for inline params and locals, one or two params, split v computation, unsigned wba reads. */
// FUNC 801196fc 500 X006
#include "TOBJ.H"

static __inline__ void calc(TObj *o, short *p, unsigned short u)
{
    short d = u;
    if (d == 0) {
        o->velV = o->wb6 / 4 + 0x200;
    } else {
        if (u < 0x800) {
            if (d >= 0x200) o->wb6 = o->wb6 / 2;
            else if (d >= 0x100) o->wb6 = o->wb6 - o->wb6 / 4;
            else o->wb6 = o->wb6 - o->wb6 / 6;
            o->velV += p[1] / 2;
        } else {
            if (d <= 0xe00) o->wb6 = o->wb6 - o->wb6 / 4;
            else if (d <= 0xf00) o->wb6 = o->wb6 - o->wb6 / 6;
            else o->wb6 = o->wb6 - o->wb6 / 8;
            o->velV += p[1] / 3;
        }
        if (p[1] < 0) p[1] = 0;
    }
}

void func_801196FC(TObj *o)
{
    int v;
    o->step = 5;
    o->velV = 0x200;
    v = o->d8c - o->wba;
    v &= 0xfff;
    o->state = 0;
    calc(o, &o->wb4, v);
    o->velV = -o->velV;
}
