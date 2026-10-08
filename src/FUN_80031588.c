// FUNC 80031588 220 MAIN0
// MATCHING 80031588 220
extern int MulCos(int a, short b);
extern int MulSin(int a, short b);
extern int g3(int a, short b);
extern int g4(int a, short b);

void FUN_80031588(short *p, int a, short *o1, short *o2)
{
    int ang = (short)a;
    int c = MulCos(ang, p[0]);
    int s = MulSin(ang, p[0]);
    short t = p[2];
    short b;
    if (ang < t) b = t + (t - a);
    else b = t - (a - t);
    *o1 = c + g3(b, p[1]);
    *o2 = s + g4(b, p[1]);
}
