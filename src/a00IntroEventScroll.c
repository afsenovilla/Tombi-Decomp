// FUNC 80116818 1276 X000
// MATCHING 80116818 1276
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"
#include "gamestate.h"
typedef struct { char pad[0x12]; short w12; char p14[2]; short w16; char p18[0x2e - 0x18]; short w2e; } G6038;
extern G6038 PLAYER;
extern unsigned char D_8009C966, D_8009C933;
extern short D_1F8001C6;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern void updateCameraFollow(TObj *);
extern void updateCameraScroll(TObj *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern unsigned char *allocObjectUnlayered(void);
extern void keyOffActiveSfxVoices(void);
extern void FUN_8001f110(int);
extern void cdSeekStream(int);
extern void FUN_8001f4bc(void);
extern void startAreaBgm(void);
extern void FUN_8001d6a4();
extern void FUN_800171b8(int, void *);

static __inline__ void slide(TObj *o)
{
    int v;
    int c;
    int *p;

    *(int *)o->d34 += o->y.raw;
    if (o->y.raw > 0) {
        p = (int *)o->d34;
        v = (short)o->animFrame;
        c = v < ((short *)p)[1];
    } else {
        p = (int *)o->d34;
        v = (short)o->animTimer;
        c = ((short *)p)[1] < v;
    }
    if (c) {
        *p = v << 16;
        o->y.raw = 0;
    }
}

void a00IntroEventScroll(TObj *o)
{
    G6038 *g = &PLAYER;
    unsigned char *p;

    switch (o->state) {
    case 0:
        updateCameraFollow(o);
        if (GAME.selectedSection != 0) break;
        if ((short)o->animFrame <= g->w12) g->w12 = o->animFrame;
        if (GVARS.b38 != 0) break;
        if (g->w12 < 0x49e) break;
        if (g->w16 >= -0xe7) break;
        D_8009C966 = 1;
        o->state++;
        setEventStarted(2, 0, 1);
        if (GCD.bd095 != 0) break;
        p = allocObjectUnlayered();
        if (p == 0) break;
        p[0] = 1;
        p[2] = 0xd;
        p[3] = 1;
        if (D_8009C933 == 0) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GV2.bb0 = 0;
            p[1] = 0;
        } else {
            p[1] = 1;
        }
        GCD.bd095 = 1;
        break;
    case 1:
        updateCameraFollow(o);
        if ((short)o->animFrame <= g->w12) g->w12 = o->animFrame;
        if (GVARS.b38 != 0) break;
        if ((GAME.area00_eventControl & 1) && GVARS.b30 == 3) {
            g->w2e = 0;
            o->animFrame = 0x52e;
            o->y.raw = 0x10000;
            o->w48 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            keyOffActiveSfxVoices();
            FUN_8001f110(1);
            cdSeekStream(2);
            o->state++;
        } else if (g->w12 < 0x49e) {
            D_8009C966 = 2;
            o->state--;
        }
        break;
    case 2:
        if (o->w48 != 0) {
            o->w48--;
            break;
        }
        slide(o);
        updateCameraScroll(o);
        if (o->y.raw != 0) break;
        if (o->visible == 0) {
            GVARS.b30 = 4;
            o->state = 4;
            o->animFrame = 0x5ae;
            o->y.raw = 0x20000;
            o->w48 = 0x78;
        } else {
            GVARS.b3e = 0;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            o->animTimer = 0xa0;
            o->animFrame = (*(unsigned short*)((char*)&o->h + 2));
            o->state++;
        }
        break;
    case 3:
        o->subtype = 0;
        o->state = 0;
        break;
    case 4:
        if (o->w48 != 0) {
            if (--o->w48 < 80) slide(o);
            if (o->w48 == 60) {
                ((unsigned char*)&GAME.fadeScreenControl)[0] = 3;
                ((unsigned char*)&GVARS.b3c)[0] = 1;
            }
        } else {
            D_8009C966 = 3;
            GVARS.b30 = 0;
            D_1F8001C6 = 2;
            FUN_8001f4bc();
            o->state++;
            D_1F8001CC = 1;
            D_1F8001CD = 2;
            FUN_800171b8(1, FUN_8001d6a4);
        }
        break;
    case 5:
        if (D_1F8001CC == 0) {
            o->animFrame = 0x52e;
            D_1F8001C6 = 0;
            *(int *)o->d34 = 0x52e0000;
            o->y.raw = 0;
            ((unsigned char*)&GAME.fadeScreenControl)[0] = 4;
            ((unsigned char*)&GVARS.b3c)[0] = 0;
            o->state = 2;
            o->animTimer = 0x4c0;
            o->y.raw = -0x10000;
            o->visible = 1;
            startAreaBgm();
            setEventComplete(2, 0);
        }
        break;
    }
}
