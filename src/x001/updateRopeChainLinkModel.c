// FUNC 80117bd4 264 X001
// MATCHING 80117bd4 264
#include "TOBJ.H"

extern int cullObjectToDrawListMargin(TObj *o, int x);
extern short getRopeChainLinkAngle(unsigned char n);
extern void freeObjectLayer8(TObj *o);

void updateRopeChainLinkModel(TObj *o)
{
    short a;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0;
        o->box2 = 0;
        o->box1 = 0;
        o->box3 = 0;
        break;
    case 1:
        if (cullObjectToDrawListMargin(o, 0x50)) {
            a = getRopeChainLinkAngle(o->subtype);
            if (a < 0x800)
                o->d8c = a * 3 / 4;
            else
                o->d8c = 0x1000 - (0x1000 - a) * 3 / 4;
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
