// VERSION SLPS_01144 x17 801153B8 1136
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
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
    char p08[0x12 - 8];
    short x;
    char p14[0x1a - 0x14];
    short z;
    char p1c[0xa0 - 0x1c];
    unsigned char ba0;
    char pa1[0xa8 - 0xa1];
    unsigned char ba8;
} PL;
typedef struct { char p[0x4c]; short w4c, w4e; } Q;
typedef void (*Fn)(O *);

extern PL D_800A7B78;
extern Fn D_80075864[];
extern unsigned char D_8009E4E2, D_8009EBA9, D_8009E4E4, D_8009E51D;
extern unsigned short D_8009E50A;
extern short D_8009E93C, D_8009E93E, D_8009E948;
extern void func_80028620_slps01144(O *);
extern int func_80027EA8_slps01144(O *, int, int);
extern void stepPlayerXToward(int);
extern void func_8002B2D4_slps01144(O *);

void func_801153B8(O *o)
{
    PL *p = &D_800A7B78;

    switch (o->step) {
    case 0:
        if (D_8009E4E2 != 0) o->step++;
        func_80028620_slps01144(o);
        o->p38->whole = 0;
        break;
    case 1:
        D_80075864[o->subtype](o);
        switch (D_8009E50A) {
        case 1:
            if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x191) {
                unsigned char *q = &D_8009EBA9;
                if (*q == 0) {
                    *q = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                func_80027EA8_slps01144(o, 1, 1);
            } else if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x104) {
                func_80027EA8_slps01144(o, 1, 0);
            }
            break;
        case 2:
            if ((unsigned short)(p->x - 0x36) < 0x40) {
                p->ba0 = 1;
                p->ba8 = 8;
                if (p->z > 0) {
                    stepPlayerXToward(0x56);
                    if (p->z >= 0x5b) {
                        func_80027EA8_slps01144(o, 1, 1);
                        break;
                    }
                }
            }
            if ((unsigned short)(p->x - 0x210) < 0x40) {
                p->ba0 = 1;
                p->ba8 = 8;
                if (p->z > 0) {
                    stepPlayerXToward(0x230);
                    if (p->z >= 0x5b) {
                        func_80027EA8_slps01144(o, 1, 3);
                        break;
                    }
                }
            }
            if ((unsigned short)(p->x - 0x110) < 0x60) {
                if (p->z > 0) {
                    stepPlayerXToward(0x140);
                    if (p->z >= 0x5b) func_80027EA8_slps01144(o, 1, 2);
                } else if (p->z < 0) {
                    stepPlayerXToward(0x140);
                    if (p->z < -0x5a) func_80027EA8_slps01144(o, 1, 0);
                }
            }
            break;
        case 3:
            if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) func_80027EA8_slps01144(o, 1, 0);
            break;
        case 4:
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x12d && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x3b) func_80027EA8_slps01144(o, 1, 0);
            break;
        case 5:
            if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0x12d && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x40) func_80027EA8_slps01144(o, 1, 0);
            break;
        }
        break;
    case 2:
        func_8002B2D4_slps01144(o);
        break;
    case 3:
        ((short*)((char*)&SPAD.if0 + 2))[0]--;
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
            D_8009E93C = 0x11;
            D_8009E93E = 2;
            D_8009E948 = 0;
            D_8009E4E4 = 0;
            D_8009E51D = 3;
            o->step++;
        }
    case 6:
        if (D_8009E51D == 1) {
            Q *t = *(Q **)0x1F8001D4;
            t->w4c = 7;
            t->w4e = 0;
        }
        ((short*)((char*)&SPAD.if0 + 2))[0]++;
        if (o->w32 < ((short*)((char*)&SPAD.if0 + 2))[0]) SPAD.if0 = o->w32 << 16;
        break;
    }
}
#endif
