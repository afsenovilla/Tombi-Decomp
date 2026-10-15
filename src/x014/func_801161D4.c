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
extern void cameraFollowPlayerX(TObj *);
extern void cameraFollowPlayerY(TObj *);
extern void syncObjectTargetY(TObj *);
extern void cameraFollowRampStep(TObj *);
extern void stepScrollApproachX(TObj *);
extern void cameraScrollInputStep(TObj *, int);
extern void FUN_800274e4(TObj *);
extern void cameraClampAroundTarget(TObj *);

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
            cameraFollowPlayerX(o);
            cameraFollowPlayerY(o);
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
            if ((*(unsigned char*)&o->d3c) == 0) cameraFollowPlayerX(o);
            cameraFollowPlayerY(o);
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
                cameraFollowPlayerX(o);
            }
            if ((*(unsigned char*)((char*)&o->d3c + 1)) == 0) cameraFollowPlayerY(o);
            break;
        }
        syncObjectTargetY(o);
    }
    cameraFollowRampStep(o);
    stepScrollApproachX(o);
    if (GVARS.b3f == 0 && p->b9e == 0 && (*(volatile unsigned short *)&GV6.s70 & 0xc00))
        cameraScrollInputStep(o, 1);
    FUN_800274e4(o);
    if ((*(short*)((char*)&SPAD.if0 + 2)) > (*(short*)((char*)&o->d30 + 2))) SPAD.if0 = (*(short*)((char*)&o->d30 + 2)) << 16;
    cameraClampAroundTarget(o);
}
