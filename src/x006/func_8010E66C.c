// FUNC 8010e66c 684 X006
// MATCHING 8010e66c 684
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
extern TObj *D_8009C330;
extern short D_8009C944;
extern short D_8009C946[];
extern int MulCos(int a, int b);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern void SfxPlay2(int a, int b);
extern void func_80030034(int);
extern int AnimAdvance(TObj *);
extern void func_8010F400(TObj *);
extern void func_8010EAF8(TObj *);
extern void ObjMotionStep(TObj *);
extern void ObjAddVelY7E(TObj *);
extern void ObjTileCollide(TObj *, int, int);
extern void func_800EEC40(TObj *);
extern int ObjCheckHeadCollision(TObj *);

void func_8010E66C(TObj *o)
{
    int v;
    switch (o->state) {
    case 0:
        o->timer = 0x31;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        B(o, 0xc3) = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        B(o, 0xa0) = 0;
        B(o, 0xa1) = 0;
        o->wb6 = 0;
        o->velX = MulCos(0, o->wb2);
        o->velY = -10;
        B(D_8009C330, 8) = 0;
        D_8009C330->animTimer = 4;
        D_8009C330->timer = 0;
        D_8009C330->step = 0;
        D_8009C330->b1d = 0;
        B(o, 0xac) = 0;
        ObjSetAnimFromTable(o);
        AnimLoadDuration(o);
        D_8009C330->animFrame = D_8009C330->animTimer;
        SfxPlay2(2, 4);
        func_80030034(2);
        o->state = 1;
    case 1:
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        AnimAdvance(o);
        func_8010F400(o);
        func_8010EAF8(o);
        o->h->raw += o->velX << 8;
        o->velY -= 0x10;
        if (o->velY < -0x800)
            o->velY = -0x800;
        if (B(o, 0xcc) == 3)
            o->state = 2;
        ObjAddVelY7E(o);
        break;
    case 2:
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        AnimAdvance(o);
        func_8010F400(o);
        ObjMotionStep(o);
        break;
    default:
        return;
    }
    ObjTileCollide(o, 0, 0);
    if (o->velY >= -0x383)
        func_800EEC40(o);
    if (ObjCheckHeadCollision(o) || o->velY > 0) {
        o->d84 = 0;
        v = 0x10;
        if (o->animFrame & 1)
            v = 0xf0;
        o->d88 = v;
        B(o, 0xac) = 1;
        o->b9c = 2;
        o->timer = 10;
        o->step = 2;
        o->state = 3;
    }
    o->b69 = 0;
}
