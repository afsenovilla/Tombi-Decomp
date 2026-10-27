// FUNC 80120734 348 X000
// MATCHING 80120734 348
#include "TOBJ.H"
extern unsigned char GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID, DAT_8009c93f, DAT_8009c942, DAT_8009c93e;
extern int l4Type1BDeformModel(TObj *);

void l4Type1BInitModel(TObj *o)
{
    if (o->b0c == 1) {
        switch (o->state) {
        case 0:
            if (GVARS_USE_ITEM_PENDING == 0)
                break;
            if (GVARS_USE_ITEM_ID != 0x33)
                break;
            DAT_8009c93f = 1;
            DAT_8009c942 = 1;
            DAT_8009c93e = 1;
            o->substep = 0;
            o->state++;
            break;
        case 1:
            if (l4Type1BDeformModel(o)) {
                o->timer = 0x78;
                o->state++;
            }
            break;
        case 2:
            if (--o->timer == -1)
                o->state++;
            break;
        case 3:
            GVARS_USE_ITEM_PENDING = 0;
            o->step++;
            ((TObj *)o->d90)->step++;
            break;
        }
    } else if (o->b0c != 0) {
        o->step = ((TObj *)o->d90)->step;
    }
}
