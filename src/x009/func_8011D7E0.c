// FUNC 8011d7e0 1668 X009
// MATCHING 8011d7e0 1668
/* debt: char pad[4] (frame), "& 0xffff & 0xff" keeps sra, volatile pad read in case 2 */
#include "TOBJ.H"
typedef struct {
    char p0;
    unsigned char b01;
    short w02;
    char p4[4];
    unsigned char b08, b09;
    char pa[2];
    short w0c, w0e;
    char p10[0x28 - 0x10];
    unsigned short w28, w2a;
    char p2c[2];
    unsigned short w2e;
} PL;

extern PL *D_8009C330;
extern TObj *D_8009F0EC;
extern unsigned short D_8009D670[];
extern unsigned short D_1F8003C6;
extern char D_80011108[];
extern void func_800EE5C8(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_800eee90(TObj *);
extern void FUN_800ef1ac(TObj *);
extern void SfxPlay3(int, int);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void func_800F4E9C(TObj *);
extern void func_8011D680(TObj *, short, short);
extern void func_80121540(TObj *, short, short, int);

static __inline__ short adj(TObj *o, short k)
{
    int v = o->h->p.whole;
    if (o->animFrame & 1) return v - k;
    return v + k;
}

#define ADJ4() \
    { \
        Fix16 *h; \
        int v; \
        h = o->h; \
        v = h->p.whole; \
        h->p.whole = (o->animFrame & 1) ? v - 4 : v + 4; \
    }

#define RELEASE(k) \
    { \
        short x, y; \
        TObj *e; \
        x = adj(o, k); \
        e = D_8009F0EC; \
        y = o->y.p.whole - 0x10; \
        x = x - (e->h->p.whole - e->box0); \
        y = y - (e->y.p.whole + (e->box3 - e->box2)); \
        if ((unsigned short)x < e->box1) func_80121540(o, x, y, e->subtype); \
    }

void func_8011D7E0(TObj *o)
{
    PL *p;
    short s1, s2;
    char pad[4];

    switch (o->state) {
    case 0:
        p = D_8009C330;
        o->wb6 = -0x420;
        p->w02 = 0x10;
        p->w0e = 0;
        o->wb2 = 2;
        p->b08 = 0;
        D_8009C330->b09 = 0;
        D_8009C330->w0c = 0;
        D_8009C330->w28 = 0xffff;
        D_8009C330->w2a = 0xffff;
        *(unsigned char *)&o->waa = 1;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        FUN_800eee90(o);
        *(unsigned char *)&o->waa = 0;
        o->anim = D_80011108;
        AnimJump(o, 0);
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole - 4;
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        func_800EE5C8(o);
        if (o->wb6 == 0 && D_8009C330->w0e > 0) {
            o->wb2--;
            func_800F4E9C(o);
        }
        s1 = o->wb6 >> 4;
        p = D_8009C330;
        p->w2e = 0xffff;
        p->b01 = 0x12;
        func_8011D680(o, s2, s1);
        if (o->wb2 < 2) o->state++;
        break;
    case 2:
        func_800EE5C8(o);
        if (o->wb6 == 0) {
            p = D_8009C330;
            if (p->w0e > 0) {
                o->wb2--;
                func_800F4E9C(o);
            } else {
                p->b09 = 1;
            }
        }
        PlayerSetAnimIfChanged(o, 7);
        AnimAdvance(o);
        s1 = o->wb6 >> 4;
        s2 = (s1 & 0xffff & 0xff) >> 2;
        func_8011D680(o, s2, s1);
        if (o->wb2 == 0) {
            o->d8c = 0;
            {
                Fix16 *h = o->h;
                int w = o->w74;
                h->p.whole = (o->animFrame & 1) ? w - 8 : w + 8;
            }
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
            ADJ4();
            RELEASE(4);
            o->step = 0x27;
            o->state = 0;
        }
        if (*(volatile unsigned short *)D_8009D670 & 0x40) {
            ADJ4();
            RELEASE(8);
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
        AnimAdvance(o);
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
            ADJ4();
            RELEASE(8);
            D_8009C330->b09 = 0;
            o->b9e = 6;
            o->step = 0x25;
            o->state = 0;
        }
        break;
    }
    FUN_800ef1ac(o);
}
