// FUNC 80040278 116 MAIN0
typedef struct { char p[0x44]; short *q; } TO;
extern int SC278;
extern int g1(int a, int b);
extern short g2(TO *o, int a, int b);

short TileCollideAt(TO *o, short a, short b)
{
    SC278 = g1(a, o->q[1]);
    return g2(o, a, b);
}
