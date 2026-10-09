// FUNC 8012df98 2896 X001
// MATCHING 8012df98 2896
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;
typedef struct {
    unsigned char a0, a1;   /* 0xb4 */
    unsigned short b;       /* 0xb6 */
    short wb8;              /* 0xb8 */
} Sub;

extern AH *D_8013DB10[], *D_8013DAF8[], *D_8013DAFC[], *D_8013DB14[], *D_8013DB18[], *D_8013DB1C[];
extern unsigned char D_8013C800[];
extern char D_80077D0C[], D_80077CF4[], D_80077CDC[];
extern short D_1F80027E, D_1F800284;
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *o, short x, short y);
extern void func_8002B920(TObj *);
extern int func_8012BB18(TObj *);

#define SETANIM(A) { unsigned char *pv; \
    o->anim = (A)[0]; \
    pv = &D_8013C800[((AH *)o->anim)->w2 * 4]; \
    o->box0 = *pv++; \
    o->box1 = *pv++; \
    o->box2 = *pv++; \
    o->box3 = *pv; \
    o->animTimer = ((AH *)o->anim)->w6 & 0x3fff; }

static __inline__ int land2(TObj *o)
{
    short t;
    short w;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        t = D_1F80027E;
        if (t < 0) t = -t;
        if (t >= 9) t = 8;
        if (D_1F80027E < 0) t = -t;
        o->d8c = -t & 0xff;
        o->wb2 = t;
        w = D_1F800284;
        o->wb6 = (-t << 2) & 0xff;
        o->b9c = 0;
        o->wae = w;
        return 1;
    }
    return 0;
}

static __inline__ void blk(TObj *o)
{
    Sub *s = (Sub *)&o->wb4;
    if (o->movetab) FUN_8001fa88(o, 1 - o->animFrame);
    if ((o->b9d & 2) && o->animFrame != (o->b9d & 1)) o->movetab = 0;
    if (o->animFrame == 1) s->wb8 = 0x10;
    else s->wb8 = -0x10;
    if (func_8004065C(o, o->h->p.whole + s->wb8, o->y.p.whole, 1 - o->animFrame)) o->movetab = 0;
}

void func_8012DF98(TObj *o)
{
    Sub *t = (Sub *)&o->wb4;
    unsigned char c;

    switch (o->state) {
    case 0:
        o->b9d = 0;
        playSFX(0xf);
        o->velV = -0x400;
        o->b69 = 0;
        o->d8c = 0;
        o->animFrame = 1 - o->w7a;
        o->movetab = D_80077D0C;
        o->wac = 10;
        o->state++;
        SETANIM(D_8013DB10);
    case 1:
        blk(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->state++;
        }
        break;
    case 2:
        blk(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (!land2(o)) break;
        if (o->movetab == 0) {
            o->d8c = t->b;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 5;
            SETANIM(D_8013DAFC);
            func_8002B920(o);
            break;
        }
        o->active = 3;
        o->velV = -0x280;
        o->d8c = 0;
        o->movetab = D_80077CF4;
        o->state++;
        t->a1 = 1;
        o->b69 = 0;
        o->wac = 4;
        SETANIM(D_8013DAF8);
        break;
    case 3:
        if (o->animFrame) o->d8c = (unsigned char)(o->d8c - 0x14);
        else o->d8c = (unsigned char)(o->d8c + 0x14);
        blk(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV <= 0) break;
        if (!land2(o)) break;
        if (o->movetab == 0) {
            o->d8c = t->b;
            o->state = 4;
            o->timer = 0x168;
            o->wac = 5;
            SETANIM(D_8013DAFC);
            func_8002B920(o);
            break;
        }
        c = t->a1--;
        if (c) {
            o->movetab = D_80077CDC;
            o->velV = ~(o->velV - 0x80) + 1;
            break;
        }
        o->d8c = t->b;
        o->timer = 0x168;
        o->wac = 5;
        o->state++;
        SETANIM(D_8013DAFC);
        func_8002B920(o);
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 0xb;
            o->state++;
            SETANIM(D_8013DB14);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 5:
        if (func_8012BB18(o)) {
            o->timer = 0x78;
            o->wac = 0xc;
            o->state++;
            SETANIM(D_8013DB18);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 6:
        func_8012BB18(o);
        if (--o->timer == -1) {
            o->timer = 0x3c;
            o->wac = 0xd;
            o->state++;
            SETANIM(D_8013DB1C);
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    case 7:
        func_8012BB18(o);
        if (--o->timer == -1) {
            *(signed char *)&o->b0f = -9;
            o->active = 1;
            o->b04 = 1;
            o->step = 3;
            o->state = 0;
            o->substep = 0;
            o->b69 = 0;
            o->b68 = 0;
        }
        o->y.p.whole += 2;
        land2(o);
        break;
    }
}
