// FUNC 8002f060 268 MAIN0
// MATCHING 8002f060 268
#include "TOBJ.H"
typedef struct { void **a; int p[2]; } A12;
extern A12 D_80079D58[];
extern unsigned char D_800A6039;
extern unsigned char D_800A6047;
extern unsigned char D_8009C990;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

void func_8002F060(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = D_80079D58[o->subtype].a[o->b0c];
        AnimLoadDuration(o);
        o->visible = D_800A6039;
        o->b0f = D_800A6047 - 1;
        o->b6b = 0x7f;
        if (D_8009C990 == 3) {
            o->b6b = 0x40;
        }
        o->state++;
        break;
    case 1:
        if (AnimAdvance(o)) {
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
        if (o->visible == 0) {
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
