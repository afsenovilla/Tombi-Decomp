// near-twin of PAL updateLeafButterfly (tools/jp_autofix.py)
// VERSION SLPS_01144 x10 80122040 548
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "gamestate.h"

extern void *D_80131DC0;
extern void *D_8012DF90;
extern unsigned char D_8009EA57;
extern unsigned char D_8009EC5C;
extern void setEventStarted(int, int, int);
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
void leafButterflyRoam(TObj *o);

void func_80122040(TObj *o)
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
            if (o->state == 0) {
                *(signed char *)&o->b0f = -7;
                o->anim = D_8012DF90;
                AnimLoadDuration(o);
                o->d8c = 0;
                o->state++;
                o->category |= 0x80;
            }
            break;
        case 1:
        case 2:
            if (o->state == 0) {
                o->anim = D_80131DC0;
                AnimLoadDuration(o);
                if (D_8009EA57 == 1 && D_8009EC5C == 0)
                    setEventStarted(0xb2, 0, 0);
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
            leafButterflyRoam(o);
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
