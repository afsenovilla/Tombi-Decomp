// FUNC 80044424 300 MAIN0
// score 14: dy test written with the full expression (no CSE with dy). Left: game CSEs it (subu v1 then move t4,v1) while ours computes dy twice; tried temps/types/order
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_1f80019e;
int func_80044424(TObj *a, TObj *b)
{
    short d; int dy; short sx; short ad; short dx;
    if ((unsigned short)(U16(PTR(a, 0x44), 2) - U16(PTR(b, 0x44), 2) + 0x2d) >= 0x5b)
        return -1;
    sx = U16(b, 0x6c) + U16(a, 0x6c);
    d = U16(PTR(a, 0x40), 2) - U16(PTR(b, 0x40), 2);
    if ((unsigned short)(d + sx) > S16(b, 0x6e) + S16(a, 0x6e))
        return -1;
    dx = d;
    dy = U16(a, 0x16) - U16(b, 0x16);
    if ((unsigned short)((U16(a, 0x16) - U16(b, 0x16)) + (U16(b, 0x70) + U16(a, 0x70))) > S16(a, 0x72) + S16(b, 0x72))
        return -1;
    DAT_1f80019e = 0;
    ad = dx;
    if ((short)d < 0)
        dx = -d;
    else
        sx = (U16(b, 0x6e) - U16(b, 0x6c)) + (U16(a, 0x6e) - U16(a, 0x6c));
    if ((unsigned short)(sx - dx) < 4)
        return (short)ad >= 0;
    if ((short)dy <= 0)
        return 3;
    return 2;
}
