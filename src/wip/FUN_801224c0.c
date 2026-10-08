// FUNC 801224c0 612 X000
#include "TOBJ.H"
extern void *PTR_8013b208[];
extern int DAT_1f8002d4[];
extern short DAT_1f800176;
extern unsigned short DAT_1f800186;
extern int DAT_800a4570;
extern int DAT_800a4574;
extern short FUN_8005e420(int, int);
extern void FUN_80018da4(TObj *);
extern void FUN_80018934(TObj *);

void FUN_801224c0(TObj *o)
{
    char pad[8];
    int iv, yv, hv;
    unsigned char st;
    unsigned char t = o->b04;
    int x;
    unsigned short m;
    switch (t) {
    case 0:
        o->w1e = 8;
        o->w08 = FUN_8005e420(0xc0, 0x1e7);
        o->b0d = 1;
        o->anim = PTR_8013b208[0];
        iv = DAT_1f8002d4[0];
        o->b0f = 2;
        o->timer = 0xf;
        o->b04++;
        yv = o->y.p.whole;
        o->w22 = 0;
        o->step = 0;
        o->d3c = iv;
        hv = o->h->p.whole;
        o->d34 = yv;
        o->d30 = hv;
        break;
    case 1:
        x = DAT_1f800176;
        if (x >= 0x35d)
            break;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(o->d30 - x) >> 1;
        o->y.p.whole = (short)(o->d34 - DAT_1f800186) >> 1;
        st = o->step;
        m = o->w08 & 0x7fc0;
        o->y.p.whole -= (DAT_800a4570 >> 8) << 2;
        o->a.p.whole -= DAT_800a4574 >> 10;
        switch (st) {
        case 0:
            if (o->timer != 0)
                goto tail;
            if (o->w22 == 6) {
                m -= 0x40;
                o->w22--;
                o->step++;
            } else {
                m += 0x40;
                o->w22++;
            }
            break;
        case 1:
            if (o->timer != 0)
                goto tail;
            if (o->w22 == 0) {
                m += 0x40;
                o->w22++;
                o->step--;
            } else {
                m -= 0x40;
                o->w22--;
            }
            break;
        default:
            goto tail;
        }
        o->timer = 0xf;
    tail:
        o->visible = 1;
        o->w08 = (o->w08 & 0x803f) | m;
        o->timer--;
        FUN_80018da4(o);
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
