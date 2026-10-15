// FUNC 801175a0 368 X014
// MATCHING 801175a0 368
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void *D_80129F98;
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjFree(TObj *);
extern void drawScreenFade(int);

void updateAnimDecorSprite(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0x10;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = D_80129F98;
        AnimLoadDuration(o);
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        if (o->subtype != 1) break;
        switch (o->step) {
        case 0:
            if (GAME.fadeScreenControl != 0) break;
            GVARS.b3c = 2;
            o->animFrame = 0x60;
            drawScreenFade(0x60);
            o->step++;
            break;
        case 1:
            drawScreenFade((short)o->animFrame);
            if (GAME.fadeScreenControl == 3) o->step--;
            break;
        case 2:
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
