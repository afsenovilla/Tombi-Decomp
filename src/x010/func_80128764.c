// FUNC 80128764 1556 X010
// MATCHING 80128764 1556
#include "TOBJ.H"

extern unsigned char D_8012F3D4[];
extern void *D_80132310[];
extern int D_1F80016C[], D_1F800170[];
extern short D_1F800172;
extern int D_1F800198;
extern unsigned short D_1F8001F8;
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001fe6c(TObj *);
extern int AnimAdvance(TObj *);
extern void playSFX(int);
extern short FUN_80040278(TObj *, short, short);

#define SETBOX(o, n) { \
    int b0 = D_8012F3D4[(n) * 4], b1 = D_8012F3D4[(n) * 4 + 1]; \
    int b2 = D_8012F3D4[(n) * 4 + 2], b3 = D_8012F3D4[(n) * 4 + 3]; \
    o->wac = n; \
    o->box0 = b0; \
    o->box1 = b1; \
    o->box2 = b2; \
    o->box3 = b3; \
    o->anim = D_80132310[n]; \
    FUN_8001fe6c(o); }

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x1a)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_80128764(TObj *o)
{
    int n;
    short t;
    short i;
    unsigned char *b;

    switch (o->state) {
    case 0:
        FUN_8001f8e4(o);
        o->state++;
        o->velH = ((o->w74 << 16) - o->h->raw) >> 15;
        o->velV = (D_1F80016C[0] - o->y.raw) >> 15;
        o->velX = (D_1F800170[0] - o->d->raw) >> 15;
        o->velY = -0x600;
        if (o->d->p.whole == D_1F800172) {
            if (D_1F800198 & 1) {
                SETBOX(o, 0xb);
            } else {
                SETBOX(o, 0x11);
            }
        } else if (D_1F800172 < o->d->p.whole) {
            SETBOX(o, 0xd);
        } else {
            SETBOX(o, 9);
        }
    case 1:
        if (AnimAdvance(o)) {
            o->timer = 0x70;
            o->state++;
            if (o->visible && (D_1F800198 & 3)) playSFX(0xa4);
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->d->raw += o->velX << 8;
        o->velY += 0x20;
        o->y.raw += o->velY << 8;
        if (o->velY > 0) o->state++;
        o->timer--;
        break;
    case 3:
        o->h->raw += o->velH << 8;
        o->y.raw += o->velV << 8;
        o->d->raw += o->velX << 8;
        if (o->velY < 0x600) {
            o->velY += 0x20;
            o->y.raw += o->velY << 8;
        }
        t = o->timer;
        if (t <= 0) {
            o->y.raw += o->velY << 8;
            if (land(o)) o->state++;
        } else {
            o->timer = t - 1;
        }
        break;
    case 4:
        FUN_8001f8e4(o);
        o->state++;
        o->d34 = o->y.p.whole;
        o->velV = -0x400;
        switch (D_1F800198 & 3) {
        case 0:
            o->wac = 0x13;
            break;
        case 1:
            o->wac = 0x26;
            break;
        case 2:
            o->wac = 0x1a;
            break;
        case 3:
            o->wac = 0x15;
            break;
        }
        i = o->wac;
        b = &D_8012F3D4[i * 4];
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = b[0];
        o->box3 = b[1];
        o->wac = i;
        setAnim(o, D_80132310[i]);
        break;
    case 5:
        AnimAdvance(o);
        o->velV += 0x30;
        o->y.raw += o->velV << 8;
        if (o->velV > 0 && o->d34 < o->y.p.whole) {
            o->y.p.whole = o->d34;
            o->velV = -0x400;
            if (o->wac == 0x26) {
                SETBOX(o, 0x26);
            }
        }
        if (((D_1F8001F8 + D_1F800198) & 0x1f) == 0) {
            switch (o->b0c & 3) {
            case 0:
                playSFX(0xa8);
                break;
            case 1:
                playSFX(0xa9);
                break;
            case 2:
                playSFX(0xaa);
                break;
            case 3:
                playSFX(0xab);
                break;
            }
        }
        break;
    }
}
