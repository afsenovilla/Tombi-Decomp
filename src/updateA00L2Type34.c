// FUNC 80134e5c 3248 X000
// MATCHING 80134e5c 3248
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { char pad[0x4c]; short w4c; short w4e; } C4C;
typedef struct { short p0, x, p4, y, p8, z; } Pos;
extern unsigned char D_8009C970_[];
#define D_8009C970 D_8009C970_[0]
extern unsigned char D_8009C93F_[];
#define D_8009C93F D_8009C93F_[0]
extern unsigned char D_8009C93B_[];
#define D_8009C93B D_8009C93B_[0]
extern unsigned char D_8009C93A_[];
#define D_8009C93A D_8009C93A_[0]
extern unsigned char D_8009C975_[];
#define D_8009C975 D_8009C975_[0]
extern signed char D_8009D2B0_[];
#define D_8009D2B0 D_8009D2B0_[0]
extern void *D_8013B2CC[], *D_8013B2D0[], *D_8013B2D4[], *D_8013B2D8[], *D_8013B2DC[], *D_8013B2E0[], *D_8013B2EC[];
extern void *D_1F8002D4;
extern char D_80077CDC[];
extern char D_801393EC[];
extern char D_80139434[];
extern char D_8013947C[];
extern char D_8013953C[];
extern char D_8013955C[];
extern char D_8013957C[];
extern char D_80139614[];
extern char D_8013963C[];
static char D_80139678[40] = {
    0x16, 0, 0, 0, 2, 0, 0, 0, 0x1b, 0, 0, 0, 0x11, 0, 0, 0,
    6, 0, 0x10, 0, 8, 0, 0, 0, 0x12, 0, 0, 0, 3, 0, 0, 0,
    0x1c,
};
static short *D_801396A0[9] = {
    (short *)D_801393EC, (short *)D_80139434, (short *)D_8013947C, (short *)D_8013953C,
    (short *)D_8013955C, (short *)D_8013957C, (short *)D_80139614, (short *)D_8013963C,
    (short *)D_80139678,
};
extern unsigned short D_1F8003C4, D_1F8001FC;
extern unsigned char D_800A603E_[];
#define D_800A603E D_800A603E_[0]
extern unsigned char D_800A603D_[];
#define D_800A603D D_800A603D_[0]
extern unsigned char D_800A603C_[];
#define D_800A603C D_800A603C_[0]
extern int D_8009C984_[];
#define D_8009C984 D_8009C984_[0]
extern unsigned char D_1F8001CD_[];
#define D_1F8001CD D_1F8001CD_[0]
extern unsigned char D_1F8001CC_[];
#define D_1F8001CC D_1F8001CC_[0]
extern short D_1F8001C6_[];
#define D_1F8001C6 D_1F8001C6_[0]
extern short D_800A60EA_[];
#define D_800A60EA D_800A60EA_[0]
extern short D_800A6066_[];
#define D_800A6066 D_800A6066_[0]
extern C4C *TASK_C;
extern short D_8009CD94_[];
#define D_8009CD94 D_8009CD94_[0]
extern short D_8009CDA0_[];
#define D_8009CDA0 D_8009CDA0_[0]
extern short D_8009CD96_[];
#define D_8009CD96 D_8009CD96_[0]
extern short D_800A60D2_[];
#define D_800A60D2 D_800A60D2_[0]
extern short D_800A60D0_[];
#define D_800A60D0 D_800A60D0_[0]
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern void applyAnimVelocityY(TObj *, int);
extern int showMessageBox(int, int, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void FUN_8001f110(int);
extern void FUN_800171b8(int, void *);
extern void FUN_8001d6a4();
extern void startAreaBgm(void);
extern void dispatchAreaItemUpdate(TObj *, Pos *, int);
extern void FUN_80026c50(int, int, int);
extern void FUN_8001e4f0(int);
extern int FUN_800202b4(TObj *);
extern void FUN_8001fec0(TObj *);
extern void freeObjectLayer2(TObj *);

void updateA00L2Type34(TObj *o)
{
    short *e;
    Pos s;

    switch (o->b04) {
    case 0:
        if (GCD.bcda5 != 0xff) {
            D_8009C93F = 1;
            switch (GCD.bd0a4) {
            case 0:
                o->wb4 = 0;
                break;
            case 1 ... 3:
                o->wb4 = 1;
                break;
            case 4:
                o->wb4 = 2;
                break;
            }
        } else if (GCD.bcdad != 0xff) {
            D_8009C93F = 1;
            switch (GCD.bd0a4) {
            case 0:
                o->wb4 = 3;
                break;
            case 1 ... 3:
                o->wb4 = 4;
                break;
            case 4:
                o->wb4 = 5;
                break;
            }
        } else if (D_8009C970 < GAME.playerHealthDisplayed) {
            o->wb4 = 7;
        } else {
            o->wb4 = 6;
        }
        o->active = 2;
        o->w1e = 1;
        o->wb6 = 0;
        o->b0d = 1;
        o->w08 = FUN_8005e420(0xc0, 0x1e0);
        {
            int d = (int)D_1F8002D4;
            void *a = D_8013B2CC[0];
            o->d3c = d;
            o->anim = a;
        }
        FUN_8001fe6c(o);
        o->b0a = 0;
        o->b0f = 0;
        o->animFrame = 0;
        o->movetab = D_80077CDC;
        o->timer = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        e = D_801396A0[(unsigned short)o->wb4];
        e += (unsigned short)o->wb6 * 2;
        switch (e[0]) {
        case 0:
            break;
        case 1:
            if (o->timer == 0) {
                o->timer = e[1];
            } else if (--o->timer == 0) {
                o->wb6++;
            }
            break;
        case 2:
            switch (o->step) {
            case 0:
                o->anim = D_8013B2DC[0];
                FUN_8001fe6c(o);
                o->h->raw = 0x880000;
                o->y.raw = -0x3c0000;
                o->step++;
                break;
            case 1:
                if (D_8009D2B0 == 3) break;
                if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 32) >= 64) break;
                if (!(D_1F8001FC & D_1F8003C4)) break;
                if (PLAYER.obj.b69 == 0) break;
                if (D_800A603C != 1) break;
                if ((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole > 0) o->animFrame = 0;
                else o->animFrame = 1;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                D_8009D2B0 = 0;
                D_8009C93F = 1;
                o->anim = D_8013B2E0[0];
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->step++;
                break;
            case 2:
                if (--o->w22 == 0) o->wb6++;
                break;
            }
            break;
        case 3:
            if (D_8009D2B0 == 3) break;
            if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 32) >= 64) break;
            if (!(D_1F8001FC & D_1F8003C4)) break;
            if (PLAYER.obj.b69 == 0) break;
            if (D_800A603C != 1) break;
            if ((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole > 0) o->animFrame = 0;
            else o->animFrame = 1;
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009D2B0 = 0;
            D_8009C93F = 1;
            o->wb6++;
            break;
        case 4:
            switch (o->step) {
            case 0:
                o->anim = D_8013B2D0[0];
                FUN_8001fe6c(o);
                o->step++;
            case 1:
                if (o->h->p.whole < 40) o->animFrame = 0;
                if (o->h->p.whole >= 161) o->animFrame = 1;
                applyAnimVelocityX(o, o->animFrame);
                {
                    Fix16 *p = (*(Fix16 **)&PLAYER.obj.h);
                    if (p->p.whole >= 180 && D_800A603C != 1) break;
                    p->p.whole = 180;
                }
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                D_8009C93A = 1;
                D_8009C984 |= 0x10;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 5:
            switch (o->step) {
            case 0:
                if (o->animFrame == 0) {
                    o->step = 2;
                    break;
                }
                o->anim = D_8013B2EC[0];
                FUN_8001fe6c(o);
                o->w22 = 21;
                o->step++;
                break;
            case 1:
                if (--o->w22 == 0) {
                    o->animFrame = 0;
                    o->step++;
                }
                break;
            case 2:
                applyAnimVelocityX(o, o->animFrame);
                if (o->h->p.whole >= 121) {
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 6:
            o->anim = D_8013B2D4[0];
            FUN_8001fe6c(o);
        case 7:
            o->d90 = showMessageBox(1, e[1], 120, 156);
            o->wb6++;
            break;
        case 8:
            if (((unsigned char *)o->d90)[4] < 2) break;
            o->anim = D_8013B2CC[0];
            FUN_8001fe6c(o);
            ((unsigned char *)o->d90)[4]++;
            o->wb6++;
            break;
        case 9:
            setEventStarted(e[1], 0, 0);
            o->wb6++;
            break;
        case 10:
            setEventComplete(e[1], 0);
            o->wb6++;
            break;
        case 11:
            D_8009C975 = 3;
            o->wb6++;
            break;
        case 12:
            if (D_8009C975 == 1) o->wb6++;
            break;
        case 13:
            D_8009C975 = 4;
            o->wb6++;
            break;
        case 14:
            if (D_8009C975 == 0) o->wb6++;
            break;
        case 15:
            D_1F8001CC = 1;
            D_1F8001CD = 3;
            D_1F8001C6 = 2;
            FUN_8001f110(0);
            FUN_800171b8(1, FUN_8001d6a4);
            o->wb6++;
            break;
        case 16:
            startAreaBgm();
            o->wb6++;
            break;
        case 17:
            D_800A603C = 5;
            D_800A603D = 0;
            D_800A603E = 0;
            {
                short t = (*(Fix16 **)&PLAYER.obj.h)->p.whole > o->h->p.whole;
                D_8009C93F = 1;
                D_800A6066 = t;
            }
            o->wb6++;
            break;
        case 18:
            D_800A60EA = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            D_8009C93F = 0;
            o->wb6++;
            break;
        case 19:
            D_800A603C = 5;
            D_800A603D = 3;
            D_800A603E = 0;
            o->wb6++;
            break;
        case 20:
            {
                C4C *c = TASK_C;
                D_8009CD96 = 2;
                D_8009CDA0 = 1;
                D_8009CD94 = 0;
                c->w4c = 7;
                c->w4e = 0;
            }
            o->wb6++;
            break;
        case 21:
            {
                unsigned char *q = (unsigned char *)o->d94;
                q[5] = 1;
                q[6] = 0;
            }
            o->wb6++;
            break;
        case 22:
            {
                unsigned char *q = (unsigned char *)o->d94;
                q[5] = 6;
                q[6] = 0;
            }
            o->wb6++;
            break;
        case 23:
            switch (o->step) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8013B2D0[0];
                FUN_8001fe6c(o);
                o->movetab = D_80077CDC;
                o->step++;
            case 1:
                applyAnimVelocityY(o, 0);
                if (o->y.p.whole < -51) break;
                o->step++;
                break;
            case 2:
                applyAnimVelocityX(o, o->animFrame);
                if (o->h->p.whole < 110) {
                    o->anim = D_8013B2D8[0];
                    FUN_8001fe6c(o);
                    D_8009C93B = 1;
                    o->w22 = 60;
                    o->step++;
                }
                break;
            case 3:
                if (--o->w22 == 0) {
                    o->step = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 24:
            switch (o->step) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8013B2D8[0];
                FUN_8001fe6c(o);
                o->w22 = 60;
                o->step++;
                break;
            case 1:
                if (--o->w22 == 0) {
                    s.x = o->h->p.whole;
                    s.y = o->y.p.whole;
                    s.z = o->d->p.whole;
                    dispatchAreaItemUpdate(o, &s, 0);
                    o->animFrame = 1;
                    o->step++;
                }
                break;
            case 2:
                applyAnimVelocityX(o, 0);
                if ((unsigned short)((*(unsigned short*)&PLAYER.obj.a.p.whole) - o->h->p.whole) < 32) o->step++;
                break;
            case 3:
                if (--o->y.p.whole < -56) {
                    o->step = 0;
                    o->animFrame = 0;
                    o->wb6++;
                }
                break;
            }
            break;
        case 25:
            FUN_80026c50(6, 1, 1);
            o->wb6++;
            break;
        case 26:
            {
                int v = GAME.playerHealthDisplayed;
                D_800A60D0 = v;
                D_800A60D2 = v;
                D_8009C970 = v;
            }
            FUN_8001e4f0(10);
            o->wb6++;
            break;
        case 27:
            o->wb6++;
            o->wb8 = o->wb6;
            break;
        case 28:
            o->wb6 = o->wb8;
            break;
        }
        if (FUN_800202b4(o)) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
