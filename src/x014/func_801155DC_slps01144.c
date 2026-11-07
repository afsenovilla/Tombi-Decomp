// VERSION SLPS_01144 x14 801155DC 1228
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
#include "raw7.h"

typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } PL;
typedef void (*Fn)(TObj *);

extern PL D_800A7B78;
extern Fn D_80122964[];
extern short D_80122980[];
extern unsigned char D_8012295C[];
extern unsigned char D_8009E94A, D_8009E4E2, D_8009E4E7, D_8009E4E8, D_8009E4E9, D_8009E51D, D_8009E4E4, D_8009EE6B;

extern unsigned short D_8009E50A, D_8009E52A, D_8009E93C, D_8009E93E, D_8009E948;
extern void D_8001D5E0(void);
extern unsigned int stepScrollReturnY(TObj *);
extern unsigned int stepScrollClearX(TObj *);
extern void func_80028A84_slps01144(TObj *);
extern void syncObjectTargetY(TObj *);
extern void stepScrollApproachX(TObj *);
extern void setEventComplete(int, int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, void (*)(void));
extern int Rand(void);
extern TObj *allocObjectLayer2(void);

void func_801155DC_slps01144(TObj *o)
{
    PL *pl = &D_800A7B78;
    TObj *q;

    switch (o->step) {
    case 0: {
        unsigned char *f = &D_8009E94A;
        if (*f != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        if (D_8009E50A == 7) {
            (*(unsigned char*)((char*)&o->box0 + 1)) = 1;
            (*(unsigned char*)&o->box1) = 2;
            (*(unsigned char*)((char*)&o->box1 + 1)) = 10;
        }
        break;
    }
    case 1:
        if (D_8009E4E2 != 0) o->step++;
        func_80028A84_slps01144(o);
        syncObjectTargetY(o);
        stepScrollApproachX(o);
        break;
    case 2:
        D_80122964[o->subtype](o);
        D_8009E52A = 0;
        break;
    case 4:
        {
        int idx = D_8009E50A;
        asm volatile("" : "=r"(idx) : "0"(idx) : "memory");
        D_8009E4E7 = 1;
        asm volatile("" ::: "memory");
        setEventComplete(D_80122980[idx], 0);
        }
        o->w48 = 0x154;
        o->step++;
        break;
    case 5:
        if (--o->w48 == -1) {
            if (D_8009E50A == 7) o->step = 12;
            else o->step++;
        }
        break;
    case 6:
        D_8009E51D = 3;
        D_8009E4E4 = 0;
        o->step++;
    case 7:
        if (D_8009E51D == 1) {
            (*(unsigned short*)&SPAD.s1c6) = 2;
            SoundStopAll();
            SPAD.b1cc = 1;
            SPAD.b1cd = D_8012295C[D_8009E50A];
            ThreadCreate(1, D_8001D5E0);
            o->step++;
        }
        break;
    case 8:
        if (SPAD.b1cc == 0) o->step++;
        break;
    case 9:
        (*(unsigned short*)&SPAD.s1c6) = 0;
        if (D_8009E50A == 7) {
            o->subtype = 1;
            o->step = 3;
            D_8009E52A = 0;
            break;
        }
        if (D_8009EE6B == 0x7f) {
            o->step++;
            D_8009E51D = 4;
            D_8009E4E4 = 0;
            break;
        }
        o->subtype = 1;
        o->step = 3;
        D_8009E52A = 1;
        break;
    case 10:
        if (D_8009E51D == 0) {
            o->w48 = 0x3c;
            o->step++;
        }
        D_80122964[o->subtype](o);
        break;
    case 11:
        if (--o->w48 == -1) {
            D_8009E93C = 2;
            D_8009E93E = 5;
            D_8009E948 = 1;
            pl->b0 = 4;
            pl->b4 = 5;
            pl->b5 = 5;
            pl->b6 = 0;
        }
    case 3:
        D_80122964[o->subtype](o);
        break;
    case 12:
        o->w52 = ((Fix16 *)o->d34)->p.whole;
        asm volatile("" ::: "memory");
        {
        unsigned char st = o->step;
        unsigned short t = *(unsigned short *)0x1F8000F2;
        o->step = st + 1;
        asm volatile("" ::: "memory");
        o->w48 = 0x78;
        S16(o, 0x54) = t;
        }
        break;
    case 13:
        if (--o->w48 == -1) {
            o->step++;
            D_8009E4E9 = 0xe;
            D_8009E4E8 = 1;
            pl->b0 = 5;
            pl->b4 = 1;
            pl->b5 = 0x30;
            pl->b6 = 0;
            q = allocObjectLayer2();
            if (q) {
                q->active = 2;
                q->type = 0x54;
                q->subtype = 1;
            }
        }
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        (*(unsigned short*)((char*)&SPAD.if0 + 2)) = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    case 14:
        if (D_8009E51D == 1) o->step = 7;
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        (*(unsigned short*)((char*)&SPAD.if0 + 2)) = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    }
}
#endif
