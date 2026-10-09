// FUNC 8011aba0 388 X004
// MATCHING 8011aba0 388
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80134D24[];
extern unsigned short D_8009C962;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001f9e0(void);
extern void playSFX(int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800187e4(TObj *);

void func_8011ABA0(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->step++;
            o->w1e = 9;
            o->b0d = 0x80;
            o->d3c = D_1F8002D4[0];
            o->anim = D_80134D24[o->subtype];
            FUN_8001fe6c(o);
            o->timer = FUN_8001f9e0() & 3;
            if ((o->animFrame >> 1) & 1) {
                if (D_8009C962 < 4) {
                    playSFX(0x7d);
                } else {
                    playSFX(0x85);
                }
            }
            break;
        case 1:
            if (--o->timer == -1) {
                o->step = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        if (FUN_800202b4(o)) FUN_8001fec0(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
