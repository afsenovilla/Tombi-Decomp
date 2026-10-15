// FUNC 8013839c 436 X001
// MATCHING 8013839c 436
#include "TOBJ.H"
#include "gamestate.h"

extern void *D_8013E6DC;
extern unsigned char D_8009C959;
extern unsigned char D_8009CF0A;
extern unsigned char D_8009CFC8;
void ObjCullRegister(TObj *o);
void AnimLoadDuration(TObj *o);
void addItemToInventory(int a, int b, int c);
void freeObjectLayer2(TObj *o);
void initLeafButterfly(TObj *o);
void leafButterflyFlutter(TObj *o);
void leafButterflyEventStart(TObj *o);
void leafButterflyReleased(TObj *o);
void func_80137D8C(TObj *o);

void updateLeafButterfly(TObj *o)
{
    switch (o->b04) {
    case 0:
        initLeafButterfly(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->step == 0) {
            leafButterflyFlutter(o);
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            leafButterflyEventStart(o);
            break;
        case 1:
        case 2:
            if (o->state == 0) {
                o->anim = D_8013E6DC;
                AnimLoadDuration(o);
                D_8009C959--;
                addItemToInventory(0x10, 1, 1);
                if (GAME.selectedArea == 1) {
                    D_8009CF0A++;
                    o->b04 = 3;
                    o->step = 0;
                    o->state = 0;
                } else {
                    D_8009CFC8++;
                    o->b04 = 3;
                    o->step = 0;
                    o->state = 0;
                }
            }
            break;
        case 3:
            leafButterflyReleased(o);
            break;
        case 4:
            func_80137D8C(o);
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
