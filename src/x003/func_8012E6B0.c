// FUNC 8012e6b0 1148 X003
// MATCHING 8012e6b0 1148
#include "TOBJ.H"
extern unsigned char D_80135E90[], D_80135E80[], D_80135E78[];
extern void *D_8013A6D4[];
extern void *D_8013A698[];
extern unsigned short D_8007A3F0[];
extern int FUN_8001f9e0(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *FUN_800183b8(void);

static __inline__ void spawn(TObj *o, int sub)
{
    TObj *n = FUN_800183b8();
    if (n) {
        n->active = 2;
        n->type = 0x39;
        n->subtype = sub;
        n->d8c = (o->velX << 5) & 0xf0;
        if (o->animFrame) n->a.p.whole = o->a.p.whole - 0xe;
        else n->a.p.whole = o->a.p.whole + 0xe;
        n->y.p.whole = o->y.p.whole;
        n->b.p.whole = o->b.p.whole;
        n->d90 = (int)o;
    }
}

void func_8012E6B0(TObj *o)
{
    TObj *n;
    unsigned int d;
    unsigned short e;

    switch (o->state) {
    case 0:
        if (o->animFrame) o->velY = D_80135E90[FUN_8001f9e0() & 0xf];
        else o->velY = D_80135E80[FUN_8001f9e0() & 0xf];
        {
            int a = o->animFrame << 2;
            d = ((unsigned short)o->velY - a) & 7;
            o->velX = a;
        }
        e = d;
        if (d != 0) {
            if (e < 4) o->velV = 1;
            else o->velV = -1;
        } else {
            o->velV = 0;
        }
        o->timer = 0x1e;
        o->w22 = 5;
        o->wac = 0xf;
        o->state++;
        o->anim = D_8013A6D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
    case 4:
        if (--o->w22 == -1) {
            o->w22 = 5;
            spawn(o, 0);
        }
        if (--o->timer == -1) {
            o->timer = 0x1e;
            if (o->velX == o->velY) {
                if (o->state == 4) {
                    o->timer = 10;
                    o->state = 5;
                } else {
                    o->w22 = 2;
                    o->timer = 0x78;
                    o->wac = D_80135E78[o->velX];
                    o->state++;
                }
            } else {
                o->velX = (o->velX + o->velV) & 7;
                o->wac = D_80135E78[o->velX];
            }
            o->anim = D_8013A698[o->wac];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        if (--o->w22 == -1) {
            o->w22 = 2;
            spawn(o, 1);
        }
        if (--o->timer == -1) {
            o->timer = 0x1e;
            o->w22 = 5;
            o->state++;
        }
        break;
    case 3:
        o->state++;
        o->velY = o->animFrame << 2;
        if (o->velV) {
            if (o->velV < 0) o->velV = 1;
            else o->velV = -1;
        }
        break;
    case 5:
        if (--o->timer == -1) {
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
    AnimAdvance(o);
    o->ba7 += 2;
    o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
    o->velH -= 8;
    if (o->velH < 0x40) o->velH = 0x40;
    if (o->animFrame) o->a.raw -= o->velH << 8;
    else o->a.raw += o->velH << 8;
}
