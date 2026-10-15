// FUNC 80117464 96 X009
// MATCHING 80117464 96
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SV;
extern void getBaseMatrix(void *);
extern void RotMatrix(SV *, void *);

void objSetRotMatrixFromAngles(TObj *o)
{
    SV r;
    void *m = &o->w48;

    getBaseMatrix(m);
    r.vx = o->d84;
    r.vy = o->d88;
    r.vz = o->d8c;
    RotMatrix(&r, m);
}
