// FUNC 80115fe0 456 X005
// MATCHING 80115fe0 456
#include "TOBJ.H"
extern void *D_8011A004[];
extern int D_1F8002D4;
extern unsigned short D_1F800176, D_1F800186[];
extern short FUN_8005e420(int, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void func_80115FE0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->b0d = 0x81;
            o->b.p.whole = 0x82;
            o->b0f = 0;
            o->w1e = 4;
            o->anim = D_8011A004[o->subtype];
            break;
        case 1:
            o->b0d = 1;
            o->b.p.whole = 0x82;
            o->b0f = 0;
            o->w1e = 4;
            o->anim = D_8011A004[o->subtype];
            break;
        case 2:
            o->b0d = 1;
            o->b0f = 1;
            o->b.p.whole = 0;
            o->w1e = 4;
            o->anim = D_8011A004[o->subtype];
            break;
        }
        o->d3c = D_1F8002D4;
        FUN_8001fe6c(o);
        break;
    case 1:
        o->visible = 1;
        o->a.p.whole = o->d30 - D_1F800176;
        o->y.p.whole = o->d34 - D_1F800186[0];
        FUN_80018ca4(o);
        FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
