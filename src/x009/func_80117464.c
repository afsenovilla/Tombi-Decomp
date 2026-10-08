// FUNC 80117464 96 X009
// MATCHING 80117464 96
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SV;
extern void FUN_80021f5c(void *);
extern void RotMatrix(SV *, void *);

void func_80117464(TObj *o)
{
    SV r;
    void *m = &o->w48;

    FUN_80021f5c(m);
    r.vx = o->d84;
    r.vy = o->d88;
    r.vz = o->d8c;
    RotMatrix(&r, m);
}
