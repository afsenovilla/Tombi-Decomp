// FUNC 80118188 656 X010
// MATCHING 80118188 656
#include "TOBJ.H"

extern void *D_80132C3C[];
extern void *D_80132C54[];
extern int D_1F8002DC;
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern unsigned short GetClut(int, int);
extern int Rand(void);
extern short MulNegSin(int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjListPush_1F800220(TObj *);
extern void ObjFree(TObj *);

void func_80118188(TObj *o)
{
    short v;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->animFrame = 1;
        o->w08 = GetClut(0x80, 0x1ff);
        o->b0a = 7;
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        switch (o->subtype) {
        case 0:
            o->b0d = 1;
            o->w08 = GetClut(0x80, 0x1ff);
            o->w1e = 4;
            o->anim = D_80132C3C[o->b0c];
            break;
        case 1:
            o->b0d = 0x81;
            o->w08 = GetClut(0x90, 0x1e0);
            o->w1e = 6;
            o->anim = D_80132C54[o->b0c];
            break;
        case 2:
            o->b0d = 1;
            o->w08 = GetClut(0x90, 0x1e0);
            o->w1e = 6;
            o->anim = D_80132C54[o->b0c];
            o->timer = Rand() & 7;
            o->w22 = 0;
            break;
        }
        o->d3c = D_1F8002DC;
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->subtype == 2) {
            v = MulNegSin(o->w22, 2);
            o->a.p.whole = v + (o->d30 - D_1F800176);
            o->w22 = (o->w22 + o->timer) & 0xff;
            o->y.p.whole = o->d34 - D_1F800186;
            if (v < 0) {
                o->animFrame = 0;
            } else {
                o->animFrame = 1;
            }
        } else {
            *(short *)((char *)o + 0x12) = o->d30 - D_1F800176;
            o->y.p.whole = o->d34 - D_1F800186;
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
