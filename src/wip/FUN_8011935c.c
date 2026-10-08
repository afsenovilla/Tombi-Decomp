// FUNC 8011935c 268 X000
#include "TOBJ.H"
extern short DAT_8013867c[];
extern void *PTR_DAT_8013b1f4[];
extern int DAT_1f8002d4;
extern short FUN_8005e420(int, int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_8011935c(TObj *o)
{
    unsigned char s = o->b04;

    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 9;
        o->w08 = FUN_8005e420(0xc0, DAT_8013867c[o->b0c]);
        o->b0d = 1;
        *(signed char *)&o->b0f = -0x14;
        o->b0a = 0;
        o->animFrame = 0;
        o->d3c = DAT_1f8002d4;
        o->anim = PTR_DAT_8013b1f4[o->subtype];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) != 0)
            FUN_8001fec0(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
