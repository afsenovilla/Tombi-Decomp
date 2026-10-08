// FUNC 80116d14 2192 X000
// MATCHING 80116d14 2192
#include "TOBJ.H"
#include "raw7.h"
extern unsigned char D_8009CDA2, D_8009C966, D_8009CDA6, D_8009C93A, D_8009CE0B, D_800A60DA;
extern unsigned char D_8009CDC0, D_8009CFED, D_8009C938, D_8009CDA8, D_8009C93F, D_8009C975;
extern unsigned char D_8009C93C, D_8009D13F, D_8009C93E, D_8009C942;
extern signed char D_8009D2B0;
extern unsigned short D_8009C962, D_8009C982;
extern short D_1F80016A, D_1F80016E, D_1F800172, D_1F8001C6;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern int D_800A609C;
extern Fix16 *D_800A6078;
extern unsigned char D_80138AC0[];
extern void (*D_80138A1C[])(TObj *);
extern void func_8001D6A4(void);
extern int stepScrollReturnY(TObj *);
extern int stepScrollClearX(TObj *);
extern void FUN_80027c74(TObj *);
extern void syncObjectTargetY(TObj *);
extern void FUN_80029078(TObj *);
extern void FUN_8002715c(int);
extern int FUN_800270a0(TObj *, int, int);
extern void setSpawnAreaFlag(void);
extern void setEventComplete(int, int);
extern void FUN_80029cb4(TObj *);
extern void FUN_8002a4d0(TObj *);
extern void MusicStopOrFadeIn(int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, void (*)(void));
extern void startAreaBgm(void);

void FUN_80116d14(TObj *o)
{
    switch (o->step) {
    case 0: {
        unsigned short f = o->animFrame;
        o->animFrame = 1200;
        o->visible = 0;
        U8(o, 0x70) = 0;
        U8(o, 0x71) = 0;
        U8(o, 0x72) = 0;
        U8(o, 0x73) = 0;
        U16(o, 0x42) = f;
        U16(o, 0x40) = o->animTimer;
        o->step++;
    }
    case 1: {
        unsigned char *p = &D_8009CDA2;
        if (*p) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) *p = 0;
            break;
        }
    }
        o->subtype = 1;
        o->step++;
        if (D_8009C966 == 3 || D_8009CDA6 == 0xff) {
            o->subtype = 0;
            o->animFrame = U16(o, 0x42);
        } else if (D_8009C966 == 1) {
            D_8009C966 = 2;
        }
        break;
    case 2:
        if (D_8009C93A) o->step++;
        FUN_80027c74(o);
        U8(o, 0x3c) = 0;
        syncObjectTargetY(o);
        FUN_80029078(o);
        break;
    case 3:
        o->step = D_80138AC0[D_8009C962];
        o->state = 0;
        break;
    case 4:
        D_80138A1C[o->subtype](o);
        {
        unsigned short *q = &D_8009C982;
        switch (*q) {
        case 0:
        case 1:
            if (D_8009CE0B && D_800A60DA == 2 && (unsigned short)(D_1F80016A - 0x400) < 0x18) {
                FUN_8002715c(0x40c);
                if (D_1F800172 >= 0x2e) {
                    if (FUN_800270a0(o, 2, 2)) {
                        D_800A609C = 1;
                        o->step = 8;
                    }
                    break;
                }
            }
            if (D_1F80016A >= 1186 && D_1F80016E >= -60) {
                if (FUN_800270a0(o, 2, 0)) {
                    D_800A609C = 1;
                    o->step = 8;
                    o->state = 0;
                }
                break;
            }
            if (o->subtype != 1 && D_1F80016A >= 1261 && D_1F80016E < -149) {
                D_8009C962 = 1;
                setSpawnAreaFlag();
                o->state = 0;
                o->step++;
            }
            break;
        case 2:
            if (D_8009CDC0 == 1) setEventComplete(28, 0);
            *q = 0;
            break;
        default:
            D_8009C982 = 0;
            break;
        }
        }
        break;
    case 5:
        D_8009C982 = 0;
        D_80138A1C[o->subtype](o);
        if (D_1F80016A < 1260) {
            o->state = 0;
            o->step--;
            D_8009C962 = 0;
            setSpawnAreaFlag();
            break;
        }
        if (D_1F80016A >= 2630 && D_1F80016E >= -290) {
            if (FUN_800270a0(o, 2, 0)) o->step = 8;
            break;
        }
        if (D_1F80016A >= 2525 && D_1F80016E < -399) {
            D_8009C962 = 2;
            o->step++;
            break;
        }
        {
            unsigned char t = D_8009CFED;
            if (t == 1 && D_1F80016A >= 2360 && D_1F80016E >= -100) {
                if (FUN_800270a0(o, 2, 1)) {
                    D_800A609C = t;
                    o->step = 8;
                }
            }
        }
        break;
    case 6:
        FUN_80029cb4(o);
        if (D_1F80016E < -749) {
            D_8009C982 = 2;
            o->step++;
            break;
        }
        if (D_1F80016A < 2524) {
            D_8009C962 = 1;
            setSpawnAreaFlag();
            o->step--;
        }
        break;
    case 7:
        FUN_80029cb4(o);
        if (D_1F80016E < -1369 && D_1F800172 == 450 && D_1F80016A < 2188) {
            if (FUN_800270a0(o, 2, 1)) {
                o->step = 8;
                o->state = 0;
            }
            break;
        }
        if (D_1F80016E >= -749) {
            D_8009C982 = 0;
            o->step--;
        }
        break;
    case 8:
        FUN_8002a4d0(o);
        break;
    case 9:
        D_80138A1C[o->subtype](o);
        if (D_8009C938) break;
        if (D_1F80016A < 120) {
            if (FUN_800270a0(o, 2, 0)) o->step = 8;
            break;
        }
        if ((unsigned short)(D_1F80016A - 240) < 60) {
            if (D_1F80016E < -64) break;
            if (D_8009CDA8) break;
            D_8009C93F = 1;
            MusicStopOrFadeIn(1);
            D_8009C975 = 3;
            D_8009C93C = 0;
            o->step = 11;
            break;
        }
        if (D_1F80016A >= 400) {
            if (FUN_800270a0(o, 2, 1)) o->step = 8;
        }
        break;
    case 10:
        D_80138A1C[o->subtype](o);
        if (D_1F80016A < 41 && D_1F80016E >= -294) {
            if (FUN_800270a0(o, 2, 0)) {
                o->step = 8;
                o->state = 0;
            }
            break;
        }
        if (D_8009D13F) {
            if (D_1F80016A >= 884 && D_1F80016E >= -359) {
                if (FUN_800270a0(o, 2, 1)) o->step = 8;
            }
            break;
        }
        if (D_8009D2B0 != 3 && D_1F80016A >= 883) D_800A6078->p.whole = 883;
        break;
    case 11:
        if (D_8009C975 == 1) {
            D_8009CDA8 = 0xff;
            D_1F8001C6 = 2;
            SoundStopAll();
            o->step++;
            D_1F8001CC = 1;
            D_1F8001CD = 4;
            ThreadCreate(1, func_8001D6A4);
        }
        break;
    case 12:
        if (D_1F8001CC == 0) {
            D_1F8001C6 = 0;
            D_8009C975 = 4;
            startAreaBgm();
            o->step++;
        }
        break;
    case 13:
        if (D_8009C975 == 0) {
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            o->step = 9;
        }
        break;
    }
}
