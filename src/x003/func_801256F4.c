// FUNC 801256f4 320 X003
// MATCHING 801256f4 320
typedef struct H { char p0[2]; short x; } H;
typedef struct O {
    char p0[0x16]; short y;
    char p2[0x40 - 0x18]; H *h;
    char p4[0x8c - 0x44]; int d8c;
    char p5[0x9d - 0x90]; unsigned char b9d;
} O;
extern short D_8007A1F0[], D_8007A5F0[];
extern short func_8004065C(O *, int, int, int);
extern short func_800411CC(O *, short, short);

int func_801256F4(O *o, short k)
{
    unsigned short a, b;
    short v;

    if ((o->b9d & 2) && k == (o->b9d & 1))
        return 1;
    if (k & 1)
        v = -0x18;
    else
        v = 0x18;
    if (o->d8c != 0) {
        a = (unsigned)(v * D_8007A1F0[o->d8c & 0xff]) >> 12;
        b = (unsigned)(v * D_8007A5F0[o->d8c & 0xff]) >> 12;
    } else {
        a = 0;
        b = v;
    }
    if (func_8004065C(o, (short)(o->h->x + b), (short)(o->y + a), k) == 0)
        return func_800411CC(o, (short)(o->h->x + b), (short)(a + o->y + 0x22)) == 0;
    return 1;
}
