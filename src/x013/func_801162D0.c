// FUNC 801162d0 200 X013
// MATCHING 801162d0 200
/* debt: second extern name D_1F8000F0_b for the clamp stores (game reloads lui per access, no CSE of the address) */
#include "TOBJ.H"
typedef struct {
    char pad[0x2c];
    short xmin, xmax, ymin, ymax;
    Fix16 *pos;
} Cam;
extern TObj D_800A6038;
extern Fix16 D_1F8000F0;
extern int D_1F8000F0_b;
extern unsigned short D_8009C962;
extern void func_80115FB4(Cam *);

void func_801162D0(Cam *c)
{
    TObj *pl;

    if (D_8009C962 == 0) {
        func_80115FB4(c);
        return;
    }
    pl = &D_800A6038;
    c->pos->raw = pl->h->raw;
    if (c->xmax < c->pos->p.whole) {
        c->pos->raw = c->xmax << 16;
    } else if (c->pos->p.whole < c->xmin) {
        c->pos->raw = c->xmin << 16;
    }
    D_1F8000F0.raw = pl->y.raw;
    if (D_1F8000F0.p.whole > c->ymax) {
        D_1F8000F0_b = c->ymax << 16;
    } else if (D_1F8000F0.p.whole < c->ymin) {
        D_1F8000F0_b = c->ymin << 16;
    }
}
