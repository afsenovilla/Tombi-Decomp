// FUNC 80109038 1116 X016
// MATCHING 80109038 1116
#include "TOBJ.H"
typedef struct { TObj t; char c0[0x20]; short we0; short we2; int de4; char e8[0xe]; short wf6; } PO;
typedef struct { char pad[0x20]; short w20; } P;
extern P *D_8009C330;
extern int D_8009D2E8;
extern char D_80010E34[];
extern unsigned char D_801152E8[];
extern int MulCos(int a, short b);
extern void PlayerSetAnimIfChanged(PO *, int);
extern void ObjGravityStep(PO *);
extern void ObjAddVelY7E(PO *);
extern int AnimAdvance(PO *);
extern int ObjCheckHeadCollision(PO *);
extern short ObjTileCollide(PO *, int, int);
extern void SfxPlay3(int, int);
extern void AnimJump(PO *, short);

#define BA3(o) (((unsigned char *)&(o)->t.da0)[3])
#define BAC(o) (*(unsigned char *)&(o)->t.wac)

void func_80109038(PO *o)
{
    switch (o->t.state) {
    case 0:
        o->t.active = 3;
        o->t.b9c = 1;
        o->t.d8c = 0;
        o->t.b69 = 0;
        o->t.b9e = 0;
        o->t.b69 = 0;
        BAC(o) = 0;
        o->t.wb0 = 0;
        BA3(o) = 2;
        o->t.wb6 = 0;
        o->t.animFrame &= 1;
        o->t.velX = MulCos(0, o->t.wb2);
        o->t.velY = -0x570;
        D_8009C330->w20 = 5;
        PlayerSetAnimIfChanged(o, 0x19);
        if (*(unsigned short *)0x1F8001C8 & 1)
            o->wf6 = (o->t.d->p.whole + 90) / 90 * 90;
        else
            o->wf6 = (o->t.d->p.whole - 90) / 90 * 90;
        o->t.state = 1;
    case 1:
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (o->t.velY > 0) {
            o->t.b9c = 2;
            o->t.state = 2;
        }
        if (ObjCheckHeadCollision(o)) {
            o->t.b9c = 2;
            o->t.velY = 0;
            o->t.velV = 0;
            o->t.state = 2;
        }
        if (*(unsigned short *)0x1F8001C8 & 1) {
            o->t.d->raw += 0x28000;
            if (o->wf6 < o->t.d->p.whole) o->t.d->p.whole = o->wf6;
        } else {
            o->t.d->raw += -0x28000;
            if (o->t.d->p.whole < o->wf6) o->t.d->p.whole = o->wf6;
        }
        break;
    case 2:
        o->t.wb0 = 0;
        o->t.wb6 = 0;
        ObjGravityStep(o);
        if (*(unsigned short *)0x1F8001C8 & 1) {
            o->t.d->raw += 0x28000;
            if (o->wf6 < o->t.d->p.whole) {
                o->t.d->p.whole = o->wf6;
                if (BAC(o) == 0) BAC(o) = 1;
            }
        } else {
            o->t.d->raw += -0x28000;
            if (o->t.d->p.whole < o->wf6) {
                o->t.d->p.whole = o->wf6;
                if (BAC(o) == 0) BAC(o) = 1;
            }
        }
        ObjAddVelY7E(o);
        AnimAdvance(o);
        if (BAC(o) == 2) {
            D_8009D2E8 = o->de4;
            if (o->we0) o->t.active = 3;
            else o->t.active = 1;
            *(signed char *)&o->t.b0f = -8;
            o->t.b04 = 1;
            BA3(o) = 0;
            o->t.step = 0xe;
            o->t.state = 0;
        } else if (o->t.b69 == 1) {
            *(signed char *)&o->t.b0f = -8;
            SfxPlay3(0x1c, 0x7f);
            BA3(o) = 1;
            o->t.anim = D_80010E34;
            AnimJump(o, 2);
            o->t.d->p.whole = o->wf6;
            o->t.d8c = D_801152E8[o->t.wb0];
            BAC(o) = 0;
            o->t.state = 3;
        } else if (ObjTileCollide(o, 0, 0)) {
            SfxPlay3(0x1c, 0x7f);
            BA3(o) = 1;
            o->t.anim = D_80010E34;
            AnimJump(o, 2);
            o->t.d->p.whole = o->wf6;
            o->t.d8c = D_801152E8[o->t.wb0];
            BAC(o) = 0;
            o->t.state = 3;
        }
        break;
    case 3:
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        ObjTileCollide(o, 0, 0);
        if (AnimAdvance(o)) {
            if (o->we0) o->t.active = 3;
            else o->t.active = 1;
            *(signed char *)&o->t.b0f = -8;
            o->t.ba5 = 0;
            BA3(o) = 0;
            o->t.b9c = 0;
            BAC(o) = 0;
            o->t.wb2 = 0;
            o->t.velX = 0;
            o->t.velY = 0;
            o->t.d8c = D_801152E8[o->t.wb0];
            D_8009C330->w20 = 0;
            o->t.b04 = 1;
            o->t.step = 0;
            o->t.state = 0;
        }
        break;
    }
}
