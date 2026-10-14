// FUNC 80135b48 328 X001
// MATCHING 80135b48 328
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"

extern void *printInfoMessage(int, int);
extern void func_80135A14(TObj *);

void func_80135B48(TObj *o)
{
    if (o->b0c != 0) {
        unsigned short *f = &GV2.sc0;
        *f |= 1 << (o->b0c - 1);
    }
    {
        unsigned char *c = &GCD.bcdb5;
        *c = *c + 1;
        if (*c >= 9) *c = 8;
        else printInfoMessage(*c - 1, 2);
    }
    switch (o->b0c) {
    case 1:
        GCD.bcec2 = 1;
        break;
    case 2:
        GCD.bcec1 = 1;
        break;
    case 3:
        GCD.bcec4 = 1;
        break;
    case 4:
        GCD.bcec3 = 1;
        break;
    case 5:
        GCD.bcec5 = 1;
        break;
    case 6:
        GCD.bcec6 = 1;
        break;
    case 7:
        GCD.bcec7 = 1;
        break;
    }
    func_80135A14(o);
}
