// FUNC 800fc9e4 1476 I0NFO2
// MATCHING 800fc9e4 1476
#include "TOBJ.H"
#include "raw7.h"
extern TObj PLAYER;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern short PLAYER_ANIM_FRAME;
extern unsigned char D_800A60E4;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009CF9F[];
extern unsigned char D_8009CFA2[];

void func_800FC9E4(TObj *o)
{
    short k = U16(o, 0x24) >= 4;

    switch (U8(o, 6)) {
    case 0:
        D_8009C93F = 1;
        D_8009C93E = 1;
        U8(o, 6)++;
        switch (U16(o, 0x24) & 3) {
        case 0:
            PLAYER_ANIM_FRAME = 0;
            break;
        case 1:
            PLAYER_ANIM_FRAME = 6;
            break;
        case 2:
            PLAYER_ANIM_FRAME = 4;
            break;
        }
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 2;
        PLAYER_STEP = 0;
        break;
    case 1:
        PLAYER_SUBSTATE = 1;
        PLAYER_STEP = 0;
        D_8009CFA2[k] = U8(o, 0x24) & 3;
        U8(o, 7) = 0;
        U8(o, 6)++;
        break;
    case 2:
        if ((D_8009CF9F[k] & 3) == (U16(o, 0x24) & 3)) {
            U8(o, 6)++;
        }
        break;
    case 3:
        switch (U8(o, 7)) {
        case 0:
            switch (U16(o, 0x24) & 3) {
            case 0:
                PLAYER_ANIM_FRAME = 0;
                U16(o, 0x26) = 0xE;
                break;
            case 1:
                PLAYER_ANIM_FRAME = 6;
                U16(o, 0x26) = 0xA;
                break;
            case 2:
                PLAYER_ANIM_FRAME = 4;
                U16(o, 0x26) = 0xE;
                break;
            }
            PLAYER_SUBSTATE = 2;
            PLAYER_STEP = 0;
            U8(o, 7)++;
            break;
        case 1:
            if (--U16(o, 0x26) == 0) {
                U8(o, 6)++;
            } else {
                switch (U16(o, 0x24) & 3) {
                case 0:
                    PLAYER.a.p.whole++;
                    break;
                case 1:
                    PLAYER.y.p.whole++;
                    break;
                case 2:
                    PLAYER.a.p.whole--;
                    break;
                }
            }
            break;
        }
        break;
    case 4:
        PLAYER_SUBSTATE = 1;
        PLAYER_STEP = 0;
        switch (U16(o, 0x24) & 3) {
        case 1:
            D_8009CFA2[k] = ((D_8009CF9F[k] & 0x80) == 0) * 2;
            break;
        case 0:
            D_8009CFA2[k] = 1;
            break;
        case 2:
            D_8009CFA2[k] = 1;
            break;
        }
        PLAYER.active = 4;
        D_8009CF9F[k] |= 0x40;
        U8(o, 7) = 0;
        U8(o, 6)++;
        break;
    case 5:
        if ((D_8009CF9F[k] & 3) != (U16(o, 0x24) & 3)) {
            U8(o, 6)++;
        }
        break;
    case 6:
        switch (U8(o, 7)) {
        case 0:
            switch (D_8009CF9F[k] & 3) {
            case 0:
                PLAYER_ANIM_FRAME = 4;
                U16(o, 0x26) = 0x1E;
                break;
            case 1:
                PLAYER_ANIM_FRAME = 2;
                U16(o, 0x26) = 0x12;
                break;
            case 2:
                PLAYER_ANIM_FRAME = 0;
                U16(o, 0x26) = 0x1E;
                break;
            }
            PLAYER_SUBSTATE = 2;
            PLAYER_STEP = 0;
            U8(o, 7)++;
            break;
        case 1:
            if (--U16(o, 0x26) == 0) {
                U8(o, 6)++;
            } else {
                switch (D_8009CF9F[k] & 3) {
                case 0:
                    PLAYER.a.p.whole--;
                    break;
                case 1:
                    PLAYER.y.p.whole--;
                    break;
                case 2:
                    PLAYER.a.p.whole++;
                    break;
                }
            }
            break;
        }
        break;
    case 7:
        switch (D_8009CF9F[k] & 3) {
        case 0:
            PLAYER_ANIM_FRAME = 4;
            break;
        case 1:
            PLAYER_ANIM_FRAME = 2;
            break;
        case 2:
            PLAYER_ANIM_FRAME = 0;
            break;
        }
        PLAYER_SUBSTATE = 1;
        PLAYER_STEP = 0;
        U8(o, 6)++;
        break;
    case 8:
        D_8009CF9F[k] &= 0xBF;
        PLAYER.active = 3;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_800A60E4 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        U8(o, 5) = 0;
        break;
    }
}
