// FUNC 800e8d60 708 I0NFO
// MATCHING 800e8d60 708
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void *D_800106B4, *D_800106B8, *D_800106BC, *D_800106C0, *D_800106C4, *D_800106C8, *D_800106CC, *D_800106D0;
extern void *D_800106D4, *D_800106D8, *D_800106DC, *D_800106E0, *D_800106E4, *D_800106E8, *D_800106EC;
extern TObj *PLAYER_EXT;
extern volatile unsigned short PAD_PRESSED[];

void overheadPlayerSetMoveAnim(TObj *o)
{
    if (PAD_PRESSED[0] & *(unsigned short *)0x1F8003C4) {
        PLAYER_EXT->animTimer = o->animFrame + 0x10;
        if (PLAYER_EXT->animTimer == PLAYER_EXT->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = D_800106E4; break;
        case 1: o->anim = D_800106E0; break;
        case 2: o->anim = D_800106DC; break;
        case 3: o->anim = D_800106E0; break;
        case 4: o->anim = D_800106E4; break;
        case 5: o->anim = D_800106E8; break;
        case 6: o->anim = D_800106EC; break;
        case 7: o->anim = D_800106E8; break;
        case 8: o->anim = D_800106BC; break;
        case 9: o->anim = D_800106B8; break;
        case 10: o->anim = D_800106B4; break;
        case 11: o->anim = D_800106B8; break;
        case 12: o->anim = D_800106BC; break;
        case 13: o->anim = D_800106C0; break;
        case 14: o->anim = D_800106C4; break;
        case 15: o->anim = D_800106C0; break;
        }
    } else {
        PLAYER_EXT->animTimer = o->animFrame;
        if (PLAYER_EXT->animTimer == PLAYER_EXT->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = D_800106D0; break;
        case 1: o->anim = D_800106CC; break;
        case 2: o->anim = D_800106C8; break;
        case 3: o->anim = D_800106CC; break;
        case 4: o->anim = D_800106D0; break;
        case 5: o->anim = D_800106D4; break;
        case 6: o->anim = D_800106D8; break;
        case 7: o->anim = D_800106D4; break;
        case 8: o->anim = D_800106BC; break;
        case 9: o->anim = D_800106B8; break;
        case 10: o->anim = D_800106B4; break;
        case 11: o->anim = D_800106B8; break;
        case 12: o->anim = D_800106BC; break;
        case 13: o->anim = D_800106C0; break;
        case 14: o->anim = D_800106C4; break;
        case 15: o->anim = D_800106C0; break;
        }
    }
    func_8001FE6C(o);
    PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
}
