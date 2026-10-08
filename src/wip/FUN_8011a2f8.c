// FUNC 8011a2f8 288 X000
typedef struct P {
    unsigned char a0, a1, a2, a3;
    char pad1[0x10 - 4];
    int s10, s14, s18;
    char pad2[0x1d - 0x1c];
    signed char b1d;
    char pad3[0x24 - 0x1e];
    int w24;
    char pad4[0x90 - 0x28];
    int w90, w94;
    char pad5[0xba - 0x98];
    unsigned short ba, bc, be, c0, c2, c4, c6, c8, ca, cc;
} P;
extern P *FUN_80018448(void);
extern int DAT_8013b1bc[];

void FUN_8011a2f8(int *s, unsigned char t)
{
    int i, a, b, c;
    int *tp, t;
    unsigned short x0, x1, x2;
    P *p;
    for (i = 0, a = 5, b = 4, c = 16, tp = DAT_8013b1bc; i < 4; i++, tp++, a += 2, b += 4, c += 4) {
        p = FUN_80018448();
        if (p) {
            p->a0 = 1;
            p->a2 = 9;
            p->a3 = t;
            p->s10 = s[4];
            p->s14 = s[5];
            p->s18 = s[6];
            p->b1d = -1;
            t = *tp;
            p->c0 = c;
            x0 = p->c0;
            p->c2 = b;
            x1 = p->c2;
            p->c4 = 1;
            x2 = p->c4;
            p->w90 = (int)s;
            p->w94 = 0;
            p->ba = a;
            p->bc = i + 2;
            p->be = a;
            p->cc = 0;
            p->w24 = t;
            p->c6 = x0;
            p->c8 = x1;
            p->ca = x2;
        }
    }
}
