// FUNC 80117c50 324 X014
// MATCHING 80117c50 324
#include "TOBJ.H"

extern TObj *ObjAlloc(void);
extern int Rand(void);

void func_80117C50(short sub, short x, short y, short z)
{
    TObj *o;
    int i;
    int a;
    int b;

    for (i = 0; i < 0x18; i++) {
        o = ObjAlloc();
        if (o != 0) {
            o->active = 1;
            o->type = 0x61;
            o->subtype = sub;
            o->b0c = i;
            o->b0a = 0x15;
            o->h->p.whole = x;
            o->d30 = x;
            o->y.p.whole = y;
            o->d34 = y;
            o->d->p.whole = z;
            o->d38 = z;
            if (i < 12) {
                o->animFrame = (i * 30) & 0xff;
            } else {
                o->animFrame = (i * 30 - 345) & 0xff;
            }
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = (unsigned char)o->animFrame << 4;
            o->timer = ((Rand() & 0x1f) >> 1) + 1;
        }
    }
}
