// FUNC 8010a168 944 X000
// MATCHING 8010a168 944
// VERSION SLPM_80165 x00 800ECAA0 944
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8010A168 (func_8010A168.c): d-pad bits 0x1000-0x8000 (PAL 0x10-0x80).
#include "gv6.h"
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 PLAYER_SPIN_AIM_ANGLES_A;
extern unsigned char *PLAYER_EXT;

void func_8010A168(TObj *o)
{
    T9 t = PLAYER_SPIN_AIM_ANGLES_A;
    volatile unsigned short *k;
    PLAYER_EXT[0xb] = 8;
    if (o->animFrame & 1) {
        k = &GV6.s70;
        if (*k & 0x1000) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x8000) PLAYER_EXT[0xb] = 7;
            if (*k & 0x2000) PLAYER_EXT[0xb] = 1;
        } else if (*k & 0x8000) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x1000) PLAYER_EXT[0xb] = 7;
            if (*k & 0x4000) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x4000) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x8000) PLAYER_EXT[0xb] = 5;
            if (*k & 0x2000) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x2000) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x1000) PLAYER_EXT[0xb] = 1;
            if (*k & 0x4000) PLAYER_EXT[0xb] = 3;
        }
    } else {
        k = &GV6.s70;
        if (*k & 0x1000) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x8000) PLAYER_EXT[0xb] = 1;
            if (*k & 0x2000) PLAYER_EXT[0xb] = 7;
        } else if (*k & 0x8000) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x1000) PLAYER_EXT[0xb] = 1;
            if (*k & 0x4000) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x4000) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x8000) PLAYER_EXT[0xb] = 3;
            if (*k & 0x2000) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x2000) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x1000) PLAYER_EXT[0xb] = 7;
            if (*k & 0x4000) PLAYER_EXT[0xb] = 5;
        }
    }
    o->d88 = t.v[PLAYER_EXT[0xb]];
}
#else
#include "gv6.h"
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 PLAYER_SPIN_AIM_ANGLES_A;
extern unsigned char *PLAYER_EXT;

void playerSpinAimDir8A(TObj *o)
{
    T9 t = PLAYER_SPIN_AIM_ANGLES_A;
    volatile unsigned short *k;
    PLAYER_EXT[0xb] = 8;
    if (o->animFrame & 1) {
        k = &GV6.s70;
        if (*k & 0x10) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x80) PLAYER_EXT[0xb] = 7;
            if (*k & 0x20) PLAYER_EXT[0xb] = 1;
        } else if (*k & 0x80) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x10) PLAYER_EXT[0xb] = 7;
            if (*k & 0x40) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x40) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x80) PLAYER_EXT[0xb] = 5;
            if (*k & 0x20) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x20) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x10) PLAYER_EXT[0xb] = 1;
            if (*k & 0x40) PLAYER_EXT[0xb] = 3;
        }
    } else {
        k = &GV6.s70;
        if (*k & 0x10) {
            PLAYER_EXT[0xb] = 0;
            if (*k & 0x80) PLAYER_EXT[0xb] = 1;
            if (*k & 0x20) PLAYER_EXT[0xb] = 7;
        } else if (*k & 0x80) {
            PLAYER_EXT[0xb] = 2;
            if (*k & 0x10) PLAYER_EXT[0xb] = 1;
            if (*k & 0x40) PLAYER_EXT[0xb] = 3;
        } else if (*k & 0x40) {
            PLAYER_EXT[0xb] = 4;
            if (*k & 0x80) PLAYER_EXT[0xb] = 3;
            if (*k & 0x20) PLAYER_EXT[0xb] = 5;
        } else if (*k & 0x20) {
            PLAYER_EXT[0xb] = 6;
            if (*k & 0x10) PLAYER_EXT[0xb] = 7;
            if (*k & 0x40) PLAYER_EXT[0xb] = 5;
        }
    }
    o->d88 = t.v[PLAYER_EXT[0xb]];
}
#endif
