// FUNC 80069ef8 632 MAIN0
/* score 42 (ncheck): library code (pad/memcard). Unreachable with CC1PSX 4.3: game epilogue is "jr ra; addiu sp" with saved s-regs, and it uses sra for (u8)>>4 where gcc 2.7 emits srl. Also s1/s2 swapped (v vs r) and buf[n] reloads n. Logic is complete. */
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
    unsigned int r;
    short v;
    unsigned int t;

    v = 0x88;
    if (((int)*s->buf >> 4) == 8 && s->n > 8) v = 0x22;
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
        t -= D_8009C33C;
        if (*(volatile unsigned short *)0x1f801124 & 0x200) {
            if (t >= D_800A0A18) return -2;
        } else {
            if ((t >> 3) >= D_800A0A18) return -2;
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
    if (s->n != 0xff) s->buf[s->n] = r;
    s->n++;
    return r;
}
