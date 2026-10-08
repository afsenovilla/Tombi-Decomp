// FUNC 8011ba3c 232 X006
// MATCHING 8011ba3c 232
#include "TOBJ.H"

extern unsigned char D_8007A7F0[];
extern void (*D_8007A890[])(TObj *);
extern void FUN_8003c980(TObj *o);
extern int func_8011EBD0(TObj *o);
extern int AnimAdvance(TObj *o);
extern void FUN_800188e0(TObj *o);

void func_8011BA3C(TObj *o)
{
    switch (o->b04) {
    case 0:
        FUN_8003c980(o);
        o->active = 2;
        o->b04++;
        break;
    case 1:
        if (func_8011EBD0(o) != 0) {
            AnimAdvance(o);
        }
        break;
    case 2:
        D_8007A890[D_8007A7F0[o->subtype]](o);
        break;
    case 3:
        FUN_800188e0(o);
        break;
    }
}
