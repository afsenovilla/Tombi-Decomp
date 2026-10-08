// FUNC 80132698 200 X001
// MATCHING 80132698 200
#include "TOBJ.H"

extern short D_8013C9A4[];
extern TObj *ObjAlloc(void);

void func_80132698(short x, short y, short z)
{
    short *t = D_8013C9A4;
    int i;
    TObj *o;

    for (i = 0; i < 7; i++) {
        o = ObjAlloc();
        if (o != 0) {
            o->active = 2;
            o->type = 0x21;
            o->subtype = i;
            o->h->raw = (x + *t++) << 16;
            o->y.raw = (y + *t++) << 16;
            o->d->raw = z << 16;
        }
    }
}
