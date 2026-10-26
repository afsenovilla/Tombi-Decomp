// FUNC 800f2a30 360 X009
// MATCHING 800f2a30 360
#include "gcd.h"
#include "plx.h"
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; char r[0x2e - 0x22]; short w2e; } G;
extern G *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern unsigned char SLOPE_TILT_TABLE[];
void SfxPlay3(int, int);
void setEventComplete(int, int);
void playerLandFromJump(TObj *o)
{
    if (*((unsigned char *)o + 0xc9)) PLAYER_EXT->w2e = 0xff;
    o->ba7 = 0;
    PLAYER_EXT->b8 = 0;
    SfxPlay3(0x1c, 0x7f);
    o->b9c = 0;
    if (*(unsigned char *)&o->wac >= 2) {
        BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
        BITTEN_OBJECT->b04 = 2;
        BITTEN_OBJECT->step = 2;
        BITTEN_OBJECT->state = 0;
    }
    *(unsigned char *)&o->wac = 0;
    if (o->velX > 0) {
        o->velX -= 0xd0;
        if (o->velX < 0) o->velX = 0;
    } else {
        o->velX += 0xd0;
        if (o->velX > 0) o->velX = 0;
    }
    o->wb2 = o->velX;
    o->velX = 0;
    o->velY = 0;
    o->d8c = SLOPE_TILT_TABLE[o->wb0];
    o->ba5 = 0;
    PLAYER_EXT->w20 = 0;
    if (((unsigned char*)&GCD.bcf06)[0] && o->subtype != 1) {
        o->subtype = 1;
        if (GCD.bce61 != 0xff) setEventComplete(0xbd, 1);
    }
}
