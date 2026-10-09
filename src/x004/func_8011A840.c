// FUNC 8011a840 508 X004
// MATCHING 8011a840 508
#include "TOBJ.H"
extern char D_80077CDC[];
extern char D_80077CF4[];
extern void *D_80134D1C[];
extern void *D_80134D20[];
extern void FUN_8001fa88(TObj *, unsigned short);

void func_8011A840(TObj *o)
{
    char pad;

    switch (o->step) {
    case 0:
        o->movetab = D_80077CDC;
        o->velV = 0x100;
        o->velY = 4;
        o->d64 = 0x1000;
        o->timer = 0x20;
        o->step++;
    case 1:
        FUN_8001fa88(o, o->animFrame);
        o->y.raw -= o->velV << 8;
        o->d64 += 0x20;
        if (--o->timer == -1) o->step++;
        break;
    case 2:
        o->step++;
        o->anim = D_80134D1C[0];
        o->d64 = 0x1000;
        o->timer = 0x40;
    case 3:
        FUN_8001fa88(o, o->animFrame);
        o->y.raw -= o->velV << 8;
        if (o->velV > 0x4000) o->velV -= o->velY;
        o->d64 += 0x20;
        if (--o->timer == -1) o->step++;
        break;
    case 4:
        o->movetab = D_80077CF4;
        o->step++;
        o->anim = D_80134D20[0];
        o->d64 = 0x1000;
        o->timer = 0xa0;
    case 5:
        FUN_8001fa88(o, o->animFrame);
        o->y.raw -= 0x4000;
        o->d64 += 0x20;
        if (--o->timer == -1) o->b04 = 3;
        break;
    }
}
