// FUNC 8012d280 664 X003
/* score 2: only `li a0,6` / `li v0,2` swapped before FUN_8002dcc8 in step 1 state 0 (sched1 orders the D_8009CF03 = 2 constant first); tried: store in arg comma exprs, volatile store, block-local k=6, do{}while(0) split, unprototyped/short callee, multi-set var for 2, statement permutations */
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009CE51;
extern unsigned char D_8009CF03;
extern int D_1F8002D0[];
extern void *D_801399C8;
extern void *D_801399BC;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_80018790(TObj *);
extern void func_8012C97C(TObj *);
extern void func_8012CDC0(TObj *);
extern void func_8012CFE0(TObj *);
extern void func_8012CC10(TObj *);

#define SET_ANIM(o, a) do { (o)->anim = (a); AnimLoadDuration(o); } while (0)

void func_8012D280(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        if (D_8009CE51 == 0xff) {
            o->b04 = 3;
            break;
        }
        o->active = 2;
        o->box0 = 10;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->animFrame = 1;
        o->d3c = D_1F8002D0[0];
        o->w1e = 1;
        *(signed char *)&o->b0f = -9;
        o->b0d = 0;
        o->w22 = 0;
        o->b6a = 0;
        o->b0a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->d8c = 0;
        o->wac = 8;
        SET_ANIM(o, D_801399C8);
        o->b04++;
        if ((o->step = D_8009CF03) == 2) {
            o->a.p.whole = 0x842;
            o->y.p.whole = -0x38e;
        }
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            func_8012C97C(o);
            break;
        case 1: {
            V6 v;
            v = *(V6 *)&o->a;
            switch (o->state) {
            case 0:
                ObjSetFacingToPlayer(o);
                o->wac = 5;
                o->state++;
                SET_ANIM(o, D_801399BC);
                D_8009CF03 = 2;
                o->d90 = FUN_8002dcc8(6, 4, &v);
                break;
            case 1:
                AnimAdvance(o);
                p = (TObj *)o->d90;
                if (p->b04 == 2) {
                    p->b04 = 3;
                    o->state = 1;
                    o->b68 = 1;
                    o->step++;
                }
                break;
            }
            break;
        }
        case 2:
            func_8012CDC0(o);
            break;
        case 3:
            func_8012CFE0(o);
            break;
        case 4:
            func_8012CC10(o);
            break;
        }
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
