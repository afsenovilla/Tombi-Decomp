// FUNC 8011eec0 228 X004
// MATCHING 8011eec0 228
#include "TOBJ.H"

void func_8011EEC0(TObj *o, TObj *e)
{
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x3c) >= 0x79) return;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole) >= 0x41) return;
    if ((unsigned short)(o->box2 + (o->y.p.whole - e->y.p.whole + 0x20)) > o->box3 + 0x40) return;
    switch (e->subtype) {
    case 0:
        *(unsigned char *)&o->wa8 = 1;
        break;
    case 4:
        *(unsigned char *)&o->wa8 = 5;
        break;
    case 8:
        *(unsigned char *)&o->wa8 = 6;
        break;
    case 10:
        *(unsigned char *)&o->wa8 = 2;
        break;
    }
}
