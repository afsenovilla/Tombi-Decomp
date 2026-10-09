// FUNC 8011aa3c 356 X004
/* score 24: spawn block in subtype 0: game keeps constant 1 in a0 (ours a2) and loads D_1F8002D4/D_80134D18 into v0/v1 after the sb 0xd store, then sb b0a, sw d3c, sw anim; ours hoists the loads (or keeps them after sw d3c with [0]). Tried: unsigned char/int/short one, scalar/[0] globals, temps for both loads at every position, all store orders, chained assignment. */
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern int D_1F8002D4;
extern void *D_80134D18;
extern TObj *ObjAlloc(void);
extern void ObjFree(TObj *);
extern void func_8011EC78(TObj *);
extern void func_8011A840(TObj *);

void func_8011AA3C(TObj *o)
{
    TObj *p;
    int d;
    unsigned char one;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (D_1F8001F8 & 0x3f) break;
            p = ObjAlloc();
            one = 1;
            if (p == 0) break;
            p->type = 0x2a;
            p->animFrame = 1;
            p->w1e = 8;
            p->active = one;
            p->subtype = one;
            d = D_1F8002D4;
            p->d3c = d;
            p->b0c = 0;
            p->b0d = 0x80;
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
        func_8011A840(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
