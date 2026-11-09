// FUNC 80139c50 1148 X001
// MATCHING 80139c50 1148
// VERSION SLPS_01144 x01 8013822C 1084
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern char *D_800A7B9C;
extern short *PLAYER_H;
extern short D_800A7B8E;
extern unsigned char D_8009E4E6, D_8009E4E7, D_8009E4EA, D_8009EA64, D_8009E95A;
extern int D_8009E52C;
extern unsigned short D_800A7BA6;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char D_800A7C38;
extern unsigned char D_800A7B7C, D_800A7B7D, D_800A7B7E;

/* halfword view of o->anim (+0x24): an in-struct store, so it does not depend on the raw PLAYER_EXT store */
typedef struct { char pad[0x24]; unsigned short a; } AnimH;
extern char D_800109F0[];
extern void func_80020DE4(TObj *);
extern TObj *func_8002E3B4(int, int, int, int);
extern void func_80020C5C(TObj *);
extern void func_8002EDB0(int, int);

void func_8013822C(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;
    unsigned char *f;
    char **q;
    int n;

    switch (o->step) {
    case 0:
        D_8009E4E7 = 1;
        D_8009E4E6 = 1;
        D_8009E4EA = 1;
        q = &D_800A7B9C;
        *q = D_800109F0;
        func_80020DE4((TObj *)((char *)q - 0x24));
        e->step = 1;
        e->state = 0;
        switch (D_8009EA64) {
        case 0:
            PLAYER_MESSAGE_BOX = func_8002E3B4(2, 10, 0x80, 0x6c);
            o->step++;
            break;
        case 1:
            PLAYER_MESSAGE_BOX = func_8002E3B4(2, 11, 0x80, 0x6c);
            o->step++;
            break;
        }
        break;
    case 1:
        p = PLAYER_MESSAGE_BOX;
        if (p->b04 != 2) break;
        f = &D_8009EA64;
        switch (*f) {
        case 0:
            p->b04 = 3;
            *f = 1;
            o->step = 0;
            o->state = 0;
            D_800A7B7C = 1;
            D_800A7B7D = 0;
            D_800A7B7E = 0;
            e->step = 0;
            e->state = 1;
            e->animFrame = e->b6b;
            ((AnimH *)o)->a = 0;
            goto tail;
        case 1:
            p->b04 = 3;
            D_800A7BA6 = 0;
            D_800A7B7C = 1;
            D_800A7B7D = 0;
            D_800A7B7E = 0;
            o->step = 2;
            o->state = 0;
            *f = 2;
            *(unsigned short *)&o->anim = 1;
            D_800A7C38 = 0;
            break;
        }
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->w08 = 0x20;
            e->d8c = 0;
            e->b69 = 0;
            e->velY = -0x300;
            e->velV = 0;
            asm("" ::: "memory");
            e->velH = (PLAYER_H[1] - e->h->p.whole) * 8;
            asm("" ::: "memory");
            e->velV = (D_800A7B8E - e->y.p.whole) * 8;
            o->state = 1;
            break;
        case 1:
            func_80020C5C(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (e->velY > 0) o->state = 2;
            if (--o->w08 == 0) {
                D_800A7B7C = 1;
                D_800A7B7D = 0;
                D_800A7B7E = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                D_8009E95A = 2;
                { int *q = &D_8009E52C; *q |= 0x100; }
                func_8002EDB0(0x13, 2);
            tail:
                D_8009E4E6 = 0;
                D_8009E4EA = 0;
                D_800A7C38 = 0;
                D_8009E4E7 = 0;
            }
            break;
        case 2:
            func_80020C5C(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (--o->w08 == 0) {
                D_800A7B7C = 1;
                D_800A7B7D = 0;
                D_800A7B7E = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                D_8009E95A = 2;
                D_8009E4EA = 0;
                D_800A7C38 = 0;
                D_8009E4E7 = 0;
                D_8009E4E6 = 0;
                { int *q = &D_8009E52C; *q |= 0x100; }
            }
            break;
        }
        break;
    }
}
#else
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
extern TObj PLAYER;
#define P PLAYER
extern unsigned short PLAYER_ANIM_FRAME;
extern TObj *PLAYER_MESSAGE_BOX;
extern TObj *PLAYER_EXT;
extern unsigned char D_800A60F8;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;

/* halfword view of o->anim (+0x24): an in-struct store, so it does not depend on the raw PLAYER_EXT store */
typedef struct { char pad[0x24]; unsigned short a; } AnimH;
extern char PLAYER_IDLE_ANIM[];
extern void AnimLoadDuration(TObj *);
extern TObj *showMessageBox(int, int, int, int);
extern void FUN_8001fce4(TObj *);
extern void printInfoMessage(int, int);

void lostSeedKidTalk(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;
    unsigned char *f;
    int n;

    switch (o->step) {
    case 0:
        GVARS.b3f = 1;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
        P.anim = PLAYER_IDLE_ANIM;
        AnimLoadDuration(&P);
        e->step = 1;
        e->state = 0;
        switch (GCD.bcebc) {
        case 0:
            PLAYER_MESSAGE_BOX = showMessageBox(2, 10, 0x80, 0x6c);
            o->step++;
            break;
        case 1:
            PLAYER_MESSAGE_BOX = showMessageBox(2, 11, 0x80, 0x6c);
            o->step++;
            break;
        }
        break;
    case 1:
        p = PLAYER_MESSAGE_BOX;
        if (p->b04 != 2) break;
        f = &GCD.bcebc;
        switch (*f) {
        case 0:
            p->b04 = 3;
            *f = 1;
            o->step = 0;
            o->state = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            e->step = 0;
            e->state = 1;
            e->animFrame = e->b6b;
            ((AnimH *)o)->a = 0;
            *(unsigned short *)((char *)PLAYER_EXT + 0x2e) = 0xffff;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
            D_800A60F8 = 0;
            GVARS.b3f = 0;
            break;
        case 1:
            p->b04 = 3;
            PLAYER_ANIM_FRAME = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            o->step = 2;
            o->state = 0;
            PLAYER_EXT->animFrame = 0xffff;
            *f = 2;
            *(unsigned short *)&o->anim = 1;
            D_800A60F8 = 0;
            break;
        }
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->w08 = 0x20;
            e->d8c = 0;
            e->b69 = 0;
            e->velY = -0x300;
            e->velV = 0;
            e->velH = (P.h->p.whole - e->h->p.whole) * 8;
            e->velV = (P.y.p.whole - e->y.p.whole) * 8;
            o->state = 1;
            break;
        case 1:
            FUN_8001fce4(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (e->velY > 0) o->state = 2;
            if (--o->w08 == 0) {
                PLAYER_STATE = 1;
                PLAYER_EXT->animFrame = 0xffff;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                GCD.bcdb2 = 2;
                { int *q = &GAME.area00_eventControl; *q |= 0x100; }
                printInfoMessage(0x13, 2);
                GVARS.b3e = 0;
                GVARS.b42 = 0;
                D_800A60F8 = 0;
                GVARS.b3f = 0;
            }
            break;
        case 2:
            FUN_8001fce4(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (--o->w08 == 0) {
                PLAYER_STATE = 1;
                PLAYER_EXT->animFrame = 0xffff;
                PLAYER_SUBSTATE = 0;
                PLAYER_STEP = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                GCD.bcdb2 = 2;
                GVARS.b42 = 0;
                D_800A60F8 = 0;
                GVARS.b3f = 0;
                GVARS.b3e = 0;
                { int *q = &GAME.area00_eventControl; *q |= 0x100; }
            }
            break;
        }
        break;
    }
}
#endif
