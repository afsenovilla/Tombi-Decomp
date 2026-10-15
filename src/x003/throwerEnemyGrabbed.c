// FUNC 8012f234 380 X003
// MATCHING 8012f234 380
#include "player.h"
#include "TOBJ.H"
extern void *D_8013A70C[];
extern void spawnItemNotification(int, int);
extern void FUN_8001fe6c(TObj *);
extern short FUN_80040278(TObj *, short, short);

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void throwerEnemyGrabbed(TObj *o)
{
    switch (o->state) {
    case 0:
        spawnItemNotification(1, 6);
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        o->velV = 0;
        o->wac = 0x1d;
        o->state++;
        o->anim = D_8013A70C[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        o->state++;
        o->velV += 0x20;
        if (o->velV > 0x300) o->velV = 0x300;
        o->y.raw += o->velV << 8;
        land(o);
        break;
    case 2:
        o->state++;
        PLAYER.obj.state = 2;
        (*(unsigned char*)&PLAYER.obj.wac) = 3;
    case 3:
        o->velV += 0x20;
        if (o->velV > 0x300) o->velV = 0x300;
        o->y.raw += o->velV << 8;
        land(o);
        break;
    case 4:
        o->state++;
        break;
    case 5:
        break;
    }
}
