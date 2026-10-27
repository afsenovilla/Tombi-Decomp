// FUNC 800f3058 564 I0NFO2
// MATCHING 800f3058 564
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern short topdownA16ActorOpenDialogue(TObj *o);
extern void func_8005A8A8(int e, int a, int b);
extern unsigned char D_8009C942[];
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char PLAYER_STATE[];
extern unsigned char PLAYER_SUBSTATE[];
extern unsigned char PLAYER_STEP[];
extern unsigned short D_800A6066[];
extern unsigned char D_800A60E4[];
extern void **D_800FEEFC[][3];

static inline void setAnimF(TObj *p, int f)
{
    p->animFrame = f;
    p->anim = D_800FEEFC[p->subtype][0][*(volatile unsigned short *)&p->animFrame];
    func_8001FE94(p, 0);
}

void topdownA16ActorTalkStep(TObj *o)
{
    TObj *p;
    switch (o->state) {
    case 0:
        PLAYER_STATE[0] = 1;
        PLAYER_SUBSTATE[0] = 2;
        PLAYER_STEP[0] = 0;
        D_800A60E4[0] = 3;
        D_800A6066[0] |= 8;
        o->wb0 = o->animFrame;
        o->animFrame = o->b69 - 1;
        o->anim = D_800FEEFC[o->subtype][0][o->animFrame + 8];
        func_8001FE94(o, 0);
        o->b68 = 2;
        o->state++;
        if (topdownA16ActorOpenDialogue(o) == 0) {
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
            setAnimF(o, U16(o, 0xB0));
        }
        break;
    }
}
