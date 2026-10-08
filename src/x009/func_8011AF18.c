// FUNC 8011af18 260 X009
// MATCHING 8011af18 260
#include "TOBJ.H"
typedef struct { unsigned char b0; signed char b1, b2, b3; } T4;
extern T4 D_8012B218[];
extern TObj *FUN_800183b8(void);

TObj *func_8011AF18(TObj *o, int k)
{
    unsigned char s;
    TObj *n = FUN_800183b8();
    if (n == 0)
        return 0;
    n->active = 1;
    n->b1d = 0;
    n->type = 0xa;
    s = D_8012B218[k].b0;
    n->b0c = 0;
    n->b0a = 0;
    n->animFrame = 1;
    n->b0f = 0;
    n->subtype = s;
    n->d30 = D_8012B218[k].b1;
    n->d34 = D_8012B218[k].b2;
    n->d38 = D_8012B218[k].b3;
    n->a.raw = (n->d30 << 16) + o->a.raw;
    n->y.raw = (n->d34 << 16) + o->y.raw;
    n->b.raw = (n->d38 << 16) + o->b.raw;
    n->d90 = (int)o;
    n->d94 = 0;
    return n;
}
