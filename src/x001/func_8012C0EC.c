// FUNC 8012c0ec 2048 X001
// MATCHING 8012c0ec 2048
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;
typedef struct {
    short a;     /* 0xb4 */
    short b;     /* 0xb6 */
    short wb8;   /* 0xb8 */
} Sub;

extern AH *D_8013DAF0[], *D_8013DAF4[];
extern unsigned char D_8013C800[];
extern unsigned short D_8013C8E8[];
extern char D_80077D30[];
extern short D_1F80027E, D_1F800284;
extern short FUN_8001fddc(short, int);
extern short FUN_8001fdac(short, int);
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_800eae0c(short, int, int, int);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001e4f0(int);
extern void func_8012BB18(TObj *o);

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

#define CHK \
    if (o->h->p.whole >= 0x943) { \
        o->active = 2; \
        o->b04 = 3; \
        if (o->visible) { \
            FUN_800eae0c(o->h->p.whole + 10, 0x2e, o->d->p.whole, 1); \
            FUN_800eae0c(o->h->p.whole - 10, 0x2e, o->d->p.whole, 1); \
        } \
    }

#define MOVE { \
    c = FUN_8001fddc(s->b, 0x250); \
    d = FUN_8001fdac(s->b, 0x250); \
    if (o->animFrame) o->h->raw -= c << 8; \
    else o->h->raw += c << 8; \
    o->y.raw += d << 8; \
    o->velV += 0x40; \
    o->y.raw += o->velV << 8; \
}

void func_8012C0EC(TObj *o)
{
    Sub *s = (Sub *)&o->wb4;
    unsigned char *p0, *p1;
    short c, d;

    switch (o->substep) {
    case 0:
        o->movetab = D_80077D30;
        o->b9c = 1;
        o->velV = -0x300;
        o->b9d = 0;
        o->wac = 2;
        o->substep++;
        o->anim = D_8013DAF0[0];
        p0 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p0++;
        o->box1 = *p0++;
        o->box2 = *p0++;
        o->box3 = *p0;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
        CHK
    case 1:
        func_8012BB18(o);
        MOVE
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->substep++;
        }
        CHK
        break;
    case 2:
        MOVE
        if (land2(o)) {
            o->velV = 0;
            o->d8c = (unsigned short)s->b;
            if (o->visible) {
                if (o->animFrame) s->wb8 = -0x10;
                else s->wb8 = 0x10;
                FUN_800eae0c(o->a.p.whole + s->wb8, o->y.p.whole, o->b.p.whole, 1);
                FUN_8001e4f0(0x3f);
            }
            o->timer = 0xf;
            o->velH = 0x250;
            o->substep++;
            break;
        }
        CHK
        break;
    case 3:
        FUN_8001fb20(o);
        if (land2(o)) {
            if (--o->timer == -1) {
                if (o->visible) {
                    if (o->animFrame) s->wb8 = -0x10;
                    else s->wb8 = 0x10;
                    FUN_800eae0c(o->a.p.whole + s->wb8, o->y.p.whole, o->b.p.whole, 1);
                    FUN_8001e4f0(0x3f);
                }
                o->timer = 0xf;
            }
            o->d8c = (unsigned short)s->b;
            if (o->animFrame) c = D_8013C8E8[-o->wb2];
            else c = D_8013C8E8[o->wb2];
            o->velH += c;
            if (o->velH > 0x300) o->velH = 0x300;
        }
        if (o->animFrame) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (o->velH < 0x150) {
            func_8012BB18(o);
            if (o->velH < 0) {
                o->velH = 0;
                o->substep++;
            }
        }
        CHK
        break;
    case 4:
        o->timer = 100;
        o->wac = 3;
        o->substep++;
        o->anim = D_8013DAF4[0];
        p1 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p1++;
        o->box1 = *p1++;
        o->box2 = *p1++;
        o->box3 = *p1;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
    case 5:
        func_8012BB18(o);
        if (--o->timer == -1) {
            if (o->b0c == 0) {
                if (o->animFrame) {
                    if (o->wb2 <= 0) goto z;
                } else {
                    if (o->wb2 >= 0) goto z;
                }
                o->state = 9;
                o->substep = 2;
                break;
            }
            o->state = 9;
        z:
            o->substep = 0;
        }
        break;
    }
}
