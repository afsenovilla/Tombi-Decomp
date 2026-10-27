// FUNC 8002a4d0 1244 MAIN0
// MATCHING 8002a4d0 1244
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
#include "gametask.h"

extern int *D_80079A48[];
extern short D_1F8000E6;
extern TObj PLAYER;
extern void updateCameraFollow(void);
extern int cameraEventScrollStep(TObj *);
extern void FUN_8001f110(int);

void cameraEventScrollHandler(TObj *o)
{
    TObj *s = &PLAYER;
    unsigned short *m;
    short r1;
    int r2, r3, done;
    short v;
    int t;
    unsigned char c;

    if (GVARS.b38) {
        GAME.disableSelectMenu = 0;
        updateCameraFollow();
        return;
    }
    switch (o->state) {
    case 0:
        r1 = 1;
        if ((*(int*)&o->anim) != 0) {
            if ((*(int*)&o->anim) > 0)
                (*(int*)&o->anim) -= 0x80;
            else
                (*(int*)&o->anim) += 0x80;
            r1 = 0;
        } else {
            (*(unsigned char*)((char*)&o->box2 + 1)) = 0;
            (*(unsigned char*)&o->box3) = 0;
            (*(unsigned char*)((char*)&o->box3 + 1)) = 0;
        }
        r2 = 0;
        if ((*(int*)&o->timer) != 0) {
            if ((*(int*)&o->timer) > 0)
                (*(int*)&o->timer) -= 0x100;
            else
                (*(int*)&o->timer) += 0x100;
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
            (*(unsigned char*)&o->box1) = 0;
            (*(unsigned char*)((char*)&o->box1 + 1)) = 0;
            done = 1;
        }
        done = done & r1;
        goto tail;
    case 1:
        (*(int*)&o->b68) = (int)((char *)D_80079A48[GAME.selectedArea][GAME.selectedSection] + GAME.selectedSpawnPoint * 8);
        o->substep = 0;
        o->state++;
        o->w4c = *(unsigned short *)(*(int*)&o->b68);
        o->w50 = ((unsigned short *)(*(int*)&o->b68))[1];
        GAME.disableSelectMenu = 1;
        c = ((unsigned char *)(*(int*)&o->b68))[4];
        (*(unsigned char*)((char*)&o->d3c + 3)) = c;
        if (c == 2) {
            GAME.disableSelectMenu = 0;
            o->state = 3;
            GVARS.fadeEffect = 0;
            GAME.fadeScreenControl = 3;
        } else if (c == 3) {
            GAME.disableSelectMenu = 0;
            o->state = 4;
        }
        m = ((unsigned short*)&GAME.nextArea);
        c = ((unsigned char *)(*(int*)&o->b68))[5];
        m[0] = c;
        GAME.nextSection = ((unsigned char *)(*(int*)&o->b68))[6];
        GAME.nextSpawnPoint = ((unsigned char *)(*(int*)&o->b68))[7];
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
            if ((*(unsigned char*)((char*)&s->da0 + 2)) != 0)
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
        done = cameraEventScrollStep(o);
    tail:
        if (done)
            o->state++;
        break;
    case 3:
        if (GAME.fadeScreenControl != 1)
            break;
    case 4:
        TASK_C->mode = 7;
        TASK_C->step = 0;
        break;
    }
}
