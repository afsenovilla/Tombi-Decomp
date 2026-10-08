// FUNC 80069ef8 632 MAIN0
// FLAGS -O2 -G0 -mno-split-addresses
// CC gcc-2.8.1
/* score 2 (b56, was 42 with 2.7.2): gcc-2.8.1 + -mno-split-addresses (2.8.1 otherwise splits %hi/%lo into two regs;
   -G8 works too). sra comes from int temps (k, r), t - D_8009C33C written in both compares, v set in if/else.
   Left: the first poll loop jumps to 0x5c (the load-delay nop after lw D_80098210) where the game jumps to 0x60
   (nop before the label); same with real ASPSX (matchcheck). Tried do/for/goto/local-pointer loop forms, ASPSX versions.
   Debt: volatile read of s->n in the 0xff compare (game reloads n for the index). */
typedef struct {
    char p0[0x3c];
    unsigned char *buf;
    char p40[4];
    unsigned char n;
    unsigned char cnt;
    char p46[0xe8 - 0x46];
    unsigned char e8;
} S;
extern volatile int *D_8009820C;
extern volatile unsigned short *D_80098210;
extern int D_800981F0;
extern unsigned int D_8009C33C;
extern unsigned int D_800A0A18;
extern void FUN_8006bf84(int);
extern int FUN_8006bfa4(void);

int func_80069EF8(S *s, int c)
{
    int r;
    short v;
    unsigned int t;
    int k;

    k = *s->buf;
    if ((k >> 4) == 8 && s->n > 8) v = 0x22;
    else v = 0x88;
    while (!(D_80098210[2] & 2));
    FUN_8006bf84(400);
    r = *(volatile unsigned char *)D_80098210;
    if (s->n != 0 || (r >> 4) != 8) D_80098210[7] = v;
    else D_80098210[7] = 0x22;
    while (!(*D_8009820C & 0x80)) {
        t = *(volatile unsigned short *)0x1f801120;
        if (t < D_8009C33C) {
            if (*(volatile unsigned short *)0x1f801128 != 0) t = *(volatile unsigned short *)0x1f801128 + t;
            else t += 0x10000;
        }
        if (*(volatile unsigned short *)0x1f801124 & 0x200) {
            if (t - D_8009C33C >= D_800A0A18) return -2;
        } else {
            if ((t - D_8009C33C) >> 3 >= D_800A0A18) return -2;
        }
    }
    if (s->e8 != 8 && D_800981F0 == 2) {
        FUN_8006bf84(60);
        while (FUN_8006bfa4() == 0);
    }
    *(volatile unsigned char *)D_80098210 = c;
    if (D_800981F0 == 3 && r == 0x80) {
        volatile int *is = D_8009820C;
        volatile unsigned short *j = D_80098210;
        *is = -129;
        j[5] |= 0x10;
    }
    s->cnt++;
    if (*(volatile unsigned char *)&s->n != 0xff) s->buf[s->n] = r;
    s->n++;
    return r;
}
