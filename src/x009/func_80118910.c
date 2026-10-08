// FUNC 80118910 96 X009
// MATCHING 80118910 96
#include "TOBJ.H"
extern TObj *ObjAlloc(void);

void func_80118910(void)
{
    TObj *p = ObjAlloc();

    if (p != 0) {
        p->active = 1;
        p->type = 0x26;
        p->h->raw = 0x7430000;
        p->y.raw = 0xfcc00000;
        p->d->raw = 0xb540000;
        p->subtype = 0;
        p->b0c = 0;
    }
}
