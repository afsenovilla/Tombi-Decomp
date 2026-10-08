// FUNC 80116234 156 X013
// MATCHING 80116234 156
#include "TOBJ.H"
typedef struct {
    char pad[0x2c];
    short xmin, xmax, ymin, ymax;
    Fix16 *pos;
} Cam;
extern TObj D_800A6038;
extern Fix16 D_1F8000F0;

void func_80116234(Cam *c)
{
    TObj *pl = &D_800A6038;
    Fix16 *q;

    c->pos->raw = pl->h->raw;
    if (c->xmax < c->pos->p.whole) {
        c->pos->raw = c->xmax << 16;
    } else if (c->pos->p.whole < c->xmin) {
        c->pos->raw = c->xmin << 16;
    }
    D_1F8000F0.raw = pl->y.raw;
    if (D_1F8000F0.p.whole > c->ymax) {
        D_1F8000F0.raw = c->ymax << 16;
    } else if (D_1F8000F0.p.whole < c->ymin) {
        D_1F8000F0.raw = c->ymin << 16;
    }
}
