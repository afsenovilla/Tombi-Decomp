// FUNC 8010ae94 304 X016
// MATCHING 8010ae94 304
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern void FUN_8001fec0(TObj *);
extern void playerReleaseBittenAndClearFlags(TObj *);
extern void playerAirSpinToFall(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void setEventComplete(int, int);

void playerEventStepRiseUp(TObj *o)
{
    short s1;
    short s3;
    unsigned char *g;

    switch (o->state) {
    case 0:
        o->b9c = 1;
        o->visible = 1;
        o->ba4 = 0;
        o->ba5 = 0;
        o->ba7 = 0;
        o->active = 4;
        g = PLAYER_EXT;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        *(short *)(g + 0x20) = 0;
        o->d8c = 0;
        o->timer = 0x1e;
        o->w22 = 0;
        playerReleaseBittenAndClearFlags(o);
        o->d8c = 0;
        o->state = o->state + 1;
        break;
    case 1:
        FUN_8001fec0(o);
        playerAirSpinToFall(o);
        o->y.raw = o->y.raw + o->velY * 0x100;
        o->velY = o->velY - 0x30;
        if (o->velY < -0x570)
            o->velY = -0x570;
        PlayerSetAnimIfChanged(o, 4);
        if (o->y.p.whole < -0x20c) {
            setEventComplete(0xa4, 0);
            o->b04 = 5;
            *(short *)((char *)o + 0xf6) = 0;
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}
