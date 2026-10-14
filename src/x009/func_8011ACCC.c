// FUNC 8011accc 456 X009
// MATCHING 8011accc 456
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { short b0, b1, b2, b3; } B4;

extern B4 D_8012B1B8[];
extern void cullObjectToDrawList(TObj *, short);
extern void ObjFreeDup(TObj *o);
extern void func_801174C4(TObj *o);
extern void func_8011AE94(TObj *o);

void func_8011ACCC(TObj *o)
{
    short d;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->step++;
            o->box0 = D_8012B1B8[o->subtype].b0;
            o->box1 = D_8012B1B8[o->subtype].b1;
            o->box2 = D_8012B1B8[o->subtype].b2;
            o->box3 = D_8012B1B8[o->subtype].b3;
            break;
        case 1:
            o->b04++;
            o->step = 0;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            *(int *)&o->w5c = o->a.p.whole;
            o->d60 = o->y.p.whole;
            o->d64 = o->b.p.whole;
            func_8011AE94(o);
            break;
        }
        break;
    case 1:
        if (GAME.selectedPlane == 0x20 || GVARS.b3a != 1) {
            func_801174C4(o);
            d = o->box1 - o->box0;
            if (d < o->box0) d = o->box0 + 0x10;
            cullObjectToDrawList(o, d);
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
