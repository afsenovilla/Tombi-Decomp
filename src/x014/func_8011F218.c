// FUNC 8011f218 712 X014
// MATCHING 8011f218 712
#include "TOBJ.H"

typedef struct { short v[6]; } V6;
extern unsigned char D_8009C93F[], D_8009D00F;
extern unsigned char D_8009CF0B[];
extern unsigned short D_8009C962[];
extern short D_80125E20[];
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern TObj *FUN_8002dcc8(int, int, V6 *);

static __inline__ void SetAnim(TObj *o, short n)
{
    unsigned char *p;
    p = (unsigned char *)o->d90;
    p += n * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->wac = n;
    o->anim = (*(void ***)&o->wa8)[n];
    AnimLoadDuration(o);
}

void func_8011F218(TObj *o)
{
    V6 v;
    TObj *e;

    ObjCullRegister(o);
    AnimAdvance(o);
    switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        D_8009C93F[0] = 1;
        o->timer = 0x3c;
        if (D_8009CF0B[D_8009C962[0]] == 2) {
            o->state = 4;
        } else {
            D_8009D00F = 2;
            D_8009CF0B[D_8009C962[0]] = 2;
            o->state++;
        }
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
            SetAnim(o, 0x13);
            D_8009D00F = 0;
        }
        break;
    case 2:
        v = *(V6 *)&o->a;
        o->da0 = (int)FUN_8002dcc8(2, D_80125E20[D_8009C962[0]], &v);
        o->state++;
        break;
    case 3:
        e = (TObj *)o->da0;
        if (e->b04 == 2) {
            e->b04 = 3;
            o->timer = 0x3c;
            o->state++;
        }
        break;
    case 4:
        if (--o->timer == -1) {
            D_8009C93F[0] = 0;
            o->b04++;
            o->step = 0;
            o->state = 0;
            if (o->subtype == 0) SetAnim(o, 0);
            else SetAnim(o, 2);
        }
        break;
    }
}
