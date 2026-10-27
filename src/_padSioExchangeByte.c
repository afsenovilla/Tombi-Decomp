// FUNC 80069ef8 632 MAIN0
// MATCHING 80069ef8 632
// FLAGS -O2 -G8
// CC gcc-2.8.1
/* Matched with the same crutch as sioWaitRxReady (debt): __asm__ volatile("nop") after the pointer load,
   because the game has the load-delay nop BEFORE the poll-loop label (lw; nop; L: lhu) while every ASPSX
   (2.34..2.86) and gcc put it after the label. The loop must be `do {} while (...)`: an empty `while` body
   costs 3 extra insns in v's live length (27 vs 24), which drops it below r in global-alloc (s1/s2 swap).
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
extern volatile unsigned short *PAD_SIO;
extern int D_800981F0;
extern unsigned int D_8009C33C;
extern unsigned int D_800A0A18;
extern void setRC2wait(int);
extern int chkRC2wait(void);

int _padSioExchangeByte(S *s, int c)
{
    int r;
    short v;
    unsigned int t;
    int k;
    volatile unsigned short *q;

    k = *s->buf;
    if ((k >> 4) == 8 && s->n > 8) v = 0x22;
    else v = 0x88;
    q = PAD_SIO;
    __asm__ volatile("nop");
    do {} while (!(q[2] & 2));
    setRC2wait(400);
    r = *(volatile unsigned char *)PAD_SIO;
    if (s->n != 0 || (r >> 4) != 8) PAD_SIO[7] = v;
    else PAD_SIO[7] = 0x22;
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
        setRC2wait(60);
        while (chkRC2wait() == 0);
    }
    *(volatile unsigned char *)PAD_SIO = c;
    if (D_800981F0 == 3 && r == 0x80) {
        volatile int *is = D_8009820C;
        volatile unsigned short *j = PAD_SIO;
        *is = -129;
        j[5] |= 0x10;
    }
    s->cnt++;
    if (*(volatile unsigned char *)&s->n != 0xff) s->buf[s->n] = r;
    s->n++;
    return r;
}
