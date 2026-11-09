// FUNC 80117ec0 292 X009
// MATCHING 80117ec0 292
#include "TOBJ.H"
typedef struct {
    unsigned char subtype, b0c, frame, pad;
    short x, y;
} E;
extern E ROTATING_PLANE_CHILD_DEFS[];
extern TObj *allocObjectLayer2(void);

TObj *spawnRotatingPlaneChild(TObj *o, int idx)
{
    TObj *p = allocObjectLayer2();

    if (p == 0) return 0;
    p->active = 1;
    p->b1d = 0;
    p->type = 0x1c;
    p->subtype = ROTATING_PLANE_CHILD_DEFS[idx].subtype;
    p->b0c = ROTATING_PLANE_CHILD_DEFS[idx].b0c;
    p->b0a = 0;
    p->animFrame = ROTATING_PLANE_CHILD_DEFS[idx].frame;
    p->b0f = 0;
    p->a.raw = ROTATING_PLANE_CHILD_DEFS[idx].x << 16;
    p->y.raw = ROTATING_PLANE_CHILD_DEFS[idx].y << 16;
    p->b.raw = 0xb400000;
    if (p->animFrame == 1) {
        p->d30 = p->a.raw - o->a.raw;
        p->d34 = p->y.raw - o->y.raw;
        p->d38 = p->b.raw - o->b.raw;
    }
    p->d90 = (int)o;
    p->d94 = 0;
    return p;
}
