// FUNC 800f61d4 1504 I0NFO2
// MATCHING 800f61d4 1504
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
typedef struct { int **anims; int p4; int p8; } AnimSet;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern TObj PLAYER;
extern unsigned char D_800A60E4;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char SCRIPT_FLAGS[];
extern unsigned char D_8009CDAB;
extern unsigned char D_8009CF13;
extern AnimSet D_800FEFFC[];
extern short topdownA19ActorOpenDialogue(TObj *o);
extern void AnimJump(TObj *o, int n);
extern void AnimAdvance(TObj *o);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void addItemToInventory(int, int, int);
extern void applyGoldenBowl(void);

void topdownA19ActorTalkStep(TObj *o)
{
    B12 v;
    short k;
    short j;
    short m;

    switch (U8(o, 6)) {
    case 0:
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 2;
        PLAYER_STEP = 0;
        D_800A60E4 = 3;
        D_8009C93F = 1;
        U16(&PLAYER, 0x2E) |= 8;
        S16(o, 0xB0) = U16(o, 0x2E);
        k = U8(o, 0x69) + 7;
        U16(o, 0x2E) = U8(o, 0x69) - 1;
        if (k != U16(o, 0xD2)) {
            PTR(o, 0x24) = (void *)D_800FEFFC[U8(o, 3)].anims[k];
            AnimJump(o, 0);
            U16(o, 0xD2) = k;
        }
        if (U16(o, 0xBA) == 0xE) {
            j = U16(o, 0x2E) + 0xC;
            if (j != U16(o, 0xD2)) {
                PTR(o, 0x24) = (void *)D_800FEFFC[U8(o, 3)].anims[j];
                AnimJump(o, 0);
                U16(o, 0xD2) = j;
            }
        }
        v = *(B12 *)&o->a;
        U8(o, 0x68) = 2;
        U8(o, 6)++;
        if (topdownA19ActorOpenDialogue(o) == 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            U8(o, 4) = 1;
            U8(o, 5) = 0;
            U8(o, 6) = 0;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            U8(o, 0x68) = 0;
            U8(o, 0x6A) = 0;
            U8(o, 5) = 0;
            U8(o, 6) = 0;
            U16(o, 0x2E) = S16(o, 0xB0);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            if (SCRIPT_FLAGS[S16(o, 0xAC)] == 0) {
                setEventStarted(S16(o, 0xAC), 0, 0);
                S16(o, 0x20) = 0xC8;
            } else {
                S16(o, 0x20) = 2;
            }
            U8(o, 6)++;
        }
        break;
    case 3:
        if (--S16(o, 0x20) <= 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            U8(o, 0x68) = 0;
            U8(o, 0x6A) = 0;
            U8(o, 5) = 0;
            U8(o, 6) = 0;
            U16(o, 0x2E) = S16(o, 0xB0);
        }
        break;
    case 4:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            addItemToInventory(0x9A, 1, 1);
            D_8009CF13 = 1;
            S16(o, 0x20) = 0x50;
            U8(o, 6)++;
        }
        break;
    case 5:
        if (--S16(o, 0x20) <= 0) {
            v = *(B12 *)&o->a;
            m = U16(o, 0x2E) + 8;
            if (m != U16(o, 0xD2)) {
                PTR(o, 0x24) = (void *)D_800FEFFC[U8(o, 3)].anims[m];
                AnimJump(o, 0);
                U16(o, 0xD2) = m;
            }
            S32(o, 0x90) = (int)showMessageBoxDirect(3, 4, &v);
            U8(o, 6)++;
        }
        break;
    case 6:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            setEventStarted(0x97, 0, 0);
            S16(o, 0x20) = 0xC8;
            U8(o, 6) = 3;
        }
        break;
    case 7:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            v = *(B12 *)&o->a;
            m = U16(o, 0x2E) + 8;
            if (m != U16(o, 0xD2)) {
                PTR(o, 0x24) = (void *)D_800FEFFC[U8(o, 3)].anims[m];
                AnimJump(o, 0);
                U16(o, 0xD2) = m;
            }
            S32(o, 0x90) = (int)showMessageBoxDirect(3, 6, &v);
            U8(o, 6)++;
        }
        break;
    case 8:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            S16(o, 0x20) = 4;
            U8(o, 6) = 3;
        }
        break;
    case 9:
        AnimAdvance(o);
        if (((TObj *)S32(o, 0x90))->b04 == 2) {
            ((TObj *)S32(o, 0x90))->b04 = 3;
            S16(o, 0x20) = 4;
            U8(o, 6) = 3;
            if (D_8009CDAB != 0xFF) {
                U8(o, 6) = 0xF;
            }
        }
        break;
    case 15:
        if (--S16(o, 0x20) <= 0) {
            S16(o, 0x20) = 0xC8;
            applyGoldenBowl();
            setEventComplete(7, 0);
            U8(o, 6) = 3;
        }
        break;
    }
}
