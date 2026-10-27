// FUNC 800ee838 276 D0SPSUB3
// MATCHING 800ee838 276
#include "TOBJ.H"
extern TObj PLAYER;
extern short D_1F80019E;
extern unsigned short CAT2_OBJECT_COUNT_SAVED;
extern TObj **CAT2_OBJECT_LIST_SAVED;

int subScreenCondNearType0A(void)
{
    TObj **list = CAT2_OBJECT_LIST_SAVED;
    TObj *o;
    TObj *pl = &PLAYER;
    for (D_1F80019E = CAT2_OBJECT_COUNT_SAVED; D_1F80019E != 0;) {
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
