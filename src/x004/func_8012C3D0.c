// FUNC 8012c3d0 420 X004
// MATCHING 8012c3d0 420
#include "TOBJ.H"

extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CE22[], D_8009CE3D[];
extern void *D_80135750[];
extern void *D_80135748[];
extern TObj *FUN_8002dcc8(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);

void func_8012C3D0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        if (D_8009CE22[0] && D_8009CE3D[0] != 0xff) o->state = 3;
        else o->state = 1;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->d90 = (int)FUN_8002dcc8(5, 7, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 3:
        if (o->b68) {
            o->state++;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            D_800A603C[0] = 5;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->d90 = (int)FUN_8002dcc8(5, 8, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
    case 4:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->wac = 0;
            o->anim = D_80135748[0];
            o->state = 0;
        }
        break;
    }
}
