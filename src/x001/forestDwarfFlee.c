// FUNC 80135c90 1236 X001
// MATCHING 80135c90 1236
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern unsigned short GAME[], GAME_SELECTED_SECTION[];
extern char MOVE_TABLE_0[];
extern void *D_8013DE0C, *D_8013DDB0, *D_8013DE2C, *D_8013DDC8;
extern void AnimLoadDuration(TObj *);
extern void forestDwarfCountCatch(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void forestDwarfFlee(TObj *o)
{
    switch (o->state) {
    case 0:
        GVARS.b3f = 1;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
        o->animFrame = o->h->p.whole > (*(Fix16 **)&PLAYER.obj.h)->p.whole;
        switch (o->subtype) {
        case 2:
        case 3:
            ((TObj *)o->d94)->b04 = 3;
        }
        forestDwarfCountCatch(o);
        o->active = 4;
        o->movetab = MOVE_TABLE_0;
        if (GAME[0] == 1 && GAME_SELECTED_SECTION[0] < 2) o->anim = D_8013DE0C;
        else o->anim = D_8013DDB0;
        AnimLoadDuration(o);
        {
            short v = 0x120;
            o->b69 = 0;
            if (o->animFrame & 1) v = -0x120;
            o->velY = -0x100;
            o->timer = 0x3c;
            o->velX = v;
        }
        o->state = 1;
        break;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) o->state++;
        break;
    case 2:
        if (--o->timer <= 0) o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0x500) o->velY = 0x500;
        if (o->b69 | TileCollideAt(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2))) {
            if (GAME[0] == 1 && GAME_SELECTED_SECTION[0] < 2) o->anim = D_8013DE2C;
            else o->anim = D_8013DDC8;
            AnimLoadDuration(o);
            { int t = (*(Fix16 **)&PLAYER.obj.h)->p.whole < o->h->p.whole;
            o->state++;
            o->animFrame = t; }
        }
        break;
    case 4:
        if (AnimAdvance(o)) o->state++;
        break;
    case 5:
        o->active = 4;
        if (GAME[0] == 1 && GAME_SELECTED_SECTION[0] < 2) o->anim = D_8013DE0C;
        else o->anim = D_8013DDB0;
        AnimLoadDuration(o);
        switch (GAME_SELECTED_SECTION[0]) {
        case 0:
        case 1:
        case 4:
            o->animFrame = 0;
            break;
        case 2:
        case 3:
        case 5:
            o->animFrame = 1;
            break;
        }
        {
            short v = 0x200;
            if (o->animFrame & 1) v = -0x200;
            o->velX = v;
            o->velY = -0x300;
            o->state++;
        }
    case 6:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0) o->state++;
        break;
    case 7:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + (o->box3 - o->box2))) o->state = 5;
        break;
    }
    if (!o->visible) {
        GVARS.b3e = 0;
        GVARS.b42 = 0;
        GVARS.b3f = 0;
        o->b04 = 3;
        o->step = 0;
        o->state = 0;
    }
}
