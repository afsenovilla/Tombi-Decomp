// FUNC 801191e4 132 X001
// MATCHING 801191e4 132
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

void func_801191E4(int x, int y, int z)
{
    TObj *e = ObjAlloc();
    if (e) {
        e->active = 1;
        e->type = 0x1f;
        e->subtype = 0;
        e->h->raw = x << 16;
        e->y.raw = y << 16;
        e->d->raw = z << 16;
        e->timer = 0xc;
    }
}
