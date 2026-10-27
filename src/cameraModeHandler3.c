// FUNC 8002a0fc 348 MAIN0
// MATCHING 8002a0fc 348
// VERSION SLPM_80165 slpm80165 80029F00 300
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002A0FC (func_8002A0FC.c): no +0x44 camera flags.
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
typedef struct {
    unsigned char b0, b1, b2, b3;
    char p4[0x34 - 4];
    unsigned short *p34;
    char p38[0x44 - 0x38];
    unsigned short f44;
} S;
typedef struct { short v; } SV;
extern SV D_1F8000E6;
extern short D_1F8000E6s;
extern unsigned char GAME_AREA00_INTRO_PHASE;
extern void FUN_80027c74(S *o);
extern void func_8002795C(S *o);

void func_8002A0FC(S *o)
{
    short v;
    v = D_1F8000E6.v;
    if (v != 0) {
        if (v > 0) {
            v -= 2;
            D_1F8000E6s = v;
            if (v < 0) D_1F8000E6s = 0;
        } else {
            v += 2;
            D_1F8000E6s = v;
            if (v > 0) D_1F8000E6s = 0;
        }
    }
    FUN_80027c74(o);
    func_8002795C(o);
    if ((unsigned short)(o->p34[1] - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x8c) < 0x118) {
        if ((unsigned short)((*(unsigned short*)((char*)&SPAD.if0 + 2)) - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0xb4) < 0x168) {
            o->b3 = 1;
            if (GAME_AREA00_INTRO_PHASE == 3 || GCD.bcda6 == 0xff) o->b3 = 0;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
        }
    }
}
#else
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
typedef struct {
    unsigned char b0, b1, b2, b3;
    char p4[0x34 - 4];
    unsigned short *p34;
    char p38[0x44 - 0x38];
    unsigned short f44;
} S;
typedef struct { short v; } SV;
extern SV D_1F8000E6;
extern short D_1F8000E6s;
extern unsigned char GAME_AREA00_INTRO_PHASE;
extern void cameraFollowPlayerX(S *o);
extern void cameraFollowPlayerY(S *o);

void cameraModeHandler3(S *o)
{
    short v;
    o->f44 = 0;
    v = D_1F8000E6.v;
    if (v != 0) {
        if (v > 0) {
            v -= 2;
            D_1F8000E6s = v;
            if (v < 0) D_1F8000E6s = 0;
        } else {
            v += 2;
            D_1F8000E6s = v;
            if (v > 0) D_1F8000E6s = 0;
        }
    }
    cameraFollowPlayerX(o);
    cameraFollowPlayerY(o);
    if ((unsigned short)(o->p34[1] - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x8c) < 0x118 || (o->f44 & 3)) {
        if ((unsigned short)((*(unsigned short*)((char*)&SPAD.if0 + 2)) - (*(unsigned short*)((char*)&SPAD.i16c + 2)) + 0xb4) < 0x168 || (o->f44 & 0xc)) {
            o->b3 = 1;
            if (GAME_AREA00_INTRO_PHASE == 3 || GCD.bcda6 == 0xff) o->b3 = 0;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
        }
    }
}
#endif
