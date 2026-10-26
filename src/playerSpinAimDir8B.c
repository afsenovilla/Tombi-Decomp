// FUNC 8010a518 1244 X000
// MATCHING 8010a518 1244
// VERSION SLPM_80165 x00 800ECE50 1244
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8010A518 (FUN_8010a518.c): D-pad bits 0x1000-0x8000 (PAL 0x10-0x80).
#include "gv6.h"
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 D_800E98B0, D_800E98C4, D_800E98D8;
extern unsigned char *PLAYER_EXT;

void FUN_8010a518(TObj *o)
{
    T9 t1 = D_800E98B0;
    T9 t2 = D_800E98C4;
    T9 t3 = D_800E98D8;
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
    switch (o->wb2) {
    case 1:
        o->d88 = t1.v[PLAYER_EXT[0xb]];
        break;
    case 2:
        o->d88 = t2.v[PLAYER_EXT[0xb]];
        break;
    case 3:
        o->d88 = t3.v[PLAYER_EXT[0xb]];
        break;
    }
}
#else
#include "gv6.h"
#include "TOBJ.H"
typedef struct { short v[9]; } T9;
extern T9 D_800E98B0, D_800E98C4, D_800E98D8;
extern unsigned char *PLAYER_EXT;

void playerSpinAimDir8B(TObj *o)
{
    T9 t1 = D_800E98B0;
    T9 t2 = D_800E98C4;
    T9 t3 = D_800E98D8;
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
    switch (o->wb2) {
    case 1:
        o->d88 = t1.v[PLAYER_EXT[0xb]];
        break;
    case 2:
        o->d88 = t2.v[PLAYER_EXT[0xb]];
        break;
    case 3:
        o->d88 = t3.v[PLAYER_EXT[0xb]];
        break;
    }
}
#endif
