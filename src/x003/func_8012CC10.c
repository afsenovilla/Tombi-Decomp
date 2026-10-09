// FUNC 8012cc10 432 X003
// MATCHING 8012cc10 432
#include "TOBJ.H"
typedef struct { char c[12]; } B12;

extern unsigned char D_8009C93F[], D_8009C942[], D_8009CF03[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void *D_801399CC[], *D_801399BC[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void func_8012CC10(TObj *o)
{
    B12 v;
    TObj *p;

    v = *(B12 *)&o->a;
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        o->wac = 9;
        o->state++;
        o->anim = D_801399CC[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        FUN_8001fec0(o);
        if (o->b68 == 0)
            break;
        D_800A603C[0] = 5;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 5;
        o->state++;
        o->animFrame = o->b68 & 1;
        o->anim = D_801399BC[0];
        FUN_8001fe6c(o);
        D_8009CF03[0] = 2;
        o->d90 = (int)FUN_8002dcc8(6, 5, &v);
        break;
    case 2:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->step = 2;
        o->state = 1;
        o->b68 = 1;
        break;
    }
}
