// FUNC 800e8d60 708 I0NFO
// MATCHING 800e8d60 708
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void *MAP_PLAYER_IDLE_ANIM_D2, *MAP_PLAYER_IDLE_ANIM_D13, *MAP_PLAYER_IDLE_ANIM_D04, *MAP_PLAYER_IDLE_ANIM_D57, *MAP_PLAYER_IDLE_ANIM_D6, *MAP_PLAYER_WALK_ANIM_D2, *MAP_PLAYER_WALK_ANIM_D13, *MAP_PLAYER_WALK_ANIM_D04;
extern void *MAP_PLAYER_WALK_ANIM_D57, *MAP_PLAYER_WALK_ANIM_D6, *MAP_PLAYER_WALK_BTN_A_ANIM_D2, *MAP_PLAYER_WALK_BTN_A_ANIM_D13, *MAP_PLAYER_WALK_BTN_A_ANIM_D04, *MAP_PLAYER_WALK_BTN_A_ANIM_D57, *MAP_PLAYER_WALK_BTN_A_ANIM_D6;
extern TObj *PLAYER_EXT;
extern volatile unsigned short PAD_PRESSED[];

void overheadPlayerSetMoveAnim(TObj *o)
{
    if (PAD_PRESSED[0] & *(unsigned short *)0x1F8003C4) {
        PLAYER_EXT->animTimer = o->animFrame + 0x10;
        if (PLAYER_EXT->animTimer == PLAYER_EXT->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D04; break;
        case 5: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D57; break;
        case 6: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D6; break;
        case 7: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D57; break;
        case 8: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 9: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 10: o->anim = MAP_PLAYER_IDLE_ANIM_D2; break;
        case 11: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 12: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 13: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        case 14: o->anim = MAP_PLAYER_IDLE_ANIM_D6; break;
        case 15: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        }
    } else {
        PLAYER_EXT->animTimer = o->animFrame;
        if (PLAYER_EXT->animTimer == PLAYER_EXT->animFrame) return;
        switch (o->animFrame) {
        case 0: o->anim = MAP_PLAYER_WALK_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_WALK_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_WALK_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_WALK_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_WALK_ANIM_D04; break;
        case 5: o->anim = MAP_PLAYER_WALK_ANIM_D57; break;
        case 6: o->anim = MAP_PLAYER_WALK_ANIM_D6; break;
        case 7: o->anim = MAP_PLAYER_WALK_ANIM_D57; break;
        case 8: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 9: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 10: o->anim = MAP_PLAYER_IDLE_ANIM_D2; break;
        case 11: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 12: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 13: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        case 14: o->anim = MAP_PLAYER_IDLE_ANIM_D6; break;
        case 15: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        }
    }
    func_8001FE6C(o);
    PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
}
