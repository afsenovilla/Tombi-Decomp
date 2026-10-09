// FUNC 8011fbb8 204 X003
// MATCHING 8011fbb8 204
#include "TOBJ.H"

void func_8011FBB8(TObj *o, TObj *p)
{
    unsigned char v;
    short d;

    d = o->b.p.whole - p->d->p.whole + 0x3c;
    if ((unsigned short)d >= 0x79) return;
    d = o->a.p.whole - p->h->p.whole;
    if ((unsigned short)d >= 0x5b) return;
    if ((unsigned short)(o->y.p.whole - p->y.p.whole + 0x40) >= 0x81) return;
    switch (p->subtype) {
    case 0:
        v = 1;
        break;
    case 4:
        v = 7;
        break;
    case 8:
        v = 0xb;
        break;
    case 0xa:
        v = 2;
        break;
    default:
        return;
    }
    *(unsigned char *)&o->wa8 = v;
}
