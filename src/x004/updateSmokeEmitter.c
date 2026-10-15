// FUNC 8011aa3c 356 X004
// MATCHING 8011aa3c 356
#include "spad.h"
#include "TOBJ.H"

extern void *D_80134D18;
extern TObj *ObjAlloc(void);
extern void ObjFree(TObj *);
extern void func_8011EC78(TObj *);
extern void smokePuffRise(TObj *);

void updateSmokeEmitter(TObj *o)
{
    TObj *p;
    char one;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (SPAD.s1f8 & 0x3f) break;
            p = ObjAlloc();
            if (p == 0) break;
            one = 1;
            p->active = one;
            p->type = 0x2a;
            p->animFrame = 1;
            p->w1e = 8;
            p->subtype = one;
            p->b0c = 0;
            p->b0d = 0x80;
            p->d3c = ((int*)&SPAD.i2d4)[0];
            p->anim = D_80134D18;
            p->b0a = one;
            p->h->raw = o->h->raw;
            p->y.raw = o->y.raw + 0x100000;
            p->d->raw = o->d->raw;
            break;
        case 1:
            o->b04++;
            o->step = 0;
            break;
        }
        break;
    case 1:
        func_8011EC78(o);
        smokePuffRise(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
