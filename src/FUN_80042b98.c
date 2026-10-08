// FUNC 80042b98 216 MAIN0
// MATCHING 80042b98 216
#include "TOBJ.H"
extern int FUN_800425c4(TObj *, char *, int);
extern void FUN_8001f96c(int, int, int, int);
extern void SfxPlay(int);
int FUN_80042b98(TObj *o, char *q, int r)
{
    int ret = 1;
    q[0x68] = 1;
    q[0x9e] = 0;
    switch (FUN_800425c4(o, q, r)) {
    case 0:
        ret = 2;
        goto L;
    case 3:
    case 6:
    case 9:
    case 11:
    case 12:
    case 13:
        ret = 2;
    case 1:
    case 4:
    case 5:
    case 7:
    case 10:
        SfxPlay(7);
    L:
        FUN_8001f96c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        break;
    case 2:
    case 8:
        ret = 0;
        q[0x68] = 0;
        q[0x9e] = 1;
        q[0x9f] = 0;
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        SfxPlay(6);
        break;
    }
    return ret;
}
