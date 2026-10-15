// FUNC 80133bdc 216 X003
// MATCHING 80133bdc 216
#include "gvars.h"
#include "TOBJ.H"
extern void setEventComplete(int, int);

void phoenixFavoriteCompleteScene(TObj *o)
{
    switch (o->state) {
    case 0:
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 0xf;
        break;
    case 2:
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
        o->w08 = 300;
        setEventComplete(0xab, 0);
        o->state = 1;
        break;
    case 0xf:
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
