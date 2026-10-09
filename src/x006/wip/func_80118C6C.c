/* score 40: game frame is 8 B bigger (0x30), case 1 has a dead copy (lh y; lw d34; move v1,v0), the div result lands in v0 (ours v1) and case 2 loads d34 before w74. Tried y/lim/r/v type brute force, compare forms, temps. */
// FUNC 80118c6c 1204 X006
#include "TOBJ.H"

typedef struct {
    short a;     /* 0xb4 */
    short b;     /* 0xb6 */
    short ang;   /* 0xb8 */
    short pad;
    int rad;     /* 0xbc */
    int div;     /* 0xc0 */
    int pad2;
    short c8;    /* 0xc8 */
    short ca;    /* 0xca */
    short cc;    /* 0xcc */
} Sub;
typedef struct { short x, y, z, w; } E;

extern TObj *D_8009C950;
extern volatile unsigned short D_8009D670[];
extern char D_8011FEA0[], D_8011F3F8[];
extern void *D_801229D0[], *D_801229A8[];
extern int Rand(void);
extern int rsin(int);
extern int rcos(int);
extern void AnimLoadDuration(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void func_80118810(TObj *);
extern void func_801184F0(TObj *);

#define SUB(o) ((Sub *)((char *)(o) + 0xb4))

void func_80118C6C(TObj *o)
{
    TObj *p = D_8009C950;
    Sub *s, *t;
    E *e;
    unsigned short y;
    short lim;
    int r;
    int v;

    func_80118810(o);
    t = SUB(o);
    switch (SUB(o)->c8) {
    case 0:
        if (o->animTimer < 0x40) {
            if (o->animTimer >= 0x3e && (D_8009D670[0] & 0x10)) {
                *(char **)&o->wa8 = D_8011FEA0;
                SUB(o)->c8 = 2;
                o->animTimer -= 0x3e;
            }
        } else {
            SUB(o)->c8 = 1;
        }
        break;
    case 1:
        if ((unsigned short)(o->animTimer - 0x72) < 0x12) SUB(o)->cc = 1;
        else SUB(o)->cc = 0;
        break;
    case 2:
        if ((unsigned short)(o->animTimer - 0x35) < 0x19) SUB(o)->cc = 1;
        else SUB(o)->cc = 0;
        if (o->animTimer == 0x77) {
            *(char **)&o->wa8 = D_8011F3F8;
            o->animTimer = 0xaa;
            t->c8 = 1;
        }
        break;
    }
    func_801184F0(o);
    s = SUB(o);
    e = *(E **)&o->wa8;
    e += o->animTimer;
    if (SUB(o)->div > 0) {
        o->d34 = SUB(o)->rad * o->w76 / SUB(o)->div;
        v = o->d34 + e->y;
    } else {
        v = e->y;
    }
    o->d34 = v - 0x20;
    o->d38 = (rsin(s->ang) * s->rad) >> 20;
    o->d30 = (rcos(s->ang) * s->rad) >> 20;
    o->a.p.whole = e->x + o->d30;
    o->b.p.whole = e->z + o->d38;
    switch (o->state) {
    case 0:
        o->state++;
        o->b9c = 0;
        break;
    case 1:
        o->b9c = 1;
        o->velX -= 0x10;
        if (o->velX < 0) o->velX = 0;
        o->velV += 0x40;
        if (o->velV > 0) {
            o->velV = 0;
            o->b9c = 2;
            o->state++;
            p->anim = D_801229D0[0];
            AnimLoadDuration(p);
        }
        o->y.raw += o->velV << 8;
        if (o->y.p.whole >= o->d34) o->y.p.whole = o->d34;
        break;
    case 2:
        o->velX -= 0x10;
        if (o->velX < 0) o->velX = 0;
        o->velV += 0x40;
        if (o->velV > 0x600) o->velV = 0x600;
        o->y.raw += o->velV << 8;
        y = o->y.p.whole;
        switch (o->w7a & 3) {
        case 0:
            o->w74 = 0;
            break;
        case 1:
            o->w74 = Rand() & 3;
            if (SUB(o)->b == 0) o->w74 = 2;
            break;
        case 2:
            o->w74 = (Rand() & 3) + 2;
            if (SUB(o)->b == 0) o->w74 = 4;
            break;
        case 3:
            o->w74 = 4;
            break;
        }
        lim = o->d34 + (unsigned short)o->w74;
        if ((short)lim <= (short)y) {
            r = 1;
            o->b69 = 1;
            o->y.p.whole += lim - y;
        } else {
            r = 0;
            o->b69 = 0;
        }
        if (r) {
            FUN_80025f40(0, 0, 0x80, 5);
            p->anim = D_801229A8[0];
            AnimLoadDuration(p);
            o->step = 1;
            o->state = 0;
            o->b9c = 0;
        }
        break;
    }
}
