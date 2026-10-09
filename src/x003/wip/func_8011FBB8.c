// FUNC 8011fbb8 204 X003
/* score 16: register choice in the first two range tests. Game: test 1 keeps o->b in v0 and adds 0x3c after the
   subtraction (this form gets the registers but folds 0x3c into p->d first; the plain form swaps v0/v1); test 2
   puts the difference in v0 (not tied to o->a). Tried int/short temps, constant in a variable, reversed and negated
   operands, > vs >= bounds. */
#include "TOBJ.H"

void func_8011FBB8(TObj *o, TObj *p)
{
    unsigned char v;

    if ((unsigned short)(o->b.p.whole - (p->d->p.whole - 0x3c)) >= 0x79) return;
    if ((unsigned short)(o->a.p.whole - p->h->p.whole) >= 0x5b) return;
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
