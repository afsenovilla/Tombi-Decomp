// FUNC 80134308 252 X000
// MATCHING 80134308 252
// VERSION SLPM_80165 x00 80129BA0 244
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80134308 (FUN_80134308.c):  Inline GetClut.
#include "TOBJ.H"
extern void *DAT_8013b270;
extern void FUN_8001fe6c();
extern void FUN_8001fec0();
extern short FUN_8005e420();

void FUN_80134308(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = DAT_8013b270;
        FUN_8001fe6c(o);
        o->velX = 0x200;
        o->animFrame = 0;
        o->velY = -0x400;
        o->w08 = (0x1f0 << 6) | (0x100 >> 4);
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        {
            short s = o->velY;
            short s2;
            o->y.raw += s << 8;
            s2 = o->velY + 0x40;
            o->velY = s2;
            if (s2 > 0x780)
                o->velY = 0x780;
        }
        if (o->visible == 0) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
    }
}
#else
#include "TOBJ.H"
extern void *DAT_8013b270;
extern void FUN_8001fe6c();
extern void FUN_8001fec0();
extern short FUN_8005e420();

void FUN_80134308(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = DAT_8013b270;
        FUN_8001fe6c(o);
        o->velX = 0x200;
        o->animFrame = 0;
        o->velY = -0x400;
        o->w08 = FUN_8005e420(0x100, 0x1f0);
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velX << 8;
        {
            short s = o->velY;
            short s2;
            o->y.raw += s << 8;
            s2 = o->velY + 0x40;
            o->velY = s2;
            if (s2 > 0x780)
                o->velY = 0x780;
        }
        if (o->visible == 0) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
    }
}
#endif
