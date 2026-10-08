// FUNC 8010ac90 516 X011
// MATCHING 8010ac90 516
#include "TOBJ.H"
extern void FUN_800eee90(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_8001fec0(TObj *);
extern void FUN_800eec40(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern TObj *DAT_8009c330;
extern unsigned char DAT_8009c93f;
extern unsigned char DAT_8009c942;
extern unsigned char DAT_8009d2b0;
extern unsigned char DAT_8009c975[];
extern unsigned char DAT_8009c93c[];
extern unsigned short DAT_1f8001f8;
extern TObj *DAT_1f8001d4;

void FUN_8010ac90(TObj *o)
{
    char pad;

    switch (o->state) {
    case 0:
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 1;
        o->visible = 1;
        o->ba7 = 0;
        o->active = 4;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        DAT_8009c330->timer = 0;
        o->d8c = 0;
        o->timer = 0x1e;
        o->w22 = 0;
        FUN_800eee90(o);
        DAT_8009c93f = 1;
        DAT_8009c942 = 1;
        DAT_8009d2b0 = 3;
        o->d8c = 0;
        o->state++;
        break;
    case 1:
        if (o->substep != 0 && (DAT_1f8001f8 & 3) == 0) FUN_8001e4f0(0x16);
        FUN_8001fec0(o);
        FUN_800eec40(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        FUN_800eeb5c(o, 4);
        if (--o->timer <= 0) {
            o->timer = 0x3c;
            DAT_8009c975[0] = 3;
            DAT_8009c93c[0] = 0;
            o->state++;
        }
        break;
    case 2:
        FUN_8001fec0(o);
        FUN_800eec40(o);
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        FUN_800eeb5c(o, 4);
        if (o->timer != 0) {
            if (--o->timer <= 0) {
                DAT_1f8001d4->w4c = 7;
                DAT_1f8001d4->w4e = 0;
            }
        }
        break;
    }
}
