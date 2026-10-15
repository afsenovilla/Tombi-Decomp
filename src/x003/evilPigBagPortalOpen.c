// FUNC 8011d40c 400 X003
// MATCHING 8011d40c 400
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern int D_1F800190;

void evilPigBagPortalOpen(TObj *o)
{
    switch (o->state) {
    case 0:
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        D_800A4553[0] = 3;
        D_800A4568[0] = 0;
        D_1F800190 = o->y.raw;
        SPAD.i18c = o->h->raw;
        o->timer = 60;
        o->state++;
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
        }
        break;
    case 2:
        o->w74 += 0x80;
        if (o->w74 > 0x1000) {
            TObj *p = (TObj *)o->d94;
            o->w74 = 0x1000;
            o->timer = 30;
            o->active = 1;
            o->state++;
            p->b6a = 1;
            p = (TObj *)p->d94;
            p->b6a = 1;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        break;
    case 3:
        if (--o->timer == -1) {
            o->state = 0;
            o->step++;
            D_800A4553[0] = 6;
            GCD.bcf0e = 1;
        }
        break;
    }
}
