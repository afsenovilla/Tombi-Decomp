// FUNC 8011a2f8 288 X000
// MATCHING 8011a2f8 288
typedef struct { int x, y, z; } V3;
typedef struct P {
    unsigned char active, b01, type, subtype;
    char pad1[0x10 - 4];
    V3 pos;
    char pad2[0x1d - 0x1c];
    signed char b1d;
    char pad3[0x24 - 0x1e];
    int d24;
    char pad4[0x90 - 0x28];
    int d90, d94;
    char pad5[0xba - 0x98];
    unsigned short ba, bc, be, c0, c2, c4, c6, c8, ca, cc;
} P;
extern P *FUN_80018448(void);
extern int DAT_8013b1bc[];

void FUN_8011a2f8(P *s, unsigned char t)
{
    int i, a, b, c;
    int *tp;
    P *p;
    int v, one;
    unsigned short x0, x1, x2;
    for (i = 0, one = 1, b = 4, c = 16, a = 5, tp = DAT_8013b1bc; i < 4; b += 4, c += 4, a += 2, i++, tp++) {
        p = FUN_80018448();
        if (p) {
            p->active = one;
            p->type = 9;
            p->subtype = t;
            p->pos = s->pos;
            p->b1d = -1;
            v = *tp;
            p->c0 = c;
            x0 = *(volatile unsigned short *)&p->c0;
            p->c2 = b;
            x1 = *(volatile unsigned short *)&p->c2;
            p->c4 = one;
            x2 = *(volatile unsigned short *)&p->c4;
            p->d90 = (int)s;
            p->d94 = 0;
            p->ba = a;
            p->bc = i + 2;
            p->be = a;
            p->cc = 0;
            p->d24 = v;
            p->c6 = x0;
            p->c8 = x1;
            p->ca = x2;
        }
    }
}
