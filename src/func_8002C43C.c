// FUNC 8002c43c 316 MAIN0
// MATCHING 8002c43c 316
#include "TOBJ.H"
extern char D_80014744[];
extern char D_8001473C[];
extern void AnimLoadDuration(TObj *);
extern void ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
extern void FUN_8002c0e0(TObj *);
extern void FUN_8002c238(TObj *);

void func_8002C43C(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->anim = D_80014744;
            AnimLoadDuration(o);
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            break;
        case 1:
            o->anim = D_8001473C;
            AnimLoadDuration(o);
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            FUN_8002c0e0(o);
            break;
        case 1:
            FUN_8002c238(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
