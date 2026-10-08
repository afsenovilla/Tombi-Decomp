// FUNC 80115f88 840 X016
// MATCHING 80115f88 840
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

extern PL D_800A6038;
extern Fn D_80079AD0[];
extern unsigned char D_8009C93A, D_8009C93C, D_8009C975;
extern unsigned char D_8009D000[];
extern unsigned short D_8009C962;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2[];
extern int D_1F8000F0;
extern Q *D_1F8001D4;
extern void func_80027810(O *);
extern int FUN_800270a0(O *, int, int);
extern void FUN_8002a4d0(O *);

void func_80115F88(O *o)
{
    PL *p = &D_800A6038;

    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        func_80027810(o);
        o->p38->whole = 0;
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
        case 4:
            if (D_1F800172 >= 0xc9) {
                if (D_8009D000[0] == 0) {
                    D_8009D000[0] = 1;
                    p->b04 = 5;
                    p->step = 0x41;
                    p->visible = 0;
                    p->state = 0;
                    o->step = 3;
                    break;
                }
                FUN_800270a0(o, 1, 0);
            } else if (D_1F800172 < -0x59) {
                FUN_800270a0(o, 1, 1);
            }
            break;
        case 2:
        case 5:
            if (D_1F80016A < 0x46 && D_1F80016E >= -0x1d) {
                FUN_800270a0(o, 1, 0);
                break;
            }
            if (D_1F80016A < 0xfb) break;
            if (D_1F80016E < -0xd8) FUN_800270a0(o, 1, 1);
            break;
        case 3:
        case 6:
            if (D_1F80016A < 0x105) break;
            if (D_1F80016E < -0x1d) break;
            FUN_800270a0(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    case 3:
        D_1F8000F2[0] -= 2;
        if (D_1F8000F2[0] < o->w30) {
            D_1F8000F0 = o->w30 << 16;
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
            D_8009CD94 = 0x10;
            D_8009CD96 = 5;
            D_8009CDA0 = 0;
            D_8009C93C = 0;
            D_8009C975 = 3;
            o->step++;
        }
    case 6:
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        D_1F8000F2[0] += 2;
        if (o->w32 < D_1F8000F2[0]) D_1F8000F0 = o->w32 << 16;
        break;
    }
}
