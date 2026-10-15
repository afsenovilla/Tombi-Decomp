// FUNC 8013c028 396 X001
// MATCHING 8013c028 396
#include "spad.h"
#include "TOBJ.H"

typedef struct { char pad[0xa]; short w0a; } W0AS;
#define W0A(p) (((W0AS *)(p))->w0a)

typedef struct { short x; short y; } XY;

extern TObj *D_8009C948[];
extern unsigned short D_8013CA20[];
extern XY D_8013CA10[];
extern int Rand(void);
extern TObj *allocObjectLayer2(void);
extern void PoolFree_1F800210(TObj *);

void updateSegmentedCrawlerSpawner(TObj *o)
{
    TObj *e;
    int r;

    switch (o->b04) {
    case 0:
        o->w08 = 600;
        W0A(o) = 0;
        D_8009C948[0] = o;
        o->b04++;
        break;
    case 1:
        if (W0A(o) < 2) {
            if (--o->w08 == -1) {
                o->w08 = D_8013CA20[Rand() & 3];
                e = allocObjectLayer2();
                if (e) {
                    e->active = 2;
                    e->type = 0x13;
                    e->subtype = ((*(unsigned char*)&SPAD.s1f8) + SPAD.i198) & 1;
                    e->b0c = 0;
                    r = Rand() & 3;
                    e->a.raw = SPAD.iec;
                    e->y.raw = D_8013CA10[r].y << 16;
                    e->b.raw = D_8013CA10[r].x << 16;
                    W0A(D_8009C948[0])++;
                }
            }
        }
        break;
    case 2:
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
