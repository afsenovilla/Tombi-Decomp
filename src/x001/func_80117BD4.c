// FUNC 80117bd4 264 X001
// MATCHING 80117bd4 264
#include "TOBJ.H"

extern int FUN_800201ac(TObj *o, int x);
extern short func_8011D264(unsigned char n);
extern void FUN_8001888c(TObj *o);

void func_80117BD4(TObj *o)
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
        if (FUN_800201ac(o, 0x50)) {
            a = func_8011D264(o->subtype);
            if (a < 0x800)
                o->d8c = a * 3 / 4;
            else
                o->d8c = 0x1000 - (0x1000 - a) * 3 / 4;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_8001888c(o);
        break;
    }
}
