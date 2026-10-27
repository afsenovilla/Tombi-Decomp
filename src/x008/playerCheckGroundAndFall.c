// FUNC 800f00ac 712 X008
// MATCHING 800f00ac 712
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern int playerGetWallContactKind(TObj *);
extern void FUN_8003fd78(TObj *, int, int);
extern TObj *PLAYER_EXT;
extern int GAME_AREA00_EVENT_CONTROL;
extern unsigned short BUTTON_MASK_A;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned char GAME_PLAYER_MODE[];

void playerCheckGroundAndFall(TObj *o)
{
    int v;

    B(o, 0xad) = 0;
    if (B(o, 0xab) != 2) {
        switch (playerGetWallContactKind(o)) {
        case 1:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (PAD_PRESSED[0] & BUTTON_MASK_A)) o->ba7 = 1;
            B(PLAYER_EXT, 8) = o->animFrame & 1;
            o->step = 0x10;
            o->state = 0;
            break;
        case 2:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(PLAYER_EXT, 8) = o->animFrame & 1;
            o->step = 0x17;
            o->state = 0;
            break;
        case 3:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(PLAYER_EXT, 8) = o->animFrame & 1;
            o->step = 0x1b;
            o->state = 0;
            break;
        case 4:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(PLAYER_EXT, 8) = o->animFrame & 1;
            if ((GAME_AREA00_EVENT_CONTROL & 0x40) && (PAD_PRESSED[0] & BUTTON_MASK_A)) o->ba7 = 1;
            o->step = 0x1e;
            o->state = 0;
            break;
        case 5:
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            B(PLAYER_EXT, 8) = o->animFrame & 1;
            o->step = 0x21;
            o->state = 0;
            break;
        }
    }
    FUN_8003fd78(o, 0, 0);
    if ((o->b69 | o->b9c | o->b9e | o->b9f | o->bbe) == 0) {
        B(o, 0xad) = 1;
        o->velY += 0x223;
        o->y.raw += o->velY << 8;
        PLAYER_EXT->timer = 0x21;
        if (o->velY >= 0x447) {
            B(o, 0xad) = 0;
            o->velY = 0;
            o->b9c = 2;
            B(o, 0xac) = 1;
            B(o, 0xc3) = 0;
            B(o, 0xcd) = 0;
            B(o, 0xce) = 0;
            GAME_PLAYER_MODE[0] = 0;
            v = 0x10;
            if (o->step == 3) {
                o->timer = 10;
                o->d84 = 0;
                if (o->animFrame & 1) v = 0xf0;
                o->d88 = v;
                o->d8c = 0;
                o->step = 4;
                o->state = 1;
            } else {
                o->timer = 10;
                o->d84 = 0;
                if (o->animFrame & 1) v = 0xf0;
                o->d88 = v;
                o->d8c = 0;
                o->step = 2;
                o->state = 3;
            }
        }
    } else {
        o->velY = 0;
        o->b9c = 0;
    }
}
