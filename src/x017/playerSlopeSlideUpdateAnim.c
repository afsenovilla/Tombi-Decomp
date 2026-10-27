// FUNC 801073c0 464 X017
// MATCHING 801073c0 464
#include "TOBJ.H"
typedef struct P { char pad0[8]; unsigned char b08; char pad1[0x2c - 9]; unsigned short w2c, w2e; } P;
extern P *PLAYER_EXT;
extern unsigned short TICK_COUNT;
extern unsigned short GAME;
extern unsigned short GAME_SELECTED_SECTION;
extern void FUN_800efc04();
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void playerSelectGroundAnim(TObj *);
extern void setFacingAndUpDownFromPad(TObj *);
extern void playerSetAngleFromSlope(TObj *);
extern void FUN_8001e560(int, int);

void playerSlopeSlideUpdateAnim(TObj *o)
{
    P *p;
    unsigned char b;
    int a;
    unsigned int d;
    if ((unsigned short)(o->wb2 + 0x144) >= 0x289) {
        p = PLAYER_EXT;
        p->w2c = 0x11;
        if (p->w2e != 0x11) {
            o->d84 = 0;
            FUN_800efc04();
            FUN_8001fe6c(o);
            PLAYER_EXT->w2e = PLAYER_EXT->w2c;
        }
        b = o->b6b;
        o->ba4 = 1;
        if (b < 0xff)
            b += 2;
        o->b6b = b;
        if (o->animFrame & 1)
            d = (0x100 - (b >> 2)) & 0xff;
        else
            d = b >> 2;
        o->d8c = d;
        if ((TICK_COUNT & 0x1f) == 0) {
            switch (GAME) {
            case 0:
                a = 0x37;
                break;
            case 1:
                switch (GAME_SELECTED_SECTION) {
                case 0:
                case 1:
                    a = 0x3f;
                    break;
                case 3:
                    a = 0x59;
                    break;
                case 5:
                    a = 0x90;
                    break;
                default:
                    goto end;
                }
                break;
            default:
                goto end;
            }
            FUN_8001e560(a, 2);
        }
    } else {
        playerSelectGroundAnim(o);
        setFacingAndUpDownFromPad(o);
        PLAYER_EXT->b08 = 0;
        o->b9c = 0;
        o->d88 = 0;
        o->ba4 = 0;
        o->step = 1;
        o->state = 0;
    }
end:
    FUN_8001fec0(o);
    playerSetAngleFromSlope(o);
    o->y.raw += 0x120000;
}
