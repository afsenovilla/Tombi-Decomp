// FUNC 80031588 220 MAIN0
extern int MulCos(int a, short b);
extern int MulSin(int a, short b);
extern int g3(int a, short b);
extern int g4(int a, short b);

void FUN_80031588(short *p, int a, short *o1, short *o2)
{
    int pad[2];
    int ang = (short)a;
    int c = MulCos(ang, p[0]);
    int s = MulSin(ang, p[0]);
    int t = p[2];
    if (ang < t) ang = t + (t - a);
    else ang = t - (a - t);
    *o1 = c + g3((short)ang, p[1]);
    *o2 = s + g4((short)ang, p[1]);
}
