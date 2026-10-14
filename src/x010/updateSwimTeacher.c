// FUNC 8012855c 520 X010
// MATCHING 8012855c 520
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "gamestate.h"
extern char D_80132894[];
extern int ObjCullRegister(char *);
extern void freeObjectLayer2(char *);
extern void swimTeacherIntroTalk(char *);
extern void swimTeacherNoBananasTalk(char *);
extern void swimTeacherJumpInTalk(char *);
extern void swimTeacherFloatTalk(char *);

void updateSwimTeacher(char *o)
{
    unsigned char s = o[4];

    switch (s) {
    case 0:
        if (GCD.bcdac == 0xff || GCD.bd078 == 4) {
            o[4] = 3;
            break;
        }
        o[0] = 2;
        *(short *)(o + 0x6c) = 10;
        *(short *)(o + 0x6e) = 0x14;
        *(short *)(o + 0x70) = 0x10;
        *(short *)(o + 0x72) = 0x20;
        *(short *)(o + 0x2e) = 1;
        *(int *)(o + 0x3c) = SPAD.i2e0;
        o[0xd] = 0;
        o[0x6a] = 0;
        o[0xa] = 0;
        o[0x69] = 0;
        o[0x68] = 0;
        *(short *)(o + 0x1e) = 0xb;
        *(char **)(o + 0xa8) = D_80132894;
        o[0xa7] = 0;
        o[4]++;
        if (GCD.bd078) {
            if (GAME.selectedSection == 0) {
                o[5] = 1;
                *(short *)(o + 0x12) = 0xc64;
                *(short *)(o + 0x16) = -0x6d;
                *(short *)(o + 0x2e) = 1;
                *(signed char *)(o + 0xf) = -7;
            } else {
                o[5] = 3;
                GVARS.b3f = 1;
            }
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o[5]) {
        case 0:
            swimTeacherIntroTalk(o);
            break;
        case 1:
            swimTeacherNoBananasTalk(o);
            break;
        case 2:
            swimTeacherJumpInTalk(o);
            break;
        case 3:
            swimTeacherFloatTalk(o);
            break;
        }
        break;
    case 2:
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
