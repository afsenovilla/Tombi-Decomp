// FUNC 8012955c 932 X009
// MATCHING 8012955c 932
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009CDAC, D_8009C93F;
extern unsigned char *D_8009C330;
extern int D_1F8002E8;
extern void *D_8012E078, *D_8012E07C;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_80018790(TObj *);

void func_8012955C(TObj *o)
{
    TObj *pl = &D_800A6038;
    TObj *p;

    switch (o->b04) {
    case 0:
        if (D_8009CDAC == 0 || D_800A6038.a.p.whole < 0x280) {
            o->b04 = 3;
            break;
        }
        o->d3c = D_1F8002E8;
        o->anim = D_8012E078;
        o->w1e = 0xd;
        o->b0a = 2;
        o->b0f = 5;
        o->animFrame = 1;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 0x18;
        o->box3 = 0x30;
        o->b0d = 0;
        o->b68 = 0;
        o->active = 3;
        AnimLoadDuration(o);
        o->step = 0;
        o->b04++;
        break;
    case 1:
        o->visible = 1;
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            {
                short v = o->a.p.whole;
                if (pl->a.p.whole < v + 0x10) pl->a.p.whole = v + 0x10;
            }
            if (o->b68) {
                o->b68 = 0;
                pl->b04 = 5;
                pl->step = 0;
                pl->state = 0;
                D_8009C93F = 1;
                o->anim = D_8012E07C;
                AnimLoadDuration(o);
                o->d90 = (int)FUN_8002dc50(3, 1, 0x64, 0x9c);
                o->step++;
            }
            break;
        case 1:
            p = (TObj *)o->d90;
            if (p->b04 >= 2) {
                p->b04++;
                pl->animFrame = 1;
                D_8009C330[9] = 5;
                pl->b9c = 1;
                pl->velX = -0x400;
                pl->b04 = 6;
                pl->b69 = 0;
                pl->wb2 = 0;
                pl->velH = 0;
                pl->velV = 0;
                pl->velY = 0;
                pl->step = 4;
                pl->state = 0;
                o->w22 = 0x1e;
                o->step++;
            }
            break;
        case 2:
            if (--o->w22 == 0) {
                o->velX = -0x200;
                o->velY = -0x180;
                o->animFrame = 0;
                o->w22 = 0x32;
                o->step++;
            }
            break;
        case 3:
            o->a.raw += o->velX << 8;
            o->y.raw += o->velY << 8;
            if (o->velY < 0x100) o->velY += 0x10;
            if (--o->w22 == 0) o->step++;
            break;
        case 4:
            o->a.p.whole = pl->a.p.whole + 0x20;
            o->y.p.whole = pl->y.p.whole - 8;
            if (pl->b04 == 6) o->step++;
            break;
        case 5:
            if (pl->a.p.whole < 0xee) o->b04++;
            break;
        }
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
