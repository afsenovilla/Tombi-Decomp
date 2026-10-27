// FUNC 801175a4 308 X000
// MATCHING 801175a4 308
#include "TOBJ.H"
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned char GVARS_PLAYER_LANDED;
extern short PLAYER_Y_WHOLE, PLAYER_H_WHOLE;
extern void updateA00ScrollController(TObj *);
extern void tryStartItemInteraction(TObj *, int, int);
extern void cameraModeHandler6(TObj *);
extern void cameraEventScrollHandler(TObj *);

void a00CameraController(TObj *o)
{
    if (GAME_SELECTED_SECTION == 3) {
        switch (o->step) {
        case 0:
            if (GVARS_PLAYER_LANDED != 0) o->step++;
            cameraModeHandler6(o);
            break;
        case 1:
            if (PLAYER_Y_WHOLE < -0xdb && PLAYER_H_WHOLE >= 0x12b) tryStartItemInteraction(o, 2, 1);
            else if (PLAYER_H_WHOLE >= 0x157 && PLAYER_Y_WHOLE >= -0x38) tryStartItemInteraction(o, 2, 0);
            cameraModeHandler6(o);
            break;
        case 2:
            cameraEventScrollHandler(o);
            break;
        }
    } else {
        updateA00ScrollController(o);
    }
}
