// FUNC 80120890 828 X000
// MATCHING 80120890 828
#include "TOBJ.H"
extern TObj D_800A6038;
extern short D_8007A5F0[];
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern void FUN_800eea7c(TObj *, int, int);
extern int AnimAdvance(TObj *);
void func_80120890(TObj *o)
{
    TObj *g;
    short d;
    if (o->b0c == 0) {
        g = &D_800A6038;
        switch (o->state) {
        case 0:
            d = o->h->p.whole - (*(unsigned short *)0x1F80016A + 0x20);
            if (d == 0) {
                o->state = 2;
                break;
            }
            if (d < 0)
                g->animFrame = 1;
            else
                g->animFrame = 0;
            FUN_800eea7c(g, 1, 0);
            g->active = 6;
            g->b04 = 5;
            g->step = 0x65;
            g->state = 0;
            o->timer = 0x20;
            o->velH = (d << 8) / 32;
            o->state++;
            break;
        case 1:
            g->h->raw += o->velH << 8;
            AnimAdvance(g);
            if (--o->timer == -1)
                o->state++;
            break;
        case 2:
            o->state++;
            FUN_800eea7c(g, 4, 0);
            g->active = 6;
            g->b04 = 5;
            g->step = 0x65;
            *(signed char *)&g->b0f = -16;
            g->state = 0;
            g->animFrame = 0;
            o->timer = 0x30;
            o->velV = 0x280;
            o->velH = 0xaa;
            o->velX = 0;
            o->velY = 0;
            o->d88 = 0;
            break;
        case 3:
            g->d8c--;
            g->h->raw += o->velH << 8;
            g->y.raw -= (D_8007A5F0[*(unsigned char *)&o->d88] * o->velV) >> 4;
            o->d88 += 2;
            if (--o->timer == -1)
                o->state++;
            break;
        case 4:
            o->state++;
            FUN_800eea7c(g, 0xd, 0);
            g->h->p.whole = o->h->p.whole;
            g->y.p.whole = o->y.p.whole - 8;
            g->d->p.whole = o->d->p.whole;
            g->d8c = 0;
            o->timer = 0x20;
            break;
        case 5:
            if (--o->timer == -1) {
                o->state = 0;
                o->substep = 0;
                o->step++;
                D_8009C93F = 0;
                D_8009C942 = 0;
                D_8009C93E = 0;
            }
            break;
        }
    } else {
        o->step = ((TObj *)o->d90)->step;
        o->state = ((TObj *)o->d90)->state;
        o->substep = ((TObj *)o->d90)->substep;
    }
}
