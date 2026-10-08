// FUNC 80119750 292 X001
// MATCHING 80119750 292
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_8007A5F0[], D_8007A3F0[];
extern TObj *ObjAlloc(void);
extern int Rand(void);

int func_80119750(TObj *o)
{
    TObj *n;
    int r, s, i;

    if ((D_1F8001F8 + D_1F800198) & 1) {
        n = ObjAlloc();
        if (n) {
            n->active = 1;
            n->type = 0x31;
            n->subtype = 1;
            n->b0c = 0;
            r = Rand();
            n->h->raw = o->h->raw;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw;
            s = ((r & 1) << 11) + 0x800;
            i = r & 0xf0;
            n->h->raw += (D_8007A5F0[i] * s) >> 4;
            n->y.raw += (D_8007A3F0[i] * s) >> 4;
        }
        return ++o->timer > 0x59;
    }
    return 0;
}
