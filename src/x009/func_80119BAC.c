// FUNC 80119bac 384 X009
// MATCHING 80119bac 384
#include "TOBJ.H"
typedef struct { unsigned char b0f; char p; unsigned short w1e; unsigned short cx, cy; void **anims; } E12;
extern E12 D_8012ABBC[];
extern int D_1F8002D4[];
extern short D_1F800176[], D_1F800186[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern unsigned short GetClut(int, int);

void func_80119BAC(TObj *o)
{
    E12 *e;
    unsigned char s = o->b04;

    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->animFrame = 1;
        o->b0d = 1;
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        e = &D_8012ABBC[o->subtype];
        o->w1e = e->w1e;
        o->w08 = GetClut(e->cx, e->cy);
        o->b0f = e->b0f;
        o->anim = e->anims[o->b0c];
        o->d3c = D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        o->visible = 1;
        o->a.p.whole = o->d30 - D_1F800176[0];
        o->y.p.whole = o->d34 - D_1F800186[0];
        ObjListPush_1F800220(o);
        AnimAdvance(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
