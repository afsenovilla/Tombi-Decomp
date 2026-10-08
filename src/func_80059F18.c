// FUNC 80059f18 252 MAIN0
// MATCHING 80059f18 252
extern int MulCos(int a, int b);
extern int MulNegSinScaled(int a, int b);
void func_80059F18(short *v, int ang, int r, int r2)
{
    int a, b2;
    int h;
    int s;
    int c;
    ang &= 0xff;
    a = ang + 0x140;
    a &= 0xff;
    h = (short)r >> 1;
    v[0] = MulCos(a, h);
    v[1] = MulNegSinScaled(a, h);
    b2 = (ang + 0xc0) & 0xff;
    v[2] = MulCos(b2, h);
    v[3] = MulNegSinScaled(b2, h);
    c = MulCos(ang, (short)r2);
    s = MulNegSinScaled(ang, (short)r2);
    a = c;
    v[4] = v[0] + a;
    v[5] = v[1] + s;
    v[6] = v[2] + a;
    v[7] = v[3] + s;
}
