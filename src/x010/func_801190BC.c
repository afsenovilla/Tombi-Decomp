// FUNC 801190bc 696 X010
// MATCHING 801190bc 696
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_8009C93C, D_8009C93F, D_8009C940, D_8009C941;
extern TObj *TASK_C;
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);
extern void setEventComplete(int, int);

void func_801190BC(TObj *o)
{
    unsigned char t = o->b04;
    TObj *p;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 8;
        o->box3 = 0x10;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->active = 1;
        o->velH = 0;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (D_8009C940 == 0) break;
            if (D_8009C941 != 0x5c) break;
            o->timer = 0x3c;
            o->step++;
            ObjCullRegister(o);
            D_8009C93F = 1;
            break;
        case 1:
            if (--o->timer == -1) {
                o->step++;
                PLAYER.obj.b04 = 1;
                PLAYER.obj.step = 0x12;
                PLAYER.obj.state = 0;
            }
            ObjCullRegister(o);
            break;
        case 2:
            if (o->b69) {
                o->step++;
                setEventComplete(0x96, 0);
            }
            ObjCullRegister(o);
            break;
        case 3:
            o->velH += 8;
            if (o->velH > 0x200) o->velH = 0x200;
            o->h->raw += o->velH << 8;
            if (o->h->p.whole >= 0x385) {
                o->step++;
                (*(short*)&GAME.nextArea) = 4;
                (*(short*)&GAME.nextSpawnPoint) = 0xb;
                (*(short*)&GAME.nextSection) = 0;
                D_8009C93C = 0;
                GAME.fadeScreenControl = 3;
            }
            ObjCullRegister(o);
            break;
        case 4:
            if (GAME.fadeScreenControl == 1) {
                p = TASK_C;
                p->w4c = 7;
                p->w4e = 0;
            }
            o->h->p.whole += 2;
            ObjCullRegister(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
