// FUNC 80122724 408 X000
// MATCHING 80122724 408
// volatile b/visible stores keep the game order (matching debt)
#include "TOBJ.H"
extern void *DAT_8013b20c[];
extern int DAT_1f8002d4[];
extern short DAT_1f800176;
extern unsigned short DAT_1f800186;
extern void FUN_80018da4(TObj *, int);
extern void FUN_80018934(TObj *);

void func_80122724(TObj *o)
{
    unsigned short v, t;
    short r;

    switch (o->b04) {
    case 0:
        o->w1e = 0xc;
        o->b0d = 0;
        o->y.p.whole += 0x180;
        o->anim = DAT_8013b20c[o->b0c];
        o->d3c = DAT_1f8002d4[0];
        o->b0f = 0;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        r = DAT_1f800176;
        v = r;
        if (r >= 0x719) {
            if (o->b0c) {
                v = (short)(o->d30 - v) >> 3;
            } else {
                t = o->d30 - v;
                if ((short)t < -0x200)
                    v = ((unsigned)t & 0xfff) >> 3;
                else
                    v = (short)t >> 3;
                if ((short)v >= 0x180)
                    break;
            }
            o->a.p.whole = v;
            o->y.p.whole = ((unsigned)(o->d34 - DAT_1f800186) & 0xfff) >> 4;
            *(volatile short *)&o->b.p.whole = 0;
            { int one = 1; *(volatile unsigned char *)&o->visible = one; FUN_80018da4(o, one); }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
