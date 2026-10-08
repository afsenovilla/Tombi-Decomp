// FUNC 801165e0 1288 X010
// MATCHING 801165e0 1288
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0xa2 - 8];
    unsigned char ba2;
} PL;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} O;
typedef void (*Fn)(O *);

extern PL D_800A6038;
extern Fn D_80079A98[];
extern unsigned char D_8009CDA2[];
extern unsigned char D_8009C93A, D_8009C93E;
extern unsigned char D_8009D078, D_8009D2C3;
extern unsigned short D_8009C962;
extern FP *D_800A6078;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028b40(O *);
extern void FUN_80027c74(O *);
extern void func_8002795C(O *);
extern void FUN_800277f8(O *);
extern void func_80029078(O *);
extern void func_80027810(O *);
extern int FUN_8002715c(int);
extern int FUN_800270a0(O *, int, int);
extern void FUN_8002a4d0(O *);

#define XU (*(unsigned short *)&D_1F80016A)
#define ZU (*(unsigned short *)&D_1F80016E)

void func_801165E0(O *o)
{
    PL *p = &D_800A6038;

    switch (o->step) {
    case 0:
        if (D_8009CDA2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) D_8009CDA2[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        func_8002795C(o);
        FUN_800277f8(o);
        func_80029078(o);
        break;
    case 2:
        if (D_8009C962 == 8) func_80027810(o);
        else D_80079A98[o->subtype](o);
        switch (D_8009C962) {
        case 0:
        case 4:
            if (p->ba2 == 2) {
                short *y = &D_1F800172;
                if (*y < 0x5a) break;
                if ((unsigned)(XU - 0x56e) >= 0x60) break;
                FUN_8002715c(0x59e);
                if (*y < 0x88) break;
                FUN_800270a0(o, 1, 0);
                break;
            }
            if (D_8009D078 < 2) break;
            if (D_1F80016A < 0xc8b) break;
            if (D_1F80016E < -0x60) break;
            FUN_800270a0(o, 1, 1);
            break;
        case 1:
        case 5:
            if (D_800A6038.ba2 == 2) {
                if (D_1F800172 >= -0x46) break;
                if (D_1F80016A < 0x21c) {
                    FUN_800270a0(o, 1, 0);
                    break;
                }
                if (D_1F80016A < 0x30d) break;
                FUN_800270a0(o, 1, 1);
                break;
            }
            if (D_8009D2C3 & 0x40) {
                if (D_1F80016A < 0x487) break;
                if (D_1F80016E < -0x171) break;
                FUN_800270a0(o, 1, 2);
                break;
            }
            if (D_1F80016A < 0x44d) break;
            D_800A6078->whole = 0x44c;
            break;
        case 2:
        case 6:
            if (D_1F80016A < 0x81 && D_1F80016E >= -0x40 && FUN_800270a0(o, 1, 0)) D_8009C93E = 1;
            break;
        case 3:
        case 7:
            if ((unsigned)(XU - 0x53f) < 0x40) {
                if (D_8009D2C3 & 0x40) {
                    if ((unsigned short)(ZU + 0x392) < 0x1e) {
                        FUN_800270a0(o, 1, 1);
                        break;
                    }
                } else {
                    if ((unsigned short)(ZU + 0x434) < 0x1e) {
                        FUN_800270a0(o, 1, 1);
                        break;
                    }
                }
            }
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x40f) {
                FUN_800270a0(o, 1, 0);
                break;
            }
            if (D_1F80016A < 0x7c2) break;
            if ((unsigned short)(ZU + 0x3fa) < 0x64) {
                FUN_800270a0(o, 1, 2);
                break;
            }
            if (D_1F80016A < 0x7c7) break;
            if ((unsigned short)(ZU + 0xe8) < 0x50) FUN_800270a0(o, 1, 3);
            break;
        case 8:
            if (D_1F80016A < 0x3a && D_1F80016E >= -0x3b) {
                if (D_8009D2C3 & 0x40) FUN_800270a0(o, 1, 1);
                else FUN_800270a0(o, 1, 0);
            }
            break;
        }
        break;
    case 3:
        FUN_8002a4d0(o);
        break;
    }
}
