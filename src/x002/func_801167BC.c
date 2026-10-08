// FUNC 801167bc 364 X002
// MATCHING 801167bc 364
#include "TOBJ.H"

extern void *D_1F8002D4[];
extern unsigned short D_1F800176, D_1F800186[];
extern void *D_8011F860[];
extern short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void func_801167BC(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->b0a = 7;
        o->b0d = 1;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        if (o->subtype == 0) {
            o->w08 = GetClut(0x80, 0x1ff);
            o->w1e = 4;
        } else {
            o->w08 = GetClut(0x130, 0x1e0);
            o->w1e = 6;
        }
        o->anim = D_8011F860[o->b0c];
        o->d3c = (int)D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->a.p.whole = o->d30 - D_1F800176;
        o->y.p.whole = o->d34 - D_1F800186[0];
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
