// FUNC 801320a8 204 X000
// FLAGS -O1 -G0
#include "TOBJ.H"
extern void FUN_80026bfc(int, int);
extern void FUN_8001fe6c(TObj *);
extern int DAT_8013ad40;
extern int DAT_8013ad44;

void FUN_801320a8(TObj *o)
{
    switch (o->state) {
    case 0: {
        *(short *)((char *)o + 0x6e) = 0x28;
                *(short *)((char *)o + 0x70) = 0;
        *(short *)((char *)o + 0x72) = *(short *)((char *)o + 0x6c) = 0x14;
        *(signed char *)((char *)o + 0xf) = -7;
        *(char *)((char *)o + 0x68) = 0;
        FUN_80026bfc(0, 6 + (*(int *)((char *)o + 0x8c) = 0));
        o->state++;
        break; }
    case 3:
        *(short *)((char *)o + 0xac) = 4;
        o->anim = (void *)DAT_8013ad40;
        FUN_8001fe6c(o);
        break;
    case 2:
    case 4:
    case 7:
        *(short *)((char *)o + 0xac) = 5;
        o->anim = (void *)DAT_8013ad44;
        FUN_8001fe6c(o);
        break;
    }
}
