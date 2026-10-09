// FUNC 8011f6c4 996 X001
// MATCHING 8011f6c4 996
#include "TOBJ.H"

typedef struct { Fix16 a, y, b; } FV;

extern TObj D_800A6038;
extern void *D_8013E704[], *D_8013E70C;
extern char D_8013C728[];
extern unsigned char D_8009CE55A[], D_8009CEAD, D_8009C93F, D_8009C942, D_8009C93E;
extern void FUN_8001fe6c(TObj *);
extern TObj *FUN_800183b8(void);
extern void FUN_8003c7c4(char *, FV *, int, int, int);
extern void FUN_8004d620(int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);

static __inline__ int vdiv(int a, int b)
{
    return a / b;
}

static __inline__ void start(TObj *o)
{
    int vy, vx;

    o->visible = D_800A6038.visible;
    o->animFrame = D_800A6038.animFrame & 1;
    o->anim = D_8013E704[0];
    o->h->p.whole = D_800A6038.h->p.whole;
    o->y.p.whole = D_800A6038.y.p.whole;
    o->d->p.whole = D_800A6038.d->p.whole;
    *(volatile short *)&o->timer = 20;
    vy = vdiv(-0x6c00 - (o->y.p.whole << 8), o->timer);
    o->d8c = 0;
    o->d30 = 0x20;
    o->d34 = -0x140;
    vx = vdiv(0x79b00 - (o->h->p.whole << 8), 20);
    o->category |= 0x80;
    o->velY = vy;
    o->velX = vx;
    o->b0f = D_800A6038.b0f - 1;
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
        n = FUN_800183b8();
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
        FUN_8003c7c4(D_8013C728, &v, 0, 0, 1);
        FUN_8004d620(0xf, 2);
        FUN_80026e0c(0x12, 1);
        FUN_80026c50(0xa, 1, 0);
        D_8009CEAD = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        o->b04 = 3;
        break;
    }
}
