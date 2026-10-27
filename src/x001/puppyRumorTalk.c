// FUNC 8013a2d8 460 X001
// MATCHING 8013a2d8 460
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern unsigned short PLAYER_ANIM_FRAME;
extern Fix16 *PLAYER_H;
extern char D_80010748[];
extern TObj *D_800A60C8;
extern unsigned char D_800A60F8;
extern void AnimLoadDuration(TObj *o);
extern TObj *showMessageBox(int, int, int, int);

void puppyRumorTalk(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    int v;
    int t;

    switch (o->step) {
    case 0:
        PLAYER.anim = D_80010748;
        AnimLoadDuration(&PLAYER);
        switch (GCD.bceab) {
        case 1:
            D_800A60C8 = showMessageBox(2, 0x1f, 0x80, 0x6c);
            break;
        case 2:
            D_800A60C8 = showMessageBox(2, 0x20, 0x80, 0x6c);
            break;
        case 3:
            D_800A60C8 = showMessageBox(2, 0x21, 0x80, 0x6c);
            break;
        default:
            return;
        }
        t = *(short *)((char *)PLAYER_H + 2) > *(short *)((char *)p->h + 2);
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
        PLAYER_ANIM_FRAME = t;
        o->step++;
        break;
    case 1:
        if (D_800A60C8->b04 == 2) {
            v = GCD.bceab;
            if (v == 1 || (v != 0 && v < 4)) {
                D_800A60C8->b04 = 3;
                o->step = 0;
                o->state = 0;
                t = *(short *)((char *)PLAYER_H + 2) > *(short *)((char *)p->h + 2);
        PLAYER_STATE = 1;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                o->animTimer = 0;
                GVARS.b3f = 0;
                GVARS.b3e = 0;
                GVARS.b42 = 0;
                D_800A60F8 = 0;
            }
        }
        break;
    }
}
