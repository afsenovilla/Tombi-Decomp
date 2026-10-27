// FUNC 801097ec 308 X003
// MATCHING 801097ec 308
#include "TOBJ.H"

extern unsigned short TICK_COUNT;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned char SLOPE_TILT_TABLE[];
extern void FUN_8001e560(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void playerSetAngleFromSlope(TObj *);
extern void playerUpdateSpeedFromPad(TObj *);
extern void playerMoveByAngleSpeed(TObj *);
extern void playerStickToGround(TObj *);

void playerStepPush(TObj *o)
{
    short s;
    if (PAD_PRESSED[0] & 0xa0) {
        if ((TICK_COUNT & 0xf) == 0)
            FUN_8001e560(0x24, 0x24);
        switch (o->state) {
        case 0:
            s = 0x200;
            if (o->animFrame & 1)
                s = -0x200;
            o->wb2 = s;
            o->velH = 0;
            o->velV = 0;
            o->velX = 0;
            o->velY = 0;
            FUN_800eeb5c(o, 0x20);
            ((unsigned char *)o)[0xc3] = 1;
            o->state++;
        case 1:
            FUN_8001fec0(o);
            playerSetAngleFromSlope(o);
            playerUpdateSpeedFromPad(o);
            playerMoveByAngleSpeed(o);
            playerStickToGround(o);
            if (o->ba6 == 0) {
                ((unsigned char *)o)[0xc3] = 0;
                o->step = 1;
                o->state = 0;
            }
        }
    } else {
        unsigned char b = SLOPE_TILT_TABLE[o->wb0];
        ((unsigned char *)o)[0xc3] = 0;
        o->wb2 = 0;
        o->step = 0;
        o->state = 0;
        o->d8c = b;
    }
}
