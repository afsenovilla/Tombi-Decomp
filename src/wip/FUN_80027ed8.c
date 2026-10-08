// FUNC 80027ed8 452 MAIN0
#include "TOBJ.H"
extern int DAT_1f80018c;
#define D34(o) (*(Fix16 **)&(o)->d34)
#define W44(o) (*(unsigned short *)&(o)->d)

static __inline__ void body(TObj *o)
{
    int d;
    unsigned char neg;
    d = DAT_1f80018c - D34(o)->raw;
    neg = d < 0;
    if (d > 0x20000) {
        if (d < o->y.raw) {
            if (d < 0x40000)
                o->y.raw = d;
            else
                o->y.raw = 0x40000;
        } else {
            if (o->y.raw < 0)
                o->y.raw = 0;
            o->y.raw += 0x2000;
        }
        D34(o)->raw += o->y.raw;
    } else if (d < -0x20000) {
        if (o->y.raw < d) {
            if (d > -0x40000)
                o->y.raw = d;
            else
                o->y.raw = -0x40000;
        } else {
            if (o->y.raw > 0)
                o->y.raw = 0;
            o->y.raw -= 0x2000;
        }
        D34(o)->raw += o->y.raw;
        if (D34(o)->p.whole < (short)o->animTimer) {
            D34(o)->raw = (short)o->animTimer << 16;
            o->y.raw = 0;
            W44(o) |= 2;
        }
        return;
    } else {
        o->y.raw = d;
        D34(o)->raw += d;
        if (neg) {
            if (D34(o)->p.whole < (short)o->animTimer) {
                D34(o)->raw = (short)o->animTimer << 16;
                o->y.raw = 0;
                W44(o) |= 2;
            }
        }
    }
    if (D34(o)->p.whole > (short)o->animFrame) {
        D34(o)->raw = (short)o->animFrame << 16;
        o->y.raw = 0;
        W44(o) |= 1;
    }
}

void FUN_80027ed8(TObj *o)
{
    body(o);
}
