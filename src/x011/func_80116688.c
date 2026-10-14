// FUNC 80116688 668 X011
// MATCHING 80116688 668
#include "spad.h"
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
extern int P_1F800334[], P_1F800318[], P_1F800314[], P_1F800310[];
extern void FUN_80025aa8(int, int);
extern void FUN_80024ea0(int, int, int);

int func_80116688(S *o)
{
    int *p;
    register int d asm("$3");
    int a;
    switch (o->substep) {
    case 0:
        o->substep++;
        o->wac = 3;
        o->da0 = ((int*)&SPAD.i334)[0] + ((int *)P_1F800334[0])[4];
        o->velH = 0x60;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800318[0];
        a = o->da0;
        d = ((int*)&SPAD.i318)[0];
        goto common;
    case 2:
        o->substep++;
        o->wac = 2;
        o->da0 = ((int*)&SPAD.i334)[0] + ((int *)P_1F800334[0])[3];
        o->velH = 0x100;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800314[0];
        a = o->da0;
        d = ((int*)&SPAD.i314)[0];
        goto common;
    case 4:
        o->substep++;
        o->wac = 1;
        o->da0 = ((int*)&SPAD.i334)[0] + ((int *)P_1F800334[0])[2];
        o->velH = 0x200;
        o->velV = 0;
        o->b0a = 0x12;
        p = (int *)P_1F800310[0];
        a = o->da0;
        d = ((int*)&SPAD.i310)[0];
        common:
        o->dbc = d + p[1];
        o->db8 = (int)D_800E3E28;
        o->da8 = o->db8;
        FUN_80025aa8(a, o->db8);
        return 0;
    case 5:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velH = -0x280;
            o->substep++;
        }
        goto tail;
    case 6:
        o->velV += o->velH;
        if (o->velV < 0x800) {
            o->velH = 0x280;
            o->substep++;
        }
        goto tail;
    case 1:
    case 3:
    case 7:
        o->velV += o->velH;
        if (o->velV > 0x1000) {
            o->velV = 0x1000;
            o->substep++;
        }
    tail:
        o->wcc = o->velV;
        FUN_80025aa8(o->da0, o->db8);
        FUN_80024ea0(o->db8, o->dbc, o->wcc);
        return 0;
    case 8:
        o->substep = 0;
        o->b0a = 0x11;
        o->wac = 0;
        o->da0 = ((int*)&SPAD.i334)[0] + ((int *)P_1F800334[0])[1];
        return 1;
    default:
        return 0;
    }
}
