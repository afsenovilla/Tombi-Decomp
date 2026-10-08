// FUNC 80130314 988 X000
/* score 2 (ncheck, era 10): solo falla la posicion de addiu v1,v1,1 (state+1) respecto al subu del timer en case 0. */
#include "TOBJ.H"
typedef struct { char pad[4]; unsigned char b4; } E;
extern unsigned char D_8009D07B;
extern E *D_8009C948[];
extern void *D_8013B224;
extern void FUN_80123b64(Fix16 *);

void func_80130314(TObj *o)
{
    Fix16 v[3];
    short d;
    int s;
    switch (o->state) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->d88 = 0;
            D_8009D07B = 1;
            break;
        case 1:
            o->d88 = 0;
            D_8009C948[0]->b4 = 3;
            break;
        case 2:
            o->d88 = 0;
            break;
        }
        s = *(volatile unsigned char *)&o->state;
        o->b6b = 0;
        o->velH = 0x400;
        o->velX = 0x40;
        o->b6a = 0;
        o->b69 = 0;
        o->w22 = 0;
        o->velY = 0;
        o->timer = 1 - *(volatile unsigned char *)&o->b6b;
        o->state = s + 1;
        break;
    case 1:
        o->velH = d = o->velH - o->velX;
        if (o->timer) {
            o->velY -= d;
        } else {
            o->velY += d;
        }
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        if (o->velH <= 0) {
            o->velH = 0;
            o->state++;
            if (o->timer != 0) o->w22++;
        }
        if (o->w22 >= 3) o->state = 3;
        break;
    case 2:
        o->velH = d = o->velH + o->velX;
        if (o->timer) {
            o->velY += d;
        } else {
            o->velY -= d;
        }
        o->d8c = ((o->velY >> 8) + o->d88) & 0xff;
        if (o->velH >= 0x400) {
            o->velH = 0x400;
            o->state--;
            o->timer = 1 - o->timer;
            if (o->timer == 0) o->w22++;
        }
        if (o->w22 >= 3) o->state = 3;
        break;
    case 3:
        switch (o->subtype) {
        case 0:
            o->anim = D_8013B224;
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
    case 4:
        o->y.p.whole += 2;
        if (o->y.p.whole < o->d34 - 8) break;
        o->y.raw = (o->d34 - 8) << 16;
        o->timer = 0x14;
        o->state++;
        if (o->subtype == 1) {
            v[0].p.whole = o->h->p.whole;
            v[1].p.whole = o->y.p.whole - 0x10;
            v[2].p.whole = o->d->p.whole;
            FUN_80123b64(v);
        }
        break;
    case 5:
        switch (o->subtype) {
        case 0:
            if (--o->timer == -1) o->b04 = 3;
            break;
        case 1:
            o->d8c = (o->d8c + 4) & 0xff;
            if (--o->timer == -1) o->b04 = 3;
            break;
        case 2:
            o->d8c = (o->d8c - 4) & 0xff;
            if (--o->timer == -1) o->b04 = 3;
            break;
        }
        break;
    }
}
