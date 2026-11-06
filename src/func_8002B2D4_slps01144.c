// JP-only (slps01144), written from the disassembly
// VERSION SLPS_01144 slps01144 8002B2D4 1252
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
#include "gametask.h"

extern int *D_800757DC[];
extern short VIEW_CAMERA_ROT_Y;
extern unsigned char D_80071694[];
extern TObj D_800A7B78;
#define PLAYER D_800A7B78
extern void func_8002AAC4(void);
extern int func_80027FAC(TObj *);
extern void stopBgm(int);

void func_8002B2D4_slps01144(TObj *o)
{
    TObj *s = &PLAYER;
    unsigned short *m;
    short r1;
    int r2, r3, done;
    short v;
    int t;
    unsigned char c;

    if (GVARS.hurtState) {
        func_8002AAC4();
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
        v = VIEW_CAMERA_ROT_Y;
        if (v != 0) {
            if (v > 0) {
                VIEW_CAMERA_ROT_Y = v - 2;
                if ((short)(v - 2) < 0)
                    VIEW_CAMERA_ROT_Y = 0;
                r3 = 1;
            } else {
                VIEW_CAMERA_ROT_Y = v + 2;
                if ((short)(v + 2) > 0)
                    VIEW_CAMERA_ROT_Y = 0;
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
        (*(int*)&o->b68) = (int)((char *)D_800757DC[GAME.selectedArea][GAME.selectedSection] + GAME.selectedSpawnPoint * 8);
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
        if (D_80071694[m[0]])
            stopBgm(1);
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
        done = func_80027FAC(o);
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
#endif
