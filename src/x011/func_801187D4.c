// FUNC 801187d4 548 X011
// MATCHING 801187d4 548
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
extern TObj DAT_800a6038;
extern unsigned char DAT_8009c940, DAT_8009c941;
extern unsigned char DAT_8009c942[];
extern unsigned char DAT_8009d2b0;
extern short D_1F8001C6;
extern AT3 D_80119C48[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void AnimLoadDuration(TObj *);

void func_801187D4(TObj *o)
{
    B12 v;
    unsigned char *c;

    switch (o->state) {
    case 0:
        c = &DAT_8009c940;
        if (*c != 0) {
            if (DAT_8009c941 != 0x3c)
                break;
            o->step = 5;
            o->state = 0;
            *c = 0;
            break;
        }
        if (o->b68 == 0)
            break;
        DAT_8009c942[0] = 1;
        DAT_8009d2b0 = 2;
        DAT_800a6038.b04 = 5;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        AnimLoadDuration(o);
        o->d90 = (int)FUN_8002dcc8(2, 0x10, &v);
        o->state++;
        break;
    case 1:
        if (((TObj *)o->d90)->b04 != 2)
            break;
        ((TObj *)o->d90)->b04 = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        AnimLoadDuration(o);
        o->state++;
        break;
    case 2:
        DAT_8009c942[0] = 0;
        DAT_800a6038.wb2 = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        DAT_800a6038.b04 = 1;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
