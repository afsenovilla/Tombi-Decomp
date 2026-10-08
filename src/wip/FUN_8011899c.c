// FUNC 8011899c 548 X000
#include "TOBJ.H"
extern unsigned char DAT_8009c966;
extern unsigned char DAT_8009cda6;
extern void FUN_8011880c(TObj *);
extern void FUN_80018ca4(TObj *);

void FUN_8011899c(TObj *o)
{
    switch (o->step) {
    case 0:
        if (DAT_8009c966 == 3 || DAT_8009cda6 == 0xff) {
            o->b04 = 3;
            break;
        }
        o->_pad0e[0] = 1;
        o->b0b = 1;
        o->b0f = 100;
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        o->step++;
    case 1:
        switch (DAT_8009c966) {
        case 0:
            break;
        case 1:
            o->step = 2;
            break;
        case 2:
            o->step = 2;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 2:
        if (o->h->p.whole >= 0x4d9)
            o->h->raw -= 0x18000;
        FUN_8011880c(o);
        o->visible = 1;
        FUN_80018ca4(o);
        switch (DAT_8009c966) {
        case 0:
            break;
        case 2:
            o->step = 3;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        if (o->h->raw >= o->d30) {
            o->h->raw = o->d30;
            o->step = 0;
            o->y.raw = o->d34;
            break;
        }
        switch (DAT_8009c966) {
        case 0:
            break;
        case 1:
            o->step = 2;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        o->h->raw += 0x18000;
        FUN_8011880c(o);
        o->visible = 1;
        FUN_80018ca4(o);
        break;
    }
}
