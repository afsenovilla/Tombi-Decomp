// VERSION SLPM_80165 slpm80165 8002BC04 592
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of the JP updateItemGetIcon (src/updateItemGetIcon.c): the higher jump depends on the word D_8009A810 only.
#include "TOBJ.H"
extern int D_8009A810[];
extern void *ITEM_GET_ICON_ANIM;
extern void **ITEM_GET_ICON_ANIMS[];
extern unsigned char D_8009C942[];
extern int D_8009C954[];
extern unsigned short GAME[];
typedef struct SP {
    char p0[0x16a];
    unsigned short x;
    union { int i; struct { unsigned short lo, hi; } s; } y;
    short p170;
    unsigned short z;
    char p1[0x2d8 - 0x174];
    int w;
} SP;
#define SPAD ((SP *)0x1F800000)
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);
extern void playSFX(int);

typedef struct P {
    char p0[4];
    unsigned short w04;
    unsigned char b06;
    char p1[0x16 - 7];
    short y16;
    char p2[0x2e - 0x18];
    unsigned short w2e;
    char p3[0x40 - 0x30];
    Fix16 *h;
    Fix16 *d;
    char p4[0xac - 0x48];
    short wac;
} P;

void func_8002BC04(TObj *o)
{
    P *p;
    int dx;
    int dy;
    int st;

    switch (o->b04) {
    case 0:
        D_8009C954[0] = 1;
        o->w1e = 0x15;
        *(signed char *)&o->b0f = -9;
        o->animFrame = 0;
        o->b04++;
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 0;
        o->anim = *ITEM_GET_ICON_ANIMS[o->b0c];
        o->d3c = SPAD->w;
        o->a.p.whole = SPAD->x;
        o->y.p.whole = SPAD->y.s.hi - 0x1e;
        o->b.p.whole = SPAD->z;
        if (D_8009A810[0] == 2) {
            o->velV = -0x3e0;
        } else {
            o->velV = -0x380;
        }
        o->timer = 0x78;
        o->d34 = 0;
        AnimLoadDuration(o);
        playSFX(0x1a);
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800220(o);
        switch (o->step) {
        case 0:
            o->velV += 0x20;
            if (o->velV > 0x100) o->step++;
            o->d34 += o->velV << 8;
            break;
        case 1:
            if (--o->timer == -1) o->b04++;
            break;
        }
        o->a.p.whole = SPAD->x;
        o->y.raw = SPAD->y.i + o->d34;
        o->b.p.whole = SPAD->z;
        break;
    case 2:
        D_8009C954[0] = 0;
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
