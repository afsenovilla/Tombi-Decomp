// FUNC 800e9c7c 520 I0NFO2
// MATCHING 800e9c7c 520
#include "TOBJ.H"
extern int rand(void);
extern void func_8001FE6C(TObj *o);
extern int func_8001FEC0(TObj *o);
extern unsigned char TALK_POSE;
extern TObj *PLAYER_EXT;
extern void *D_800106F0[];
extern void *D_80010700[];
extern void *D_8001070C[];

void topdownPlayerStatusAnim(TObj *arg)
{
    register TObj *o asm("$17") = arg;
    short d;
    switch (o->animFrame & 7) {
    case 2:
        d = 0;
        break;
    case 6:
        d = 2;
        break;
    case 0: case 1: case 3: case 4: case 5: case 7:
        d = 1;
        break;
    }
    switch (TALK_POSE) {
    case 1:
    case 5:
        if ((rand() & 0x3F) == 0) {
            PLAYER_EXT->animTimer = 0x5A;
            if (PLAYER_EXT->animFrame != 0x5A) {
                o->anim = D_800106F0[d];
                func_8001FE6C(o);
                PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
            }
        }
        if (func_8001FEC0(o)) {
            PLAYER_EXT->animFrame = 0xFF;
        }
        break;
    case 2:
        PLAYER_EXT->animTimer = 0x5B;
        if (PLAYER_EXT->animFrame != 0x5B) {
            o->anim = D_8001070C[d];
            func_8001FE6C(o);
            PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        }
        func_8001FEC0(o);
        break;
    case 3: {
        TObj *p = PLAYER_EXT;
        p->animTimer = 0x5B;
        if (p->animFrame != 0x5B) {
            o->anim = D_8001070C[d];
            p->animTimer = p->animFrame;
        }
        func_8001FEC0(o);
        break;
    }
    case 4:
        PLAYER_EXT->animTimer = 0x5C;
        if (PLAYER_EXT->animFrame != 0x5C) {
            o->anim = D_80010700[d];
            func_8001FE6C(o);
            PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
        }
        func_8001FEC0(o);
        break;
    }
}
