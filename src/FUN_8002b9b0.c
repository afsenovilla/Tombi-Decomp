// FUNC 8002b9b0 1232 MAIN0
// MATCHING 8002b9b0 1232
#include "TOBJ.H"
extern void *D_8001226C;
extern void **D_80079D34[];
extern unsigned char D_8009C942[];
extern int D_8009C954[];
extern unsigned short D_8009C960[];
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

void FUN_8002b9b0(TObj *o)
{
    P *p;
    int dx;
    int dy;
    int st;

    switch (o->b04) {
    case 0:
        if (o->subtype != 0) {
            o->w1e = 0x14;
            o->b0d = 1;
            o->w08 = 0x7ed7;
            o->animFrame = 0;
            o->anim = D_8001226C;
            o->d3c = SPAD->w;
            o->b04++;
            o->timer = 0x78;
            AnimLoadDuration(o);
            break;
        }
        D_8009C954[0] = 1;
        o->w1e = 0x15;
        *(signed char *)&o->b0f = -9;
        o->animFrame = 0;
        o->b04++;
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 0;
        o->anim = *D_80079D34[o->b0c];
        o->d3c = SPAD->w;
        o->a.p.whole = SPAD->x;
        o->y.p.whole = SPAD->y.s.hi - 0x1e;
        o->b.p.whole = SPAD->z;
        if (D_8009C960[0] == 2 && (D_8009C960[1] == 0 || D_8009C960[1] == 3)) {
            o->velV = -0x3e0;
            *(signed char *)&o->b0f = -0x12;
        } else {
            o->velV = -0x380;
            *(signed char *)&o->b0f = -9;
        }
        o->timer = 0x78;
        o->d34 = 0;
        AnimLoadDuration(o);
        playSFX(0x1a);
        break;
    case 1:
        if (o->subtype != 0) {
            p = (P *)o->d90;
            if (p->w04 == 2) {
            switch (o->b0c) {
            case 0:
                if (p->wac != 15) o->b04 = 3;
                dx = 0;
                dy = -0x10;
                break;
            case 2:
                dx = 0;
                dy = 0;
                st = p->b06;
                if (st != 4) {
                    if (st < 4) goto fail;
                    if (st >= 8) goto fail;
                    dy = -0x26;
                    break;
                }
                dx = (p->w2e & 1) ? -12 : 12;
                dy = -0x10;
                break;
            case 8:
                dx = 0;
                dy = -0x20;
                break;
            case 30:
                dx = (p->w2e & 1) ? -12 : 12;
                dy = -0x1c;
                break;
            case 19:
                dx = 0;
                dy = -0x10;
                break;
            case 56:
                st = p->b06;
                if (st < 3) goto fail;
                if (st < 6) goto twenty;
                if (st == 6) goto twelve;
                goto fail;
            twenty:
                dx = (p->w2e & 1) ? -20 : 20;
                dy = -0x10;
                break;
            twelve:
                dx = (p->w2e & 1) ? -12 : 12;
                dy = -0x18;
                break;
            fail:
                o->b04 = 3;
                break;
            }
            o->h->p.whole = p->h->p.whole + dx;
            o->y.p.whole = p->y16 + dy;
            o->d->p.whole = p->d->p.whole;
            if (D_8009C942[0] == 0) AnimAdvance(o);
            if (o->b04 == 1) ObjCullRegister(o);
            } else {
                o->b04 = 3;
            }
            break;
        }
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
