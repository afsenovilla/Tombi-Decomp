// FUNC 800fa424 408 I0NFO2
// MATCHING 800fa424 408
#include "TOBJ.H"
extern void func_8005A8A8(int a, int b, int c);
extern void npcSet07SceneStep1(TObj *o);
extern void npcSet07SceneStep2(TObj *o);
extern void npcSet07SceneStep3(TObj *o);
extern void npcSet07SceneStep6(TObj *o);
extern void npcSet07SceneStep4(TObj *o);
extern void npcSet07SceneStep5(TObj *o);
extern void npcSet07SceneStep11(TObj *o);
extern void npcSet07SceneStep12(TObj *o);
extern void npcSet07SceneStep0(TObj *o);
extern void npcSet07SceneStep8(TObj *o);
extern void npcSet07SceneStep10(TObj *o);
extern unsigned char GVARS_PLAYER_LANDED;
extern unsigned char D_8009CDD9;
extern unsigned char D_8009CF2A[];
extern unsigned char D_800A60A3;

void stepNpcSet07(TObj *o)
{
    if (D_800A60A3 != 1 && D_8009CF2A[0] == 1) D_8009CF2A[0] = 2;
    switch (o->step) {
    case 0: npcSet07SceneStep0(o); break;
    case 1: npcSet07SceneStep1(o); break;
    case 2: npcSet07SceneStep2(o); break;
    case 3: npcSet07SceneStep3(o); break;
    case 4: npcSet07SceneStep4(o); break;
    case 5: npcSet07SceneStep5(o); break;
    case 6: npcSet07SceneStep6(o); break;
    case 8: npcSet07SceneStep8(o); break;
    case 9:
        switch (o->state) {
        case 0:
            if (D_8009CDD9 == 0 && GVARS_PLAYER_LANDED != 0) {
                func_8005A8A8(0x35, 0, 0);
                o->state++;
            }
            break;
        case 1:
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    case 10: npcSet07SceneStep10(o); break;
    case 11: npcSet07SceneStep11(o); break;
    case 12: npcSet07SceneStep12(o); break;
    }
}
