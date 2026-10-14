// FUNC 80117494 2352 X003
// MATCHING 80117494 2352
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x16 - 8];
    short yw;
    char p18[0x1a - 0x18];
    short bw;
    char p1c[0xa0 - 0x1c];
    unsigned char ba0, ba1, ba2;
    char pa3[0xa8 - 0xa3];
    unsigned char ba8;
} PL;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
} O;
typedef void (*Fn)(O *);

extern PL PLAYER;
extern Fn D_80079A98[];
extern char D_8001D6A4[];
extern unsigned int stepScrollReturnY(O *);
extern unsigned int stepScrollClearX(O *);
extern void FUN_80027c74(O *);
extern void syncObjectTargetY(O *);
extern void func_80029078(O *);
extern int stepPlayerXToward(int);
extern int tryStartItemInteraction(O *, int, int);
extern void setEventComplete(int, int);
extern void keyOffActiveSfxVoices(void);
extern void stopBgm(int);
extern void SoundStopAll(void);
extern void ThreadCreate(int, char *);
extern void func_80029CB4(O *);

#define XU (*(unsigned short *)&(*(short*)((char*)&SPAD.i168 + 2)))
#define ZU (*(unsigned short *)&(*(short*)((char*)&SPAD.i16c + 2)))

void func_80117494(O *o)
{
    switch (o->step) {
    case 0:
        if (((unsigned char*)&GAME.disableSelectMenu)[0] != 0) {
            if (stepScrollReturnY(o) & stepScrollClearX(o)) ((unsigned char*)&GAME.disableSelectMenu)[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (GVARS.b3a != 0) o->step++;
        FUN_80027c74(o);
        syncObjectTargetY(o);
        func_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        switch (GAME.selectedSection) {
        case 0:
        case 4: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            switch (*p) {
            case 0:
                if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x14 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x59) {
                    if (GCD.bcef0 != 0) tryStartItemInteraction(o, 1, 0);
                } else if ((*(short*)((char*)&SPAD.i168 + 2)) < 0xa7 && (*(short*)((char*)&SPAD.i170 + 2)) < -0x5a) {
                    tryStartItemInteraction(o, 1, 1);
                } else if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0xbc1 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x489) {
                    tryStartItemInteraction(o, 1, 2);
                }
                if (GCD.bcdc8 != 0xff) break;
                if ((unsigned)(XU - 0x778) >= 0x18) break;
                if ((unsigned short)(ZU + 0x56a) >= 0x10) break;
                PLAYER.ba0 = 1;
                PLAYER.ba8 = 10;
                if ((*(short*)((char*)&SPAD.i170 + 2)) > 0) stepPlayerXToward(0x784);
                break;
            case 4:
                if (GCD.bcdc3 != 0xff) setEventComplete(0x1f, 0);
                *p = 0;
                break;
            default:
                GAME.selectedSpawnPoint = 0;
                break;
            }
            break;
        }
        case 1:
        case 5: {
            unsigned char *c;
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p != 0) goto clr;
            if (PLAYER.ba2 != 0) {
                if ((unsigned)(XU - 0xcac) < 0x60) stepPlayerXToward(0xcdc);
                else if ((unsigned)(XU - 0x12dd) < 0x60) stepPlayerXToward(0x130d);
                else if ((unsigned)(XU - 0x155b) < 0x60) stepPlayerXToward(0x158b);
            }
            if (PLAYER.ba2 == 2 && (unsigned)(XU - 0x107a) < 0x40 && (unsigned short)(ZU + 0x78e) < 0x20)
                stepPlayerXToward(0x109a);
            if (GCD.bcdc5 == 0xff && (unsigned)(XU - 0xc72) < 0x14 && (unsigned short)(ZU + 0x63e) < 0x10) {
                PLAYER.ba0 = 1;
                PLAYER.ba8 = 0xc;
                if ((*(short*)((char*)&SPAD.i170 + 2)) > 0) stepPlayerXToward(0xc7c);
            }
            if ((unsigned)(XU - 0x10a1) < 0x10 && (unsigned short)(ZU + 0x5b4) < 0x10) {
                PLAYER.ba0 = 1;
                if ((*(short*)((char*)&SPAD.i170 + 2)) > 0) {
                    stepPlayerXToward(0x10a9);
                    if ((*(short*)((char*)&SPAD.i170 + 2)) >= 0x51) {
                        o->step = 6;
                        PLAYER.active = 5;
                        PLAYER.b04 = 5;
                        PLAYER.visible = 0;
                        PLAYER.step = 0x40;
                        GVARS.b3f = 1;
                        break;
                    }
                }
            }
            c = &GCD.bcdc7;
            if (*c == 2) {
                *c = 3;
                keyOffActiveSfxVoices();
                stopBgm(1);
                o->step = 4;
                GAME.fadeScreenControl = 3;
                GVARS.b3f = 1;
                break;
            }
            if ((*(short*)((char*)&SPAD.i170 + 2)) == 0 && (*(short*)((char*)&SPAD.i168 + 2)) < 0xb95 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x485) {
                tryStartItemInteraction(o, 1, 0);
                break;
            }
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x986) {
                if ((unsigned)(XU - 0x1536) >= 0x35) break;
                if ((*(short*)((char*)&SPAD.i170 + 2)) <= 0) break;
                stepPlayerXToward(0x1550);
                if ((*(short*)((char*)&SPAD.i170 + 2)) < 0x2e) break;
                tryStartItemInteraction(o, 1, 1);
                break;
            }
            if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x937) break;
            if ((unsigned)(XU - 0x153e) >= 0x1d) break;
            if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x59) break;
            stepPlayerXToward(0x154d);
            if ((*(short*)((char*)&SPAD.i170 + 2)) < -0x2c) break;
            tryStartItemInteraction(o, 1, 2);
            break;
        clr:
            *p = 0;
            break;
        }
        case 2: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            if (*p == 0) {
                if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x22) {
                    if ((unsigned short)(ZU + 0x16d) < 0x20) tryStartItemInteraction(o, 1, 0);
                    break;
                }
                if ((*(short*)((char*)&SPAD.i168 + 2)) >= 0xb00 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x4c5) {
                    tryStartItemInteraction(o, 1, 1);
                    break;
                }
                if ((*(short*)((char*)&SPAD.i16c + 2)) >= -0x730) break;
                if ((unsigned)(XU - 0x5b3) >= 0xc0) break;
                tryStartItemInteraction(o, 1, 2);
            } else {
                *p = 0;
            }
            break;
        }
        case 3: {
            unsigned short *p = &GAME.selectedSpawnPoint;
            switch (*p) {
            case 0:
                if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x20 && (unsigned short)(ZU + 0x268) < 0x20) tryStartItemInteraction(o, 1, 0);
                break;
            case 1:
                *p = 0;
                break;
            }
            break;
        }
        }
        break;
    case 3:
        D_80079A98[o->subtype](o);
        break;
    case 4:
        if (GAME.fadeScreenControl != 1) break;
        o->step++;
        SPAD.b1cc = 1;
        SPAD.b1cd = 6;
        SPAD.s1c6 = 2;
        SoundStopAll();
        ThreadCreate(1, D_8001D6A4);
        break;
    case 5:
        if (SPAD.b1cc != 0) break;
        o->subtype = 1;
        o->step = 3;
        GAME.selectedSpawnPoint = 3;
        ((unsigned char*)&GAME.disableSelectMenu)[0] = 1;
        break;
    case 6:
        PLAYER.yw -= 2;
        PLAYER.bw -= 2;
        if (PLAYER.bw < 0) PLAYER.bw = 0;
        if (PLAYER.yw < -0x78e) {
            PLAYER.yw = -0x7ee;
            PLAYER.visible = 1;
            PLAYER.active = 2;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->step++;
        }
        func_80029CB4(o);
        break;
    case 7:
        if (PLAYER.yw >= -0x787) {
            PLAYER.active = 1;
            PLAYER.bw = 0;
            GVARS.b3f = 0;
            o->step = 2;
        }
        func_80029CB4(o);
        break;
    }
}
