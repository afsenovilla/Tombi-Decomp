// VERSION SLPS_01144 slps01144 8002BA74 904
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gamestate.h"
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7;
    int d8;
    int dc;
    int d10;
    char p14[0x3c - 0x14];
    unsigned char b3c, b3d, b3e, b3f;
    char p40[0x58 - 0x40];
    unsigned short w58;
    char p5a[0x6c - 0x5a];
    unsigned char b6c, b6d, b6e, b6f;
    unsigned char b70, b71, b72, b73;
    unsigned char b74, b75, b76, b77;
} S;
extern short DOOR_SWING_ANGLE;
extern unsigned short PLAYER_ANIM_FRAME;
extern short FRAME_BUFFER_INDEX, TICK_COUNT;
extern int GAME_RAND_SEED;
extern unsigned char D_1F8003CC, D_1F8001AE;
extern volatile unsigned short D_8009F200[], D_8009F202[];
extern unsigned char D_8009FF05, D_8009FF06, D_8009FF07, D_800A0C15, D_800A0C16, D_800A0C17;
extern unsigned char D_800A7B78, D_800A7B79, D_800A7B7C, D_800A7B7D;
extern void (*D_80075880[])(S *);
extern int DAT_1f800174[];
extern int DAT_1f800178[];
extern int DAT_1f80017c[];
extern int DAT_1f800180;
extern int DAT_1f800184[];
extern int DAT_1f8000e4;
extern void SetGeomScreen(int);
extern void computeCameraMatrix(S *);

void func_8002BA74_slps01144(S *o)
{
    unsigned short w;
    int t, u;

    switch (o->b4) {
    case 0:
        o->b3c = 0;
        o->b3d = 0;
        o->b3e = 0;
        w = PLAYER_ANIM_FRAME;
        o->b3 = 0;
        o->b70 = 0;
        o->b71 = 0;
        o->b72 = 0;
        o->b73 = 0;
        o->b6c = 10;
        o->b6d = 0;
        o->b6e = 0;
        o->b6f = 0;
        o->b74 = 0;
        o->b75 = 0;
        o->b76 = 0;
        o->b77 = 0;
        DOOR_SWING_ANGLE = 0;
        o->w58 = w;
        o->b4++;
        if ((*(int*)&GAME.selectedArea) == 6) {
            o->b4 = 2;
            SetGeomScreen(0x220);
        } else if ((*(int*)&GAME.selectedArea) == 0x60009) {
            o->b4 = 2;
            D_8009FF05 = 0;
            D_8009FF06 = 0;
            D_8009FF07 = 0;
            D_800A0C15 = 0;
            D_800A0C16 = 0;
            D_800A0C17 = 0;
        } else {
            SetGeomScreen(0x220);
        }
        break;
    case 1:
        D_80075880[GAME.selectedArea](o);
        computeCameraMatrix(o);
        goto tail;
    case 2:
        D_80075880[GAME.selectedArea](o);
    tail:
        DAT_1f800174[0] = o->d8;
        t = o->dc;
        DAT_1f800178[0] = t;
        u = o->d10;
        DAT_1f800184[0] = DAT_1f8000e4 + t;
        DAT_1f80017c[0] = u;
        DAT_1f800180 = o->d8;
        break;
    case 3:
        switch (o->b5) {
        case 0:
            D_800A7B78 = 5;
            D_800A7B7C = 5;
            D_800A7B79 = 0;
            D_800A7B7D = 0x40;
            o->b5++;
            break;
        case 1:
            if (D_8009F200[0] & 0x10) {
                o->b5++;
                goto c6;
            }
            break;
        case 2:
            if (D_8009F202[0] & 0x20) o->b5++;
            break;
        case 3:
            if (FRAME_BUFFER_INDEX == 0) {
                D_1F8003CC = 1;
                TICK_COUNT = 0;
                GAME_RAND_SEED = 0x45;
                o->b5++;
            }
            break;
        case 4:
            if (FRAME_BUFFER_INDEX == 0) {
                D_1F8003CC = 0;
                o->b5++;
            }
            break;
        case 5:
            if (D_8009F200[0] & 0x10) o->b5++;
            break;
        case 6:
        c6:
            D_1F8003CC = 2;
            break;
        }
        DAT_1f800174[0] = o->d8;
        t = o->dc;
        DAT_1f800178[0] = t;
        u = o->d10;
        DAT_1f800184[0] = DAT_1f8000e4 + t;
        DAT_1f80017c[0] = u;
        DAT_1f800180 = o->d8;
        if (D_1F8001AE == 0) {
            o->b4 = 1;
            o->b5 = 0;
        }
        break;
    }
}
#endif
