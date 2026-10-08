// FUNC 80120734 348 X000
#include "TOBJ.H"
extern unsigned char DAT_8009c940, DAT_8009c941, DAT_8009c93f, DAT_8009c942, DAT_8009c93e;
extern int FUN_80120494(TObj *);

void FUN_80120734(TObj *o)
{
    unsigned char st, u;
    if (o->b0c == 1) {
        st = o->state;
        if (st == 1) {
            if (FUN_80120494(o) == 0)
                return;
            u = o->state;
            o->timer = 0x78;
        } else if (st < 2) {
            if (st != 0)
                return;
            if (DAT_8009c940 == 0)
                return;
            if (DAT_8009c941 != '3')
                return;
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            u = o->state;
            o->substep = 0;
        } else {
            if (st != 2) {
                if (st != 3)
                    return;
                DAT_8009c940 = 0;
                o->step = o->step + 1;
                *(char *)(o->d90 + 5) = *(char *)(o->d90 + 5) + 1;
                return;
            }
            o->timer--;
            if (o->timer != -1)
                return;
            u = o->state;
        }
        o->state = u + 1;

        return;
    }
    if (o->b0c == 0)
        return;
    o->step = *(unsigned char *)(o->d90 + 5);
}
