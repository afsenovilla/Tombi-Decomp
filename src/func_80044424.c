// FUNC 80044424 300 MAIN0
// MATCHING 80044424 300
#include "TOBJ.H"
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
#define PTR(o, k) (*(char **)((char *)(o) + (k)))
extern unsigned short DAT_1f80019e;
int func_80044424(TObj *a, TObj *b)
{
    short dy; short sx; short ad; short dx;
    if ((unsigned short)(U16(a->d, 2) - U16(b->d, 2) + 0x2d) >= 0x5b)
        return -1;
    sx = (*(unsigned short*)&b->box0) + (*(unsigned short*)&a->box0);
    dx = U16(a->h, 2) - U16(b->h, 2);
    if ((unsigned short)(dx + sx) > b->box1 + a->box1)
        return -1;
    {
        unsigned short c = ((*(unsigned short*)&a->y.p.whole) - (*(unsigned short*)&b->y.p.whole)) + ((*(unsigned short*)&b->box2) + (*(unsigned short*)&a->box2));
        dy = (*(unsigned short*)&a->y.p.whole) - (*(unsigned short*)&b->y.p.whole);
        if (c > a->box3 + b->box3)
            return -1;
    }
    DAT_1f80019e = 0;
    ad = dx;
    if (dx < 0)
        dx = -dx;
    else
        sx = ((*(unsigned short*)&b->box1) - (*(unsigned short*)&b->box0)) + ((*(unsigned short*)&a->box1) - (*(unsigned short*)&a->box0));
    if ((unsigned short)(sx - dx) < 4)
        return (short)ad >= 0;
    if ((short)dy <= 0)
        return 3;
    return 2;
}
