// FUNC 80109494 856 X000
// MATCHING 80109494 856
// VERSION SLPS_01144 x00 80108F78 868
// VERSION BETA_980203 x00 8010D3B8 856
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
typedef struct {
    char pad0[0x20];
    short w20;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
#define WF6(o) (*(short *)((char *)(o) + 0xe6))
extern G330 *PLAYER_EXT;
extern unsigned short GAME;
extern unsigned char GVARS_PLAYER_LANDED[];
extern unsigned char DAT_8009ce61;
extern unsigned char DAT_8009d119;
extern unsigned char SLOPE_TILT_TABLE[];
extern char PLAYER_LANE_JUMP_ANIM[];
extern int FUN_8001fddc(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void playSFXWithPitchSlide(int, int, int, int);
extern void startAreaBgm(void);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern void setEventStarted(int, int, int);

void func_80108F78(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        OB(o, 0xa3) = 2;
        o->wb6 = 0;
        o->animFrame &= 1;
        o->velX = FUN_8001fddc(0, o->wb2);
        o->velY = -0x570;
        PLAYER_EXT->w20 = 5;
        FUN_800eeb5c(o, 0x19);
        if (GAME == 0) {
            playSFXWithPitchSlide(0xd, 1, -8, 0x96);
            startAreaBgm();
        }
        o->state = 1;
    case 1:
        FUN_8010f0f4(o);
        if (GAME == 0)
            o->d->raw -= 0x80000;
        else
            o->d->raw -= 0x50000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 2;
        }
        if (FUN_8003facc(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        break;
    case 2:
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8010f0f4(o);
        o->d->raw -= 0x28000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->b69 == 1) {
            FUN_8001e5f4(0x1c, 0x7f);
            *(signed char *)&o->b0f = -8;
            OB(o, 0xa3) = 0;
            o->anim = PLAYER_LANE_JUMP_ANIM;
            FUN_8001fe94(o, 2);
            o->d->p.whole = WF6(o);
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            o->state = 3;
        } else if (FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            OB(o, 0xa3) = 0;
            o->anim = PLAYER_LANE_JUMP_ANIM;
            FUN_8001fe94(o, 2);
            o->d->p.whole = WF6(o);
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            o->state = 3;
        }
        break;
    case 3:
        o->velY = 0x780;
        FUN_8001fd94(o);
        FUN_8003fd78(o, 0, 0);
        if (FUN_8001fec0(o)) {
            GVARS_PLAYER_LANDED[0] = 1;
            *(signed char *)&o->b0f = -8;
            OB(o, 0xa5) = 0;
            o->b9c = 0;
            OB(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            PLAYER_EXT->w20 = 0;
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
            asm("");
            if (DAT_8009ce61 == 0 && DAT_8009d119 != 0)
                setEventStarted(0xbd, 0, 1);
        }
        break;
    }
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL x000 80109494 (playerEventStepLaneJumpBack.c): PAL C of src/playerEventStepLaneJumpBack.c, edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
typedef struct {
    char pad0[0x20];
    short w20;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
#define WF6(o) (*(short *)((char *)(o) + 0xea))
extern G330 *PLAYER_EXT;
extern unsigned short GAME;
extern unsigned char GVARS_PLAYER_LANDED[];
extern unsigned char DAT_8009ce61;
extern unsigned char DAT_8009d119;
extern unsigned char SLOPE_TILT_TABLE[];
extern char PLAYER_LANE_JUMP_ANIM[];
extern int FUN_8001fddc(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void playSFXWithPitchSlide(int, int, int, int);
extern void startAreaBgm(void);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern void setEventStarted(int, int, int);

void playerEventStepLaneJumpBack(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        OB(o, 0xa3) = 2;
        o->wb6 = 0;
        o->animFrame &= 1;
        o->velX = FUN_8001fddc(0, o->wb2);
        o->velY = -0x570;
        PLAYER_EXT->w20 = 5;
        FUN_800eeb5c(o, 0x19);
        if (GAME == 0) {
            playSFXWithPitchSlide(0xd, 1, -8, 0x96);
            startAreaBgm();
        }
        o->state = 1;
    case 1:
        FUN_8010f0f4(o);
        if (GAME == 0)
            o->d->raw -= 0x80000;
        else
            o->d->raw -= 0x50000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 2;
        }
        if (FUN_8003facc(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        break;
    case 2:
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8010f0f4(o);
        o->d->raw -= 0x28000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->b69 == 1 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            OB(o, 0xa3) = 1;
            o->anim = PLAYER_LANE_JUMP_ANIM;
            FUN_8001fe94(o, 2);
            o->d->p.whole = WF6(o);
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            OB(o, 0xac) = 0;
            o->state = 3;
            OB(o, 0xa3) = 0;
        }
        break;
    case 3:
        o->velY = 0x780;
        FUN_8001fd94(o);
        FUN_8003fd78(o, 0, 0);
        if (FUN_8001fec0(o)) {
            GVARS_PLAYER_LANDED[0] = 1;
            *(signed char *)&o->b0f = -8;
            OB(o, 0xa5) = 0;
            OB(o, 0xa3) = 0;
            o->b9c = 0;
            OB(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            PLAYER_EXT->w20 = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
            if (DAT_8009ce61 == 0 && DAT_8009d119 != 0)
                setEventStarted(0xbd, 0, 1);
        }
        break;
    }
}
#else
#include "TOBJ.H"
typedef struct {
    char pad0[0x20];
    short w20;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
#define WF6(o) (*(short *)((char *)(o) + 0xf6))
extern G330 *PLAYER_EXT;
extern unsigned short GAME;
extern unsigned char GVARS_PLAYER_LANDED[];
extern unsigned char DAT_8009ce61;
extern unsigned char DAT_8009d119;
extern unsigned char SLOPE_TILT_TABLE[];
extern char PLAYER_LANE_JUMP_ANIM[];
extern int FUN_8001fddc(int, int);
extern void FUN_800eeb5c(TObj *, int);
extern void playSFXWithPitchSlide(int, int, int, int);
extern void startAreaBgm(void);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(TObj *, int);
extern void setEventStarted(int, int, int);

void playerEventStepLaneJumpBack(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->b69 = 0;
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        o->wb0 = 0;
        OB(o, 0xa3) = 2;
        o->wb6 = 0;
        o->animFrame &= 1;
        o->velX = FUN_8001fddc(0, o->wb2);
        o->velY = -0x570;
        PLAYER_EXT->w20 = 5;
        FUN_800eeb5c(o, 0x19);
        if (GAME == 0) {
            playSFXWithPitchSlide(0xd, 1, -8, 0x96);
            startAreaBgm();
        }
        o->state = 1;
    case 1:
        FUN_8010f0f4(o);
        if (GAME == 0)
            o->d->raw -= 0x80000;
        else
            o->d->raw -= 0x50000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->velY > 0) {
            o->b9c = 2;
            o->state = 2;
        }
        if (FUN_8003facc(o)) {
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        break;
    case 2:
        o->wb0 = 0;
        o->wb6 = 0;
        FUN_8010f0f4(o);
        o->d->raw -= 0x28000;
        if (o->d->p.whole < WF6(o))
            o->d->p.whole = WF6(o);
        FUN_8001fd94(o);
        FUN_8001fec0(o);
        if (o->b69 == 1 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            OB(o, 0xa3) = 1;
            o->anim = PLAYER_LANE_JUMP_ANIM;
            FUN_8001fe94(o, 2);
            o->d->p.whole = WF6(o);
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            OB(o, 0xac) = 0;
            o->state = 3;
            OB(o, 0xa3) = 0;
        }
        break;
    case 3:
        o->velY = 0x780;
        FUN_8001fd94(o);
        FUN_8003fd78(o, 0, 0);
        if (FUN_8001fec0(o)) {
            GVARS_PLAYER_LANDED[0] = 1;
            *(signed char *)&o->b0f = -8;
            OB(o, 0xa5) = 0;
            OB(o, 0xa3) = 0;
            o->b9c = 0;
            OB(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            o->d8c = SLOPE_TILT_TABLE[o->wb0];
            PLAYER_EXT->w20 = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
            if (DAT_8009ce61 == 0 && DAT_8009d119 != 0)
                setEventStarted(0xbd, 0, 1);
        }
        break;
    }
}
#endif
