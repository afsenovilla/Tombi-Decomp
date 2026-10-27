// FUNC 801195f4 624 X000
// MATCHING 801195f4 624
// VERSION SLPM_80165 x00 80112124 692
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 801195F4 (FUN_801195f4.c): subtype 0 also takes over the player (active 4, mode 5, GVARS
// flags) and starts sound 0x36; subtypes 0/1 start at step 0 (PAL 10).
#include "TOBJ.H"
#define H(o, n) (*(short *)((char *)(o) + (n)))
#define I(o, n) (*(int *)((char *)(o) + (n)))
extern int DAT_1f8002d4;
extern unsigned char DAT_8009c930;
extern unsigned char D_800A57B8, D_800A57BC, D_800A57BD, D_800A57BE, D_8009A7EF, D_8009A7E0;
extern short func_8001E3D8(int);
extern unsigned char PLAYER_DRAW_DEPTH;
extern void FUN_8001fe6c(TObj *);
extern void a00TornadoStep(TObj *);
extern void a00TornadoOrbitStep(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_80026e0c(int, int);
extern void queueSfxKeyOff(int);
extern void FUN_800187e4(TObj *);

typedef struct P { char p[0xcc]; short cc; } P;
void FUN_801195f4(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 9;
        o->b0a = 1;
        H(o, 0x2e) = 1;
        o->d3c = DAT_1f8002d4;
        I(o, 0x5c) = 0x1000;
        I(o, 0x60) = 0x1000;
        I(o, 0x64) = 0x1000;
        o->timer = 0;
        o->wb4 = 0;
        o->wb6 = 0;
        o->wb8 = 0;
        ((P *)o)->cc = 0x78;
        switch (o->subtype) {
        case 0:
            D_800A57B8 = 4;
            D_800A57BC = 5;
            D_800A57BD = 0;
            D_800A57BE = 0;
            D_8009A7EF = 1;
            D_8009A7E0 = 1;
            H(o, 0xce) = func_8001E3D8(0x36);
        case 1:
            o->b0d = 0x80;
            o->step = 0;
            break;
        case 2:
        case 3:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 7;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        case 4:
            o->b0d = 0;
            break;
        case 5:
            break;
        case 6:
            DAT_8009c930 = 2;
        case 7:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 5;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        }
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype != 4) {
            a00TornadoStep(o);
            if (o->subtype == 1)
                o->b0f = PLAYER_DRAW_DEPTH + 10;
            else
                o->b0f = PLAYER_DRAW_DEPTH - 2;
        } else {
            a00TornadoOrbitStep(o);
            o->b0f = PLAYER_DRAW_DEPTH;
        }
        FUN_8001fec0(o);
        if (((TObj *)o->d90)->step != 5)
            FUN_800202b4(o);
        if (DAT_8009c930 != 0)
            break;
        o->b04++;
        break;
    case 2:
        if (o->subtype == 0 || o->subtype == 6) {
            FUN_80026e0c(4, 1);
            queueSfxKeyOff(*(unsigned short *)((char *)o + 0xce));
        }
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#else
#include "TOBJ.H"
#define H(o, n) (*(short *)((char *)(o) + (n)))
#define I(o, n) (*(int *)((char *)(o) + (n)))
extern int DAT_1f8002d4;
extern unsigned char DAT_8009c930;
extern unsigned char PLAYER_DRAW_DEPTH;
extern void FUN_8001fe6c(TObj *);
extern void a00TornadoStep(TObj *);
extern void a00TornadoOrbitStep(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_80026e0c(int, int);
extern void queueSfxKeyOff(int);
extern void FUN_800187e4(TObj *);

typedef struct P { char p[0xcc]; short cc; } P;
void updateA00Tornado(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 9;
        o->b0a = 1;
        H(o, 0x2e) = 1;
        o->d3c = DAT_1f8002d4;
        I(o, 0x5c) = 0x1000;
        I(o, 0x60) = 0x1000;
        I(o, 0x64) = 0x1000;
        o->timer = 0;
        o->wb4 = 0;
        o->wb6 = 0;
        o->wb8 = 0;
        ((P *)o)->cc = 0x78;
        switch (o->subtype) {
        case 0:
        case 1:
            o->b0d = 0x80;
            o->step = 10;
            break;
        case 2:
        case 3:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 7;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        case 4:
            o->b0d = 0;
            break;
        case 5:
            break;
        case 6:
            DAT_8009c930 = 2;
        case 7:
            H(o, 0xc0) = 0;
            H(o, 0xc2) = 0;
            H(o, 0xc4) = 0;
            o->b0d = 0x80;
            o->step = 5;
            H(o, 0xc6) = H(o, 0xc0);
            H(o, 0xc8) = H(o, 0xc2);
            H(o, 0xca) = H(o, 0xc4);
            break;
        }
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype != 4) {
            a00TornadoStep(o);
            if (o->subtype == 1)
                o->b0f = PLAYER_DRAW_DEPTH + 10;
            else
                o->b0f = PLAYER_DRAW_DEPTH - 2;
        } else {
            a00TornadoOrbitStep(o);
            o->b0f = PLAYER_DRAW_DEPTH;
        }
        FUN_8001fec0(o);
        if (((TObj *)o->d90)->step != 5)
            FUN_800202b4(o);
        if (DAT_8009c930 != 0)
            break;
        o->b04++;
        break;
    case 2:
        if (o->subtype == 0 || o->subtype == 6) {
            FUN_80026e0c(4, 1);
            queueSfxKeyOff(*(unsigned short *)((char *)o + 0xce));
        }
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#endif
