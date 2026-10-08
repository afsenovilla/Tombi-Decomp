// FUNC 80118b30 80 X017
// MATCHING 80118b30 80
typedef struct { short s0, s2; } S2;
typedef struct { char p[0x1e]; unsigned char b1e; } PLY;
extern S2 *D_800A6078;
extern unsigned char D_8009CFD6;
extern PLY *D_8009C330;

void func_80118B30(void)
{
    if ((unsigned short)(D_800A6078->s2 - 0x100) < 0x90 && D_8009CFD6 == 0) D_8009C330->b1e = 0;
}
