// FUNC 800e9dc0 216 X004
// MATCHING 800e9dc0 216
#include "TOBJ.H"
extern unsigned short D_80114A48[];
extern void cullObjectToDrawList(TObj *, short);
extern void cullObjectToDrawListMargin(TObj *, short);
extern void freeObjectLayer8(TObj *);

void FUN_800e9dc0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = D_80114A48[o->b0c];
        if (o->animFrame != 0) {
            o->d88 = -0x400;
            o->b0a = 0x11;
        }
        break;
    case 1:
        if (o->b0c == 0) cullObjectToDrawListMargin(o, o->box0);
        else cullObjectToDrawList(o, o->box0);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
