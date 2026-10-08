// FUNC 8012ea30 328 X004
// MATCHING 8012ea30 328
#include "TOBJ.H"

typedef struct { unsigned short f, s, c, x, y, z; } T;
extern unsigned char D_8009D2C3, D_8009CF2D, D_8009CF29[];
extern T D_8013144C;
extern TObj *FUN_800183b8(void);
extern void PoolFree_1F800210(TObj *);

void func_8012EA30(TObj *o)
{
    TObj *n;

    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 8) {
            o->b04 = 3;
            break;
        }
        if (!D_8009CF2D) {
            n = FUN_800183b8();
            if (n) {
                n->active = 3;
                n->type = 0x32;
                n->animFrame = D_8013144C.f;
                n->subtype = D_8013144C.s;
                n->b0c = D_8013144C.c;
                n->a.p.whole = D_8013144C.x;
                n->y.p.whole = D_8013144C.y;
                n->b.p.whole = D_8013144C.z;
                n->b6b = 0;
                D_8009CF29[0] = 0;
            }
        }
        o->step = 0;
        o->state = 0;
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
