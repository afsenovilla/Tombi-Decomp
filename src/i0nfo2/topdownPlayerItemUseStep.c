// FUNC 800ea69c 1028 I0NFO2
// MATCHING 800ea69c 1028
#include "TOBJ.H"
#include "raw7.h"
extern void topdownPlayerSetMoveAnim(TObj *o);
extern void func_80026E0C(int item, int n);
extern TObj *PLAYER_EXT;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C940, D_8009C941;
extern unsigned char D_8009C942[];
extern unsigned char D_8009CE55;
extern unsigned char D_8009CE57;
extern unsigned char D_8009CF13;
extern unsigned char GAME_PLAYER_MODE[];
extern unsigned char D_8009D2B1;
extern unsigned char D_8009CDCF, D_8009CE4B, D_8009CEED, D_8009CFC6, D_8009CFC7, D_8009CFD7;
#include "gamestate.h"

#define CHK9(o) ({ short r = 0; \
    if (D_8009CEED >= 2 && D_8009CDCF != 0xFF && GAME.selectedArea == 5 \
        && o->h->p.whole >= 0x90 && o->h->p.whole < 0xC0 && o->y.p.whole >= 0xB6 && o->y.p.whole < 0xCA) { \
        r = 1; \
        D_8009CFC6 = 1; \
    } \
    r; })

void topdownPlayerItemUseStep(TObj *o)
{
    switch (o->state) {
    case 0:
        o->ba4 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->b9c = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        U8(PLAYER_EXT, 8) = 0;
        PLAYER_EXT->timer = 0;
        o->d8c = 0;
        S16(o, 0xE0) = 0;
        o->w22 = 0;
        o->animFrame |= 8;
        topdownPlayerSetMoveAnim(o);
        o->state++;
    case 1:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        if (D_8009C940 == 0) break;
        switch (D_8009C941) {
        case 0xF:
            D_8009CE55 = 5;
            func_80026E0C(D_8009C941, 1);
            D_8009C940 = 0;
            break;
        case 0x9A:
            D_8009CF13 = 0;
            D_8009C940 = 0;
            break;
        case 0x11:
            D_8009CE57 = 4;
            func_80026E0C(D_8009C941, 1);
            D_8009C940 = 0;
            break;
        case 9:
            if (!CHK9(o)) D_8009D2B1 = 0;
            func_80026E0C(D_8009C941, 1);
            D_8009C940 = 0;
            break;
        case 0x1E:
        case 0x1F:
            if (*(int *)&GAME.selectedArea == 0x20013 && o->h->p.whole >= 6 && o->h->p.whole < 0x36
                && o->y.p.whole >= 0xE9 && o->y.p.whole < 0x149) {
                D_8009CFD7 = 1;
                D_8009C940 = 0;
            }
            break;
        case 0x23:
            if (D_8009CE4B != 0xFF && GAME.selectedArea == 5
                && o->h->p.whole >= 0x90 && o->h->p.whole < 0xC0 && o->y.p.whole >= 0xB6 && o->y.p.whole < 0xCA) {
                D_8009CFC7 = 1;
            }
            func_80026E0C(D_8009C941, 1);
            D_8009C940 = 0;
            break;
        case 5:
        case 0x7C:
        case 0x97:
        case 0x98:
            D_8009C940 = 0;
            o->b04 = 4;
            o->step = 4;
            o->state = 0;
            o->substep = 1;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            GAME_PLAYER_MODE[0] = 3;
            break;
        case 7:
            D_8009C940 = 0;
            o->b04 = 4;
            o->step = 4;
            o->state = 0;
            o->substep = 0;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            GAME_PLAYER_MODE[0] = 3;
            func_80026E0C(D_8009C941, 1);
            break;
        case 0xE:
            o->active = 4;
            o->ba4 = 0;
            o->ba5 = 0;
            o->b9c = 1;
            o->visible = 1;
            o->ba7 = 0;
            o->active = 4;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            PLAYER_EXT->timer = 0;
            o->timer = 0x1E;
            o->d8c = 0;
            o->w22 = 0;
            GAME_PLAYER_MODE[0] = 3;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            o->step = 0x30;
            o->d8c = 0;
            o->b04 = 1;
            o->state = 2;
            o->substep = 0;
            break;
        case 3:
        case 0xA:
        case 0x10:
        case 0x12:
        default:
            D_8009C940 = 0;
            break;
        }
        break;
    }
}
