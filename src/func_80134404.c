// FUNC 80134404 700 X000
// MATCHING 80134404 700
#include "TOBJ.H"
extern void *D_8013B270[];
extern int D_1F8002D4[];
extern unsigned char D_8009CDA9;
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);
void ObjCullRegister(TObj *o);
short GetClut(int, int);
void FUN_80134190(TObj *o);
void FUN_80018790(TObj *o);
void func_80134404(TObj *o)
{
    int t;
    void *a;
    switch (o->b04) {
    case 0:
        o->box0 = 10;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->w1e = 8;
        o->animFrame = 1;
        t = D_1F8002D4[0];
        a = D_8013B270[0];
        o->d3c = t;
        o->anim = a;
        AnimLoadDuration(o);
        o->b0d = 1;
        o->b0a = 0;
        o->w08 = GetClut(0x100, 0x1f1 - o->subtype);
        AnimLoadDuration(o);
        o->d8c = 0;
        o->category |= 0x80;
        o->b04++;
        o->step = o->subtype;
        if (D_8009CDA9 == 0xff) o->b04 = 3;
        break;
    case 1:
        ObjCullRegister(o);
        if (D_8009CDA9 == 0xff) {
            if (!o->visible) o->b04 = 3;
            FUN_80134190(o);
        } else if (o->visible) {
            FUN_80134190(o);
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->visible) o->step = 1;
            break;
        case 1:
            break;
        case 2:
            switch (o->state) {
            case 0:
                o->anim = D_8013B270[0];
                AnimLoadDuration(o);
                o->velX = 0x200;
                o->animFrame = 0;
                o->velY = -0x400;
                o->w08 = GetClut(0x100, 0x1f0);
                o->state++;
            case 1:
                AnimAdvance(o);
                o->h->raw += o->velX << 8;
                o->y.raw += o->velY << 8;
                o->velY += 0x40;
                if (o->velY > 0x780) o->velY = 0x780;
                if (!o->visible) {
                    o->b04 = 3;
                    o->step = 0;
                    o->state = 0;
                }
                break;
            }
            break;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
