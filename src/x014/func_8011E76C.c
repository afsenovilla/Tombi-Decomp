// FUNC 8011e76c 924 X014
// MATCHING 8011e76c 924
#include "TOBJ.H"
extern char D_80077CF4[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern int func_8011D604(TObj *);
extern int func_8011D7B0(TObj *);
extern void func_80116C10(TObj *);

#define TBL(o) (*(void ***)&(o)->wa8)
static __inline__ void SetAnim(TObj *o, int k)
{
    unsigned char *p;
    p = (unsigned char *)o->d90;
    p += k * 4;
    o->box0 = *p;
    p++;
    o->box1 = *p;
    p++;
    o->box2 = *p;
    p++;
    o->box3 = *p;
    o->wac = k;
    o->anim = TBL(o)[k];
    AnimLoadDuration(o);
}

void func_8011E76C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velV = -0x400;
        o->movetab = D_80077CF4;
        o->b68 = 0;
        o->d8c = 0;
        o->b9c = 1;
        o->state++;
        SetAnim(o, 16);
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->w7a);
        func_8011D604(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x30;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, o->w7a);
        func_8011D604(o);
        if (func_8011D7B0(o)) {
            o->b9c = 1;
            o->velV = -0x200;
            o->velH = 0x180;
            o->state++;
            SetAnim(o, 15);
        }
        break;
    case 3:
        if (o->w7a & 1) {
            o->h->raw -= o->velH << 8;
            o->d8c = (o->d8c + 0x14) & 0xff;
        } else {
            o->h->raw += o->velH << 8;
            o->d8c = (o->d8c - 0x14) & 0xff;
        }
        if (func_8011D604(o))
            o->velH = 0;
        o->velV += 0x20;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_8011D7B0(o)) {
                o->timer = 0xf0;
                o->b9c = 0;
                o->d8c = 0;
                o->velH = 0;
                o->animFrame &= 1;
                o->state++;
                SetAnim(o, 13);
                func_80116C10(o);
            }
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step = 2;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
