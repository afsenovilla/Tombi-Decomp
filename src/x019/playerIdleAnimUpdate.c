// FUNC 800f0e7c 1164 X019
// MATCHING 800f0e7c 1164
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    unsigned char p0[0x1e];
    unsigned char b1e;
    unsigned char b1f;
    unsigned char p20[0x2c - 0x20];
    unsigned short w2c;
    unsigned short w2e;
} Q800F0E7C;

extern Q800F0E7C *PLAYER_EXT;
extern short playerIdleFidgetAllowed();
extern short playerIdlePickFidgetAnim(TObj *);
extern void ObjSetAnimFromTable(TObj *);
extern void AnimJump(TObj *, int);

#define B(o, off) (((unsigned char *)(o))[off])

#define SETANIM(n) \
    PLAYER_EXT->w2c = n; \
    ObjSetAnimFromTable(o); \
    AnimJump(o, 0); \
    PLAYER_EXT->w2e = PLAYER_EXT->w2c;

#define SETANIM_IF(n) \
    PLAYER_EXT->w2c = n; \
    if (PLAYER_EXT->w2e != n) { \
        PLAYER_EXT->w2c = n; \
        ObjSetAnimFromTable(o); \
        AnimJump(o, 0); \
        PLAYER_EXT->w2e = PLAYER_EXT->w2c; \
    }

short playerIdleAnimUpdate(TObj *o)
{
    short r = 0;
    unsigned short f = o->animFrame;

    if (f & 2) {
        if (f & 8) {
            PLAYER_EXT->b1f = 0;
            switch (B(o, 0xa0) & 0xf) {
            case 1:
                B(o, 0xa2) = 1;
                if (playerIdleFidgetAllowed()) {
                    SETANIM(0x49);
                    if (B(o, 0xa8)) {
                        r = playerIdlePickFidgetAnim(o);
                    } else {
                        r = 3;
                        PLAYER_EXT->b1e++;
                    }
                }
                break;
            case 2:
                if (GAME.selectedSection == 1 && (GAME.selectedArea == 2 || GAME.selectedArea == 0x13)) {
                    if (GCD.bcdb2 != 0xff || GCD.bcdb5 != 0xff) {
                        return r;
                    }
                }
                SETANIM(0x14);
                r = 1;
                B(o, 0xa2) = 1;
                break;
            case 3:
                SETANIM_IF(0x24);
                r = 5;
                break;
            case 4:
                SETANIM_IF(0x24);
                B(o, 0xa2) = 1;
                r = 4;
                break;
            case 5:
                SETANIM_IF(0x24);
                B(o, 0xa2) = 1;
                r = 0x75;
                break;
            }
        } else if (f & 4) {
            PLAYER_EXT->b1e = 0;
            switch (B(o, 0xa0) >> 4) {
            case 1:
                SETANIM(0x18);
                r = 2;
                B(o, 0xa3) = 1;
                break;
            case 2:
                SETANIM(0x48);
                r = 7;
                PLAYER_EXT->b1f++;
                break;
            case 3:
                SETANIM_IF(0x24);
                r = 6;
                break;
            case 4:
                SETANIM(0x18);
                B(o, 0xa3) = 1;
                PLAYER_EXT->b1f++;
                r = 8;
                break;
            }
        } else {
            PLAYER_EXT->b1e = 0;
            PLAYER_EXT->b1f = 0;
        }
    }
    return r;
}
