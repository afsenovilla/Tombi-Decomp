// FUNC 801203cc 176 X009
// MATCHING 801203cc 176
#include "TOBJ.H"
typedef struct {
    TObj o;
    unsigned char pad[0xe4 - 0xc0];
    TObj *de4;
} TObjX;
extern unsigned char D_8009CDCA;
extern short func_80043260(TObjX *, TObj *);
extern void FUN_8001f96c(int, int, int, int);

void func_801203CC(TObjX *o, TObj *e)
{
    short r = func_80043260(o, e);

    if (r != 0 && D_8009CDCA == 1 && r < 3 && *(unsigned char *)&o->o.wac == 1) {
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->b69 = 0;
        e->b0f = o->o.b0f + 1;
        o->de4 = e;
        *(unsigned char *)&o->o.wac = 2;
        FUN_8001f96c(2, o->o.a.p.whole, o->o.y.p.whole, o->o.b.p.whole);
    }
}
