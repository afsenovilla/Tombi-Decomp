// FUNC 800eefa0 544 I0NFO2
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **D_800FED84[][3];

void func_800EEFA0(TObj *o)
{
    short f;
    switch (o->state) {
    case 0:
        o->animFrame &= 0xE;
        o->velX = (o->animFrame & 2) ? -0x80 : 0x80;
        f = o->animFrame + 4;
        if (f != U16(o, 0xD2)) {
            o->anim = D_800FED84[o->subtype][0][f];
            func_8001FE94(o, 0);
            S16(o, 0xD2) = f;
        }
        o->state++;
    case 1:
        func_8001FEC0(o);
        if (o->b69 - 1 != o->animFrame) o->h->raw += o->velX << 8;
        o->b69 = 0;
        if (o->w78 - o->w74 < (unsigned short)(o->h->p.whole - o->w74)) {
            if (o->h->p.whole < o->w74) o->h->p.whole = o->w74;
            if (o->w78 < o->h->p.whole) o->h->p.whole = o->w78;
            if (o->y.p.whole < o->w76) o->y.p.whole = o->w76;
            if (o->w7a < o->y.p.whole) o->y.p.whole = o->w7a;
            o->animFrame ^= 2;
            f = o->animFrame + 4;
            o->velX = -o->velX;
            if (f != U16(o, 0xD2)) {
                o->anim = D_800FED84[o->subtype][0][f];
                func_8001FE94(o, 0);
                S16(o, 0xD2) = f;
            }
        }
        break;
    }
}
