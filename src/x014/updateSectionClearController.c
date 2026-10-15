// FUNC 801163bc 1252 X014
// MATCHING 801163bc 1252
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"

typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } PL;
typedef void (*Fn)(TObj *);

extern PL PLAYER;
extern void sectionClearCameraFollow();
extern void FUN_8002a4d0();
extern char D_80116934[];
extern void sectionClearCameraReturn();
extern void func_8002A0FC();
extern void updateLookScroll();
extern void func_8002A258();
static Fn D_8012568C[7] = {
    (Fn)sectionClearCameraFollow, (Fn)FUN_8002a4d0, (Fn)D_80116934, (Fn)sectionClearCameraReturn,
    (Fn)func_8002A0FC, (Fn)updateLookScroll, (Fn)func_8002A258,
};
static short D_801256A8[8] = {
    0x1e, 0x6f, 0x7c, 0x22, 0x50, 0x55, 0x36, 0xa0,
};
extern unsigned char D_80125684[];
extern unsigned char D_8009C93Fx[];
extern unsigned short D_1F8000F2x[];
extern void D_8001D6A4(void);
extern unsigned int stepScrollReturnY(TObj *);
extern unsigned int stepScrollClearX(TObj *);
extern void cameraFollowPlayerX(TObj *);
extern void syncObjectTargetY(TObj *);
extern void stepScrollApproachX(TObj *);
extern void setEventComplete(int, int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, void (*)(void));
extern int Rand(void);
extern TObj *allocObjectLayer2(void);
extern void spawnSectionClearEffect(void);

void updateSectionClearController(TObj *o)
{
    PL *pl = &PLAYER;
    TObj *q;

    switch (o->step) {
    case 0: {
        unsigned char *f = &GAME.disableSelectMenu;
        if (*f != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        if (GAME.selectedSection == 7) {
            (*(unsigned char*)((char*)&o->box0 + 1)) = 1;
            (*(unsigned char*)&o->box1) = 2;
            (*(unsigned char*)((char*)&o->box1 + 1)) = 10;
        }
        break;
    }
    case 1:
        if (GVARS.b3a != 0) o->step++;
        cameraFollowPlayerX(o);
        syncObjectTargetY(o);
        stepScrollApproachX(o);
        break;
    case 2:
        D_8012568C[o->subtype](o);
        GAME.selectedSpawnPoint = 0;
        break;
    case 4:
        D_8009C93Fx[0] = 1;
        setEventComplete(D_801256A8[GAME.selectedSection], 0);
        o->w48 = 0x154;
        o->step++;
        break;
    case 5:
        if (--o->w48 == -1) {
            if (GAME.selectedSection == 7) o->step = 12;
            else o->step++;
        }
        break;
    case 6:
        GAME.fadeScreenControl = 3;
        GVARS.b3c = 0;
        o->step++;
    case 7:
        if (GAME.fadeScreenControl == 1) {
            (*(unsigned short*)&SPAD.s1c6) = 2;
            SoundStopAll();
            SPAD.b1cc = 1;
            SPAD.b1cd = D_80125684[GAME.selectedSection];
            ThreadCreate(1, D_8001D6A4);
            o->step++;
        }
        break;
    case 8:
        if (SPAD.b1cc == 0) o->step++;
        break;
    case 9:
        (*(unsigned short*)&SPAD.s1c6) = 0;
        if (GAME.selectedSection == 7) {
            SPAD.b3ce = 1;
            o->subtype = 1;
            o->step = 3;
            GAME.selectedSpawnPoint = 0;
            break;
        }
        if (GAME.purifiedAreas == 0x7f) {
            o->step++;
            GAME.fadeScreenControl = 4;
            GVARS.b3c = 0;
            break;
        }
        o->subtype = 1;
        o->step = 3;
        GAME.selectedSpawnPoint = 1;
        break;
    case 10:
        if (GAME.fadeScreenControl == 0) {
            o->w48 = 0x3c;
            o->step++;
        }
        D_8012568C[o->subtype](o);
        break;
    case 11:
        if (--o->w48 == -1) {
            (*(short*)&GAME.nextArea) = 2;
            (*(short*)&GAME.nextSection) = 5;
            (*(short*)&GAME.nextSpawnPoint) = 1;
            pl->b0 = 4;
            pl->b4 = 5;
            pl->b5 = 5;
            pl->b6 = 0;
            pl->b7 = 1;
        }
    case 3:
        D_8012568C[o->subtype](o);
        break;
    case 12:
        o->w52 = ((Fix16 *)o->d34)->p.whole;
        S16(o, 0x54) = D_1F8000F2x[0];
        o->step++;
        o->w48 = 0x78;
        spawnSectionClearEffect();
        break;
    case 13:
        if (--o->w48 == -1) {
            o->step++;
            GVARS.b41 = 0xe;
            GVARS.b40 = 1;
            pl->b0 = 5;
            pl->b4 = 1;
            pl->b5 = 0x30;
            pl->b6 = 0;
            q = allocObjectLayer2();
            if (q) {
                q->active = 2;
                q->type = 0x54;
                q->subtype = 1;
            }
        }
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        (*(unsigned short*)((char*)&SPAD.if0 + 2)) = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    case 14:
        if (GAME.fadeScreenControl == 1) o->step = 7;
        ((Fix16 *)o->d34)->p.whole = (short)(o->w52 - 4) + (Rand() & 7);
        (*(unsigned short*)((char*)&SPAD.if0 + 2)) = S16(o, 0x54) - 2 + (Rand() & 3);
        break;
    }
}
