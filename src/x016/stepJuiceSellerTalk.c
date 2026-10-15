// FUNC 80118898 220 X016
// MATCHING 80118898 220
#include "gvars.h"
#include "TOBJ.H"
extern unsigned char D_8009D12F;
extern void juiceSellerTalk(TObj *);
extern void juiceSellerSqueezeTalk(TObj *);

void stepJuiceSellerTalk(TObj *o)
{
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
}
