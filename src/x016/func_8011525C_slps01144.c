// VERSION SLPS_01144 x16 8011525C 836
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gametask.h"
typedef struct { unsigned short frac; short whole; } FP;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x38 - 0x34];
    FP *p38;
    char p3c[0x48 - 0x3c];
    short w48;
} O;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} PL;
typedef struct { char p[0x4c]; short w4c, w4e; } Q;
typedef void (*Fn)(O *);

extern PL D_800A7B78;
extern unsigned short D_8009E50A;
extern struct { unsigned char b0; } D_8009EBA8;
extern unsigned char D_8009E4E4, D_8009E51D;
extern short D_8009E93C, D_8009E93E, D_8009E948;
extern unsigned char D_8009E4E2;
extern Fn D_80075864[];
extern void func_80028620(O *);
extern int func_80027EA8(O *, int, int);
extern void func_8002B2D4(O *);

void func_8011525C(O *o)
{
    PL *p = &D_800A7B78;

    switch (o->step) {
    case 0:
        if (D_8009E4E2 != 0) o->step++;
        func_80028620(o);
        break;
    case 1:
        D_80075864[o->subtype](o);
        switch (D_8009E50A) {
        case 1:
        case 4:
            if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0xc9) {
                if (D_8009EBA8.b0 == 0) {
                    D_8009EBA8.b0 = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                func_80027EA8(o, 1, 0);
            } else if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x104) {
                func_80027EA8(o, 1, 1);
            }
            break;
        case 2:
        case 5:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x46 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x1d) {
                func_80027EA8(o, 1, 0);
                break;
            }
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0xfb) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0xd8) func_80027EA8(o, 1, 1);
            break;
        case 3:
        case 6:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x105) break;
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x1d) break;
            func_80027EA8(o, 1, 0);
            break;
        }
        break;
    case 2:
        func_8002B2D4(o);
        break;
    case 3:
        ((short*)((char*)&SPAD.if0 + 2))[0] -= 2;
        if (((short*)((char*)&SPAD.if0 + 2))[0] < o->w30) {
            SPAD.if0 = o->w30 << 16;
            o->w48 = 0x1e;
            o->step++;
        }
        break;
    case 4:
        if (--o->w48 == -1) {
            o->w48 = 0x3c;
            o->step++;
        }
        break;
    case 5:
        if (--o->w48 == -1) {
            D_8009E93C = 0x10;
            D_8009E93E = 5;
            D_8009E948 = 0;
            D_8009E4E4 = 0;
            D_8009E51D = 3;
            o->step++;
        }
    case 6:
        if (D_8009E51D == 1) {
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        ((short*)((char*)&SPAD.if0 + 2))[0] += 2;
        if (o->w32 < ((short*)((char*)&SPAD.if0 + 2))[0]) SPAD.if0 = o->w32 << 16;
        break;
    }
}
#endif
