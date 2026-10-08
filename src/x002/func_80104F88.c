// FUNC 80104f88 1804 X002
// MATCHING 80104f88 1804
#include "TOBJ.H"
extern TObj *D_8009D2E8;
extern TObj *D_8009C330;
extern unsigned short D_1F8001FC, D_1F8003C8, D_1F8003C6;
extern unsigned short D_8009D670;
extern short D_8009C944;
extern short D_8009C946[];
extern unsigned short D_8009C960;
extern void FUN_8010eaf8(TObj *o);
extern void FUN_800ef490(TObj *o);
extern void ObjGravityStep(TObj *o);
extern void ObjAddVelY7E(TObj *o);
extern int AnimAdvance(TObj *o);
extern void TileCollideAt(TObj *o, int x, int y);
extern int ObjCheckHeadCollision(TObj *o);
extern void ObjMotionStep(TObj *o);
extern short func_80041EBC(TObj *o, int x, int y);
extern short ObjTileCollide(TObj *o, int a, int b);
extern void ObjSetAnimFromTable(TObj *o);
extern void AnimLoadDuration(TObj *o);
extern void PlayerSetAnimIfChanged(TObj *o, int n);
extern void SfxPlay2(int a, int b);
extern TObj *FUN_800182ac(void);
extern void FUN_80104cd8(TObj *o);
extern void func_80104A04(TObj *o);

void func_80104F88(TObj *o)
{
    TObj *p;
    TObj *q;
    unsigned short t;

    switch (o->substep) {
    case 0:
        D_8009D2E8->animFrame = o->animFrame & 1;
        D_8009D2E8->d8c = o->d88 - 0xc0;
        FUN_8010eaf8(o);
        o->h->raw += o->velX << 8;
        D_8009D2E8->h->p.whole = o->h->p.whole;
        D_8009D2E8->y.p.whole = o->y.p.whole + D_8009D2E8->box2;
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
            if (*(volatile unsigned short *)&D_8009D670 & 0x40) {
                D_8009C330->animTimer = 0xf;
            } else {
                D_8009C330->animTimer = 0xe;
            }
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 1:
        FUN_800ef490(o);
        p = D_8009D2E8;
        p->animFrame = o->animFrame & 1;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole + p->box2;
        p->d8c = o->d88 - 0xc0;
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        ObjMotionStep(o);
        AnimAdvance(o);
        if (o->b69 != 0 || func_80041EBC(o, o->h->p.whole, (short)(o->y.p.whole + 0x30)) != 0 ||
            (D_1F8001FC & D_1F8003C8) || (D_1F8001FC & D_1F8003C6) || ObjTileCollide(o, 4, 0) != 0) {
            if (*(volatile unsigned short *)&D_8009D670 & 0x40) {
                D_8009C330->animTimer = 0xf;
            } else {
                D_8009C330->animTimer = 0xe;
            }
            o->b9c = 0;
            o->velY = 0;
            o->substep = 2;
        }
        break;
    case 2:
        if (D_8009C330->animFrame != D_8009C330->animTimer) {
            D_8009D2E8->state = 7;
            ObjSetAnimFromTable(o);
            AnimLoadDuration(o);
            D_8009C330->animFrame = D_8009C330->animTimer;
        }
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            switch (D_8009D2E8->type) {
            case 0x21:
            case 0x3b: case 0x3c: case 0x3d: case 0x3e:
            case 0x3f: case 0x40: case 0x41: case 0x42:
                break;
            default:
                {TObj *q = FUN_800182ac();
                if (q) {
                    q->active = 1;
                    q->type = 1;
                    switch (D_8009D2E8->type) {
                    case 0:
                        q->subtype = 3;
                        break;
                    case 2:
                        switch (D_8009C960) {
                        case 0: case 1: case 2:
                            q->subtype = 0;
                            break;
                        case 3:
                            q->subtype = 8;
                            break;
                        }
                        break;
                    case 0x3a:
                        q->subtype = 0xc;
                        break;
                    case 0x1a:
                        q->type = 4;
                        q->subtype = 0;
                        break;
                    case 0x27:
                        q->subtype = 0xd;
                        break;
                    case 8:
                        q->subtype = 1;
                        break;
                    case 0x1e:
                        q->subtype = 9;
                        break;
                    case 0xa:
                        q->subtype = 4;
                        q->b0d = D_8009D2E8->b0d;
                        break;
                    case 0x13:
                        q->subtype = 5;
                        break;
                    case 0x28:
                        q->subtype = 7;
                        break;
                    case 0xb:
                        q->subtype = D_8009D2E8->subtype << 1;
                        break;
                    case 0x38:
                        q->subtype = D_8009D2E8->subtype + 10;
                        break;
                    }
                    t = o->animFrame & 1;
                    q->animFrame = t;
                    if (*(volatile unsigned short *)&D_8009D670 & 0x40) {
                        q->animFrame = t | 2;
                    }
                    q->a.p.whole = o->a.p.whole;
                    q->y.p.whole = o->y.p.whole;
                    q->b.p.whole = o->b.p.whole;
                    {
int c = o->animFrame & 1; Fix16 *h = q->h; int w = h->p.whole; int r;
                        if (c) r = w - 0x10; else r = w + 0x10; h->p.whole = r;
                    }
                }}
                break;
            }
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 0x1d);
            o->d8c = 0x200;
            o->substep = 3;
        } else {
            FUN_80104cd8(o);
        }
        break;
    case 3: {
        TObj *p = D_8009D2E8;
        switch (p->type) {
        case 0x13:
            for (q = (TObj *)p->d90; q != 0; q = (TObj *)q->d90) {
                q->b04 = 3;
            }
            for (q = (TObj *)D_8009D2E8->d94; q != 0; q = (TObj *)q->d94) {
                q->b04 = 3;
            }
            break;
        case 0x21:
            p->b04 = 2;
            D_8009D2E8->step = 2;
            D_8009D2E8->state = 0;
            break;
        case 0x3b: case 0x3c: case 0x3d: case 0x3e:
        case 0x3f: case 0x40: case 0x41: case 0x42:
            D_8009D2E8->b04 = 2;
            D_8009D2E8->step = 3;
            D_8009D2E8->state = 0;
            {TObj *q = D_8009D2E8; unsigned short t;
            t = o->animFrame & 1;
            q->animFrame = t;
            if (*(volatile unsigned short *)&D_8009D670 & 0x40) {
                q->animFrame = t | 2;
            }}
            break;
        default:
            D_8009D2E8->b04 = 3;
            break;
        }
        o->wb2 = 0x100;
        o->b9c = 2;
        *(unsigned char *)&o->wac = 1;
        o->velY = 0;
        o->substep = 4;
        break; }
    case 4:
        func_80104A04(o);
        break;
    }
}
