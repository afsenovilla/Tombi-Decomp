// FUNC 800e9dc0 216 X000
// MATCHING 800e9dc0 216
#include "TOBJ.H"
extern unsigned short L8_TYPE00_BOX0_BY_VARIANT[];
extern void cullObjectToDrawList(TObj *, short);
extern void cullObjectToDrawListMargin(TObj *, short);
extern void freeObjectLayer8(TObj *);

void updateL8Type00Object(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = L8_TYPE00_BOX0_BY_VARIANT[o->b0c];
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
