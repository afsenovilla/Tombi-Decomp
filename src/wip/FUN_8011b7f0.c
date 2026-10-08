// FUNC 8011b7f0 208 X000
#include "TOBJ.H"
extern unsigned char DAT_8009cdac;
extern int DAT_1f8002d4;
extern void *PTR_DAT_8013b168[];
extern void FUN_800202b4(void);
extern void FUN_800187e4(void);

void FUN_8011b7f0(TObj *o)
{
    unsigned char b = o->b04;
    switch (b) {
    case 0:
        if (DAT_8009cdac == 0xff) {
            o->b04 = b + 1;
            o->w1e = 8;
            o->b0d = 0;
            o->d3c = DAT_1f8002d4;
            o->anim = PTR_DAT_8013b168[o->b0c];
        } else {
            o->b04 = 3;
        }
        break;
    case 1:
        FUN_800202b4();
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4();
        break;
    }
}
