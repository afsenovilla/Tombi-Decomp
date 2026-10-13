// FUNC 8002a4d0 1244 MAIN0
// MATCHING 8002a4d0 1244
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"

extern unsigned char D_8009C938;
extern unsigned char D_8009C93C;
extern int *D_80079A48[];
extern short D_1F8000E6;
extern TObj *TASK_C;
extern TObj PLAYER;
extern void func_80029CB4(void);
extern int func_800271B8(TObj *);
extern void FUN_8001f110(int);

void FUN_8002a4d0(TObj *o)
{
    TObj *s = &PLAYER;
    unsigned short *m;
    short r1;
    int r2, r3, done;
    short v;
    int t;
    unsigned char c;

    if (D_8009C938) {
        GAME.disableSelectMenu = 0;
        func_80029CB4();
        return;
    }
    switch (o->state) {
    case 0:
        r1 = 1;
        if (S32(o, 0x24) != 0) {
            if (S32(o, 0x24) > 0)
                S32(o, 0x24) -= 0x80;
            else
                S32(o, 0x24) += 0x80;
            r1 = 0;
        } else {
            U8(o, 0x71) = 0;
            U8(o, 0x72) = 0;
            U8(o, 0x73) = 0;
        }
        r2 = 0;
        if (S32(o, 0x20) != 0) {
            if (S32(o, 0x20) > 0)
                S32(o, 0x20) -= 0x100;
            else
                S32(o, 0x20) += 0x100;
            r2 = 1;
        }
        v = D_1F8000E6;
        if (v != 0) {
            if (v > 0) {
                D_1F8000E6 = v - 2;
                if ((short)(v - 2) < 0)
                    D_1F8000E6 = 0;
                r3 = 1;
            } else {
                D_1F8000E6 = v + 2;
                if ((short)(v + 2) > 0)
                    D_1F8000E6 = 0;
                r3 = 1;
            }
        } else {
            r3 = 0;
        }
        if (r2 | r3) {
            done = 0;
        } else {
            U8(o, 0x6e) = 0;
            U8(o, 0x6f) = 0;
            done = 1;
        }
        done = done & r1;
        goto tail;
    case 1:
        S32(o, 0x68) = (int)((char *)D_80079A48[GAME.selectedArea][GAME.selectedSection] + GAME.selectedSpawnPoint * 8);
        o->substep = 0;
        o->state++;
        S16(o, 0x4c) = *(unsigned short *)S32(o, 0x68);
        S16(o, 0x50) = ((unsigned short *)S32(o, 0x68))[1];
        GAME.disableSelectMenu = 1;
        c = ((unsigned char *)S32(o, 0x68))[4];
        U8(o, 0x3f) = c;
        if (c == 2) {
            GAME.disableSelectMenu = 0;
            o->state = 3;
            D_8009C93C = 0;
            GAME.fadeScreenControl = 3;
        } else if (c == 3) {
            GAME.disableSelectMenu = 0;
            o->state = 4;
        }
        m = ((unsigned short*)&GAME.nextArea);
        c = ((unsigned char *)S32(o, 0x68))[5];
        m[0] = c;
        GAME.nextSection = ((unsigned char *)S32(o, 0x68))[6];
        GAME.nextSpawnPoint = ((unsigned char *)S32(o, 0x68))[7];
        if (c != GAME.selectedArea)
            FUN_8001f110(1);
        switch (GAME.selectedArea) {
        case 1:
            switch (GAME.selectedSection) {
            case 2:
                if (GAME.nextSection == 4) {
                    s->visible = 1;
                    s->b04 = 5;
                    s->step = 10;
                    s->state = 0;
                    s->timer = 0x1e;
                    s->animFrame = 0;
                    return;
                }
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x40;
                s->state = 0;
                return;
            case 4:
                if (GAME.nextSection == 2)
                    return;
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x40;
                s->state = 0;
                return;
            default:
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x40;
                s->state = 0;
                return;
            }
        case 9:
            if (GAME.selectedSection == 0) {
                if (m[0] == 1)
                    return;
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x40;
                s->state = 0;
                return;
            }
            s->b04 = 5;
            s->visible = 0;
            s->step = 0x40;
            s->state = 0;
            return;
        case 3:
            if (GAME.selectedSection == 0 || GAME.selectedSection == 4) {
                if (m[0] == 9)
                    return;
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x40;
                s->state = 0;
                return;
            }
            s->b04 = 5;
            s->visible = 0;
            s->step = 0x40;
            s->state = 0;
            return;
        case 4:
            if (U8(s, 0xa2) != 0)
                return;
            s->b04 = 5;
            s->step = 0x41;
            s->state = 0;
            return;
        default:
            t = U16(TASK_C, 0x4c);
            if (t == 0)
                return;
            if (t < 3) {
                s->b04 = 5;
                s->visible = 0;
                s->step = 0x41;
                s->state = 0;
                return;
            }
            if (t < 7) {
                if (t >= 4) {
                    s->b04 = 4;
                    s->visible = 0;
                    s->step = 7;
                }
            }
            return;
        }
    case 2:
        done = func_800271B8(o);
    tail:
        if (done)
            o->state++;
        break;
    case 3:
        if (GAME.fadeScreenControl != 1)
            break;
    case 4:
        TASK_C->w4c = 7;
        TASK_C->w4e = 0;
        break;
    }
}
