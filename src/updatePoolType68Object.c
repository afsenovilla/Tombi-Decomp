// FUNC 8002ca88 604 MAIN0
// MATCHING 8002ca88 604
#include "player.h"
#include "TOBJ.H"
extern char D_8001289C[];
extern char D_800126FC[];
extern char D_80012724[];
extern char D_8001273C[];
static void *D_800121F4[4] = {
    D_8001289C, D_800126FC, D_80012724, D_8001273C,
};
extern unsigned short PLAYER_Y[];
extern unsigned short D_800A6052[];
extern void AnimLoadDuration(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
extern short MulCos(int, int);
void updatePoolType68Object(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (o->step == 0) {
            o->anim = (D_800121F4 + o->subtype)[o->b0c];
            AnimLoadDuration(o);
            o->a.p.whole = ((unsigned short*)&PLAYER.obj.a.p.whole)[0];
            o->y.p.whole = PLAYER_Y[0] - 8;
            *(short *)((char *)o + 0x1a) = D_800A6052[0];
            { unsigned char t = PLAYER.obj.b0f;
            o->b04 = 1;
            o->step = 0;
            o->b0f = t - 1; }
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->visible == 0)
            break;
        switch (o->step) {
        case 0:
            o->timer = 200;
            o->d84 = 0;
            switch (o->b0c) {
            case 0: o->velX = 0x10; o->velY = -0x100; o->d88 = 1; break;
            case 1: o->velX = 0xe; o->velY = -0x140; o->d88 = 2; break;
            case 2: o->velX = 0x12; o->velY = -0x180; o->d88 = 1; break;
            case 3: o->velX = 0x11; o->velY = -0x200; o->d88 = 1; break;
            case 4: o->velX = 0xf; o->velY = -0x220; o->d88 = 2; break;
            case 5: o->velX = 0x10; o->velY = -0x240; o->d88 = 1; break;
            }
            o->step++;
        case 1:
            o->d84 = (o->d84 + o->d88) & 0xff;
            o->h->p.whole = MulCos(o->d84, o->velX);
            o->y.raw -= 0x10000;
            if (--o->timer == 0)
                o->b04 = 2;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
