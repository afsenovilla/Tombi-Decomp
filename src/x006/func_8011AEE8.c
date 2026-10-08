// FUNC 8011aee8 156 X006
// MATCHING 8011aee8 156
#include "TOBJ.H"
extern void FUN_80020078(TObj *, int);
extern void FUN_80018838(TObj *);

void func_8011AEE8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->active = 2;
        o->d84 = 0;
        o->d8c = 0;
        o->d88 = o->animFrame << 8;
        o->b04++;
        break;
    case 1:
        FUN_80020078(o, 0x80);
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
