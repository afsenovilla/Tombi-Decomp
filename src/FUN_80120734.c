// FUNC 80120734 348 X000
// MATCHING 80120734 348
#include "TOBJ.H"
extern unsigned char DAT_8009c940, DAT_8009c941, DAT_8009c93f, DAT_8009c942, DAT_8009c93e;
extern int FUN_80120494(TObj *);

void FUN_80120734(TObj *o)
{
    if (o->b0c == 1) {
        switch (o->state) {
        case 0:
            if (DAT_8009c940 == 0)
                break;
            if (DAT_8009c941 != 0x33)
                break;
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            o->substep = 0;
            o->state++;
            break;
        case 1:
            if (FUN_80120494(o)) {
                o->timer = 0x78;
                o->state++;
            }
            break;
        case 2:
            if (--o->timer == -1)
                o->state++;
            break;
        case 3:
            DAT_8009c940 = 0;
            o->step++;
            ((TObj *)o->d90)->step++;
            break;
        }
    } else if (o->b0c != 0) {
        o->step = ((TObj *)o->d90)->step;
    }
}
