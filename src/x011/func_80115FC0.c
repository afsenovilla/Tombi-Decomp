// FUNC 80115fc0 444 X011
// MATCHING 80115fc0 444
typedef struct { unsigned short frac; short whole; } FP;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x38 - 8];
    FP *p38;
} O;
typedef void (*Fn)(O *);

extern Fn D_80079AD0[];
extern unsigned char D_8009C93A;
extern unsigned short D_8009C962;
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern void func_80027810(O *);
extern int FUN_800270a0(O *, int, int);
extern void FUN_8002a4d0(O *);

void func_80115FC0(O *o)
{
    switch (o->step) {
    case 0:
        if (D_8009C93A != 0) o->step++;
        o->p38->whole = 0;
        func_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        switch (D_8009C962) {
        case 1:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x3b) {
                FUN_800270a0(o, 1, 0);
                break;
            }
            if (D_1F80016A < 0x123) break;
            if (D_1F80016E < -0x3b) break;
            if (D_1F800172 < 0x5b) break;
            FUN_800270a0(o, 1, 1);
            break;
        case 2:
            if (D_1F80016A >= 0x67) break;
            if (D_1F80016E < -0x3b) break;
            FUN_800270a0(o, 1, 0);
            break;
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
