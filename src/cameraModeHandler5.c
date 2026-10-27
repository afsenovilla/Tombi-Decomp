// FUNC 8002a258 380 MAIN0
// MATCHING 8002a258 380
#include "gcd.h"
#include "gvars.h"
typedef struct {
    unsigned char b0, b1, b2, b3;
    char pad[0x34 - 4];
    unsigned short *p34;
    char pad2[0x44 - 0x38];
    unsigned short w44;
} S8002A258;

extern short DAT_1f8000e6;
extern short VIEW_CAMERA_ROT_Y[];
extern unsigned short VIEW_CAMERA_Y;
extern int PLAYER_H_COPY;
extern unsigned short PLAYER_H_WHOLE;
extern int PLAYER_Y_COPY;
extern unsigned short PLAYER_Y_WHOLE;
extern int CAMERA_TARGET_H_FIX;
extern int CAMERA_TARGET_Y_FIX;
extern unsigned char GAME_AREA00_INTRO_PHASE;
void cameraFollowTargetY(S8002A258 *o);
void cameraFollowTargetYB(S8002A258 *o);

void cameraModeHandler5(S8002A258 *o)
{
    short v;
    o->w44 = 0;
    v = VIEW_CAMERA_ROT_Y[0];
    if (v != 0) {
        if (v > 0) {
            DAT_1f8000e6 = v - 2;
            if (DAT_1f8000e6 < 0) DAT_1f8000e6 = 0;
        } else {
            DAT_1f8000e6 = v + 2;
            if (DAT_1f8000e6 > 0) DAT_1f8000e6 = 0;
        }
    }
    CAMERA_TARGET_H_FIX = PLAYER_H_COPY;
    CAMERA_TARGET_Y_FIX = PLAYER_Y_COPY;
    cameraFollowTargetY(o);
    cameraFollowTargetYB(o);
    if ((unsigned short)(o->p34[1] - PLAYER_H_WHOLE + 0x40) < 0x80 || (o->w44 & 3)) {
        if ((unsigned short)(VIEW_CAMERA_Y - PLAYER_Y_WHOLE + 0x5a) < 0xb4 || (o->w44 & 0xc)) {
            o->b3 = 1;
            if (GAME_AREA00_INTRO_PHASE == 3 || GCD.bcda6 == 0xff) o->b3 = 0;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
        }
    }
}
