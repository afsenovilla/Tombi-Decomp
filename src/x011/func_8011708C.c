// FUNC 8011708c 1236 X011
// MATCHING 8011708c 1236
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned char c[12]; } B12;
typedef struct { void **p; int x, y; } AT;
extern int showMessageBoxDirect(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern TObj *allocObjectLayer5(void);
extern void setEventStarted(int, int, int);
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;
extern short D_1F8001C6;
extern AT D_80119C48[];

void func_8011708C(TObj *o)
{
    B12 v;
    signed char *p;
    TObj *q;

    switch (o->state) {
    case 0:
        p = &D_8009D2B0;
        if (*p == 3) break;
        if ((unsigned short)((*(unsigned short*)&PLAYER.obj.a.p.whole) - 0x40) >= 0x20) break;
        if ((*(Fix16 **)&PLAYER.obj.d)->p.whole != 0) break;
        if (!(D_1F8001FC & D_1F8003C4)) break;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        *p = 0;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[1];
        FUN_8001fe6c(o);
        o->d90 = showMessageBoxDirect(2, 6, &v);
        o->state++;
        break;
    case 1:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        D_1F8001C6 = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->animFrame = 1;
        q = allocObjectLayer5();
        if (q != 0) {
            q->active = 2;
            q->type = 0xf;
            q->subtype = 0x33;
            q->b0c = 0x80;
            q->b0f = 0;
            q->animFrame = 0;
        }
        o->anim = D_80119C48[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state = 2;
        break;
    case 2:
        if (GCD.bd082 != 1) break;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        FUN_8001fe6c(o);
        o->d90 = showMessageBoxDirect(2, 7, &v);
        o->state++;
        break;
    case 3:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        GCD.bd082 = 2;
        o->state++;
        break;
    case 4:
        if (GCD.bd082 != 3) break;
        v = *(B12 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 8, &v);
        o->state++;
        break;
    case 5:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        setEventStarted(0x68, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 6:
        if (--o->timer != 0) break;
        o->animFrame = 0;
        v = *(B12 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 9, &v);
        o->state++;
        break;
    case 7:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        v = *(B12 *)&o->a;
        o->d90 = showMessageBoxDirect(2, 0xa, &v);
        o->state++;
        break;
    case 8:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state++;
        break;
    case 9:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
