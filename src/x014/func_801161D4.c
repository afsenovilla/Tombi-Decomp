// FUNC 801161d4 488 X014
// MATCHING 801161d4 488
/* Whole function (csv pieces func_80116258/func_801162B8 start inside it; 0x80116100..0x801161d4 is jump-table data). */
#include "TOBJ.H"
#include "raw7.h"

extern TObj D_800A6038;
extern unsigned char D_8009C939, D_8009C93F, D_800A60D6;
extern unsigned short D_8009D670;
extern short D_1F8000F2;
extern int D_1F8000F0;
extern void FUN_80027c74(TObj *);
extern void func_8002795C(TObj *);
extern void FUN_800277f8(TObj *);
extern void FUN_80028be0(TObj *);
extern void FUN_800284d8(TObj *);
extern void FUN_800288c4(TObj *, int);
extern void FUN_800274e4(TObj *);
extern void FUN_8002771c(TObj *);

void func_801161D4(TObj *o)
{
    TObj *p = &D_800A6038;
    int t;

    if (D_8009C939 == 0) {
        switch (D_800A60D6) {
        case 4:
            if (U8(p, 0xa9) == 0) goto c4;
        case 0:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            FUN_80027c74(o);
            func_8002795C(o);
            U8(o, 0x3d) = 0;
            U8(o, 0x3c) = 0;
            break;
        c4:
            t = p->animFrame & 1;
            if (t != U16(o, 0x58)) {
                U16(o, 0x58) = t;
                U8(o, 0x3d) = 0;
                U8(o, 0x3c) = 0;
            }
            if (U8(o, 0x3c) == 0) FUN_80027c74(o);
            func_8002795C(o);
            U8(o, 0x3d) = 0;
            break;
        case 3:
            t = p->animFrame & 1;
            if (t != U16(o, 0x58)) {
                U16(o, 0x58) = t;
                U8(o, 0x3d) = 0;
            }
            goto f0;
        case 1:
        case 2:
        case 5:
            t = p->animFrame & 1;
            if (t != U16(o, 0x58)) {
                U16(o, 0x58) = t;
                U8(o, 0x3d) = 0;
                U8(o, 0x3c) = 0;
            }
            if (U8(o, 0x3c) == 0) {
            f0:
                FUN_80027c74(o);
            }
            if (U8(o, 0x3d) == 0) func_8002795C(o);
            break;
        }
        FUN_800277f8(o);
    }
    FUN_80028be0(o);
    FUN_800284d8(o);
    if (D_8009C93F == 0 && p->b9e == 0 && (*(volatile unsigned short *)&D_8009D670 & 0xc00))
        FUN_800288c4(o, 1);
    FUN_800274e4(o);
    if (D_1F8000F2 > S16(o, 0x32)) D_1F8000F0 = S16(o, 0x32) << 16;
    FUN_8002771c(o);
}
