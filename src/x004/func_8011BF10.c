// FUNC 8011bf10 680 X004
// MATCHING 8011bf10 680
#include "TOBJ.H"

extern unsigned char D_8009D091;
extern short D_800A457E;
extern TObj *FUN_80018448(void);
extern void FUN_80018838(TObj *);
extern int ObjCullRegister(TObj *);

void func_8011BF10(TObj *o)
{
    TObj *e;

    switch (o->b04) {
    case 0:
        if (D_8009D091) {
            if (o->subtype != 1) {
                short *k = &D_800A457E;
                *k += 0x140;
                o->b04 = 3;
                break;
            }
            o->b04 = 3;
            break;
        }
        o->b04++;
        if (o->subtype == 1) {
            o->active = 2;
            break;
        }
        o->box0 = 2;
        o->box1 = 0x20;
        o->box2 = 0x82;
        o->box3 = 0xa0;
        o->da0 = 0;
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype == 1) {
            switch (o->step) {
            case 0:
                if (D_8009D091 == 1) o->step++;
                break;
            case 1:
                if (o->b0c == 1) {
                    o->b04 = 3;
                    break;
                }
                o->step++;
                o->b0a = 0x13;
                o->w74 = 0x1000;
                o->w76 = 0x1000;
                o->w78 = 0x1000;
                o->timer = 0x78;
                break;
            case 2:
                o->w74 -= 0x10;
                o->w76 -= 0x10;
                o->w78 -= 0x10;
                if (--o->timer == -1) o->b04 = 3;
                break;
            }
        }
        break;
    case 2:
        if (o->subtype == 0) {
            e = FUN_80018448();
            if (e) {
                e->active = 1;
                e->type = 0x3c;
                e->subtype = 1;
                e->h->raw = (o->h->p.whole - 8) << 16;
                e->y.raw = (o->y.p.whole + 0x10) << 16;
                e->d->raw = o->d->p.whole << 16;
                e->w22 = 0x8c;
            }
            {
                short *k = &D_800A457E;
                D_8009D091 = 1;
                *k += 0x140;
            }
            o->b04 = 3;
        }
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
