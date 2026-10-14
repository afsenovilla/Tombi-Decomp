// FUNC 80118ea0 2208 X017
// MATCHING 80118ea0 2208
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char c[12]; } B12;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2DC;
extern TObj PLAYER;
extern unsigned char D_8009CE3A;
extern unsigned char D_8009CDF1;
extern unsigned char *D_8009C330;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern char D_80010814[];
extern void FUN_8001d6a4();
extern void FUN_8001fe6c(TObj *);
extern void FUN_800eea7c(TObj *, int, int);
extern void cdSeekStream(int);
extern void FUN_8001f110(int);
extern void FUN_800171b8(int, void *);
extern void startAreaBgm(void);
extern void setEventComplete(int, int);
extern void setEventStarted(int, int, int);
extern void FUN_80026c50(int, int, int);
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void func_801180BC(TObj *, unsigned short, short);

void func_80118EA0(TObj *o)
{
    TObj *s1 = SCRIPT_OBJECTS;
    TObj *s2 = D_8009F2DC;
    TObj *p;
    B12 v;

    switch (o->state) {
    case 0:
        if (D_8009CE3A != 0) break;
        if (GVARS.b3a == 0) break;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        PLAYER.b04 = 5;
        PLAYER.step = 0x64;
        PLAYER.state = 0;
        PLAYER.anim = D_80010814;
        FUN_8001fe6c(&PLAYER);
        o->state++;
        break;
    case 1:
        PLAYER.h->raw -= 0x12800;
        if (PLAYER.h->p.whole < 0x4a) {
            FUN_800eea7c(&PLAYER, 0, 0);
            o->w08 = 0x14;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 > 0) break;
        PLAYER.animFrame = 1;
        D_8009C330[9] = 5;
        PLAYER.b9c = 1;
        PLAYER.b04 = 6;
        PLAYER.b69 = 0;
        PLAYER.wb2 = 0;
        PLAYER.velH = 0;
        PLAYER.velV = 0;
        PLAYER.velX = 0;
        PLAYER.velY = 0;
        PLAYER.step = 4;
        PLAYER.state = 0;
        o->state++;
        break;
    case 3:
        if (PLAYER.b69 == 0) break;
        PLAYER.b04 = 5;
        PLAYER.step = 0x64;
        PLAYER.state = 0;
        FUN_800eea7c(&PLAYER, 0, 0);
        o->w08 = 0x1e;
        o->state++;
        break;
    case 4:
        if (--o->w08 > 0) break;
        PLAYER.animFrame = 0;
        PLAYER.anim = D_80010814;
        FUN_8001fe6c(&PLAYER);
        PLAYER.b04 = 5;
        PLAYER.step = 0x64;
        PLAYER.state = 0;
        o->state++;
        break;
    case 5:
        PLAYER.h->raw += 0x12800;
        if (PLAYER.h->p.whole >= 0x79) {
            FUN_800eea7c(&PLAYER, 0, 0);
            o->w08 = 0x1e;
            o->state++;
        }
        break;
    case 6:
        if (--o->w08 > 0) break;
        o->state = 10;
        break;
    case 10:
        cdSeekStream(0xb);
        PLAYER.b04 = 5;
        PLAYER.step = 0x62;
        GVARS.b3f = 1;
        GAME.fadeScreenControl = 3;
        o->state++;
        break;
    case 11:
        if (GAME.fadeScreenControl != 1) break;
        D_1F8001CC = 1;
        D_1F8001CD = 0xc;
        D_1F8001C6 = 2;
        FUN_8001f110(0);
        FUN_800171b8(1, FUN_8001d6a4);
        o->state++;
        break;
    case 12:
        GAME.fadeScreenControl = 4;
        startAreaBgm();
        D_8009CDF1 = 0xff;
        o->state++;
    case 13:
        if (GAME.fadeScreenControl != 0) break;
        o->w08 = 0x12c;
        setEventComplete(0x49, 0);
        o->state++;
        break;
    case 14:
        if (--o->w08 > 0) break;
        o->w08 = 1;
        o->state = 0xf;
        break;
    case 15:
        if (--o->w08 > 0) break;
        func_801180BC(s1, 1, 0);
        func_801180BC(s2, 1, 0);
        s1->step = 0;
        s2->step = 0;
        {
            short t = s1->h->p.whole;
            o->state++;
            o->w08 = t - 0xc0;
        }
        break;
    case 16:
        s1->h->p.whole--;
        s2->h->p.whole--;
        if (--o->w08 > 0) break;
        func_801180BC(s1, 0, 0);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x50;
        o->state++;
        break;
    case 17:
        if (--o->w08 > 0) break;
        func_801180BC(s1, 1, 0);
        o->w08 = 0x10;
        o->state++;
        break;
    case 18:
        s1->h->p.whole--;
        if (--o->w08 > 0) break;
        func_801180BC(s1, 0, 0);
        o->w08 = 0x1e;
        o->state++;
        break;
    case 19:
        if (--o->w08 > 0) break;
        o->state++;
        break;
    case 20:
        v = *(B12 *)&s1->a;
        s1->d90 = (int)showMessageBoxDirect(2, 0x20, &v);
        func_801180BC(s1, 2, 0);
        o->state++;
        break;
    case 21:
        {
            TObj *q = (TObj *)s1->d90;
            if (q->b04 != 2) break;
            q->b04 = 3;
        }
        v = *(B12 *)&s1->a;
        s1->d90 = (int)showMessageBoxDirect(2, 0x23, &v);
        o->state++;
        break;
    case 22:
        p = (TObj *)s1->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setEventStarted(0x76, 0, 0);
        func_801180BC(s1, 0, 0);
        o->w08 = 0x12c;
        o->state++;
        break;
    case 23:
        if (--o->w08 > 0) break;
        v = *(B12 *)&s2->a;
        s2->d90 = (int)showMessageBoxDirect(2, 0x26, &v);
        func_801180BC(s2, 2, 0);
        o->state++;
        break;
    case 24:
        p = (TObj *)s2->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x9b, 1, 1);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x78;
        o->state++;
        break;
    case 25:
        if (--o->w08 > 0) break;
        v = *(B12 *)&s2->a;
        s2->d90 = (int)showMessageBoxDirect(2, 0x27, &v);
        func_801180BC(s2, 2, 0);
        o->state++;
        break;
    case 26:
        p = (TObj *)s2->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        setEventStarted(0x96, 0, 0);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x12c;
        o->state = 0x63;
        break;
    case 99:
        if (--o->w08 > 0) break;
        D_1F8001C6 = 0;
        GVARS.b3e = 0;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        s1->step = 1;
        s2->step = 1;
        PLAYER.b04 = 1;
        PLAYER.animFrame = 0;
        PLAYER.wb2 = 0;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->b04 = 3;
        break;
    }
}
