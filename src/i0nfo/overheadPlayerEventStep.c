// FUNC 800e9cd8 648 I0NFO
// MATCHING 800e9cd8 648
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FC14(TObj *o, int a, int b);
extern void func_8001FCE4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void overheadPlayerStatusAnim(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void overheadPlayerDropExitStep(TObj *o);
extern void overheadPlayerLandStep(TObj *o);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);
extern TObj *PLAYER_EXT;
extern unsigned char TALK_POSE;

void overheadPlayerEventStep(TObj *o)
{
    unsigned short k;
    switch (o->step) {
    case 0:
        if (TALK_POSE != 0) {
            overheadPlayerStatusAnim(o);
        } else if (o->active == 5) {
            func_8001FEC0(o);
        } else {
            PLAYER_EXT->animTimer = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            PLAYER_EXT->animTimer = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_8001FEC0(o);
            if (--o->timer <= 0) {
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 2:
        U8(o, 0xAC) = 0;
        switch (o->state) {
        case 0:
            PLAYER_EXT->animTimer = o->animFrame &= 7;
            overheadPlayerSetMoveAnim(o);
            o->b9e = 1;
            o->state++;
        case 1:
            func_8001FEC0(o);
            k = o->animFrame & 7;
            o->wb6 = k << 5;
            func_8001FC14(o, k << 5, o->wb2);
            func_8001FCE4(o);
            overheadCollideMapTile(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
            if (--o->timer <= 0) {
                o->b9e = 0;
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        break;
    case 3:
        U8(o, 0xAC) = 0;
        switch (o->state) {
        case 0:
            PLAYER_EXT->animTimer = o->animFrame |= 8;
            overheadPlayerSetMoveAnim(o);
            o->state++;
        case 1:
            if (TALK_POSE != 0) overheadPlayerStatusAnim(o);
            else overheadPlayerSetMoveAnim(o);
            break;
        }
        break;
    case 4:
        overheadPlayerDropExitStep(o);
        break;
    case 5:
        overheadPlayerLandStep(o);
        break;
    case 6:
        o->visible = 1;
        break;
    case 7:
        break;
    }
}
