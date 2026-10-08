// FUNC 801102b4 1196 X000
// r11: score 322, structure right (4 chains). Same k problem as func_80110CAC (game: no andi on k, sll not CSE'd); wb2 read per branch, inline nesting / frame 0x40 not reproduced.
typedef struct S { char p[0x2e]; unsigned short animFrame; char p2[0xb2 - 0x30]; short wb2; char q[0xbe - 0xb4]; unsigned char bBE; char q2[2]; unsigned char bC1; } S;
typedef struct T { char p[0x10]; short f10, f12, f14, f16; char q[0x48 - 0x18]; unsigned short g48, g4a, g4c, g4e, g50, g52, g54, g56, g58, g5a, g5c; char r[0x74 - 0x5e]; } T;
extern T D_80115500[];
extern unsigned char D_8009D2B3;
extern unsigned char D_8009C990;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D006;

#define TB D_80115500[k]
static __inline__ void body(S *o)
{
    unsigned short k;
    int u;
    short v;
    k = D_8009D2B3 + o->bC1 * 4;
    if (D_8009C990 & 3)
        k = (o->bC1 << 2) | 3;
    if (D_8009CF06)
        k = 8;
    if (D_8009D006)
        k = 8;
    switch (o->bBE & 1) {
    case 0:
        if (o->animFrame & 1) {
            v = o->wb2;
            u = v;
            if (TB.f12 < v) o->wb2 = u - TB.g48;
            else if (TB.f10 < v) o->wb2 = u - TB.g4a;
            else if (v > 0) o->wb2 = u - TB.g4c;
            else if (v == 0) o->wb2 = -TB.g4e;
            else if (-TB.f12 < v) o->wb2 = u - TB.g50;
            else if (v < -TB.f12) o->wb2 = -TB.f12;
        } else {
            v = o->wb2;
            u = v;
            if (v < -TB.f12) o->wb2 = u + TB.g52;
            else if (v < -TB.f10) o->wb2 = u + TB.g54;
            else if (v < 0) o->wb2 = u + TB.g56;
            else if (v == 0) o->wb2 = TB.g58;
            else if (v < TB.f12) o->wb2 = u + TB.g5a;
            else if (v < TB.f16) o->wb2 = u + TB.g5c;
            else o->wb2 = TB.f16;
        }
        break;
    case 1:
        if (o->animFrame & 1) {
            v = o->wb2;
            u = v;
            if (TB.f12 < v) o->wb2 = u - TB.g52;
            else if (TB.f10 < v) o->wb2 = u - TB.g54;
            else if (v > 0) o->wb2 = u - TB.g56;
            else if (v == 0) o->wb2 = -TB.g58;
            else if (-TB.f12 < v) o->wb2 = u - TB.g5a;
            else if (-TB.f16 < v) o->wb2 = u - TB.g5c;
            else o->wb2 = -TB.f16;
        } else {
            v = o->wb2;
            u = v;
            if (v < -TB.f12) o->wb2 = u + TB.g48;
            else if (v < -TB.f10) o->wb2 = u + TB.g4a;
            else if (v < 0) o->wb2 = u + TB.g4c;
            else if (v == 0) o->wb2 = TB.g4e;
            else if (v < TB.f12) o->wb2 = u + TB.g50;
            else if (TB.f12 < v) o->wb2 = TB.f12;
        }
        break;
    }
}

void func_801102B4(S *o)
{
    body(o);
}
