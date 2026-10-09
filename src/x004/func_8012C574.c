// FUNC 8012c574 420 X004
// MATCHING 8012c574 420
#include "TOBJ.H"

extern unsigned char D_8009CE22[];
extern unsigned char D_8009CE3D[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_800A603C[];
extern unsigned char D_800A603D[];
extern unsigned char D_800A603E[];
extern void *D_80135784[];
extern void *D_8013577C[];
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void func_8012C574(TObj *o)
{
    int n, m;
    TObj *e;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (D_8009CE22[0] && D_8009CE3D[0] != 0xff)
            o->state = 3;
        else
            o->state = 1;
        break;
    case 1:
        if (o->b68) {
            m = 5;
            n = 4;
            goto common;
        }
        break;
    case 3:
        if (o->b68) {
            m = 5;
            n = 5;
        common:
            o->state++;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->d90 = FUN_8002dcc8(m, n, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135784[0];
            FUN_8001fe6c(o);
        }
        break;
    case 2:
    case 4:
        FUN_8001fec0(o);
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->wac = 0;
            o->anim = D_8013577C[0];
            o->state = 0;
        }
        break;
    }
}
