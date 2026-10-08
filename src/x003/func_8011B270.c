// FUNC 8011b270 180 X003
// MATCHING 8011b270 180
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void func_8011B0B8(TObj *);
extern void FUN_80018838(TObj *);

void func_8011B270(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0xd;
        o->box1 = 0x1c;
        o->box2 = 0;
        o->box3 = 0x2e;
        o->b68 = 0;
        break;
    case 1:
        FUN_800202b4(o);
        func_8011B0B8(o);
        o->b68 = 0;
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
