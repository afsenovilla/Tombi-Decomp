// FUNC 80125c84 288 X000
// MATCHING 80125c84 288
#include "TOBJ.H"
extern short FUN_8004461c(TObj *, TObj *);
extern int FUN_80042b98(TObj *, TObj *);
extern void FUN_8002b920(TObj *);
extern void FUN_800e9f74(int, int, int, int);
extern unsigned char DAT_800a6038, DAT_800a603c, DAT_800a603d, DAT_800a603e;
extern short DAT_1f80019e;

void FUN_80125c84(TObj *a, TObj *o)
{
    int r;
    unsigned short t;
    if (FUN_8004461c(a, o) != -1) {
        if (o->b6a == 2) {
            DAT_800a6038 = 1;
            DAT_800a603c = 1;
            DAT_800a603d = 0;
            DAT_800a603e = 0;
        }
        o->b6a = 0;
        r = FUN_80042b98(a, o);
        if (r != 0) {
            if (r == 1) {
                o->active = 3;
                o->animFrame = 1 - (a->animFrame & 1);
                if (o->b04 != 2)
                    FUN_8002b920(o);
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            } else {
                FUN_800e9f74(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                o->active = 2;
                t = a->animFrame;
                o->b04 = 2;
                o->step = 2;
                o->state = 0;
                o->animFrame = 1 - (t & 1);
            }
        }
        DAT_1f80019e = 0;
    }
}
