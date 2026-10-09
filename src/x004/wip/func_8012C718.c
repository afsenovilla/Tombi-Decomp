// FUNC 8012c718 364 X004
/* score 4: only the case 2 tail order: game sh wac; lw anim; sb state (in the load delay); sw anim, ours sb state; lw; sh wac. Scalar D_80135748 (array [0]: 9). Tried: all orders of the tail stores, raw-offset wac/state, scalar externs for the player fields, inline setst(o, anim), block temp for the anim. */
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93F, D_8009C942;
extern void *D_80135748, *D_80135750[];
extern int FUN_8002dcc8(int, int, void *);
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);

void func_8012C718(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A6038.b04 = 5;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->d90 = FUN_8002dcc8(5, 6, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        AnimAdvance(o);
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->wac = 0;
            o->state = 0;
            o->anim = D_80135748;
        }
        break;
    }
}
