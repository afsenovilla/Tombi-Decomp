// FUNC 80122bb4 916 X010
// MATCHING 80122bb4 916
/* Whole function (covers csv pieces 80122C10, 80122CBC, 80122F14). Case 3 compares (unsigned char)t so CSE does
   not propagate t == 1 into the D_800A6038.animFrame subtraction (game keeps subtype in a0). */#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C970;
extern void *D_80131CC8;

void func_80122BB4(TObj *o)
{
    int t;

    switch (o->state) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->d88 = 0;
            break;
        case 1:
            o->d88 = 0x10;
            D_800A6038.a.p.whole = o->h->p.whole;
            D_800A6038.y.p.whole = o->y.p.whole - 10;
            break;
        case 2:
            o->d88 = -0x10;
            break;
        }
        o->b6a = 0;
        o->b69 = 0;
        o->b6b = D_800A6038.animFrame & 1;
        o->state++;
        o->w22 = 0;
        o->velH = 0x400;
        o->velY = 0;
        o->velX = 0x40;
        o->timer = 1 - o->b6b;
        break;
    case 1:
        o->velH -= o->velX;
        if (o->timer) o->velY -= o->velH;
        else o->velY += o->velH;
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        D_800A6038.d8c = o->d8c;
        if (o->velH <= 0) {
            o->velH = 0;
            o->state++;
            if (o->timer) o->w22++;
        }
        if (o->w22 >= 3) o->state = 3;
        break;
    case 2:
        o->velH += o->velX;
        if (o->timer) o->velY += o->velH;
        else o->velY -= o->velH;
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        D_800A6038.d8c = o->d8c;
        if (o->velH >= 0x400) {
            o->velH = 0x400;
            o->state--;
            o->timer = 1 - o->timer;
            if (o->timer == 0) o->w22++;
        }
        if (o->w22 >= 3) o->state = 3;
        break;
    case 3:
        if (D_8009C970 == 0) {
            o->state = 6;
            D_800A6038.state++;
            break;
        }
        t = o->subtype;
        if ((unsigned char)t == 1) {
            D_800A6038.state++;
            D_800A6038.animFrame = t - o->b6b;
        }
        o->state++;
        break;
    case 4:
        switch (o->subtype) {
        case 0:
            o->anim = D_80131CC8;
            o->d8c = 0;
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
        break;
    }
}
