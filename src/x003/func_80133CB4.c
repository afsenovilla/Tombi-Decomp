// FUNC 80133cb4 308 X003
// MATCHING 80133cb4 308
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char D_8009CE4F;
extern unsigned char D_8009D0E6;
extern void setEventComplete(int, int);

void func_80133CB4(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CE4F == 0xff) break;
        if (D_8009D0E6 < 5) break;
        o->step = 1;
        o->state = 2;
        break;
    case 1:
        switch (o->state) {
        case 1:
            if (--o->w08 <= 0) {
                o->state = 0xf;
            }
            break;
        case 2:
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->w08 = 300;
            setEventComplete(0xab, 0);
            o->state = 1;
            break;
        case 0:
            break;
        case 0xf:
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    }
}
