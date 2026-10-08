// FUNC 800352f4 308 MAIN0
// MATCHING 800352f4 308
extern unsigned char *D_8009C338;
extern unsigned char D_800A6100;
extern int D_800A6068;
extern int D_800A606C;
extern unsigned char *D_800A6078;
extern short D_800A604E;
extern unsigned char D_800A60D4;
extern short D_800A60EC;
extern short D_800A60B2;
extern int D_8009C934;
void playSFX(int id);
int abs(int);
int csqrt(int x);
short FUN_800335d4(unsigned char *o);
void func_800352F4(unsigned char *o)
{
    int dx, dy, d, n;

    D_8009C338[0xd] = 0;
    playSFX(0x26);
    D_800A6100 = 1;
    D_800A6068 = *(short *)(*(unsigned char **)(o + 0x40) + 2);
    D_800A606C = *(short *)(o + 0x16) + 4;
    dx = *(short *)(*(unsigned char **)(o + 0x40) + 2) - *(short *)(D_800A6078 + 2);
    dx = abs(dx);
    dy = *(short *)(o + 0x16) - D_800A604E;
    dy = abs(dy);
    d = csqrt((dx * dx + dy * dy) << 12) >> 12;
    D_800A60EC = d;
    if (D_800A60D4 == 0) D_800A60EC = d - 0x10;
    if (D_800A60EC < 0x28) D_800A60EC = 0x28;
    d = FUN_800335d4(o);
    n = D_8009C934;
    D_800A60B2 = d;
    o[5] = 3;
    o[6] = 0;
    *(int *)(o + 0x90) = n;
}
