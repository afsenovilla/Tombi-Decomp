// FUNC 80105694 928 X018
// MATCHING 80105694 928
#include "TOBJ.H"
extern TObj *D_8009D2E8;
extern TObj *D_8009C330;
extern unsigned short D_1F8001FC, D_1F8003C8, D_1F8003C6;
extern void FUN_800ef490(TObj *o);
extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern short func_80041EBC(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);
extern void PlayerSetAnimIfChanged(TObj *o, int n);
extern void SfxPlay2(int a, int b);
extern TObj *FUN_800182ac(void);
extern void FUN_80104cd8(TObj *o);
extern void func_80104A04(TObj *o);

void func_80105694(TObj *o)
{
    TObj *p;
    switch (o->substep) {
    case 0:
        p = D_8009D2E8;
        p->d8c = o->d88 - 0xc0;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        FUN_800ef490(o);
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->d84 = 0;
            o->velY = 0;
            o->substep = 1;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            o->d84 = 0;
            o->velY = 0;
            o->substep = 1;
        }
        if ((D_1F8001FC & D_1F8003C8) || (D_1F8001FC & D_1F8003C6)) {
            D_8009C330->animTimer = 0xf;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 1:
        FUN_800ef490(o);
        p = D_8009D2E8;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->b69 != 0 || func_80041EBC(o, o->h->p.whole, (short)(o->y.p.whole + 0x40)) != 0 ||
            (D_1F8001FC & D_1F8003C8) || (D_1F8001FC & D_1F8003C6) || ObjTileCollide(o, 4, 0) != 0) {
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
            break;
        }
        break;
    case 2:
        PlayerSetAnimIfChanged(o, 0xf);
        if (AnimAdvance(o)) {
            TObj *q;
            SfxPlay2(0x22, 0x23);
            q = FUN_800182ac();
            if (q) {
                TObj *r;
                q->active = 1;
                q->type = 2;
                r = D_8009D2E8;
                q->animFrame = (o->animFrame & 1) | 2;
                q->a.p.whole = o->a.p.whole;
                q->y.p.whole = o->y.p.whole + r->box2;
                q->b.p.whole = o->b.p.whole;
                q->b6b = r->b6b;
                q->b1d = D_8009D2E8->b1d;
                q->subtype = D_8009D2E8->subtype;
                q->b0c = D_8009D2E8->b0c;
            }
            D_8009D2E8->b04 = 3;
            *(unsigned char *)&o->wac = 1;
            o->velY = 0;
            o->substep = 3;
        } else {
            FUN_80104cd8(o);
        }
        break;
    case 3:
        PlayerSetAnimIfChanged(o, 0x1d);
        o->d8c = 0x200;
        o->wb2 = 0x100;
        o->b9c = 2;
        o->velY = 0;
        o->substep = 4;
        break;
    case 4:
        func_80104A04(o);
        break;
    }
}
