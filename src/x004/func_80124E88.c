// FUNC 80124e88 960 X004
// MATCHING 80124e88 960
#include "TOBJ.H"

extern Fix16 *D_800A6078;
extern short D_800A604E;
extern unsigned char D_800A6047;
extern short D_800A6066;
extern unsigned char D_800A6066A[]; /* byte view of D_800A6066 (debt) */
extern int D_800A60C4[];
extern unsigned char D_8009C970;
extern unsigned char D_800A603E;
extern void *D_80134D38;

void func_80124E88(TObj *o)
{
    unsigned char *k;

    switch (o->state) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->d88 = 0;
            break;
        case 1:
            o->d88 = 0x10;
            D_800A6078->p.whole = o->h->p.whole;
            D_800A604E = o->y.p.whole - 10;
            goto bf;
        case 2:
            o->d88 = -0x10;
        bf:
            o->b0f = D_800A6047 + *(unsigned char *)&o->w7a;
            break;
        default:
            o->b6a = 0;
            goto L58;
        }
        o->b6a = 0;
    L58:
        o->b69 = 0;
        o->b6b = D_800A6066A[0] & 1;
        o->velH = 0x400;
        o->velX = 0x40;
        o->state++;
        o->w22 = 0;
        o->velY = 0;
        o->timer = 1 - o->b6b;
        break;
    case 1:
        o->velH -= o->velX;
        if (o->timer) o->velY -= o->velH;
        else o->velY += o->velH;
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        D_800A60C4[0] = o->d8c;
        if (o->velH > 0) goto check;
        o->velH = 0;
        o->state++;
        if (o->timer == 0) goto check;
        o->w22++;
        goto check;
    case 2:
        o->velH += o->velX;
        if (o->timer) o->velY += o->velH;
        else o->velY -= o->velH;
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        D_800A60C4[0] = o->d8c;
        if (o->velH < 0x400) goto check;
        o->velH = 0x400;
        o->state--;
        o->timer = 1 - o->timer;
        if (o->timer) goto check;
        o->w22++;
    check:
        if (o->w22 >= 3) o->state = 3;
        break;
    case 3:
        if (D_8009C970 == 0) {
            k = &D_800A603E;
            o->state = 6;
            *k = *k + 1;
            break;
        }
        if (o->subtype == 1) {
            { unsigned char *q = &D_800A603E; *q = *q + 1; }
            D_800A6066 = 1 - o->b6b;
        }
        o->state++;
        break;
    case 4:
        switch (o->subtype) {
        case 0:
            o->d8c = 0;
            o->anim = D_80134D38;
            o->b0a = 2;
            break;
        case 1:
            o->d8c = 0;
            o->b6a = 0;
            o->box2 = 0x20;
            o->box3 = 0x20;
            break;
        case 2:
            o->d8c = 0;
            break;
        }
        o->state++;
        break;
    case 5:
        o->y.p.whole += 2;
        if (o->y.p.whole >= o->d34) {
            o->y.raw = o->d34 << 16;
            o->step = 0;
            o->state = 0;
            if (o->subtype != 2) o->active = 1;
        }
        o->b0f = o->w7a;
        break;
    case 6:
        break;
    }
}
