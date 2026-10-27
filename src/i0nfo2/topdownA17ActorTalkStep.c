// FUNC 800f4d3c 564 I0NFO2
// MATCHING 800f4d3c 564
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern short topdownA17ActorOpenDialogue(TObj *o);
extern void func_8005A8A8(int e, int a, int b);
extern unsigned char D_8009C942[];
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char PLAYER_STATE[];
extern unsigned char PLAYER_SUBSTATE[];
extern unsigned char PLAYER_STEP[];
extern unsigned short PLAYER_ANIM_FRAME[];
extern unsigned char D_800A60E4[];
extern void **D_800FEF6C[][3];

void topdownA17ActorTalkStep(TObj *o)
{
    short f;
    TObj *p;
    switch (o->state) {
    case 0:
        PLAYER_STATE[0] = 1;
        PLAYER_SUBSTATE[0] = 2;
        PLAYER_STEP[0] = 0;
        D_800A60E4[0] = 3;
        PLAYER_ANIM_FRAME[0] |= 8;
        o->wb0 = o->animFrame;
        f = o->b69 + 7;
        o->animFrame = o->b69 - 1;
        o->anim = D_800FEF6C[o->subtype][0][f];
        func_8001FE94(o, 0);
        S16(o, 0xD2) = f;
        o->b68 = 2;
        o->state++;
        if (topdownA17ActorOpenDialogue(o) == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    case 1:
        func_8001FEC0(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            goto done;
        }
        break;
    case 2:
        func_8001FEC0(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            if (SCRIPT_FLAGS[o->wac] == 0) {
                func_8005A8A8(o->wac, 0, 0);
                o->timer = 0xC8;
            } else {
                o->timer = 2;
            }
            o->state++;
        }
        break;
    case 3:
        if (--o->timer <= 0) {
        done:
            D_8009C942[0] = 0;
            D_800A60E4[0] = 0;
            PLAYER_STATE[0] = 1;
            PLAYER_SUBSTATE[0] = 0;
            PLAYER_STEP[0] = 0;
            o->b68 = 0;
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
            o->animFrame = o->wb0;
        }
        break;
    }
}
