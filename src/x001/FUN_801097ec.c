// FUNC 801097ec 308 X001
// MATCHING 801097ec 308
#include "TOBJ.H"

extern unsigned short DAT_1f8001f8;
extern volatile unsigned short DAT_8009d670[];
extern unsigned char DAT_801152e8[];
extern void FUN_8001e560(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_800eea3c(TObj *);
extern void FUN_8010f400(TObj *);
extern void FUN_800ee9cc(TObj *);
extern void FUN_800ee88c(TObj *);

void FUN_801097ec(TObj *o)
{
    short s;
    if (DAT_8009d670[0] & 0xa0) {
        if ((DAT_1f8001f8 & 0xf) == 0)
            FUN_8001e560(0x24, 0x24);
        switch (o->state) {
        case 0:
            s = 0x200;
            if (o->animFrame & 1)
                s = -0x200;
            o->wb2 = s;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            FUN_800eeb5c(o, 0x20);
            ((unsigned char *)o)[0xc3] = 1;
            o->state++;
        case 1:
            FUN_8001fec0(o);
            FUN_800eea3c(o);
            FUN_8010f400(o);
            FUN_800ee9cc(o);
            FUN_800ee88c(o);
            if (o->ba6 == 0) {
                ((unsigned char *)o)[0xc3] = 0;
                o->step = 1;
                o->state = 0;
            }
        }
    } else {
        unsigned char b = DAT_801152e8[o->wb0];
        ((unsigned char *)o)[0xc3] = 0;
        o->wb2 = 0;
        o->step = 0;
        o->state = 0;
        o->d8c = b;
    }
}
