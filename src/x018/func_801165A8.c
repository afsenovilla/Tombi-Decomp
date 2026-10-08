// FUNC 801165a8 328 X018
// MATCHING 801165a8 328
#include "TOBJ.H"

extern void *D_8011CA8C[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176, D_1F800186[];
extern unsigned short FUN_8005e420(int, int);
extern void AnimLoadDuration(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void func_801165A8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 1;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->w08 = FUN_8005e420(0x80, 0x1ff);
        o->w1e = 4;
        o->anim = D_8011CA8C[o->b0c];
        o->d3c = D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->a.p.whole = o->d30 - D_1F800176;
        {
            int y = o->d34;
            o->y.p.whole = y - D_1F800186[0];
        }
        o->visible = 1;
        ObjListPush_1F800220(o);
        AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
