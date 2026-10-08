// FUNC 80110cac 576 X000
// r11: score 16 with nested inlines body->fc(o,k)->clamp(o,k) (ushort k, int, uchar). Game has plain copies (move t3,v1 / move v1,t3) where ours has andi 0xffff/0xff.
typedef struct S { char p[0xb2]; short wb2; char q[0xc1 - 0xb4]; unsigned char bC1; } S;
typedef struct T { char p[0x10]; short f10, f12, f14, f16; char q[0x66 - 0x18]; unsigned short g66, g68, g6a, g6c; char r[0x74 - 0x6e]; } T;
extern T D_80115500[];
extern unsigned char D_8009D2B3;
extern unsigned char D_8009C990;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D006;
static __inline__ void clamp(S *o, unsigned char k)
{
    if (o->wb2 < -D_80115500[k].f16) o->wb2 = -D_80115500[k].f16;
    if (D_80115500[k].f16 < o->wb2) o->wb2 = D_80115500[k].f16;
}
static __inline__ void fc(S *o, int k)
{
    unsigned short u;
    short v;
    u = o->wb2;
    if ((unsigned short)(u + 0x50) < 0xa1) {
        o->wb2 = 0;
    } else {
        v = u;
        if (D_80115500[k].f14 < v) o->wb2 = u - D_80115500[k].g66;
        else if (D_80115500[k].f12 < v) o->wb2 = u - D_80115500[k].g68;
        else if (D_80115500[k].f10 < v) o->wb2 = u - D_80115500[k].g6a;
        else if (v > 0) o->wb2 = u - D_80115500[k].g6c;
        else if (v < -D_80115500[k].f14) o->wb2 = u + D_80115500[k].g66;
        else if (v < -D_80115500[k].f12) o->wb2 = u + D_80115500[k].g68;
        else if (v < -D_80115500[k].f10) o->wb2 = u + D_80115500[k].g6a;
        else if (v < 0) o->wb2 = u + D_80115500[k].g6c;
    }
    clamp(o, k);
}
static __inline__ void body(S *o)
{
    unsigned short k;
    k = D_8009D2B3 + o->bC1 * 4;
    if (D_8009C990 & 3)
        k = (o->bC1 << 2) | 3;
    if (D_8009CF06)
        k = 8;
    if (D_8009D006)
        k = 8;
    fc(o, k);
}

void func_80110CAC(S *o)
{
    body(o);
}
