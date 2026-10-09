// FUNC 8011ce24 600 X003
// MATCHING 8011ce24 600
#include "TOBJ.H"

extern int D_1F8002E0[];
extern unsigned char D_8009D2C2;
extern unsigned char D_8009C93F, D_8009C942;
extern void *D_8013987C[];
extern TObj *ObjAlloc(void);
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018838(TObj *);
extern void func_8011CA70(TObj *);
extern void func_8011CC8C(TObj *);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

void func_8011CE24(TObj *o)
{
    TObj *p;
    int hw;
    unsigned char t = o->b04;

    switch (t) {
    case 0:
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box3 = 0x20;
        o->box2 = 0x10;
        o->wb0 = 0;
        o->ba6 = 0;
        o->ba7 = 0;
        o->w1e = 0xe;
        o->d3c = D_1F8002E0[0];
        *(signed char *)&o->b0f = -12;
        o->b0d = 1;
        o->w08 = 0x7c90;
        if (o->subtype & 1) o->d38 = 0x80;
        else o->d38 = 0;
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->w74 = 0;
        hw = o->h->p.whole;
        o->wb4 = o->y.p.whole;
        o->d30 = hw;
        p = ObjAlloc();
        if (p) {
            p->active = 2;
            p->type = 0x38;
            p->b0a = 2;
            p->d84 = 0;
            p->d88 = 0;
            p->d8c = 0;
            p->a.p.whole = o->a.p.whole;
            p->y.p.whole = o->y.p.whole + 0x28;
            p->b.p.whole = o->b.p.whole;
            p->d30 = o->h->p.whole << 16;
            p->d34 = p->y.p.whole << 16;
            p->d90 = (int)o;
        }
        if (o->b0c & 8) {
            if (D_8009D2C2) {
                D_8009C93F = 1;
                D_8009C942 = 1;
                o->step = 8;
                o->d38 = 0x40;
                o->state = 0;
                o->b0a = 0;
                o->wac = 6;
                o->y.p.whole += 0x30;
                setAnim(o, D_8013987C[0]);
            }
        }
        break;
    case 1:
        ObjCullRegister(o);
        { int k = o->subtype & 4;
        switch (k != 0) {
        case 0:
            func_8011CA70(o);
            break;
        case 1:
            func_8011CC8C(o);
            break;
        }}
        break;
    case 2:
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
