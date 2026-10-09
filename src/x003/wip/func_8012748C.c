/* score 14: (1) Ground2 d8c +-1 result lands in v1 (game v0, same non-inverted branch shape); w=v-1 temps give v0 but reorg then inverts the branch. (2) case 4 double state store: game keeps both `state++` and `state = 1` stores at 6(s0); plain C lets flow delete the first, the x-relative store keeps both but emits -0xae(s1). Tried volatile, inline Jump(), labels, q=o copy, store after call. */
// FUNC 8012748c 2236 X003
#include "TOBJ.H"

typedef struct { short w0, w2; } X;
#define XS(o) ((X *)((char *)(o) + 0xb4))

extern char D_80077CF4[];
extern unsigned char D_80135CB0[];
extern void *D_80139520[];
extern unsigned short D_1F80027E;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern TObj *ObjAlloc(void);
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fb20(TObj *);
extern int func_8004065C(TObj *, int, int, int);
extern int FUN_800408d8(TObj *, int, int);
extern int TileCollideAt(TObj *, int, int);

static __inline__ int Side(TObj *o)
{
    unsigned short w = o->w7a;
    int d;
    if (w & 1) d = -0x18;
    else d = 0x18;
    return func_8004065C(o, (short)(o->h->p.whole + d), o->y.p.whole, (short)w);
}

static __inline__ int Ground(TObj *o)
{
    unsigned char t;
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if ((short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0xe)) == 0) return 0;
    if (o->wac >= 0xc) {
        o->d8c = 0;
    } else {
        t = D_1F80027E * 4 + o->d8c;
        if (t) {
            if (t >= 0x80) o->d8c++;
            else o->d8c--;
            o->d8c = *(unsigned char *)&o->d8c;
        }
    }
    return 1;
}

static __inline__ int Ground2(TObj *o)
{
    unsigned char t;
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if ((short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0xe)) == 0) return 0;
    if (o->wac >= 0xc) {
        o->d8c = 0;
    } else {
        t = D_1F80027E * 4 + o->d8c;
        if (t) {
            int v = o->d8c;
            if (t < 0x80) v--; else v++;
            o->d8c = v;
            o->d8c = *(unsigned char *)&o->d8c;
        }
    }
    return 1;
}

#define SETANIM8(o) { \
    unsigned char *p; \
    x->w0 = 1; \
    *(volatile short *)&o->wac = 8; \
    p = &D_80135CB0[o->wac * 4]; \
    o->anim = D_80139520[0]; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; \
    AnimLoadDuration(o); \
}

#define SPAWN(o) { \
    TObj *e = ObjAlloc(); \
    if (e) { \
        e->active = 2; \
        e->type = 0x2f; \
        e->animFrame = 1 - o->w7a; \
        e->a.p.whole = o->a.p.whole; \
        e->y.p.whole = o->y.p.whole; \
        e->b.p.whole = o->b.p.whole; \
        e->d8c = o->d8c; \
    } \
}

void func_8012748C(TObj *o)
{
    X *x = XS(o);

    switch (o->state) {
    case 0:
        if (x->w0 == 0 && o->w98 == 0) {
            o->state = 4;
            break;
        }
        if (x->w2) {
            x->w2 += o->b68;
            if (x->w2 > 5) x->w2 = 5;
            o->b68 = 1;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->w9a = 0;
            o->state = 4;
            o->substep = 0;
            break;
        }
        o->velV = -0x400;
        o->movetab = D_80077CF4;
        o->d8c = 0;
        o->b9c = 1;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, (unsigned short)o->w7a);
        Side(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if ((short)FUN_800408d8(o, o->h->p.whole, (short)(o->y.p.whole - 0x10))) o->velV = 0;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, (unsigned short)o->w7a);
        Side(o);
        if (Ground2(o)) {
            o->b9c = 0;
            o->velH = 0x180;
            o->state++;
        }
        break;
    case 3:
        if ((short)Side(o)) o->velH = 0;
        FUN_8001fb20(o);
        Ground(o);
        if (o->w7a & 1) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        o->velH -= 0x20;
        if (o->velH < 0) {
            o->velH = 0;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 1;
            o->substep = 0;
        }
        break;
    case 4:
        if (o->subtype == 2 && o->b9c) {
            SETANIM8(o);
            SPAWN(o);
            o->velV = -0x400;
            o->movetab = D_80077CF4;
            o->d8c = 0;
            o->b9c = 1;
            o->state++;
            ((TObj *)((char *)x - 0xb4))->state = 1;
            playSFX(0x77);
            break;
        }
        if ((unsigned char)(o->d8c - 0x70) < 0x21) {
            o->active = 5;
            o->state++;
            break;
        }
        playSFX(0x77);
        o->active = 1;
        o->b04 = 1;
        o->step = 1;
        o->state = 1;
        o->substep = 0;
        SETANIM8(o);
        SPAWN(o);
        break;
    case 5:
        o->velV = 0;
        o->b69 = 0;
        o->b9c = 2;
        if (o->d8c >= 0x80) o->state = 6;
        else o->state = 7;
        break;
    case 6:
    case 7:
        if (o->state == 6) o->d8c += 8;
        else o->d8c -= 8;
        o->d8c = *(unsigned char *)&o->d8c;
        o->velV += 0x20;
        if (o->velV > 0x600) o->velV = 0x600;
        o->y.raw += o->velV << 8;
        if (Ground2(o)) {
            o->d8c = (-(short)D_1F80027E << 2) & 0xff;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 2;
            o->substep = 0;
            SETANIM8(o);
            SPAWN(o);
        }
        break;
    }
}
