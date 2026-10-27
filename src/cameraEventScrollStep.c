// FUNC 800271b8 812 MAIN0
// MATCHING 800271b8 812
#include "gvars.h"
#include "gamestate.h"
typedef struct {
    char p0[7]; unsigned char step;
    char p8[0x20 - 8]; int d20; int d24;
    char p28[0x3f - 0x28]; unsigned char b3f;
    char p40[0x4a - 0x40]; short w4a, w4c; short w4e; short w50, w52; short w54; short w56;
    short w58; short w5a; short w5c; short w5e, w60, w62;
} S;
extern short VIEW_CAMERA_X[];
extern short D_1F8000F6[];
#define F6 D_1F8000F6[0]

static __inline__ int body(S *s)
{
    int r;
    short n, v;
    volatile short *ee;
    r = 0;
    switch (s->step) {
    case 0:
        ee = VIEW_CAMERA_X;
        s->w4c -= *ee;
        s->w50 -= F6;
        s->w52 = *ee;
        s->w56 = F6;
        s->w62 = 0;
        s->w5e = 0;
        s->w60 = 0;
        s->w4a = 0;
        s->w5a = 0;
        s->step++;
        break;
    case 1:
        if (s->w4a != 90) {
            s->w4a++;
            if (s->w4a == 26) {
                GVARS.fadeEffect = 0;
                GAME.fadeScreenControl = 3;
            }
            VIEW_CAMERA_X[0] = s->w52 + s->w4c * s->w4a / 90;
            F6 = s->w56 + s->w50 * s->w4a / 90;
        }
        n = s->w4a;
        if (n != 90) {
            if (n >= 45) n = 90 - n;
            if (n < 32) s->w60 = ((n & 0x38) << 1) | 0x80;
            else s->w60 = 0x120;
        } else {
            s->w60 = 0x80;
        }
        if (s->b3f == 0) {
            s->w5e += s->w60;
            if (s->w5e >= 0x5a00) {
                s->w5e = 0x5a00;
                s->w5a++;
            }
            if (s->w5e <= 0x2d00) goto inc;
            if (s->w5e >= 0x4600) goto dec;
        } else {
            s->w5e -= s->w60;
            if (s->w5e < -0x59ff) {
                s->w5e = -0x5a00;
                s->w5a++;
            }
            if (s->w5e >= -0x2d00) {
            inc:
                s->w62 += 0x80;
                if (s->w62 > 0x1200) s->w62 = 0x1200;
            } else if (s->w5e < -0x1bff) {
            dec:
                s->w62 -= 0x80;
                if (s->w62 < 0) s->w62 = 0;
            }
        }
        s->d24 = s->w5e;
        s->d20 = s->w62;
        if (s->w5a != 0 || GAME.fadeScreenControl == 1) s->step++;
        break;
    case 2:
        r = 1;
        break;
    }
    return r;
}

int cameraEventScrollStep(S *s)
{
    return body(s);
}
