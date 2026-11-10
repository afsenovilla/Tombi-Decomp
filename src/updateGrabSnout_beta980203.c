// VERSION BETA_980203 beta980203 80030BF0 524
#if defined(VERSION_BETA_980203)
// Beta version: PAL updateGrabSnout.c: the third coordinate goes to o->b (0x1a), not through o->d.
#include "spad.h"
#include "TOBJ.H"
extern int GetGraphType(void);
extern short GetClut(int x, int y);
extern char GRAB_SNOUT_ANIM[];
extern short PLAYER_H_WHOLE[];
extern unsigned short PLAYER_Y_WHOLE[];
extern unsigned short PLAYER_D_WHOLE[];
extern void AnimLoadDuration(TObj *o);
extern void ObjListPush_1F800220(TObj *o);
extern short MulNegSin(short a, short b);
extern void ObjFree(TObj *o);

void updateGrabSnout(TObj *o)
{
    short v;
    short t;
    switch (o->b04) {
    case 0:
        o->b04++;
        if (GetGraphType() == 1 || GetGraphType() == 2) v = 0xa5;
        else v = 0x35;
        o->w1e = v;
        *(signed char *)&o->b0f = -9;
        o->b0a = 14;
        o->b0d = 0x81;
        o->w08 = GetClut(0x160, o->b0c + 0x1f1);
        o->animFrame = 0;
        o->anim = GRAB_SNOUT_ANIM;
        o->d3c = ((int*)&SPAD.i2d8)[0];
        o->d30 = PLAYER_H_WHOLE[0] << 16;
        o->y.p.whole = PLAYER_Y_WHOLE[0];
        t = PLAYER_D_WHOLE[0];
        o->velV = -0xc00;
        o->timer = 1;
        o->wb4 = 0x80;
        o->wb6 = 0;
        o->wb8 = 100;
        o->b.p.whole = t;
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        if (--o->timer == 0) {
            o->timer = 1;
            if (--*(unsigned short *)&o->wb4 == 0) o->b04++;
        }
        o->wb6 = (o->wb6 + 8) & 0xff;
        o->wb8 += 2;
        o->h->raw = o->d30 + (MulNegSin(o->wb6, 0x10) << 16);
        o->y.raw += o->velV << 4;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
