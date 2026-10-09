// FUNC 8012194c 512 X014
// MATCHING 8012194c 512
#include "TOBJ.H"
extern TObj *FUN_800184d8(void);

#define SPAWN(o, X, Y, Z) { \
    e = FUN_800184d8(); \
    if (e) { \
        e->active = 1; \
        e->type = 0x43; \
        e->d30 = X; \
        e->d34 = Y; \
        e->subtype = 0; \
        e->d38 = Z; \
        e->h->raw = o->h->raw + e->d30; \
        e->y.raw = o->y.raw + e->d34; \
        e->d->raw = o->d->raw + e->d38; \
        e->d90 = (int)o; \
    } }

void func_8012194C(TObj *o, short n)
{
    TObj *e;

    switch (n) {
    case 0:
        SPAWN(o, -0x350000, -0x190000, -0x5a0000);
        SPAWN(o, -0x30000, 0x440000, -0x5a0000);
        break;
    case 1:
        SPAWN(o, 0x130000, -0x2b0000, -0x5a0000);
        SPAWN(o, -0x120000, 0x2a0000, -0x5a0000);
        break;
    }
}
