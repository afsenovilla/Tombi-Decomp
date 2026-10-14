// FUNC 8011f6c4 996 X001
// MATCHING 8011f6c4 996
#include "gvars.h"
#include "TOBJ.H"

typedef struct { Fix16 a, y, b; } FV;

extern TObj PLAYER;
extern void *D_8013E704[], *D_8013E70C;
extern char D_8013C728[];
extern unsigned char D_8009CE55A[], D_8009CEAD;
extern void FUN_8001fe6c(TObj *);
extern TObj *allocObjectLayer2(void);
extern void applyItemEffect(char *, FV *, int, int, int);
extern void printInfoMessage(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);

static __inline__ int vdiv(int a, int b)
{
    return a / b;
}

static __inline__ void start(TObj *o)
{
    int vy, vx;

    o->visible = PLAYER.visible;
    o->animFrame = PLAYER.animFrame & 1;
    o->anim = D_8013E704[0];
    o->h->p.whole = PLAYER.h->p.whole;
    o->y.p.whole = PLAYER.y.p.whole;
    o->d->p.whole = PLAYER.d->p.whole;
    *(volatile short *)&o->timer = 20;
    vy = vdiv(-0x6c00 - (o->y.p.whole << 8), o->timer);
    o->d8c = 0;
    o->d30 = 0x20;
    o->d34 = -0x140;
    vx = vdiv(0x79b00 - (o->h->p.whole << 8), 20);
    o->category |= 0x80;
    o->velY = vy;
    o->velX = vx;
    o->b0f = PLAYER.b0f - 1;
    o->state++;
}

void func_8011F6C4(TObj *o)
{
    TObj *n;
    FV v;
    int t;

    switch (o->state) {
    case 0:
        start(o);
        break;
    case 1:
        if (--o->timer > 0)
            break;
        o->timer = 0xf;
        o->state++;
        break;
    case 2:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->y.raw += o->d34 << 8;
        o->d34 += o->d30;
        if (--o->timer > 0)
            break;
        o->h->p.whole = 0x79b;
        o->y.p.whole = -0x6c;
        o->timer = 20;
        o->state++;
        break;
    case 3:
        if (--o->timer > 0)
            break;
        o->d8c = 0x100;
        o->timer = 0x40;
        o->state++;
        break;
    case 4:
        t = o->d8c;
        if (o->animFrame & 1)
            t += 2;
        else
            t -= 2;
        o->d8c = t & 0xff;
        if (--o->timer > 0)
            break;
        o->anim = D_8013E70C;
        FUN_8001fe6c(o);
        o->timer = 0x50;
        n = allocObjectLayer2();
        if (n != 0) {
            n->active = 1;
            n->subtype = 1;
            n->type = 0x22;
            n->a.p.whole = o->a.p.whole;
            n->y.p.whole = o->y.p.whole + 0x20;
            n->b.p.whole = o->b.p.whole;
            D_8009CE55A[0] = 2;
        }
        o->state++;
        break;
    case 5:
        if (--o->timer > 0)
            break;
        v.a.p.whole = o->a.p.whole;
        v.y.p.whole = o->y.p.whole + 0x30;
        v.b.p.whole = o->b.p.whole;
        applyItemEffect(D_8013C728, &v, 0, 0, 1);
        printInfoMessage(0xf, 2);
        FUN_80026e0c(0x12, 1);
        FUN_80026c50(0xa, 1, 0);
        D_8009CEAD = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        o->b04 = 3;
        break;
    }
}
