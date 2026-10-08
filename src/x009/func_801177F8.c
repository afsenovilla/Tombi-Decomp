// FUNC 801177f8 184 X009
// MATCHING 801177f8 184
#include "TOBJ.H"
typedef struct { char p0[8]; int d08; } X;
#define DBC(o) (*(int *)((char *)(o) + 0xbc))

void func_801177F8(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->velH = 0x80;
        o->w74 = -2;
        DBC(o) = 0;
        o->w7a = o->velH;
        o->substep++;
        break;
    case 1: {
        X *x;
        DBC(o) += o->velH;
        o->velH += o->w74;
        x = (X *)((char *)o + 0xb4);
        if (o->velH == o->w7a || o->velH == -o->w7a) o->w74 *= -1;
        o->d8c = x->d08 >> 8;
        break;
    }
    }
}
