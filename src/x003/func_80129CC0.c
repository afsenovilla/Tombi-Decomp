// FUNC 80129cc0 2584 X003
// MATCHING 80129cc0 2584
#include "TOBJ.H"
typedef struct { void **p; int pad[2]; } E12;
typedef struct { TObj o; char pc0[2]; unsigned short wc2; } TX;
#define X(o) ((TX *)(o))
extern TObj D_800A6038;
extern E12 D_80135DC0[];
extern unsigned char D_800A60D4[];
extern short D_800A60EA[];
extern unsigned char D_8009C93F[], D_8009C942[], D_8009C93E[], D_8009C940[];
extern unsigned char D_8009CDC7;
extern unsigned char D_8009C975;
extern int AnimAdvance(TObj *);
extern void AnimJump(TObj *, int);
extern void FUN_800eea7c(TObj *, int, int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

#define P D_800A6038
#define ANIM(k) D_80135DC0[o->subtype].p[k]

void func_80129CC0(TObj *o)
{
    TObj *q;
    int v;
    char pad[16];

    AnimAdvance(o);
    switch (o->state) {
    case 0:
        P.b04 = 5;
        P.step = 0x65;
        P.velY = -0x200;
        P.state = 0;
        P.animFrame = 0;
        D_800A60D4[0] = 1;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        if (D_8009CDC7 != 0xff) {
            FUN_800eea7c(&P, 0x14, 0);
            o->timer = 0x78;
        } else {
            FUN_800eea7c(&P, 0x14, 0);
            o->timer = 0x3c;
        }
        o->state = 2;
        break;
    case 2:
        if (--o->timer > 0) break;
        P.velH = (o->h->raw - P.h->raw) >> 6;
        v = P.y.raw + 0x1600;
        P.velV = (o->y.raw - v) >> 6;
        P.velY = -0x480;
        o->timer = 0x40;
        PlayerSetAnimIfChanged(&P, 0x15);
        o->state++;
        break;
    case 3:
        P.h->raw += P.velH;
        P.y.raw += P.velV;
        P.y.raw += P.velY << 8;
        P.velY += 0x20;
        if (--o->timer > 0) break;
        FUN_800eea7c(&P, 0xd, 0);
        P.h->p.whole = o->h->p.whole;
        P.y.p.whole = o->y.p.whole - 0x10;
        P.velY = 0;
        o->state++;
        break;
    case 4:
        {
            short *py = &P.y.p.whole;
            unsigned short u = o->y.p.whole;
            P.b04 = 5;
            P.step = 0x65;
            P.state = 0;
            P.animFrame = 0;
            D_8009C93F[0] = 1;
            D_8009C942[0] = 1;
            *py = u - 0x10;
            P.h->p.whole = o->h->p.whole;
            *py = o->y.p.whole - 0x10;
            FUN_800eea7c(&P, 0xd, 0);
        }
        o->state = 10;
        break;
    case 10:
        o->state = 0x46;
        break;
    case 50:
        FUN_8005a8a8(0x23, 0, 0);
        o->timer = 200;
        o->state = 0x78;
        X(o)->wc2 = 0xfe;
        break;
    case 70:
        o->b69 = 0;
        if (D_8009CDC7 != 0xff) {
            o->state = 0x47;
            o->anim = ANIM(1);
            AnimJump(o, 0);
        } else {
            o->anim = ANIM(2);
            AnimJump(o, 0);
            o->timer = 0x3c;
            o->state = 0x4b;
        }
        break;
    case 71:
        o->state++;
        break;
    case 72:
        o->state++;
        break;
    case 73:
        o->anim = ANIM(0);
        AnimJump(o, 0);
        o->timer = 100;
        o->state++;
        break;
    case 74:
        if (--o->timer) break;
        o->anim = ANIM(2);
        AnimJump(o, 0);
        o->timer = 0x3c;
        o->state++;
        break;
    case 75:
        if (--o->timer) break;
        o->anim = ANIM(3);
        AnimJump(o, 0);
        o->timer = 0x3c;
        o->state++;
        break;
    case 76:
        if (--o->timer) break;
        o->anim = ANIM(3);
        AnimJump(o, 0);
        o->timer = 0x1e;
        o->state++;
        break;
    case 77:
        if (--o->timer) break;
        o->anim = ANIM(3);
        AnimJump(o, 0);
        o->timer = 0x1e;
        o->state++;
        break;
    case 78:
        if (--o->timer) break;
        o->anim = ANIM(4);
        AnimJump(o, 0);
        o->state++;
        break;
    case 79:
        o->timer = 100;
        o->state++;
    case 80:
        P.h->p.whole = o->h->p.whole;
        P.y.p.whole = o->y.p.whole - 0x10;
        if (--o->timer) break;
        o->velY = -0x200;
        o->anim = ANIM(5);
        AnimJump(o, 0);
        o->timer = 0x3c;
        o->state++;
        break;
    case 81:
        P.h->p.whole = o->h->p.whole;
        P.y.p.whole = o->y.p.whole - 0x10;
        if (--o->timer) goto move;
        o->animFrame = 0;
        o->anim = ANIM(5);
        AnimJump(o, 0);
        o->timer = 0x3c;
        o->velX = -0x50;
        o->state++;
        {
            int w = 0xa0;
            if (o->animFrame & 1)
                w = -0xa0;
            o->velX = w;
        }
        break;
    case 82:
        P.h->p.whole = o->h->p.whole;
        P.y.p.whole = o->y.p.whole - 0x10;
        if (o->visible == 0) {
            if (D_8009CDC7 != 0xff) {
                P.visible = 0;
                FUN_8005a9a4(0x23, 0);
                o->timer = 0x168;
            } else {
                P.visible = 0;
                o->timer = 8;
            }
            o->state = 0x78;
            X(o)->wc2 = 0xfe;
            break;
        }
    move:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        break;
    case 90:
        o->anim = ANIM(1);
        AnimJump(o, 0);
        o->state = 0x5b;
        break;
    case 91:
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            q->b04 = 3;
            o->anim = ANIM(1);
            AnimJump(o, 0);
            goto next;
        }
        break;
    case 110:
        o->timer = 200;
        o->state = 0x6f;
        break;
    case 111:
        if (--o->timer == 0)
            goto next;
        break;
    case 100:
        D_8009C975 = 4;
        goto next;
    case 101:
        D_8009C975 = 3;
        goto next;
    case 102:
        D_8009C975 = 4;
        o->state = 0x68;
        break;
    case 103:
        D_8009C975 = 3;
        o->state = 0x68;
        break;
    case 104:
        if ((unsigned char)(D_8009C975 - 3) < 2) break;
    next:
        o->state = X(o)->wc2;
        break;
    case 120:
        o->state = 0x79;
        break;
    case 121:
        if (--o->timer == 0)
            o->state = 0x7a;
        break;
    case 122:
        P.b04 = 5;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_8009C940[0] = 0;
        D_800A60D4[0] = 0;
        D_800A60EA[0] = 0;
        P.step = 0x65;
        P.state = 0;
        o->b68 = 0;
        switch (X(o)->wc2) {
        case 240:
            o->state = 0xf0;
            break;
        case 250:
            o->state = 0xfa;
            break;
        case 252:
            o->state = 0;
            o->step++;
            break;
        case 253:
            o->b04 = 0;
            o->step = 0;
            o->state = 0;
            break;
        case 254:
            o->b04++;
            o->step = 0;
            o->state = 0;
            break;
        default:
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    case 240:
        o->anim = 0;
        break;
    case 250:
        o->b04 = 2;
        break;
    }
}
