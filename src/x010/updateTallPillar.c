// FUNC 80118fe4 216 X010
// MATCHING 80118fe4 216
#include "TOBJ.H"
extern int cullObjectToDrawListMargin(TObj *, int);
extern void ObjFreeDup(TObj *);

void updateTallPillar(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->b0c == 0) {
            o->box0 = 0x10;
            o->box1 = 0x20;
            o->box2 = 0x1b0;
            o->box3 = 0x1b0;
            o->d38 = 0x190;
            o->b68 = 0;
            o->d34 = o->y.p.whole - 0x18;
        } else {
            o->active = 2;
        }
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        break;
    case 1:
        cullObjectToDrawListMargin(o, 0x40);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
