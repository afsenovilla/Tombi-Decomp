// FUNC 8011a63c 264 X000
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void FUN_8011a418(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern void *PTR_DAT_8013b214[];
extern int DAT_1f8002d4;
void FUN_8011a63c(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w98 = 10;
        o->w1e = 0xc;
        *(signed char *)&o->b0f = -5;
        o->w9a = 0;
        o->b0d = 0;
        o->b0a = 0;
        o->animFrame = 0;
        o->d3c = DAT_1f8002d4;
        o->b04 = o->b04 + 1;
        o->anim = PTR_DAT_8013b214[o->subtype];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0 && o->subtype == 0)
            FUN_8011a418(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
