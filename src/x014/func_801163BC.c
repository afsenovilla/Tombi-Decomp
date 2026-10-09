// FUNC 801163bc 1252 X014
// MATCHING 801163bc 1252
#include "TOBJ.H"
#include "raw7.h"

typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } PL;
typedef void (*Fn)(TObj *);

extern PL D_800A6038;
extern Fn D_8012568C[];
extern short D_801256A8[];
extern unsigned char D_80125684[];
extern unsigned char D_8009CDA2, D_8009C93A, D_8009C93F, D_8009C975, D_8009C93C, D_8009D2C3;
extern unsigned char D_8009C93Fx[];
extern unsigned char D_8009C941, D_8009C940;
extern unsigned short D_8009C962, D_8009C982;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned short D_1F8000F2x[];
extern unsigned short D_1F8001C6, D_1F8000F2;
extern unsigned char D_1F8001CC, D_1F8001CD, D_1F8003CE;
extern void D_8001D6A4(void);
extern unsigned int FUN_80028420(TObj *);
extern unsigned int FUN_80028b40(TObj *);
extern void FUN_80027c74(TObj *);
extern void FUN_800277f8(TObj *);
extern void FUN_800284d8(TObj *);
extern void FUN_8005a9a4(int, int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, void (*)(void));
extern int Rand(void);
extern TObj *FUN_800183b8(void);
extern void func_801184CC(void);

void func_801163BC(TObj *o)
{
    PL *pl = &D_800A6038;
    TObj *q;

    switch (o->step) {
    case 0: {
        unsigned char *f = &D_8009CDA2;
        if (*f != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        if (D_8009C962 == 7) {
            U8(o, 0x6d) = 1;
            U8(o, 0x6e) = 2;
            U8(o, 0x6f) = 10;
        }
        break;
    }
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        FUN_800277f8(o);
        FUN_800284d8(o);
        break;
    case 2:
        D_8012568C[o->subtype](o);
        D_8009C982 = 0;
        break;
    case 4:
        D_8009C93Fx[0] = 1;
        FUN_8005a9a4(D_801256A8[D_8009C962], 0);
        o->w48 = 0x154;
        o->step++;
        break;
    case 5:
        if (--o->w48 == -1) {
            if (D_8009C962 == 7) o->step = 12;
            else o->step++;
        }
        break;
    case 6:
        D_8009C975 = 3;
        D_8009C93C = 0;
        o->step++;
    case 7:
        if (D_8009C975 == 1) {
            D_1F8001C6 = 2;
            SoundStopAll();
            D_1F8001CC = 1;
            D_1F8001CD = D_80125684[D_8009C962];
            ThreadCreate(1, D_8001D6A4);
            o->step++;
        }
        break;
    case 8:
        if (D_1F8001CC == 0) o->step++;
        break;
    case 9:
        D_1F8001C6 = 0;
        if (D_8009C962 == 7) {
            D_1F8003CE = 1;
            o->subtype = 1;
            o->step = 3;
            D_8009C982 = 0;
            break;
        }
        if (D_8009D2C3 == 0x7f) {
            o->step++;
            D_8009C975 = 4;
            D_8009C93C = 0;
            break;
        }
        o->subtype = 1;
        o->step = 3;
        D_8009C982 = 1;
        break;
    case 10:
        if (D_8009C975 == 0) {
            o->w48 = 0x3c;
            o->step++;
        }
        D_8012568C[o->subtype](o);
        break;
    case 11:
        if (--o->w48 == -1) {
            D_8009CD94 = 2;
            D_8009CD96 = 5;
            D_8009CDA0 = 1;
            pl->b0 = 4;
            pl->b4 = 5;
            pl->b5 = 5;
            pl->b6 = 0;
            pl->b7 = 1;
        }
    case 3:
        D_8012568C[o->subtype](o);
        break;
    case 12:
        o->w52 = ((Fix16 *)o->d34)->p.whole;
        S16(o, 0x54) = D_1F8000F2x[0];
        o->step++;
        o->w48 = 0x78;
        func_801184CC();
        break;
    case 13:
        if (--o->w48 == -1) {
            o->step++;
            D_8009C941 = 0xe;
            D_8009C940 = 1;
            pl->b0 = 5;
            pl->b4 = 1;
            pl->b5 = 0x30;
            pl->b6 = 0;
            q = FUN_800183b8();
            if (q) {
                q->active = 2;
                q->type = 0x54;
                q->subtype = 1;
            }
        }
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        D_1F8000F2 = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    case 14:
        if (D_8009C975 == 1) o->step = 7;
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        D_1F8000F2 = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    }
}
