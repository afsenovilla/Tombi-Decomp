// FUNC 801243b8 1984 X003
// MATCHING 801243b8 1984
#include "camera.h"
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
typedef struct { short w0, w2, w4, w6, w8, wa, wc; } X;
extern unsigned char D_800A4553[];
extern int CAMERA_TARGET_H_FIX;
extern int CAMERA_TARGET_Y_FIX;
extern char MOVE_TABLE_1[], MOVE_TABLE_0[];
extern B D_80135B30[];
extern char D_80138A0C[];
extern char D_80138A8C[];
extern char D_80138B1C[];
extern char D_80138BAC[];
extern char D_80138BD4[];
extern char D_80138BFC[];
static A *D_801386F8[6] = {
    (A *)D_80138A0C, (A *)D_80138A8C, (A *)D_80138B1C, (A *)D_80138BAC,
    (A *)D_80138BD4, (A *)D_80138BFC,
};
extern int CAMERA_TARGET_H_FIX, CAMERA_TARGET_Y_FIX;
extern TObj *ObjAlloc(void);
extern int komaPigCheckLanded(TObj *);
extern void func_80120438(TObj *);
extern void setEventStarted(int, int, int);

#define SETANIM(k) \
    o->anim = D_801386F8[k]; \
    { unsigned char *p; { B *t = D_80135B30; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define DUST() \
    if (!(o->b69 & 8)) { \
        TObj *e = ObjAlloc(); \
        if (e) { \
            e->active = 1; \
            e->type = 0x10; \
            e->subtype = 1; \
            e->a.p.whole = o->a.p.whole; \
            e->y.p.whole = o->y.p.whole + 0x10; \
            e->b.p.whole = o->b.p.whole; \
        } \
    }

void komaPigDropInIntro(TObj *o)
{
    X *x = (X *)&o->wb4;

    switch (o->state) {
    case 0:
        if (GCD.bcdc3) {
            o->b04 = 3;
            break;
        }
        o->d8c = 0x100;
        o->anim = 0;
        o->state++;
        break;
    case 1:
        break;
    case 2:
        o->state++;
        x->wc = 0x100;
        o->b9c = 2;
        o->movetab = MOVE_TABLE_1;
        o->b69 = 0;
        o->velV = 0;
        o->wac = 0x17;
        SETANIM(0x17);
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        D_800A4553[0] = 3;
        CAMERA_SCROLL.velY = 0;
        CAMERA_TARGET_H_FIX = o->h->raw;
        CAMERA_TARGET_Y_FIX = o->y.raw;
        break;
    case 3:
        o->velV += 0x20;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (komaPigCheckLanded(o)) {
            DUST();
            o->b69 = 0;
            x->wc = 0x100;
            o->movetab = MOVE_TABLE_0;
            o->timer = 2;
            o->wac = 0x16;
            o->state++;
            SETANIM(0x16);
        }
        goto tail;
    case 4:
        if (--o->timer == -1) {
            o->timer = 0;
            o->b69 = 0;
            x->wc = 0;
            o->velV = -0x300;
            o->b9c = 1;
            o->wac = 0x17;
            o->state++;
            SETANIM(0x17);
        }
        goto tail;
    case 5:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (komaPigCheckLanded(o)) {
                DUST();
                o->timer = 2;
                o->wac = 0x16;
                o->state++;
                SETANIM(0x16);
            }
        }
        goto tail;
    case 6:
        if (--o->timer == -1) {
            o->timer = 0;
            x->wc = 0;
            o->b9c = 1;
            o->velV = -0x200;
            o->b69 = 0;
            o->wac = 0x17;
            o->state++;
            SETANIM(0x17);
        }
        goto tail;
    case 7:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (o->timer == 0) {
            x->wc += 2;
            if (x->wc >= 0x16) {
                x->wc = 0xe0;
                o->timer = 1;
                o->wac = 0x18;
                SETANIM(0x18);
            }
        } else {
            x->wc += 4;
            if (x->wc >= 0x100) x->wc = 0x100;
        }
        if (o->velV > 0) {
            o->b9c = 2;
            if (komaPigCheckLanded(o)) {
                DUST();
                ((unsigned char*)&GVARS.b3f)[0] = 0;
                ((unsigned char*)&GVARS.b42)[0] = 0;
                setEventStarted(0x1f, 0, 1);
                o->b69 = 0;
                x->wc = 0;
                o->timer = 0x64;
                o->wac = 0x10;
                o->state++;
                SETANIM(0x10);
            }
        }
    tail:
        CAMERA_TARGET_Y_FIX = o->y.raw;
        CAMERA_TARGET_H_FIX = o->h->raw;
        break;
    case 8:
        func_80120438(o);
        if (!((unsigned char*)&GVARS.b3f)[0]) {
            o->state++;
            D_800A4553[0] = 6;
            o->wac = 9;
            SETANIM(9);
        }
        break;
    case 9:
        func_80120438(o);
        if (--o->timer == -1) o->state++;
        break;
    case 10:
        o->active = 1;
        o->step = 1;
        o->subtype = 0;
        o->state = 0;
        o->substep = 6;
        o->b69 = 0;
        o->b68 = 0;
        break;
    }
    if (o->animFrame) o->d8c = *(unsigned char *)&x->wc;
    else o->d8c = (-x->wc) & 0xff;
}
