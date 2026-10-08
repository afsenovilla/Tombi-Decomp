// FUNC 80116278 808 X009
// MATCHING 80116278 808
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3c - 0x34];
    unsigned char b3c;
} O;
typedef void (*Fn)(O *);

extern Fn D_80079A98[];
extern unsigned char D_8009CDA2[];
extern unsigned char D_8009C93A, D_8009C93D, D_8009CFEE, D_800A60DA;
extern unsigned short D_8009C982;
extern unsigned short D_8009C962;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2;
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028b40(O *);
extern void FUN_80027c74(O *);
extern void FUN_800277f8(O *);
extern void func_80029078(O *);
extern int FUN_8002715c(int);
extern int FUN_800270a0(O *, int, int);

#define XU (*(unsigned short *)&D_1F80016A)

void func_80116278(O *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CDA2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) D_8009CDA2[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        D_8009C93D = 1;
        if (D_8009C962 == 0 && D_8009C982 == 5)
            D_8009C93A = 1;
        break;
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        o->b3c = 0;
        FUN_800277f8(o);
        func_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        if (D_8009CFEE == 1 && D_800A60DA == 2 && (unsigned)(XU - 0xb44) < 0x60) {
            FUN_8002715c(0xb74);
            if (D_1F800172 >= 0xbb9) {
                FUN_800270a0(o, 1, 3);
                break;
            }
        }
        if (D_1F80016A < 0x4b0 && D_1F80016E >= -0x2cf)
            FUN_800270a0(o, 1, 0);
        else if (D_1F800172 == 0xb40 && D_1F80016A >= 0x11d5 && D_1F80016E >= -0x1d5)
            FUN_800270a0(o, 1, 2);
        else if ((unsigned)(XU - 0x1050) < 0x20 && D_1F800172 < 0xaf0)
            FUN_800270a0(o, 1, 1);
        if (D_1F800172 < 0xb36) {
            short *k = &D_1F8000F2;
            *k += 2;
            if (o->w32 < *k)
                *k = o->w32;
        }
        break;
    case 3:
        D_80079A98[o->subtype](o);
        break;
    }
}
