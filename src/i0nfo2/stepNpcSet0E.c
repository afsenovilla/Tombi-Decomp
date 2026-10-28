// FUNC 800fbf5c 232 I0NFO2
// MATCHING 800fbf5c 232
#include "TOBJ.H"
extern void npcSet0ESceneStep1(TObj *o);
extern void npcSet0ESceneStep2(TObj *o);
extern void npcSet0ESceneStep3(TObj *o);
extern unsigned char D_8009CFA7, D_8009CFA8, D_8009CFAA;

void stepNpcSet0E(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_8009CFA8 == 1) {
            o->step = 2;
            o->state = 0;
        } else if (D_8009CFA7 == 1) {
            D_8009CFA7++;
            o->step = 1;
            o->state = 0;
        } else if (D_8009CFAA == 1) {
            D_8009CFAA++;
            o->step = 3;
            o->state = 0;
        }
        break;
    case 1:
        npcSet0ESceneStep1(o);
        break;
    case 2:
        npcSet0ESceneStep2(o);
        break;
    case 3:
        npcSet0ESceneStep3(o);
        break;
    }
}
