// FUNC 80116234 156 X013
// MATCHING 80116234 156
#include "TOBJ.H"
typedef struct {
    char pad[0x2c];
    short xmin, xmax, ymin, ymax;
    Fix16 *pos;
} Cam;
extern TObj PLAYER;
extern Fix16 VIEW_CAMERA_Y_FIX;

void cameraFollowPlayerClamped(Cam *c)
{
    TObj *pl = &PLAYER;
    Fix16 *q;

    c->pos->raw = pl->h->raw;
    if (c->xmax < c->pos->p.whole) {
        c->pos->raw = c->xmax << 16;
    } else if (c->pos->p.whole < c->xmin) {
        c->pos->raw = c->xmin << 16;
    }
    VIEW_CAMERA_Y_FIX.raw = pl->y.raw;
    if (VIEW_CAMERA_Y_FIX.p.whole > c->ymax) {
        VIEW_CAMERA_Y_FIX.raw = c->ymax << 16;
    } else if (VIEW_CAMERA_Y_FIX.p.whole < c->ymin) {
        VIEW_CAMERA_Y_FIX.raw = c->ymin << 16;
    }
}
