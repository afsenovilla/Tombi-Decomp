// FUNC 80029078 3132 MAIN0
// MATCHING 80029078 3132
#include "gvars.h"
#include "gamestate.h"
#include "camera.h"
typedef struct {
    char p0[0x20];
    int spd;
    char p24[0x30 - 0x24];
    short w30, w32;
    char p34[0x6c - 0x34];
    signed char b6c, b6d, b6e, b6f;
    char p70[0x75 - 0x70];
    unsigned char b75, b76, b77;
} S;
typedef struct {
    char p0[0x9e];
    unsigned char b9e;
    char p9f;
    union { int i; unsigned char b[4]; } a0;
} G;
extern G PLAYER;
extern unsigned char D_800A60D8;
extern volatile unsigned short PAD_PRESSED[];
extern unsigned short D_1f8001fc;
extern short D_1f8001c8;
extern int handleLookScrollInput(S *);
extern void cameraScrollInputStep(S *, int);
extern void loadSectionHeight(S *);
extern void a00SectionScrollAdjust(S *);
extern void FUN_801165c0(S *);
extern void FUN_80116f74(S *);

static __inline__ short ramp(S *o)
{
    switch (o->b6d) {
    case 0:
        if (o->spd > o->b6f << 8) {
            o->spd -= 0x100;
            return 0;
        }
        return 1;
    case 1:
        if (o->spd >= o->b6f << 8) return 1;
        o->spd += 0x100;
        return 0;
    }
    return 0;
}

static __inline__ int decay(void)
{
    short v = VIEW_CAMERA.rot[1].p.hi;
    if (v != 0) {
        if (v > 0) {
            v -= 2;
            VIEW_CAMERA.rot[1].p.hi = v;
            if (v < 0) VIEW_CAMERA.rot[1].p.hi = 0;
        } else {
            v += 2;
            VIEW_CAMERA.rot[1].p.hi = v;
            if (v > 0) VIEW_CAMERA.rot[1].p.hi = 0;
        }
        return 1;
    }
    return 0;
}

static __inline__ void clamp(S *o)
{
    short f = VIEW_CAMERA.pos[1].p.hi;
    int s = f + VIEW_CAMERA.rot[1].p.hi;
    if (o->w30 > s) VIEW_CAMERA.rot[1].p.hi = o->w30 - f;
    else if (o->w32 < s) VIEW_CAMERA.rot[1].p.hi = o->w32 - f;
}

void updateCameraScroll(S *o)
{
    int f;
    G *g = &PLAYER;
    int a, b;
    unsigned char c;
    unsigned char t1, t2, t3;

    switch (o->b6c) {
    case 0:
        if (GVARS.b3a == 0) break;
        if (GVARS.b3f != 0) {
            o->b6c = 6;
            break;
        }
        if (!(g->a0.b[0] & 1) && g->a0.b[2] >= 2) {
            o->b6c = 8;
            o->b6e = 1;
            o->b6d = 0;
            o->b6f = -10;
            break;
        }
        if ((g->a0.i & 0xff000002) == 0x2000000) {
            o->b6c = 4;
            o->b6d = 1;
            o->b6e = 2;
            o->b6f = 10;
            break;
        }
        switch (g->b9e) {
        case 0:
            c = g->a0.b[0];
            if (c & 0x12) {
                if (PAD_PRESSED[0] & 0xc00) {
                    if ((D_1f8001fc & 0x10) && D_800A60D8 != 4) {
                        if (o->b6e != 1) {
                            o->b6c = 7;
                            o->b6e = 1;
                            o->b6d = 0;
                            o->b6f = -10;
                        }
                    } else if ((D_1f8001fc & 0x40) && o->b6e != 2) {
                        o->b6c = 7;
                        o->b6d = 1;
                        o->b6e = 2;
                        o->b6f = 10;
                    }
                    cameraScrollInputStep(o, 0);
                } else if ((unsigned char)c == 0x12) {
                    if (handleLookScrollInput(o) == 0) break;
                    decay();
                } else if (c & 0x10) {
                    if ((D_1f8001fc & 0x10) && D_800A60D8 != 4) {
                        f = 0;
                        if (o->b6e == 0) goto t2;
                        o->b6c = 7;
                        o->b6d = 0;
                        o->b6e = 0;
                        o->b6f = 0;
                    } else {
                        if (!(D_1f8001fc & 0x40)) goto t2;
                        if (o->b6e == 2) goto t2;
                        if (o->b6e == 0) {
                            o->b6c = 7;
                            o->b6d = 1;
                            o->b6e = 2;
                            o->b6f = 10;
                        } else {
                            o->b6c = 7;
                            o->b6d = 1;
                            o->b6e = 0;
                            o->b6f = 0;
                        }
                        f = 0;
                        if (0) {
                        t2:
                            f = 1;
                        }
                    }
                    if (f) decay();
                } else if (c & 2) {
                    if (c & 1) break;
                    if ((D_1f8001fc & 0x10) && D_800A60D8 != 4) {
                        if (o->b6e == 1) goto t1;
                        {
                            if (o->b6e == 0) {
                                o->b6c = 7;
                                o->b6e = 1;
                                o->b6d = 0;
                                o->b6f = -10;
                            } else {
                                o->b6c = 7;
                                o->b6d = 0;
                                o->b6e = 0;
                                o->b6f = 0;
                            }
                            f = 0;
                        }
                    } else {
                        f = 1;
                        if (D_1f8001fc & 0x40) {
                            f = 0;
                            if (o->b6e != 0) {
                                o->b6c = 7;
                                o->b6d = 1;
                                o->b6e = 0;
                                o->b6f = 0;
                            } else {
                            t1:
                                f = 1;
                            }
                        }
                    }
                    if (f) decay();
                }
            } else if (PAD_PRESSED[0] & 0xc00) {
                handleLookScrollInput(o);
                cameraScrollInputStep(o, 0);
            } else {
                decay();
            }
            break;
        case 2:
            t1 = o->b6d;
            t2 = o->b6e;
            t3 = o->b6f;
            o->b6c = 1;
            o->b6d = 1;
            o->b6f = 10;
            o->b75 = t1;
            o->b76 = t2;
            o->b77 = t3;
            break;
        case 1:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
        default:
            if ((g->a0.b[0] & 0x12) || (PAD_PRESSED[0] & 0xc00)) cameraScrollInputStep(o, 1);
            else decay();
            break;
        }
        break;
    case 1:
        if (ramp(o)) o->b6c = 2;
    case 2:
        if (g->b9e != 2) {
            o->b6c = 3;
            o->b6d = o->b75;
            o->b6f = o->b77;
            if (o->b6f == 0) o->b6d = 0;
        }
        decay();
        break;
    case 3:
        a = decay();
        if (ramp(o) && !a) {
            o->b6c = 0;
            o->b6e = o->b76;
        }
        break;
    case 4:
        if (ramp(o)) o->b6c++;
        clamp(o);
        break;
    case 5:
        clamp(o);
        if (g->a0.b[3] != 0) break;
        if ((g->a0.b[0] & 0x10) && (PAD_PRESSED[0] & 0x40)) goto zero;
        if (GAME.selectedPlane > GAME.area00_fogControl) goto zero;
        o->b6c = 7;
        o->b6d = 0;
        o->b6e = 0;
        o->b6f = 0;
        break;
    case 6:
        a = 0;
        if (o->spd != 0) {
            if (o->spd > 0) o->spd -= 0x100;
            else o->spd += 0x100;
            a = 1;
        }
        b = decay();
        if (!(a | b)) {
            o->b6e = 0;
            o->b6f = 0;
            b = 1;
        } else b = 0;
        if (b) o->b6c = 10;
        break;
    case 7:
        cameraScrollInputStep(o, 0);
        if (ramp(o)) o->b6c = 0;
        break;
    case 8:
        if (ramp(o)) o->b6c++;
        clamp(o);
        break;
    case 9:
        if (g->a0.b[2] != 0) break;
        if (g->a0.b[0] & 2) {
            if (PAD_PRESSED[0] & 0x10) {
                if (!(g->a0.b[0] & 1)) o->b6c = 0;
                break;
            }
        }
        o->b6c = 7;
        o->b6d = 1;
        o->b6e = 2;
        o->b6f = 10;
        break;
    case 10:
        if (GVARS.b3f != 0) break;
        if (D_1f8001c8 == 0) {
            if (GAME.selectedPlane <= GAME.area00_fogControl) {
                o->b6c = 0;
                break;
            }
        } else if (GAME.selectedPlane >= GAME.area00_fogControl) {
            goto zero;
        }
        o->b6d = 1;
        o->b6e = 2;
        o->b6f = 10;
        o->b6c++;
        break;
    case 11:
        if (ramp(o)) {
        zero:
            o->b6c = 0;
        }
        break;
    }
    loadSectionHeight(o);
    switch (GAME.selectedArea) {
    case 0:
        a00SectionScrollAdjust(o);
        break;
    case 1:
    case 7:
        FUN_801165c0(o);
        break;
    case 3:
        FUN_80116f74(o);
        break;
    }
    if (VIEW_CAMERA.pos[1].p.hi > o->w32) VIEW_CAMERA.pos[1].raw = o->w32 << 16;
}
