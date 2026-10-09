// FUNC 801213d0 960 X001
// MATCHING 801213d0 960
#include "TOBJ.H"

typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7, b8;
    char p9[0x20 - 9];
    short w20;
    char p22[0x2c - 0x22];
    unsigned short w2c;
    unsigned short w2e;
} P;

extern P *D_8009C330;
extern TObj *D_8009D2E8;
extern unsigned short D_8009D670;
extern short D_8009C944[], D_8009C946[];
extern unsigned short D_1F8001FC, D_1F8003C8, D_1F8003C6;
extern TObj *FUN_800182ac(void);
extern void FUN_8010eaf8(TObj *);
extern void FUN_80104dc8(TObj *);
extern void func_80104EA0(TObj *);
extern void FUN_800ef490(TObj *);
extern void ObjMotionStep(TObj *);
extern int AnimAdvance(TObj *);
extern short func_80041EBC(TObj *, short, short);
extern short ObjTileCollide(TObj *, int, int);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimLoadDuration(TObj *);
extern void SfxPlay2(int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_80104cd8(TObj *);
extern void func_80104A04(TObj *);

void func_801213D0(TObj *o)
{
    TObj *n;
    TObj *e;
    unsigned short f;

    switch (o->substep) {
    case 0:
        D_8009D2E8->d84 = o->d88 - 0xc0;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8010eaf8(o);
        o->h->raw += o->velX << 8;
        FUN_80104dc8(o);
        func_80104EA0(o);
        break;
    case 1:
        FUN_800ef490(o);
        e = D_8009D2E8;
        e->h->p.whole = o->h->p.whole;
        e->y.p.whole = o->y.p.whole + e->box2;
        e->d84 = o->d88 - 0xc0;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        ObjMotionStep(o);
        AnimAdvance(o);
        if (o->b69 == 0 && func_80041EBC(o, o->h->p.whole, o->y.p.whole + 0x30) == 0
            && (D_1F8001FC & D_1F8003C8) == 0 && (D_1F8001FC & D_1F8003C6) == 0
            && ObjTileCollide(o, 4, 0) == 0) break;
        if (*(volatile unsigned short *)&D_8009D670 & 0x40) D_8009C330->w2c = 0xf;
        else D_8009C330->w2c = 0xe;
        o->b9c = 0;
        o->velY = 0;
        o->substep = 2;
        break;
    case 2:
        if (D_8009C330->w2e != D_8009C330->w2c) {
            ObjSetAnimFromTable(o);
            AnimLoadDuration(o);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        if (AnimAdvance(o)) {
            SfxPlay2(0x22, 0x23);
            n = FUN_800182ac();
            if (n) {
                n->active = 1;
                n->type = 1;
                n->subtype = 5;
                f = o->animFrame & 1;
                n->animFrame = f;
                if (*(volatile unsigned short *)&D_8009D670 & 0x40) n->animFrame = f | 2;
                n->a.p.whole = o->a.p.whole;
                n->y.p.whole = o->y.p.whole;
                n->b.p.whole = o->b.p.whole;
                n->h->p.whole += (o->animFrame & 1) ? -0x10 : 0x10;
            }
            o->velY = 0;
            PlayerSetAnimIfChanged(o, 0x1d);
            o->d8c = 0x200;
            o->substep = 3;
        } else {
            FUN_80104cd8(o);
        }
        break;
    case 3:
        D_8009D2E8->b04 = 2;
        D_8009D2E8->step = 4;
        D_8009D2E8->state = 0;
        D_8009D2E8->substep = 0;
        o->wb2 = 0x100;
        o->velY = 0;
        o->b9c = 2;
        *(unsigned char *)&o->wac = 1;
        o->substep = 4;
        break;
    case 4:
        func_80104A04(o);
        break;
    }
}
