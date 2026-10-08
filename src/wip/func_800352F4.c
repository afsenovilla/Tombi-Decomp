// FUNC 800352f4 308 MAIN0
/* diff: game hoists lw D_800A6078 above the D_800A606C store, and does mult dx*dx right after the first abs */
extern unsigned char *D_8009C338;
extern unsigned char D_800A6100;
extern int D_800A6068[];
extern unsigned char *D_800A6078;
extern short D_800A604E;
extern unsigned char D_800A60D4;
extern short D_800A60EC;
extern short D_800A60B2;
extern int D_8009C934;
void playSFX(int id);
int csqrt(int x);
short FUN_800335d4(unsigned char *o);
void func_800352F4(unsigned char *o)
{
    int dx, dy, d;

    D_8009C338[0xd] = 0;
    playSFX(0x26);
    D_800A6100 = 1;
    D_800A6068[0] = *(short *)(*(unsigned char **)(o + 0x40) + 2);
    D_800A6068[1] = *(short *)(o + 0x16) + 4;
    dx = *(short *)(*(unsigned char **)(o + 0x40) + 2) - *(short *)(D_800A6078 + 2);
    if (dx < 0) dx = -dx;
    dy = *(short *)(o + 0x16) - D_800A604E;
    if (dy < 0) dy = -dy;
    d = csqrt((dx * dx + dy * dy) << 12) >> 12;
    D_800A60EC = d;
    if (D_800A60D4 == 0) D_800A60EC = d - 0x10;
    if (D_800A60EC < 0x28) D_800A60EC = 0x28;
    D_800A60B2 = FUN_800335d4(o);
    o[5] = 3;
    o[6] = 0;
    *(int *)(o + 0x90) = D_8009C934;
}
