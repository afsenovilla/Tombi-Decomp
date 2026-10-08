// FUNC 80119468 284 X000
#include "TOBJ.H"
extern int DAT_1f8002d4;
extern char DAT_8013b810[];
extern int FUN_800202b4(TObj *);
extern void FUN_80020490(TObj *);
extern int FUN_8001fec0(TObj *);
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_80119468(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 10;
        o->w08 = FUN_8005e420(0xe0, 0x1e4);
        o->b0d = 1;
        o->b0a = 0;
        o->subtype = 0;
        *(signed char *)&o->b0f = -13;
        o->animFrame = 0;
        o->anim = DAT_8013b810;
        o->d3c = DAT_1f8002d4;
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            FUN_80020490(o);
        if (FUN_8001fec0(o) != 0)
            o->b04++;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
