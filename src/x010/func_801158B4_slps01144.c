// VERSION SLPS_01144 x10 801158B4 1252
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

typedef void (*Fn)(TObj *);
extern Fn D_8007582C[];
extern unsigned char D_800A7B78[];
extern unsigned char D_800A7C1A, D_8009EC20, D_8009EE6B, D_8009E94A, D_8009E4E2, D_8009E4E6;
extern unsigned short GAME_SELECTED_SECTION;
extern Fix16 *D_800A7BB8;
#define S16A (*(short *)0x1F80016A)
#define U16A (*(unsigned short *)0x1F80016A)
#define S16E (*(short *)0x1F80016E)
#define U16E (*(unsigned short *)0x1F80016E)
#define S172 (*(short *)0x1F800172)
extern int stepScrollReturnY(TObj *);
extern int stepScrollClearX(TObj *);
extern void func_80028A84_slps01144(TObj *);
extern void func_8002876C_slps01144(TObj *);
extern void syncObjectTargetY(TObj *);
extern void func_80029E88(TObj *);
extern void stepPlayerXToward(int);
extern int func_80027EA8_slps01144(TObj *, int, int);
extern void func_8002B2D4_slps01144(TObj *);

void func_801158B4_slps01144(TObj *o)
{
    unsigned char *pl = D_800A7B78;
    unsigned char *p;

    switch (o->step) {
    case 0:
        p = &D_8009E94A;
        if (*p) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) *p = 0;
        } else {
            o->subtype = 0;
            o->step++;
        }
        break;
    case 1:
        if (D_8009E4E2) o->step++;
        func_80028A84_slps01144(o);
        func_8002876C_slps01144(o);
        syncObjectTargetY(o);
        func_80029E88(o);
        break;
    case 2:
        D_8007582C[o->subtype](o);
        switch (GAME_SELECTED_SECTION) {
        case 0:
        case 4:
            if (pl[162] == 2) {
                short *q = (short *)((char *)&SPAD.i170 + 2);
                if (*q < 90) break;
                if ((unsigned short)(U16A - 1390) >= 96) break;
                stepPlayerXToward(1438);
                if (*q < 136) break;
                func_80027EA8_slps01144(o, 1, 0);
            } else {
                if (D_8009EC20 < 2) break;
                if (S16A < 3211) break;
                if (S16E < -96) break;
                func_80027EA8_slps01144(o, 1, 1);
            }
            break;
        case 1:
        case 5:
            if (D_800A7C1A == 2) {
                if (S172 >= -70) break;
                if (S16A < 500) {
                    func_80027EA8_slps01144(o, 1, 0);
                } else {
                    if (S16A < 801) break;
                    func_80027EA8_slps01144(o, 1, 1);
                }
            } else if (D_8009EE6B & 0x40) {
                if (S16A < 1159) break;
                if (S16E < -369) break;
                func_80027EA8_slps01144(o, 1, 2);
            } else {
                if (S16A < 1101) break;
                D_800A7BB8->p.whole = 1100;
            }
            break;
        case 2:
        case 6:
            if (S16A >= 129) break;
            if (S16E < -64) break;
            if (func_80027EA8_slps01144(o, 1, 0)) D_8009E4E6 = 1;
            break;
        case 3:
        case 7:
            if ((unsigned short)(U16A - 1343) < 64) {
                if (D_8009EE6B & 0x40) {
                    if ((unsigned short)(U16E + 914) < 30) {
                        func_80027EA8_slps01144(o, 1, 1);
                        break;
                    }
                } else {
                    if ((unsigned short)(U16E + 1076) < 30) {
                        func_80027EA8_slps01144(o, 1, 1);
                        break;
                    }
                }
            }
            if (S16A < 20) {
                if (S16E >= -1039) {
                    func_80027EA8_slps01144(o, 1, 0);
                    break;
                }
            }
            if (S16A < 1986) break;
            if ((unsigned short)(U16E + 1018) < 100) {
                func_80027EA8_slps01144(o, 1, 2);
            } else {
                if (S16A < 1991) break;
                if ((unsigned short)(U16E + 232) < 80) func_80027EA8_slps01144(o, 1, 3);
            }
            break;
        case 8:
            if (S16A >= 58) break;
            if (S16E < -59) break;
            if (D_8009EE6B & 0x40) {
                func_80027EA8_slps01144(o, 1, 1);
            } else {
                func_80027EA8_slps01144(o, 1, 0);
            }
            break;
        }
        break;
    case 3:
        func_8002B2D4_slps01144(o);
        break;
    }
}
#endif
