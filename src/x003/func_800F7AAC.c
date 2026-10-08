// FUNC 800f7aac 1408 X003
// MATCHING 800f7aac 1408
#include "TOBJ.H"
extern TObj *D_8009C330;
extern unsigned char DAT_801152e8[];
extern char LAB_80010c50[];
extern char LAB_80010748[];
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fe6c(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);
extern void ObjSetAnimFromTable(TObj *);

#define U8F(o, n) (*(unsigned char *)((char *)(o) + (n)))

static __inline__ void setanim(TObj *o, unsigned short anim)
{
    TObj *p = D_8009C330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        FUN_8001fe94(o, 0);
        D_8009C330->animFrame = D_8009C330->animTimer;
    }
}

void func_800F7AAC(TObj *o)
{
    unsigned char b;
    short v;
    int k;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        setanim(o, 9);
        U8F(o, 0xaa) = 0;
        o->wb6 = 0;
        *(short *)((char *)D_8009C330 + 2) = 0;
        k = o->b9e;
        o->wb2 = 0;
        if (k == 0 || (k >= 3 && k != 11)) {
            Fix16 *h = o->h;
            int pv = h->p.whole;
            short d;
            if (o->animFrame & 1) d = pv + 8; else d = pv - 8;
            h->p.whole = d;
        }
        {
            int hw = o->h->p.whole;
            o->d34 = o->y.p.whole;
            o->state++;
            o->d30 = hw;
        }
    case 1:
        FUN_8001fec0(o);
        if (*(unsigned short *)o->anim == 0x61) {
            if (o->animFrame & 1)
                v = -0x1c0;
            else
                v = 0x1c0;
            o->velY = -0x300;
            o->d88 = 0x100;
            o->velX = v;
            o->b9e = 0;
            o->state = 2;
        } else {
            o->h->p.whole = o->d30;
            o->y.p.whole = ((unsigned short *)o->anim)[2] + o->d34;
        }
        break;
    case 2:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x170)
                o->d88 = 0x170;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xb0)
                o->d88 = 0xb0;
        }
        o->d8c = U8F(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0) {
            if (o->animFrame & 1)
                v = -0x100;
            else
                v = 0x100;
            o->b9c = 2;
            o->velX = v;
            o->b9e = 0;
            o->state = 3;
        }
        break;
    case 3:
        if (o->animFrame & 1) {
            o->d88 += 0x20;
            if (o->d88 <= 0x160) goto skip;
            goto turn;
        } else {
            o->d88 -= 0x20;
            if (o->d88 < 0xa0) {
            turn:
                o->anim = LAB_80010c50;
                FUN_8001fe94(o, 3);
                o->d88 = 0x100;
                o->timer = 0xc;
                o->state++;
            }
        }
    skip:
        o->d8c = U8F(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            o->b9c = 0;
            o->anim = LAB_80010c50;
            FUN_8001fe94(o, 4);
            o->d30 = 0;
            o->d34 = 0;
            o->d88 = 0;
            o->d8c = 0;
            U8F(o, 0xaa) = 0;
            o->state = 5;
        }
        break;
    case 4:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x130)
                o->d88 = 0x130;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xd0)
                o->d88 = 0xd0;
        }
        o->d8c = U8F(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            o->b9c = 0;
            U8F(o, 0xaa) = 0;
            o->anim = LAB_80010c50;
            FUN_8001fe94(o, 4);
            o->d88 = 0;
            b = DAT_801152e8[o->wb0];
            o->state++;
            o->d8c = b;
        } else if (--o->timer <= 0) {
            int d;
            U8F(o, 0xac) = 1;
            o->timer = 10;
            o->d84 = 0;
            if (o->animFrame & 1)
                d = 0xf0;
            else
                d = 0x10;
            o->step = 2;
            o->d88 = d;
            o->d8c = 0;
            o->state = 3;
        }
        break;
    case 5:
        if (FUN_8001fec0(o)) {
            *(signed char *)&o->b0f = -8;
            *(short *)((char *)D_8009C330 + 2) = 0;
            U8F(o, 0xaa) = 0;
            o->velX = 0;
            o->velY = 0;
            b = DAT_801152e8[o->wb0];
            o->anim = LAB_80010748;
            o->d8c = b;
            FUN_8001fe6c(o);
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
