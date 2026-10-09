// FUNC 8012cce8 408 X010
// MATCHING 8012cce8 408
#include "TOBJ.H"
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void *D_8013238C;
extern void *D_80132388;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern TObj *FUN_8002dcc8(int, int, Fix16 *);

void func_8012CCE8(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            *(TObj **)&o->d90 = FUN_8002dcc8(2, 3, &o->a);
            o->wac = 0x1f;
            o->animFrame = o->b68 & 1;
            do {} while (0); /* debt: ends the sched block so the a0 copy is not hoisted above the stores */
            o->anim = D_8013238C;
            AnimLoadDuration(o);
        }
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_800A603C[0] = 1;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->wac = 0x1e;
            o->anim = D_80132388;
            o->state++;
        }
        break;
    case 3:
        o->b6b = Rand() & 1;
        o->state = 0;
        break;
    }
}
