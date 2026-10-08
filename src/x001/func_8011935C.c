// FUNC 8011935c 136 X001
// MATCHING 8011935c 136
#include "TOBJ.H"

extern TObj *ObjAlloc(void);

void func_8011935C(unsigned char sub, int x, int y, int z)
{
    TObj *n = ObjAlloc();

    if (n != 0) {
        n->active = 1;
        n->type = 0x2d;
        n->subtype = sub;
        n->h->raw = x << 16;
        n->y.raw = y << 16;
        n->d->raw = z << 16;
    }
}
