// FUNC 80049cdc 1096 MAIN0
// MATCHING 80049cdc 1096
#include "TOBJ.H"
typedef struct { char p[0x4c]; short w4c; short w4e; } G4;
extern TObj D_800A6038;
extern short D_1F800176;
extern unsigned char D_8009C93C, D_8009C975, D_8009CFF9, D_8009D2C3;
extern int D_8009C960;
extern G4 *D_1F8001D4;
extern int AnimAdvance(TObj *);
extern void func_800EEA7C(TObj *, int, int);
extern void AnimLoadDuration(TObj *);

void func_80049CDC(TObj *o)
{
    TObj *pl = &D_800A6038;
    switch (o->state) {
    case 0:
        if ((*(int *)&pl->b04 & 0xffffff) == 0x23001) {
            pl->active = 5;
            pl->visible = 1;
            pl->b04 = 5;
            pl->step = 0x41;
            pl->state = 0;
            o->timer = 50;
            o->state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        if (o->timer == 0) {
            o->timer = 16;
            o->state++;
            pl->visible = 1;
            pl->b04 = 5;
            pl->step = 0x41;
            pl->state = 0;
            pl->d8c = 0;
            pl->animFrame = 0;
            func_800EEA7C(pl, 0, 0);
        } else {
            o->timer--;
        }
        break;
    case 2:
        o->h->p.whole++;
        if (D_1F800176 + 0x134 < o->h->p.whole)
            o->h->p.whole = D_1F800176 + 0x134;
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->timer = 40;
            o->state++;
            o->wac = 0;
            o->anim = (*(void ***)&o->wa8)[0];
            AnimLoadDuration(o);
            o->b0f = pl->b0f - 2;
        }
        break;
    case 3:
        if (--o->timer == -1)
            o->state++;
        break;
    case 4:
        pl->animFrame = 0;
        func_800EEA7C(pl, 1, 0);
        o->state++;
        break;
    case 5:
        AnimAdvance(pl);
        pl->h->p.whole++;
        if (pl->h->p.whole >= o->h->p.whole) {
            pl->h->p.whole = o->h->p.whole;
            o->timer = 30;
            o->state++;
            func_800EEA7C(pl, 0x2c, 0);
        }
        break;
    case 6:
        if (--o->timer == -1)
            o->state++;
        pl->y.raw -= 0x8000;
        break;
    case 7:
        o->wac = 1;
        o->anim = (*(void ***)&o->wa8)[1];
        AnimLoadDuration(o);
        o->timer = 70;
        o->velV = 0;
        o->state++;
        break;
    case 8:
        if (--o->timer == -1) {
            o->state++;
            D_8009C93C = 0;
            D_8009C975 = 3;
        }
        o->velV += 0x20;
        if (o->velV > 0x600)
            o->velV = 0x600;
        o->y.raw -= o->velV << 8;
        pl->y.raw -= o->velV << 8;
        break;
    case 9:
        o->velV += 0x20;
        if (o->velV > 0x600)
            o->velV = 0x600;
        o->y.raw -= o->velV << 8;
        pl->y.raw -= o->velV << 8;
        if (D_8009C975 == 1) {
            D_8009CFF9 = 1;
            if (D_8009C960 == 0x7000E && D_8009D2C3 == 0xff) {
                o->state++;
            } else {
                D_1F8001D4->w4c = 7;
                D_1F8001D4->w4e = 0;
            }
        }
        break;
    case 10:
        break;
    }
}
