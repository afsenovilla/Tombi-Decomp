// FUNC 800ee838 276 D0SPSUB3
// MATCHING 800ee838 276
#include "TOBJ.H"
extern TObj D_800A6038;
extern short D_1F80019E;
extern unsigned short D_1F800250;
extern TObj **D_1F800260;

int subScreenCondNearType0A(void)
{
    TObj **list = D_1F800260;
    TObj *o;
    TObj *pl = &D_800A6038;
    for (D_1F80019E = D_1F800250; D_1F80019E != 0;) {
        o = *list;
        D_1F80019E--;
        list++;
        if (o->active & 1) {
            if (o->type == 0xA) {
                if (!(o->subtype & 0x80)
                    && (unsigned short)(pl->d->p.whole - o->d->p.whole + 0x2D) < 0x5B
                    && (unsigned short)(pl->h->p.whole - o->h->p.whole + 0x60) < 0xC1
                    && (unsigned short)(pl->y.p.whole - o->y.p.whole + 0x40) < 0x81)
                    return 1;
            }
        }
    }
    return 0;
}
