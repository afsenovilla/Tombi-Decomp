// FUNC 8011b7f0 208 X000
#include "TOBJ.H"
extern unsigned char DAT_8009cdac;
extern int DAT_1f8002d4;
extern void *PTR_DAT_8013b168[];
extern void FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_8011b7f0(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (DAT_8009cdac != 0xff) {
            o->b04 = 3;
        } else {
            o->b04++;
            o->w1e = 8;
            o->b0d = 0;
            o->d3c = DAT_1f8002d4;
            o->anim = PTR_DAT_8013b168[o->b0c];
        }
        break;
    case 1:
        FUN_800202b4(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
