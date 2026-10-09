// FUNC 8011ca00 728 X010
// MATCHING 8011ca00 728
#include "TOBJ.H"

extern TObj *D_8009F0EC;
extern int rcos(int);
extern int rsin(int);

#define SETW(o, a)                                                   \
    o->wb8 = rcos(a) * (D_8009F0EC->box0 - 12) >> 12;               \
    o->wba = -(rsin(a) * (D_8009F0EC->box0 - 12)) >> 12

short func_8011CA00(TObj *o)
{
    TObj *p = D_8009F0EC;
    short a = p->d30;
    short b = (a + 0x800) & 0xfff;
    short c = b;
    short r = 0;
    short f;

    switch (p->w7a) {
    case 0:
        if (o->animFrame & 1) {
            a &= 0xfff;
            SETW(o, a);
            if (b < 0x600) {
                o->b69 = 1;
                r = 1;
            }
        } else {
            a = b;
            SETW(o, a);
            if (a >= 0xa01) {
                o->b69 = 1;
                r = 1;
            }
        }
        break;
    case 1:
        a &= 0xfff;
        SETW(o, a);
        break;
    case 2:
        if (p->subtype == 3)
            a = a & 0xfff;
        else
            a = (a + 0x400) & 0xfff;
        SETW(o, a);
        if (D_8009F0EC->subtype == 3) f = c < 0xa01;
        else f = c < 0xe01;
        if (!f) {
            o->b69 = 1;
            r++;
        }
        break;
    }
    D_8009F0EC->b69 = 1;
    return r;
}
