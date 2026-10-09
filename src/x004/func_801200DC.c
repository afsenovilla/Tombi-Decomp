// FUNC 801200dc 1712 X004
// MATCHING 801200dc 1712
#include "TOBJ.H"
typedef struct { unsigned char _p[0xa]; unsigned char ba, bb; short wc, we; } EX;
extern void FUN_8001faf4(TObj *);
extern void func_8011FEF0(TObj *);
extern int func_8011FDD4(TObj *);
extern void func_8011FB1C(TObj *);
extern TObj *FUN_80018448(void);
extern char D_80077CF4[], D_80077CDC[];
extern unsigned char D_80130FD4[];
extern unsigned short *D_80133BD4[], *D_80133BD0[], *D_80133BD8[], *D_80133BB8[];

#define SPAWN()                                         \
    if (!(o->b69 & 8)) {                                \
        q = FUN_80018448();                             \
        if (q != 0) {                                   \
            q->active = 1;                              \
            q->type = 0x10;                             \
            q->subtype = 1;                             \
            q->a.p.whole = o->a.p.whole;                \
            q->y.p.whole = o->y.p.whole + 0x10;         \
            q->b.p.whole = o->b.p.whole;                \
        }                                               \
    }

#define FALL(dv)                                        \
    FUN_8001faf4(o);                                    \
    func_8011FEF0(o);                                   \
    o->velV += dv;                                      \
    if (o->velV > 0x500) o->velV = 0x500;               \
    o->y.raw += o->velV << 8;

void func_801200DC(TObj *o)
{
    EX *x = (EX *)((char *)o + 0xb4);
    unsigned char *p;
    unsigned short *a;
    TObj *q;

    switch (o->substep) {
    case 0:
        {
            int t = o->d8c;
            o->b9c = 2;
            x->we = 0;
            x->wc = t;
        }
        o->substep++;
        o->b69 = 0;
        o->velV = 0;
        if (o->movetab == 0) o->movetab = D_80077CF4;
        break;
    case 1:
        FALL(0x20)
        x->wc += 8;
        if (x->wc < 0x33) break;
        o->substep++;
        o->b69 = 0;
        x->wc = 0xc0;
        o->wac = 0x1e;
        o->anim = a = D_80133BD4[0];
        goto common;
    case 2:
        FALL(0x20)
        x->wc += 5;
        if (x->wc >= 0x100) x->wc = 0x100;
        if (!func_8011FDD4(o)) break;
        SPAWN()
        o->b69 = 0;
        x->wc = 0x100;
        o->movetab = D_80077CDC;
        o->timer = 2;
        o->wac = 0x1d;
        o->substep++;
        o->anim = a = D_80133BD0[0];
        goto common;
    case 3:
        if (--o->timer != -1) break;
        o->timer = 0;
        o->b69 = 0;
        x->wc = 0;
        o->velV = -0x300;
        o->b9c = 1;
        o->wac = 0x1e;
        o->substep++;
        o->anim = a = D_80133BD4[0];
        goto common;
    case 4:
        FALL(0x40)
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!func_8011FDD4(o)) break;
        SPAWN()
        o->timer = 2;
        o->wac = 0x1d;
        o->substep++;
        o->anim = a = D_80133BD0[0];
        goto common;
    case 5:
        if (--o->timer != -1) break;
        o->timer = 0;
        x->wc = 0;
        o->b9c = 1;
        o->velV = -0x200;
        o->b69 = 0;
        o->wac = 0x1e;
        o->substep++;
        o->anim = a = D_80133BD4[0];
        goto common;
    case 6:
        FALL(0x40)
        if (o->timer == 0) {
            x->wc += 2;
            if (x->wc >= 0x16) {
                x->wc = 0xe0;
                o->timer = 1;
                o->wac = 0x1f;
                {
                    unsigned short *b = D_80133BD8[0];
                    unsigned char *r;
                    o->anim = b;
                    r = &D_80130FD4[b[1] * 4];
                    o->box0 = *r++;
                    o->box1 = *r++;
                    o->box2 = *r++;
                    o->box3 = *r++;
                    o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
                }
            }
        } else {
            x->wc += 4;
            if (x->wc >= 0x100) x->wc = 0x100;
        }
        if (o->velV <= 0) break;
        o->b9c = 2;
        if (!func_8011FDD4(o)) break;
        SPAWN()
        o->b69 = 0;
        x->wc = 0;
        o->timer = 0x14;
        o->wac = 0x17;
        o->substep++;
        o->anim = a = D_80133BB8[0];
    common:
        p = &D_80130FD4[a[1] * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p++;
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 7:
        func_8011FB1C(o);
        if (--o->timer == -1) {
            o->state = x->ba;
            o->substep = x->bb;
            o->b68 = 0;
        }
        break;
    }
    if (o->animFrame) o->d8c = *(unsigned char *)&x->wc;
    else o->d8c = -x->wc & 0xff;
}
