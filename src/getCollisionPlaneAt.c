// FUNC 8003f200 804 MAIN0
// MATCHING 8003f200 804
#include "camera.h"
#include "spad.h"
#include "player.h"
#include "gamestate.h"
typedef struct { short x, y; } XY;
extern unsigned short D_8009C960h;
extern char D_8007B5DC[];
extern XY *D_8007B460[];
extern XY D_8007B450[];
extern XY D_8007B44C[];
extern XY *D_8007B46C;
extern unsigned char *D_8007B55C[];
extern char *WHM_DATA[];
#define M960 D_8009C960h

char *getCollisionPlaneAt(short x, short y)
{
    XY *t;
    char *b;
    short r, c;
    unsigned char lim;

    if (x < CAMERA_SCROLL.minX - 0xaa || CAMERA_SCROLL.maxX + 0xaa < x || ((*(int*)&GAME.selectedArea) != 0xe && x < 0))
        return D_8007B5DC;
    if (M960 == 0 && GAME.selectedSection != 3) {
        if (y >= 0x10f) {
            b = WHM_DATA[0];
            y -= 0x10e;
        } else {
            b = (*(char **)&SPAD.p30c);
        }
        t = D_8007B460[M960];
        t += GAME.selectedSection;
    } else if ((M960 == 4 || M960 == 12) && GAME.selectedSection < 4) {
        t = &D_8007B450[GAME.selectedSection];
        b = WHM_DATA[SPAD.s1c8 & 1];
    } else if (M960 == 3 && (GAME.selectedSection == 1 || GAME.selectedSection == 5)) {
        if ((*(short **)&PLAYER.obj.d)[1] < 0) {
            b = (*(char **)&SPAD.p30c);
            t = D_8007B44C;
            y += 0x5a;
        } else {
            t = D_8007B46C;
            b = WHM_DATA[0];
            t += GAME.selectedSection;
        }
    } else {
        t = D_8007B460[M960];
        b = WHM_DATA[0];
        t += GAME.selectedSection;
    }
    if (SPAD.s1c8 & 1)
        r = (t->y - y) / 90;
    else
        r = (y - t->y) / 90;
    { unsigned char *q = D_8007B55C[M960]; lim = q[GAME.selectedSection]; }
    if (lim < r) r = lim;
    if (r < 0) r = 0;
    b += *(unsigned short *)(b + 8 + r * 2);
    c = (x - t->x) / 8;
    if (c < 0) c = 0;
    return b + *(unsigned short *)(b + (c << 1));
}
