// FUNC 8012ff0c 916 X000
// MATCHING 8012ff0c 916
// probably: bitingPlantStep3: subtype 0/1/2 jaw offsets d88, subtype 1 puts the player on the plant and advances PLAYER.state (the bite carry)
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern unsigned char D_800A6066[];
extern short D_800A6066s;
extern void *BITING_PLANT_ANIMS;

void func_8012FF0C(TObj *o)
{
    switch (o->state) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->d88 = 0;
            o->b6a = 0;
            break;
        case 1:
            o->d88 = 0x10;
            ((short*)&PLAYER.obj.a.p.whole)[0] = o->h->p.whole;
            ((short*)&PLAYER.obj.y.p.whole)[0] = o->y.p.whole - 10;
            o->b6a = 0;
            break;
        case 2:
            o->d88 = -0x10;
            o->b6a = 0;
            break;
        default:
            o->b6a = 0;
            break;
        }
        o->b69 = 0;
        o->b6b = D_800A6066[0] & 1;
        o->state++;
        o->w22 = 0;
        o->velY = 0;
        o->timer = 1 - o->b6b;
        o->velH = 0x400;
        o->velX = 0x40;
        break;
    case 1:
        o->velH -= o->velX;
        if (o->timer != 0)
            o->velY -= o->velH;
        else
            o->velY += o->velH;
        ((int*)&PLAYER.obj.d8c)[0] = o->d8c = (((unsigned short)o->velY << 16) >> 24) + o->d88 & 0xff;
        if (o->velH <= 0) {
            o->velH = 0;
            o->state++;
            if (o->timer == 0) goto chk;
            goto inc;
        }
        goto chk;
    case 2:
        o->velH += o->velX;
        if (o->timer != 0)
            o->velY += o->velH;
        else
            o->velY -= o->velH;
        ((int*)&PLAYER.obj.d8c)[0] = o->d8c = (((unsigned short)o->velY << 16) >> 24) + o->d88 & 0xff;
        if (o->velH >= 0x400) {
            o->velH = 0x400;
            o->state--;
            o->timer = 1 - o->timer;
            if (o->timer == 0) {
inc:
                o->w22++;
            }
        }
chk:
        if (o->w22 > 2) o->state = 3;
        break;
    case 3:
        if (GAME.playerHealth == 0) {
            o->state = 6;
            ((unsigned char*)&PLAYER.obj.state)[0]++;
            break;
        }
        if (o->subtype == 1) {
            ((unsigned char*)&PLAYER.obj.state)[0]++;
            D_800A6066s = 1 - o->b6b;
        }
        goto next;
    case 4:
        switch (o->subtype) {
        case 0:
            {
                void *a = BITING_PLANT_ANIMS;
                o->d8c = 0;
                o->b0a = 2;
                o->anim = a;
            }
            break;
        case 1:
            o->d8c = 0;
            o->b6a = 0;
            o->box2 = 0x20;
            o->box3 = 0x20;
            break;
        case 2:
            o->d8c = 0;
            break;
        }
next:
        o->state++;
        break;
    case 5:
        o->y.p.whole += 2;
        if (o->y.p.whole >= o->d34) {
            o->y.raw = o->d34 << 16;
            o->step = 0;
            o->state = 0;
            if (o->subtype != 2) o->active = 1;
        }
        break;
    case 6:
        break;
    }
}
