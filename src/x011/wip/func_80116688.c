// FUNC 80116688 668 X011
/* score 107: scratchpad words as two non-volatile array names per word (P_ = pointer read, D_ = value read; both [0]) give the game's lui+lw pairs and case 8 exactly. Cases 0/2/4: the game cross-jumps their tails from `lw a2,4(v0)` (P in v0, D in v1, a0=da0 loaded per case); full copies give P/D in v1/v0 so no cross-jump (189); this goto-common form is closer but loads D in the case and a0 in the tail. Tried: volatile scalars/arrays, raw wac store, operand orders, temps, statement-order permutations.
   o29: with full copies case 0 is identical to the game except the second pair: game P in v0, `lw a0,da0` between the
   P and D loads, D in v1; ours loads a0 first and swaps P/D (v1/v0), so the tails never cross-jump. Searched dbc forms
   (D+P[1], P[1]+D, p/d temps in both orders) at every position, da0 operand order, da8 forms: >= 167. */
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[2];
    unsigned char b0a, b0b, b0c, b0d, b0e, b0f;
    char p10[0x80 - 0x10];
    short velH, velV;
    char p84[0xa0 - 0x84];
    int da0;
    char pa4[4];
    int da8;
    short wac;
    char pae[0xb4 - 0xae];
    int db4;
    int db8;
    int dbc;
    char pc0[0xcc - 0xc0];
    short wcc;
} S;
typedef struct { int d0; int b8; int bc; } E;
extern char D_800E3E28[];
extern int D_1F800334[], D_1F800318[], D_1F800314[], D_1F800310[];
extern int P_1F800334[], P_1F800318[], P_1F800314[], P_1F800310[];
extern void FUN_80025aa8(int, int);
extern void FUN_80024ea0(int, int, int);

int func_80116688(S *o)
{
    int *p;
    int d;
    switch (o->substep) {
    case 0:
        o->substep++;
        o->wac = 3;
        o->da0 = D_1F800334[0] + ((int *)P_1F800334[0])[4];
        o->velH = 0x60;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800318[0];
        d = D_1F800318[0];
        goto common;
    case 2:
        o->substep++;
        o->wac = 2;
        o->da0 = D_1F800334[0] + ((int *)P_1F800334[0])[3];
        o->velH = 0x100;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800314[0];
        d = D_1F800314[0];
        goto common;
    case 4:
        o->substep++;
        o->wac = 1;
        o->da0 = D_1F800334[0] + ((int *)P_1F800334[0])[2];
        o->velH = 0x200;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800310[0];
        d = D_1F800310[0];
        common:
        o->dbc = d + p[1];
        o->db8 = (int)D_800E3E28;
        o->da8 = o->db8;
        FUN_80025aa8(o->da0, o->db8);
        return 0;
    case 5:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velH = -0x280;
            o->substep++;
        }
        break;
    case 6:
        o->velV += o->velH;
        if (o->velV < 0x800) {
            o->velH = 0x280;
            o->substep++;
        }
        break;
    case 1:
    case 3:
    case 7:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velV = 0x1000;
            o->substep++;
        }
        break;
    case 8:
        o->substep = 0;
        o->b0a = 0x11;
        o->wac = 0;
        o->da0 = D_1F800334[0] + ((int *)P_1F800334[0])[1];
        return 1;
    default:
        return 0;
    }
    o->wcc = o->velV;
    FUN_80025aa8(o->da0, o->db8);
    FUN_80024ea0(o->db8, o->dbc, o->wcc);
    return 0;
}
