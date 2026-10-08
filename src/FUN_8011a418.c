// FUNC 8011a418 548 X000
// MATCHING 8011a418 548
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);
extern TObj *FUN_800183b8(void);
extern unsigned short DAT_1f8001f8;
extern unsigned short DAT_1f800172;
extern unsigned short DAT_1f80016a;
extern void *DAT_8013b214[];

void FUN_8011a418(TObj *o)
{
    TObj *n;
    int ok;

    switch (o->step) {
    case 0:
        if (o->w9a == 0 && (DAT_1f8001f8 & 3) == 0) {
            if ((unsigned short)(o->d->p.whole - DAT_1f800172 + 0x2d) >= 0x5b) {
                ok = 0;
            } else {
                ok = (unsigned short)(o->h->p.whole - DAT_1f80016a + 0x60) < 0xc1;
            }
            if (ok) {
                o->timer = 0x29;
                o->step++;
            }
        }
        break;
    case 1:
        FUN_8001fec0(o);
        if (--o->timer == -1) {
            n = FUN_800183b8();
            if (n != 0) {
                n->active = 2;
                n->type = 2;
                n->h->raw = o->h->p.whole << 16;
                n->y.raw = o->y.p.whole << 16;
                n->d->raw = 0x5a0000;
                n->subtype = 5;
                n->b1d = o->b1d;
                n->animFrame = 0;
                o->w9a++;
                n->d94 = (int)&o->w9a;
            }
            o->timer = 2000;
            o->step++;
            o->w98--;
        }
        break;
    case 2:
        if (--o->timer == -1) {
            if (o->w98 == 0) {
                o->step++;
            } else {
                o->step = 0;
            }
            o->anim = DAT_8013b214[o->subtype];
            FUN_8001fe6c(o);
        }
        break;
    }
}
