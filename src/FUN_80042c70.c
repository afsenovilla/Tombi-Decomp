// FUNC 80042c70 236 MAIN0
// MATCHING 80042c70 236
#include "TOBJ.H"
extern int FUN_800425c4();
extern void SfxPlay(int);
extern void FUN_8001f96c(int, int, int, int);

int FUN_80042c70(TObj *o, unsigned char *q, int c)
{
    int r = 1;
    q[0x68] = 1;
    q[0x9e] = 0;
    switch (FUN_800425c4(o, q, c)) {
    case 0:
        r = 2;
        goto M;
    case 1: case 4: case 5: case 7: case 10:
        SfxPlay(7);
        goto L;
    case 3: case 6: case 9: case 11: case 12: case 13:
        r = 2;
        SfxPlay(7);
    M:
        *(short *)(q + 0x98) = 0;
    L:
        FUN_8001f96c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        return r;
    case 2: case 8:
        r = 0;
        q[0x68] = 0;
        q[0x9e] = 1;
        q[0x9f] = 0;
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        SfxPlay(6);
        break;
    }
    return r;
}
