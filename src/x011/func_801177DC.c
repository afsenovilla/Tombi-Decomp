// FUNC 801177dc 1112 X011
// MATCHING 801177dc 1112
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009D082[];
extern short D_1F8001C6;
extern void AnimLoadDuration(TObj *);
extern int showMessageBoxDirect(int, int, V6 *);
extern void addItemToInventory(int, int, int);
extern void setEventComplete(int, int);
extern TObj *allocObjectLayer5(void);

void func_801177DC(TObj *o)
{
    V6 v;
    TObj *p;
    TObj *q;

    switch (o->state) {
    case 0:
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_80119C48[o->subtype].anims[2];
        AnimLoadDuration(o);
        o->d90 = showMessageBoxDirect(3, 7, &v);
        o->state++;
        break;
    case 1:
    {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 8, &v);
        o->state++;
        break;
    }
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        addItemToInventory(0x33, 1, 1);
        setEventComplete(0x68, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        v = *(V6 *)&o->a;
        o->d90 = showMessageBoxDirect(3, 0x14, &v);
        o->state++;
        break;
    case 4:
    {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_1F8001C6 = 0;
        D_8009C942[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        q = allocObjectLayer5();
        if (q != 0) {
            q->active = 2;
            q->type = 0xf;
            q->subtype = 0x33;
            q->b0c = 0x80;
            q->b0f = 0;
            q->animFrame = 0;
        }
        o->anim = D_80119C48[o->subtype].anims[1];
        AnimLoadDuration(o);
        o->state++;
        break;
    }
    case 5:
        if (D_8009D082[0] != 1) break;
        o->timer = 8;
        o->state++;
        break;
    case 6:
        if (--o->timer != -1) break;
        o->animFrame = 1;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->timer = 0x20;
        o->velV = -0x280;
        o->velY = 0x20;
        o->state++;
        break;
    case 7:
        if (--o->timer == -1) {
            o->velX = -0x200;
            o->timer = 0x3c;
            D_8009D082[0] = 4;
            o->state++;
            break;
        }
        o->a.raw -= 0x4000;
        o->y.raw += o->velV << 8;
        o->velV += o->velY;
        break;
    case 8:
        o->a.raw += -0x18000;
        if (--o->timer != 0) break;
        o->state++;
        break;
    case 9:
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->animFrame = o->wbc;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->b04 = 2;
        break;
    }
}
