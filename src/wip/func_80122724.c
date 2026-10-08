// r9 wip: score 26. Remaining: game copies DAT_1f800176 (lh v0; move v1,v0) and uses srl for (t&0xfff)>>3; unsigned cast makes it worse.
// FUNC 80122724 408 X000
#include "TOBJ.H"
extern void *DAT_8013b20c[];
extern int DAT_1f8002d4[];
extern short DAT_1f800176;
extern unsigned short DAT_1f800186;
extern void FUN_80018da4(TObj *, int);
extern void FUN_80018934(TObj *);

void func_80122724(TObj *o)
{
    int v, t;
    short r;
    char pad[8];
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
        v = DAT_1f800176;
        if (v >= 0x719) {
            if (o->b0c) {
                v = (short)(o->d30 - v) >> 3;
            } else {
                t = o->d30 - v;
                if ((short)t < -0x200)
                    v = (t & 0xfff) >> 3;
                else
                    v = (short)t >> 3;
                if ((short)v >= 0x180)
                    break;
            }
            o->a.p.whole = v;
            o->b.p.whole = 0;
            o->y.p.whole = ((unsigned)(o->d34 - DAT_1f800186) & 0xfff) >> 4;
            FUN_80018da4(o, o->visible = 1);
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
