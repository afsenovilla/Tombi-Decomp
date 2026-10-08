// FUNC 80118110 544 X017
// MATCHING 80118110 544
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;
extern TObj DAT_800a6038;
extern unsigned char DAT_8009c942;
extern unsigned char DAT_8009d2b0;
extern short D_1F8001C6;
extern AT3 D_8011999C[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);

void func_80118110(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        DAT_8009c942 = 1;
        DAT_8009d2b0 = 2;
        DAT_800a6038.b04 = 5;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        o->anim = D_8011999C[o->subtype].p[2];
        AnimJump(o, 0);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        switch (o->subtype) {
        case 0:
            o->d90 = (int)FUN_8002dcc8(2, 0x23, &v);
            break;
        case 1:
            o->d90 = (int)FUN_8002dcc8(2, 0x27, &v);
            break;
        }
        o->state = 1;
        break;
    case 1:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->state = 9;
        }
        break;
    case 9:
        D_1F8001C6 = 0;
        DAT_8009c942 = 0;
        DAT_800a6038.wb2 = 0;
        o->anim = D_8011999C[o->subtype].p[0];
        AnimJump(o, 0);
        o->animFrame = o->wbc;
        DAT_800a6038.b04 = 1;
        DAT_800a6038.step = 0;
        DAT_800a6038.state = 0;
        o->b68 = 0;
        o->step = 1;
        o->state = 0;
        break;
    }
}
