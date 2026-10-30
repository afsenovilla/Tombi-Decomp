// near-twin of sectionClearCameraFollow (PAL)
// VERSION SLPS_01144 x14 801153F4 488
#if defined(VERSION_SLPS_01144)
extern int D_1F8000F0;
extern unsigned short D_8009F200;
extern unsigned char D_8009E4E1, D_8009E4E7;
#define PLAYER D_800A7B78
#define D_800A60D6 D_800A7C16
#define cameraFollowPlayerX func_80028A84
#define cameraFollowPlayerY func_8002876C
#define syncObjectTargetY func_80028608
#define cameraFollowRampStep func_800299F0
#define stepScrollApproachX func_800292E8
#define cameraScrollInputStep func_800296D4
#define FUN_800274e4 func_80028300
#define cameraClampAroundTarget func_80028538
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

void func_801153F4_slps01144(TObj *o)
{
    TObj *p = &PLAYER;
    int t;

    if (D_8009E4E1 == 0) {
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
    if (D_8009E4E7 == 0 && p->b9e == 0 && (*(volatile unsigned short *)&D_8009F200 & 0xc))
        cameraScrollInputStep(o, 1);
    FUN_800274e4(o);
    if ((*(short*)((char*)&D_1F8000F0 + 2)) > (*(short*)((char*)&o->d30 + 2))) D_1F8000F0 = (*(short*)((char*)&o->d30 + 2)) << 16;
    cameraClampAroundTarget(o);
}
#endif
