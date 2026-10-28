// FUNC 800fdb34 368 I0NFO2
// MATCHING 800fdb34 368
#include "TOBJ.H"
extern void func_80018980(TObj *o);
extern void spawnNpcSet0F(TObj *o);
extern void npcSet0FSceneStep1(TObj *o);
extern void npcSet0FSceneStep2(TObj *o);
extern void npcSet0FSceneStep3(TObj *o);
extern void npcSet0FSceneStep8(TObj *o);
extern void npcSet0FSceneStep9(TObj *o);
extern void npcSet0FSceneStep10(TObj *o);
extern void npcSet0FSceneStep11(TObj *o);
extern void npcSet0FSceneStep4(TObj *o);
extern void npcSet0FPickStep(TObj *o);

void updateNpcSet0F(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnNpcSet0F(o);
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 0:
            npcSet0FPickStep(o);
            break;
        case 1:
            npcSet0FSceneStep1(o);
            break;
        case 2:
            npcSet0FSceneStep2(o);
            break;
        case 3:
            npcSet0FSceneStep3(o);
            break;
        case 4:
            npcSet0FSceneStep4(o);
            break;
        case 5:
        case 6:
        case 7:
            if (o->state == 0) {
                o->step = 0;
                o->state = 0;
            }
            break;
        case 8:
            npcSet0FSceneStep8(o);
            break;
        case 9:
            npcSet0FSceneStep9(o);
            break;
        case 10:
            npcSet0FSceneStep10(o);
            break;
        case 11:
            npcSet0FSceneStep11(o);
            break;
        }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        func_80018980(o);
        break;
    }
}
