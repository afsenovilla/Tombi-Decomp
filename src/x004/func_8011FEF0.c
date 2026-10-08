// FUNC 8011fef0 236 X004
// MATCHING 8011fef0 236
typedef struct P { short w0; unsigned short w2; short w4; } P;
typedef struct H { char p0[2]; short x; } H;
typedef struct O {
    char p0[3]; unsigned char subtype;
    char p1[0x16 - 4]; short y;
    char p2[0x2e - 0x18]; unsigned short af;
    char p3[0x40 - 0x30]; H *h;
    char p4[0x9d - 0x44]; unsigned char b9d;
    char p5[0xb4 - 0x9e]; P p;
} O;
extern short D_8007A1F0[], D_8007A5F0[];
extern short func_8004065C(O *, int, int, int);

int func_8011FEF0(O *o)
{
    unsigned short a, b;
    P *p = &o->p;
    if ((o->b9d & 2) && o->af == (o->b9d & 1))
        return 1;
    if (o->af == 0)
        p->w4 = 0x10;
    else
        p->w4 = -0x10;
    if (p->w2 != 0) {
        int i = (p->w2 & 0xff) * 2;
        a = (unsigned)(p->w4 * *(short *)((char *)D_8007A1F0 + i)) >> 12;
        b = (unsigned)(p->w4 * *(short *)((char *)D_8007A5F0 + i)) >> 12;
    } else {
        a = 0;
        b = p->w4;
    }
    return func_8004065C(o, (short)(o->h->x + b), (short)(o->y + a), (short)o->af) != 0;
}
