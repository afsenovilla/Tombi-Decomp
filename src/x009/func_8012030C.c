// FUNC 8012030c 192 X009
// MATCHING 8012030c 192
#include "TOBJ.H"

extern unsigned char D_8009CFEE;

void func_8012030C(TObj *o, TObj *e)
{
    if (e->subtype != 0) return;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x3c) >= 0x79) return;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole) >= 0x21) return;
    if ((unsigned short)(o->box2 + (o->y.p.whole - e->y.p.whole + 0x20)) > o->box3 + 0x40) return;
    if (D_8009CFEE != 1) return;
    *(unsigned char *)&o->wa8 = 8;
    *(unsigned char *)&o->da0 = 1;
}
