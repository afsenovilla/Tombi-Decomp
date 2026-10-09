// FUNC 801196fc 500 X006
// MATCHING 801196fc 500
#include "TOBJ.H"

static __inline__ void calc(TObj *o, short *p)
{
    unsigned short u = (o->d8c - p[3]) & 0xfff;
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
    o->step = 5;
    o->velV = 0x200;
    o->state = 0;
    calc(o, &o->wb4);
    o->velV = -o->velV;
}
