// FUNC 80118ae8 364 X016
// MATCHING 80118ae8 364
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char D_8009D12F;
extern void initJuiceSeller(TObj *);
extern int FUN_800202b4(TObj *);
extern void juiceSellerTalk(TObj *);
extern void juiceSellerSqueezeTalk(TObj *);
extern void freeObjectLayer2(TObj *);

void updateJuiceSeller(TObj *o)
{
    switch (o->b04) {
    case 0:
        initJuiceSeller(o);
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            if (GVARS.b40 != 0) {
                if (GVARS.b41 == 3 && D_8009D12F == 0) {
                    o->step = 2;
                    o->state = 0;
                }
                GVARS.b40 = 0;
            } else if (o->b68 != 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            juiceSellerTalk(o);
            break;
        case 2:
            juiceSellerSqueezeTalk(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
