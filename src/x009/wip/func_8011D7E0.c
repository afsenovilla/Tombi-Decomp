/* score 4: only two insns differ: game computes (s1 & 0xff) >> 2 with sra (gcc folds to srl in every form tried: casts, inline, multi-set temp, arg assignment) and loads o->w74 after o->h in case 2. Tried int/short/char types of s1/s2/x, if/else and ternary forms. */
// FUNC 8011d7e0 1668 X009
#include "TOBJ.H"

typedef struct {
    unsigned char b00, b01;
    short w02;
    unsigned char p04[4];
    unsigned char b08, b09;
    unsigned char p0a[2];
    short w0c, w0e;
    unsigned char p10[0x18];
    unsigned short w28, w2a;
    unsigned char p2c[2];
    unsigned short w2e;
} PL;

extern PL *D_8009C330;
extern TObj *D_8009F0EC;
extern volatile unsigned short D_8009D670[];
extern unsigned short D_1F8003C6;
extern char D_80011108[];
extern void FUN_800ee5c8(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_800f4e9c(TObj *);
extern void FUN_800eee90(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_800ef1ac(TObj *);
extern void func_8011D680(TObj *, short, short);
extern void func_80121540(TObj *, short, short, unsigned char);

static __inline__ void hit(TObj *o, short x, short y)
{
    TObj *q = D_8009F0EC;
    short dx = x - (q->h->p.whole - q->box0);
    short dy = y - (q->y.p.whole + (q->box3 - q->box2));
    if ((unsigned short)dx < q->box1) func_80121540(o, dx, dy, q->subtype);
}

#define HIT(o, k) { int t = o->h->p.whole; hit(o, (o->animFrame & 1) ? t - k : t + k, o->y.p.whole - 0x10); }

void func_8011D7E0(TObj *o)
{
    short s1, s2;
    char pad[8];
    int x;

    switch (o->state) {
    case 0:
        o->wb6 = -0x420;
        D_8009C330->w02 = 0x10;
        D_8009C330->w0e = 0;
        o->wb2 = 2;
        D_8009C330->b08 = 0;
        D_8009C330->b09 = 0;
        D_8009C330->w0c = 0;
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        *((unsigned char *)o + 0xaa) = 1;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        FUN_800eee90(o);
        *((unsigned char *)o + 0xaa) = 0;
        o->anim = D_80011108;
        FUN_8001fe94(o, 0);
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole - 4;
        FUN_8001e5f4(4, 0x7f);
        o->state++;
    case 1:
        FUN_800ee5c8(o);
        if (o->wb6 == 0 && D_8009C330->w0e > 0) {
            o->wb2--;
            FUN_800f4e9c(o);
        }
        {
            short t = o->wb6 >> 4;
            D_8009C330->w2e = 0xffff;
            D_8009C330->b01 = 0x12;
            func_8011D680(o, s2, t);
        }
        if (o->wb2 < 2) o->state++;
        break;
    case 2:
        FUN_800ee5c8(o);
        if (o->wb6 == 0) {
            if (D_8009C330->w0e > 0) {
                o->wb2--;
                FUN_800f4e9c(o);
            } else {
                D_8009C330->b09 = 1;
            }
        }
        FUN_800eeb5c(o, 7);
        FUN_8001fec0(o);
        s1 = o->wb6 >> 4;
        s2 = (short)(s1 & 0xff) >> 2;
        func_8011D680(o, s2, s1);
        if (o->wb2 == 0) {
            o->d8c = 0;
            x = o->w74;
            o->h->p.whole = (o->animFrame & 1) ? x - 8 : x + 8;
            o->y.p.whole = o->w76 + 0xc;
            o->state++;
        }
        if (D_8009C330->b09 == 0) break;
        func_8011D680(o, s2, s1);
        if (D_8009D670[0] & D_1F8003C6) {
            D_8009C330->b09 = 0;
            o->step = 0x27;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x10) {
            D_8009C330->b09 = 0;
            { Fix16 *h = o->h; x = h->p.whole; h->p.whole = (o->animFrame & 1) ? x - 4 : x + 4; }
            HIT(o, 4);
            o->step = 0x27;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x40) {
            { Fix16 *h = o->h; x = h->p.whole; h->p.whole = (o->animFrame & 1) ? x - 4 : x + 4; }
            HIT(o, 8);
            D_8009C330->b09 = 0;
            o->b9e = 6;
            o->step = 0xd;
            o->state = 0;
            o->y.p.whole += 9;
        }
        break;
    case 3:
        o->h->p.whole += D_8009F0EC->velX;
        o->y.p.whole += D_8009F0EC->velY;
        FUN_8001fec0(o);
        if (D_8009D670[0] & D_1F8003C6) {
            D_8009C330->b09 = 0;
            o->step = 0x27;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x10) {
            D_8009C330->b09 = 0;
            o->step = 0x27;
            o->state = 0;
        }
        if (D_8009D670[0] & 0x40) {
            o->y.p.whole += 9;
            { Fix16 *h = o->h; x = h->p.whole; h->p.whole = (o->animFrame & 1) ? x - 4 : x + 4; }
            HIT(o, 8);
            D_8009C330->b09 = 0;
            o->b9e = 6;
            o->step = 0x25;
            o->state = 0;
        }
        break;
    }
    FUN_800ef1ac(o);
}
