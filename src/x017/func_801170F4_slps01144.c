// VERSION SLPS_01144 x17 801170F4 648
// Matched with the idioms of PAL x017 a17EntranceEventStep.c: VIEW_CAMERA.pos[].p.hi, the `k` temporary and the
// `D_xxxA[0]` array spellings keep the stores in order (gcc 2.7 reorders scalar globals around struct members).
// JP-only: like x16 func_80116128 (player slides a counter at D_800A7BBC+2 up/down) plus a camera shake state.
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "camera.h"
typedef struct { short s0, s2; } S2;
typedef struct { short s0; unsigned short s2; } U2;
extern unsigned short D_8009E52A;
extern unsigned char D_8009E4E1, D_8009E4E2, D_8009EB7E, D_8009EBAF;
extern unsigned char D_8009E4E1A[];
extern S2 *D_800A7BBC;
extern short D_1F8000EEA[], D_1F8000F2A[];
extern char D_800A7B78[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void func_8001E458(int, int);
extern unsigned int Rand(void);

void func_801170F4_slps01144(TObj *o)
{
    short *p;
    int k;

    switch (o->state) {
    case 0:
        if (D_8009E52A != 0) {
            if (D_800A7BBC->s2 < 0x50) {
                PlayerSetAnimIfChanged((TObj *)D_800A7B78, 0x2c);
                ((TObj *)D_800A7B78)->b04 = 5;
                ((TObj *)D_800A7B78)->step = 0x65;
                ((TObj *)D_800A7B78)->state = 0;
                o->state = 1;
                if (D_8009EB7E != 0 && D_8009EBAF == 0) {
                    o->w08 = 0x1e;
                    o->a.raw = D_1F8000EEA[0];
                    o->y.raw = D_1F8000F2A[0];
                    D_8009E4E1A[0] = 1;
                    func_8001E458(0x28, 0x7f);
                    o->state = 3;
                    D_8009EBAF = 1;
                }
                D_8009E4E2 = 1;
            }
        } else if (D_800A7BBC->s2 < 0x64) {
            PlayerSetAnimIfChanged((TObj *)D_800A7B78, 0x2b);
            ((TObj *)D_800A7B78)->b04 = 5;
            ((TObj *)D_800A7B78)->step = 0x65;
            D_8009E4E2 = 1;
            ((TObj *)D_800A7B78)->state = 0;
            o->state = 2;
        }
        break;
    case 1:
        ((U2 *)D_800A7BBC)->s2 -= 3;
        break;
    case 2:
        ((U2 *)D_800A7BBC)->s2 += 3;
        break;
    case 3:
        p = &VIEW_CAMERA.pos[0].p.hi;
        k = 4;
        *p = o->a.raw - k + (int)(Rand() & 7);
        k = 2;
        D_1F8000F2A[0] = o->y.raw - k + (int)(Rand() & 3);
        if (--o->w08 <= 0) {
            *p = o->a.raw;
            D_8009E4E1 = 0;
            VIEW_CAMERA.pos[1].p.hi = o->y.raw;
            o->state = 1;
        }
        break;
    }
}
#endif
