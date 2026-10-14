// FUNC 801161d4 488 X014
// MATCHING 801161d4 488
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
/* Whole function (csv pieces func_80116258/func_801162B8 start inside it; 0x80116100..0x801161d4 is jump-table data). */
#include "TOBJ.H"
#include "raw7.h"

extern TObj PLAYER;
extern unsigned char D_800A60D6;
extern void FUN_80027c74(TObj *);
extern void func_8002795C(TObj *);
extern void syncObjectTargetY(TObj *);
extern void FUN_80028be0(TObj *);
extern void stepScrollApproachX(TObj *);
extern void FUN_800288c4(TObj *, int);
extern void FUN_800274e4(TObj *);
extern void FUN_8002771c(TObj *);

void func_801161D4(TObj *o)
{
    TObj *p = &PLAYER;
    int t;

    if (GVARS.b39 == 0) {
        switch (D_800A60D6) {
        case 4:
            if ((*(unsigned char*)((char*)&p->wa8 + 1)) == 0) goto c4;
        case 0:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            FUN_80027c74(o);
            func_8002795C(o);
            (*(unsigned char*)((char*)&o->d3c + 1)) = 0;
            (*(unsigned char*)&o->d3c) = 0;
            break;
        c4:
            t = p->animFrame & 1;
            if (t != o->w58) {
                o->w58 = t;
                (*(unsigned char*)((char*)&o->d3c + 1)) = 0;
                (*(unsigned char*)&o->d3c) = 0;
            }
            if ((*(unsigned char*)&o->d3c) == 0) FUN_80027c74(o);
            func_8002795C(o);
            (*(unsigned char*)((char*)&o->d3c + 1)) = 0;
            break;
        case 3:
            t = p->animFrame & 1;
            if (t != o->w58) {
                o->w58 = t;
                (*(unsigned char*)((char*)&o->d3c + 1)) = 0;
            }
            goto f0;
        case 1:
        case 2:
        case 5:
            t = p->animFrame & 1;
            if (t != o->w58) {
                o->w58 = t;
                (*(unsigned char*)((char*)&o->d3c + 1)) = 0;
                (*(unsigned char*)&o->d3c) = 0;
            }
            if ((*(unsigned char*)&o->d3c) == 0) {
            f0:
                FUN_80027c74(o);
            }
            if ((*(unsigned char*)((char*)&o->d3c + 1)) == 0) func_8002795C(o);
            break;
        }
        syncObjectTargetY(o);
    }
    FUN_80028be0(o);
    stepScrollApproachX(o);
    if (GVARS.b3f == 0 && p->b9e == 0 && (*(volatile unsigned short *)&GV6.s70 & 0xc00))
        FUN_800288c4(o, 1);
    FUN_800274e4(o);
    if ((*(short*)((char*)&SPAD.if0 + 2)) > (*(short*)((char*)&o->d30 + 2))) SPAD.if0 = (*(short*)((char*)&o->d30 + 2)) << 16;
    FUN_8002771c(o);
}
