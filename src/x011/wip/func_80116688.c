// FUNC 80116688 668 X011
/* score 169: logic complete (csv size 664 misses the epilogue delay-slot nop, real size 668). Scratchpad words 0x1F800310..334 are each read twice (volatile). With literal volatile addresses the case-0 schedule matches the game but gcc CSEs the lui/ori address into a register; with extern volatile symbols the wac store is scheduled after the first load. The three init cases are not cross-jumped into one tail (game shares from 'lw a2,4(v0)'). Tried: extern volatile scalars/arrays, operand order, hill-climb of the case statement order. */
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
extern void FUN_80025aa8(int, int);
extern void FUN_80024ea0(int, int, int);

int func_80116688(S *o)
{
    switch (o->substep) {
    case 0:
        o->substep++;
        o->wac = 3;
        o->da0 = (*(volatile int *)0x1F800334) + ((int *)(*(volatile int *)0x1F800334))[4];
        o->velH = 0x60;
        o->velV = 0;
        o->b0a = 0x12;
        o->dbc = (*(volatile int *)0x1F800318) + ((int *)(*(volatile int *)0x1F800318))[1];
        o->db8 = (int)D_800E3E28;
        o->da8 = o->db8;
        FUN_80025aa8(o->da0, o->db8);
        return 0;
    case 2:
        o->substep++;
        o->wac = 2;
        o->da0 = (*(volatile int *)0x1F800334) + ((int *)(*(volatile int *)0x1F800334))[3];
        o->velH = 0x100;
        o->velV = 0;
        o->b0a = 0x12;
        o->dbc = (*(volatile int *)0x1F800314) + ((int *)(*(volatile int *)0x1F800314))[1];
        o->db8 = (int)D_800E3E28;
        o->da8 = o->db8;
        FUN_80025aa8(o->da0, o->db8);
        return 0;
    case 4:
        o->substep++;
        o->wac = 1;
        o->da0 = (*(volatile int *)0x1F800334) + ((int *)(*(volatile int *)0x1F800334))[2];
        o->velH = 0x200;
        o->velV = 0;
        o->b0a = 0x12;
        o->dbc = (*(volatile int *)0x1F800310) + ((int *)(*(volatile int *)0x1F800310))[1];
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
        o->da0 = (*(volatile int *)0x1F800334) + ((int *)(*(volatile int *)0x1F800334))[1];
        return 1;
    default:
        return 0;
    }
    o->wcc = o->velV;
    FUN_80025aa8(o->da0, o->db8);
    FUN_80024ea0(o->db8, o->dbc, o->wcc);
    return 0;
}
