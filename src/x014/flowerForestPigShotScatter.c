// FUNC 80121d08 648 X014
// MATCHING 80121d08 648
#include "gvars.h"
#include "TOBJ.H"

extern TObj *allocObjectLayer2(void);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern char D_8012A344[];
extern char D_8012A35C[];
extern char D_8012A364[];
extern char D_8012A34C[];
extern char D_8012A370[];
extern char D_8012A378[];
extern char D_8012A354[];
extern char D_8012A384[];
extern char D_8012A38C[];
static void *D_80129FCC[9] = {
    D_8012A344, D_8012A35C, D_8012A364, D_8012A34C,
    D_8012A370, D_8012A378, D_8012A354, D_8012A384,
    D_8012A38C,
};
extern short D_801265C4[];
extern short COS_TABLE[], D_8007A1F0[];
extern unsigned short TICK_COUNT;
extern int UPDATE_OBJECT_INDEX;

void flowerForestPigShotScatter(TObj *o)
{
    TObj *e;
    int i;
    unsigned char st;
    short v;
    short dx, dy;

    switch (o->step) {
    case 0:
        if (o->b0c == 0) {
            st = o->subtype;
            for (i = 0; i < 6; i++) {
                e = allocObjectLayer2();
                if (e) {
                    e->type = 0x47;
                    e->active = 2;
                    e->subtype = st;
                    e->b0c = i + 1;
                    e->b04 = 2;
                    e->b0a = o->b0a;
                    e->w1e = o->w1e;
                    e->b0d = o->b0d;
                    e->d3c = o->d3c;
                    e->b0f = o->b0f;
                    e->a.raw = o->a.p.whole << 16;
                    e->y.raw = o->y.p.whole << 16;
                    e->b.raw = o->b.p.whole << 16;
                }
            }
            o->b04 = 3;
            o->active = 2;
        } else {
            o->step++;
            v = o->subtype * 3 + 1;
            o->wac = v;
            o->anim = D_80129FCC[v];
            AnimLoadDuration(o);
            o->d38 = D_801265C4[o->b0c];
            o->timer = 0x12;
            o->velH = 0x200;
        }
        ObjCullRegister(o);
        break;
    case 1:
        if (GVARS.b42 == 1) {
            ObjCullRegister(o);
            break;
        }
        AnimAdvance(o);
        dx = o->velH * COS_TABLE[o->d38] >> 12;
        dy = o->velH * D_8007A1F0[o->d38] >> 12;
        o->h->raw += dx << 8;
        o->y.raw += dy << 8;
        if ((TICK_COUNT + UPDATE_OBJECT_INDEX) & 1) ObjCullRegister(o);
        if (--o->timer == -1) {
            o->step = 0;
            o->b04++;
        }
        break;
    }
}
