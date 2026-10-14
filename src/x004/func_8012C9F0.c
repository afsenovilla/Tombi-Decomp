// FUNC 8012c9f0 1960 X004
// MATCHING 8012c9f0 1960
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern unsigned short D_800A6066;
extern short D_1F80016E;
extern void *D_8013577C[], *D_80135784[];
extern short D_8007A5F0[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, void *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_8003e474(int, int, void *);

void func_8012C9F0(TObj *o)
{
    short y;

    switch (o->state) {
    case 0:
        if (GCD.bce62 != 0xff) {
            o->b04 = 3;
            break;
        }
        o->active = 2;
        o->box0 = 0x20;
        o->box1 = 0x40;
        o->b68 = 0;
        if (((unsigned char*)&GCD.bd086)[0] == 0) {
            o->state = 1;
        } else if (((unsigned char*)&GCD.bd086)[0] == 1) {
            o->state = 4;
        } else {
            o->b04 = 3;
        }
        break;
    case 1:
        if (GVARS.b40 != 0 && GVARS.b41 != 0x90) break;
        y = D_1F80016E;
        if (!((y < -0x40b && ((*(unsigned char*)&PLAYER.obj.waa) || PLAYER.obj.b9e)) || y == -0x450)) break;
        o->state++;
        ((unsigned char*)&GCD.bd086)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        o->d90 = showMessageBoxDirect(3, 0, &o->a);
        o->wac = 2;
        o->animFrame = ~D_800A6066 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 2:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        if (GCD.bce63 == 0) {
            o->timer = 0x140;
            o->state++;
            setEventStarted(0xbf, 0, 0);
        } else {
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            o->wac = 0;
            o->anim = D_8013577C[0];
            o->state = 0;
        }
        break;
        }
    case 3:
        if (--o->timer != -1) break;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        o->wac = 0;
        o->anim = D_8013577C[0];
        o->state = 0;
        break;
    case 4:
        if (GVARS.b40 != 0 && GVARS.b41 == 0x90) {
            o->state = 6;
            break;
        }
        if (o->b68 == 0) break;
        o->state++;
        ((unsigned char*)&GCD.bd086)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->d90 = showMessageBoxDirect(3, 0, &o->a);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 5:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 0;
        o->anim = D_8013577C[0];
        o->state = 0;
        break;
        }
    case 6:
        o->state++;
        GVARS.b40 = 0;
        FUN_80026e0c(0x90, 1);
        o->d90 = showMessageBoxDirect(3, 1, &o->a);
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wac = 2;
        o->animFrame = ~D_800A6066 & 1;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 7:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        break;
        }
    case 8:
        if (o->animFrame == 0) o->velH = -0x40;
        else o->velH = 0x40;
        o->d88 = 0;
        o->velV = 0x200;
        o->animFrame = 1 - o->animFrame;
        o->state++;
        break;
    case 9:
        o->h->raw += o->velH << 8;
        o->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
        o->d88 += 2;
        if (o->d88 < 0x60) break;
        o->wac = 2;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 10:
        o->state++;
        o->d90 = showMessageBoxDirect(3, 2, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 11:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        o->timer = 0x140;
        setEventComplete(0xbf, 0);
        break;
        }
    case 12:
        if (--o->timer == -1) o->state++;
        break;
    case 13:
        o->state++;
        o->d90 = showMessageBoxDirect(3, 3, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 14:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->anim = D_8013577C[0];
        o->timer = 0x78;
        break;
        }
    case 15:
        if (--o->timer == -1) o->state++;
        if (o->timer == 0x64 || o->timer == 0x50) FUN_8003e474(0x37, 0, &o->a);
        break;
    case 16:
        o->state++;
        o->d90 = showMessageBoxDirect(3, 4, &o->a);
        o->wac = 2;
        o->anim = D_80135784[0];
        AnimLoadDuration(o);
        break;
    case 17:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->wac = 0;
        o->state++;
        o->animFrame = 1 - o->animFrame;
        o->anim = D_8013577C[0];
        break;
        }
    case 18:
        if (o->animFrame == 0) o->velH = 0x280;
        else o->velH = -0x280;
        o->velV = -0x380;
        o->velY = 0x40;
        o->state++;
        break;
    case 19:
        if (!o->visible) {
            o->state++;
            break;
        }
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        if (o->velV < 0x400) o->velV += o->velY;
        break;
    case 20:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        ((unsigned char*)&GCD.bd086)[0] = 2;
        o->state = 0;
        break;
    }
}
