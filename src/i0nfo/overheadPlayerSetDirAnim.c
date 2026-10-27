// FUNC 800e8a98 712 I0NFO
// MATCHING 800e8a98 712
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void *MAP_PLAYER_IDLE_ANIM_D2, *MAP_PLAYER_IDLE_ANIM_D13, *MAP_PLAYER_IDLE_ANIM_D04, *MAP_PLAYER_IDLE_ANIM_D57, *MAP_PLAYER_IDLE_ANIM_D6, *MAP_PLAYER_WALK_ANIM_D2, *MAP_PLAYER_WALK_ANIM_D13, *MAP_PLAYER_WALK_ANIM_D04;
extern void *MAP_PLAYER_WALK_ANIM_D57, *MAP_PLAYER_WALK_ANIM_D6, *MAP_PLAYER_WALK_BTN_A_ANIM_D2, *MAP_PLAYER_WALK_BTN_A_ANIM_D13, *MAP_PLAYER_WALK_BTN_A_ANIM_D04, *MAP_PLAYER_WALK_BTN_A_ANIM_D57, *MAP_PLAYER_WALK_BTN_A_ANIM_D6;

void overheadPlayerSetDirAnim(TObj *o, short mode, int dir)
{
    char d = dir;
    switch (mode) {
    case 0:
        switch (dir &= 7) {
        case 0: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_IDLE_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 6: o->anim = MAP_PLAYER_IDLE_ANIM_D6; break;
        case 5: case 7: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        }
        break;
    case 1:
        switch (dir &= 7) {
        case 0: o->anim = MAP_PLAYER_WALK_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_WALK_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_WALK_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_WALK_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_WALK_ANIM_D04; break;
        case 5: o->anim = MAP_PLAYER_WALK_ANIM_D57; break;
        case 6: o->anim = MAP_PLAYER_WALK_ANIM_D6; break;
        case 7: o->anim = MAP_PLAYER_WALK_ANIM_D57; break;
        }
        break;
    case 2:
        switch (dir &= 7) {
        case 0: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D04; break;
        case 5: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D57; break;
        case 6: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D6; break;
        case 7: o->anim = MAP_PLAYER_WALK_BTN_A_ANIM_D57; break;
        }
        break;
    default:
        switch (d & 7) {
        case 0: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 1: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 2: o->anim = MAP_PLAYER_IDLE_ANIM_D2; break;
        case 3: o->anim = MAP_PLAYER_IDLE_ANIM_D13; break;
        case 4: o->anim = MAP_PLAYER_IDLE_ANIM_D04; break;
        case 6: o->anim = MAP_PLAYER_IDLE_ANIM_D6; break;
        case 5: case 7: o->anim = MAP_PLAYER_IDLE_ANIM_D57; break;
        }
        break;
    }
    func_8001FE6C(o);
}
