// VERSION SLPM_80165 slpm80165 80046DDC 468
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80050EA4 (FUN_80050ea4.c): another draw list (scratchpad 0x22C/0x240/0x25A/0x270),
// kinds 0-3, no visibility test.
typedef struct { char p0[1]; unsigned char vis; char p1[8]; unsigned char k; } O;
#define G1c6 D_1F8001C6
#define G24e D_1F80025A
#define G244 (*(short *)&D_1F800240)
#define G244u D_1F800240
#define G25c D_1F800270
#define G218 D_1F80022C
extern short D_1F8001C6, D_1F80025A;
extern unsigned short D_1F800240;
extern O **D_1F800270, **D_1F80022C;
extern void func_800438CC(O *), func_80046FB0(O *), func_800472B8(O *), func_800EA120(O *);

void FUN_80050ea4(void)
{
    int n;
    O **pp;
    O *o;
    if (G1c6 != 0) {
        n = G24e;
        pp = G25c;
        if (n != 0) {
            do {
                o = *pp++;
                n--;
                switch (o->k) {
                case 0: func_800438CC(o); break;
                case 1: func_80046FB0(o); break;
                case 2: func_800472B8(o); break;
                case 3: func_800EA120(o); break;
                }
            } while (n != 0);
        }
    } else {
        G24e = G244u;
        G25c = G218;
        if (G244u != 0) {
            do {
                o = *G218++;
                G244u = G244u - 1;
                switch (o->k) {
                case 0: func_800438CC(o); break;
                case 1: func_80046FB0(o); break;
                case 2: func_800472B8(o); break;
                case 3: func_800EA120(o); break;
                }
            } while (G244 != 0);
        }
    }
}
#endif
