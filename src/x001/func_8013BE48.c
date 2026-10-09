// FUNC 8013be48 480 X001
// MATCHING 8013be48 480
#include "TOBJ.H"

typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    short w08;
    char p0a[0x1c - 0xa];
    short w1c, w1e, w20;
} O;

extern unsigned short D_8013CA00[];
extern unsigned char D_8009CDB5[];
extern unsigned short D_1F800176;
extern short D_1F80016E[];
extern int Rand(void);
extern TObj *FUN_800183b8(void);
extern void PoolFree_1F800210(O *);

void func_8013BE48(O *o)
{
    unsigned short t;
    TObj *e;
    short *p = &o->w1c;

    switch (o->b04) {
    case 0:
        o->w1c = 0;
        o->w1e = 0;
        o->w20 = 0;
        o->w08 = D_8013CA00[Rand() & 7];
        if (D_8009CDB5[0]) o->b04++;
        else o->b04 = 3;
        break;
    case 1:
        t = D_1F800176;
        if ((unsigned short)(t - 1000) < 0x49d && o->w1c < 2) {
            if (!(t & 0x1f)) {
                o->w1e = 1;
                if (--o->w08 != -1) break;
            } else {
                if (o->w1e != 1) break;
                if (--o->w08 != -1) break;
                o->w1e = 0;
            }
            o->w08 = D_8013CA00[Rand() & 7];
            e = FUN_800183b8();
            if (e) {
                e->active = 2;
                e->type = 2;
                e->animFrame = 0;
                e->subtype = 8;
                e->b0c = 0;
                e->a.p.whole = 0;
                e->y.p.whole = D_1F80016E[0];
                e->b.p.whole = 0;
                o->w1c++;
                e->d94 = (int)p;
            }
        }
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
