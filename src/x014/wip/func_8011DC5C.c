/* score 22: whole 1428 B state machine (csv pieces 8011DD48/8011DE64/8011E0D8/8011E1A4). Left: case 4 q pointer in a0 instead of a1,
   case 5 anim temp in v1 instead of v0 and the in-range flag is branched directly instead of the game`s 0/1 value in v0
   (with a flag variable it lands in a0/s2). Tried: flag var scopes/types, inline helpers, statement-order search. */
// FUNC 8011dc5c 1428 X014
#include "TOBJ.H"
typedef struct { char p0[0xe]; unsigned short w0e; } X;
typedef struct { unsigned char c[4]; } B;
extern volatile unsigned short D_1F8000EE;
extern volatile unsigned short D_1F8000F2;
extern unsigned short D_1F8001F8;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C942;
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_8001f8e4(TObj *);
extern void func_80117C50(short, short, short, short);

#define DA8(o) (*(void ***)((char *)(o) + 0xa8))
#define SETBOX(k) \
    { unsigned char *p = (unsigned char *)o->d90; p += (k); \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; }

static __inline__ int inrange(TObj *o)
{
    return (unsigned short)(o->h->p.whole - D_1F800176 + 0x10) < 0x161
        && (unsigned short)(D_1F800186 - o->y.p.whole + 0x10) < 0x101;
}

void func_8011DC5C(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    int i;
    int found;
    int n;
    int k;

    switch (o->state) {
    pick:
        o->wae = k;
        found = 1;
        goto next;
    case 0: {
        unsigned short *p;
        n = 0;
        p = (unsigned short *)o->d94;
        o->d8c = 0;
        o->wb0 = 0;
        for (k = 0; k < 16; k++) {
            found = (unsigned short)(p[0] - D_1F8000EE + 0xa0) < 0x141
                && (unsigned short)(p[1] - D_1F8000F2 + 0x6e) < 0xdd;
            n += found;
            p += 4;
            o->wb0 |= found << k;
        }
        if (n != 0) {
            found = 0;
            for (i = 0; i < 16; i++) {
                k = Rand() & 0xf;
                if ((o->wb0 >> k) & 1) goto pick;
            }
        next:
            if (!found) {
                for (i = 0; i < 16; i++) {
                    if ((o->wb0 >> i) & 1) {
                        o->wae = i;
                        break;
                    }
                }
            }
        } else {
            o->wae = 0;
        }
        o->ba7 = n;
        o->state++;
        func_80117C50(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        break;
    }
    case 1:
        SETBOX(0xc);
        o->wac = 3;
        o->anim = DA8(o)[3];
        AnimLoadDuration(o);
        o->active = 2;
        o->timer = 0x1e;
        o->state++;
        if (x->w0e == 1) {
            D_8009C93E = 0;
            D_8009C942 = 0;
            x->w0e = 0;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == -1) {
            SETBOX(0x10);
            o->wac = 4;
            o->anim = DA8(o)[4];
            AnimLoadDuration(o);
            o->velV = -0x400;
            o->timer = 0x1e;
            o->state++;
        }
        if (D_1F8001F8 & 3) o->visible = 0;
        break;
    case 3:
        if (D_1F8001F8 & 1) o->visible = 0;
        AnimAdvance(o);
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV < -0x1ff) break;
        o->visible = 0;
        o->timer = 0x1e;
        o->state++;
        break;
    case 4:
        o->visible = 0;
        if (--o->timer == -1) {
            unsigned short *q;
            unsigned char *p;
            q = (unsigned short *)o->d94;
            q += o->wae * 4;
            o->a.p.whole = q[0];
            o->y.p.whole = q[1];
            o->subtype = q[3];
            o->state++;
            p = (unsigned char *)o->d90;
            p += 0x10;
            o->box0 = *p++;
            o->box1 = *p++;
            o->box2 = *p;
            o->box3 = p[1];
            o->wac = 4;
            o->anim = DA8(o)[4];
            AnimLoadDuration(o);
            func_80117C50(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->timer = 0x3c;
            FUN_8001f8e4(o);
        }
        break;
    case 5: {
        void *a;
        AnimAdvance(o);
        if (D_1F8001F8 & 3) o->visible = 0;
        if (--o->timer == -1) {
            if (o->subtype == 1) {
                SETBOX(0);
                o->wac = 0;
                a = DA8(o)[0];
            } else {
                SETBOX(8);
                o->wac = 2;
                a = DA8(o)[2];
            }
            o->anim = a;
            AnimLoadDuration(o);
            if (!((unsigned short)(o->h->p.whole - D_1F800176 + 0x10) < 0x161
                && (unsigned short)(D_1F800186 - o->y.p.whole + 0x10) < 0x101)) {
                o->state = 0;
                o->visible = 0;
            } else {
                o->visible = 1;
                o->active = 1;
                o->state = 0;
                o->substep = 0;
                o->step++;
            }
        }
        break;
    }
    }
}
