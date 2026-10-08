// FUNC 80117ec0 292 X009
// MATCHING 80117ec0 292
#include "TOBJ.H"
typedef struct {
    unsigned char subtype, b0c, frame, pad;
    short x, y;
} E;
extern E D_8012A878[];
extern TObj *FUN_800183b8(void);

TObj *func_80117EC0(TObj *o, int idx)
{
    TObj *p = FUN_800183b8();

    if (p == 0) return 0;
    p->active = 1;
    p->b1d = 0;
    p->type = 0x1c;
    p->subtype = D_8012A878[idx].subtype;
    p->b0c = D_8012A878[idx].b0c;
    p->b0a = 0;
    p->animFrame = D_8012A878[idx].frame;
    p->b0f = 0;
    p->a.raw = D_8012A878[idx].x << 16;
    p->y.raw = D_8012A878[idx].y << 16;
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
