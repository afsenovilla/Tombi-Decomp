// FUNC 800f8130 1328 X000
// MATCHING 800f8130 1328
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
    char pad4[0x2c - 4];
    unsigned short w2c;
    unsigned short w2e;
} G330;
extern G330 *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern char LAB_80010c50[];
extern char LAB_80010748[];
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fe6c(TObj *);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_8001e5f4(int, int);

#define ADJ(a, b) { Fix16 *h = o->h; int pv = h->p.whole; short d; if (o->animFrame & 1) d = pv + (a); else d = pv + (b); h->p.whole = d; }
#define U8F(o, n) (*(unsigned char *)((char *)(o) + (n)))

void FUN_800f8130(TObj *o)
{
    unsigned char b;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        DAT_8009c330->w2c = 9;
        o->wb6 = 0;
        DAT_8009c330->w02 = 0;
        o->wb2 = 0;
        ADJ(8, -8);
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        if (*(unsigned short *)o->anim == 0x87) {
            short v;
            if (o->animFrame & 1)
                v = -0x200;
            else
                v = 0x200;
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
            if (o->d88 > 0x150)
                o->d88 = 0x150;
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
            o->b9c = 2;
            o->b9e = 0;
            o->state = 3;
        }
        break;
    case 3:
        if (o->animFrame & 1) {
            o->d88 += 0x20;
            if (o->d88 > 0x160) {
                o->anim = LAB_80010c50;
                FUN_8001fe94(o, 0);
                o->d88 = 0x100;
                o->state++;
            }
        } else {
            o->d88 -= 0x20;
            if (o->d88 < 0xa0) {
                o->anim = LAB_80010c50;
                FUN_8001fe94(o, 0);
                o->d88 = 0x100;
                o->state++;
            }
        }
        o->d8c = U8F(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) {
            FUN_8001e5f4(0x1c, 0x7f);
            o->b9c = 0;
            U8F(o, 0xaa) = 0;
            o->anim = LAB_80010c50;
            FUN_8001fe94(o, 1);
            o->d88 = 0;
            b = DAT_801152e8[o->wb0];
            o->state = 5;
            o->d8c = b;
        }
        break;
    case 4:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x120)
                o->d88 = 0x120;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xe0)
                o->d88 = 0xe0;
        }
        o->d8c = U8F(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->b69) {
            FUN_8001e5f4(0x1c, 0x7f);
            o->b9c = 0;
            U8F(o, 0xaa) = 0;
            o->anim = LAB_80010c50;
            FUN_8001fe94(o, 1);
            ADJ(-12, 12);
        } else {
            if (!FUN_8003fd78(o, 0, 0))
                break;
            FUN_8001e5f4(0x1c, 0x7f);
            o->b9c = 0;
            U8F(o, 0xaa) = 0;
            o->anim = LAB_80010c50;
            FUN_8001fe94(o, 1);
            ADJ(-12, 12);
        }
        o->d88 = 0;
        {
            unsigned char c = DAT_801152e8[o->wb0];
            o->state++;
            o->d8c = c;
        }
        break;
    case 5:
        if (FUN_8001fec0(o)) {
            *(signed char *)&o->b0f = -8;
            DAT_8009c330->w02 = 0;
            o->b9e = 0;
            U8F(o, 0xaa) = 0;
            o->velX = 0;
            o->velY = 0;
            o->animFrame = 0;
            o->timer = 0x46;
            o->d8c = DAT_801152e8[o->wb0];
            ADJ(10, -10);
            o->anim = LAB_80010748;
            FUN_8001fe6c(o);
            o->b04 = 5;
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
