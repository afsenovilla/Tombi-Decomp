// FUNC 8011f4e0 880 X014
// MATCHING 8011f4e0 880
#include "TOBJ.H"
extern TObj *D_8009C948;
extern unsigned char D_8009C93A, D_8009C93E, D_8009C942;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018790(TObj *);
extern void func_80116C10(TObj *);
extern void func_8011D9D0(TObj *);
extern void func_8011F218(TObj *);
extern void func_8011DC5C(TObj *);
extern void func_8011E550(TObj *);
extern void func_8011DB10(TObj *);
extern void func_8011E76C(TObj *);
extern void func_8011EB08(TObj *);
extern void func_8011EE50(TObj *);

#define WC2(o) (*(unsigned short *)((char *)(o) + 0xc2))

static __inline__ void setanim(TObj *o, short k)
{
    unsigned char *p;
    p = (unsigned char *)o->d90;
    p += k * 4;
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p;
    o->box3 = p[1];
    o->wac = k;
    o->anim = ((void **)*(int *)&o->wa8)[k];
    FUN_8001fe6c(o);
}

void func_8011F4E0(TObj *o)
{
    unsigned char *p;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            FUN_8001f8e4(o);
            func_8011D9D0(o);
            if (o->subtype == 0) {
                setanim(o, 0);
            } else {
                setanim(o, 2);
            }
            D_8009C948 = o;
            o->step++;
            break;
        case 1:
            if (D_8009C93A) func_8011F218(o);
            break;
        }
        break;
    case 1:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            func_8011DC5C(o);
            break;
        case 1:
            func_8011E550(o);
            break;
        case 2:
            func_8011DB10(o);
            break;
        }
        o->wb4++;
        break;
    case 2:
        if (WC2(o) == 1) {
            D_8009C93E = 0;
            D_8009C942 = 0;
            WC2(o) = 0;
        }
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            func_8011E76C(o);
            break;
        case 1:
            func_8011EB08(o);
            break;
        case 2:
            switch (o->state) {
            case 0:
                func_80116C10(o);
                o->d8c = 0;
                o->state++;
                setanim(o, 0xe);
                break;
            case 1:
                if (FUN_8001fec0(o)) {
                    o->b68 = 0;
                    o->active = 1;
                    o->b04 = 1;
                    o->step = 0;
                    o->state = 0;
                    o->substep = 0;
                }
                break;
            }
            break;
        case 3:
            func_8011EE50(o);
            break;
        case 4:
            if (o->state == 1) o->visible = 0;
            break;
        }
        o->wb4++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
