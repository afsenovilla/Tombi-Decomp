// FUNC 80117fe4 200 X009
// MATCHING 80117fe4 200
#include "TOBJ.H"

typedef struct { short x, y; } XY;
typedef struct { char c[6]; } S6;
extern S6 D_8012A938[];
extern XY D_8012A930[];
extern TObj *func_80020D98(S6 *);

void func_80117FE4(TObj *o, int i)
{
    TObj *n = func_80020D98(&D_8012A938[i]);

    n->animFrame = 0;
    n->a.raw = D_8012A930[i].x << 16;
    n->y.raw = D_8012A930[i].y << 16;
    n->b.raw = 0xb400000;
    n->d30 = n->a.raw - o->a.raw;
    n->d34 = n->y.raw - o->y.raw;
    n->d38 = n->b.raw - o->b.raw;
    n->d90 = (int)o;
    n->d94 = 0;
}
