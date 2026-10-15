// FUNC 8012ef3c 760 X003
// MATCHING 8012ef3c 760
/* includes the csv piece 8012F220 (the shared epilogue) */
#include "TOBJ.H"

extern TObj PLAYER;
extern char D_80077CD0[];
extern void *D_8013A6C0[], *D_8013A6C4[];
extern unsigned short D_8007A3F0[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void spawnItemNotification(int, int);
extern void applyAnimVelocityX(TObj *, int);
extern void applyFrameVelocityX(TObj *);
extern int TileCollideAt(TObj *, int, int);
extern void probeCollisionAtDepthA(TObj *, int, int);
extern void func_8012D66C(TObj *);

void throwerEnemyGrabbedB(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b9c = 0;
        spawnItemNotification(1, 6);
        *(signed char *)&o->b0f = -7;
        o->velV = 0x300;
        o->movetab = D_80077CD0;
        o->b68 = 0;
        o->d8c = 0;
        o->wac = 0xa;
        o->state++;
        o->anim = D_8013A6C0[0];
        AnimLoadDuration(o);
        applyAnimVelocityX(o, *(unsigned short *)&o->w7a);
        break;
    case 1:
        o->state++;
        o->velV -= 0x20;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1 || (short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) o->b69 = 0;
        applyAnimVelocityX(o, *(unsigned short *)&o->w7a);
        break;
    case 2:
        o->state++;
        PLAYER.state = 2;
        *(unsigned char *)&PLAYER.wac = 3;
        PLAYER.b9c = 0;
    case 3:
        o->velV -= 0x20;
        if (o->velV < 0) o->state++;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1 || (short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) o->b69 = 0;
        AnimAdvance(o);
        o->animFrame = PLAYER.animFrame & 1;
        applyFrameVelocityX(o);
        func_8012D66C(o);
        break;
    case 4:
        o->velV -= 0x20;
        if (o->velV < -0x300) {
            o->velV = -0x300;
            o->state++;
        }
        o->y.raw += o->velV << 8;
        probeCollisionAtDepthA(o, o->h->p.whole, (short)(o->y.p.whole - 0x20));
        AnimAdvance(o);
        o->animFrame = PLAYER.animFrame & 1;
        applyFrameVelocityX(o);
        func_8012D66C(o);
        break;
    case 5:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        AnimAdvance(o);
        o->animFrame = PLAYER.animFrame & 1;
        applyFrameVelocityX(o);
        func_8012D66C(o);
        break;
    case 6:
        o->animFrame = PLAYER.animFrame & 1;
        o->wac = 0xb;
        o->state++;
        o->anim = D_8013A6C4[0];
        AnimLoadDuration(o);
        break;
    case 7:
        break;
    }
}
