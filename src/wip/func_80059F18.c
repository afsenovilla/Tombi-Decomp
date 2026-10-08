// FUNC 80059f18 252 MAIN0
/* size ok; regalloc differs: game v=s2, ang=s4, a/c=s0, h=s1, r2=s3 */
extern int MulCos(int a, int b);
extern int MulNegSinScaled(int a, int b);
void func_80059F18(short *v, int ang, int r, int r2)
{
    int a;
    int h;
    int s;
    ang &= 0xff;
    a = (ang + 0x140) & 0xff;
    h = (short)r >> 1;
    v[0] = MulCos(a, h);
    v[1] = MulNegSinScaled(a, h);
    a = (ang + 0xc0) & 0xff;
    v[2] = MulCos(a, h);
    v[3] = MulNegSinScaled(a, h);
    a = MulCos(ang, (short)r2);
    s = MulNegSinScaled(ang, (short)r2);
    v[4] = v[0] + a;
    v[5] = v[1] + s;
    v[6] = v[2] + a;
    v[7] = v[3] + s;
}
